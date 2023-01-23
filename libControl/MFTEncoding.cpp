#include "header.h"
#ifdef WIN32
#include "MFTEncoding.h"
#include <CompressVideo.h>
#include <iostream>
#include <string>
#include <ConvertUtility.h>
#include <mfidl.h>            // Media Foundation interfaces
#include <mfapi.h>            // Media Foundation platform APIs
#include <mferror.h>        // Media Foundation error codes
#include <mfreadwrite.h>
#include <propvarutil.h>
#include <chrono>
#include <OpenCLEffectVideo.h>
#include <fstream>
#include <VideoStabilization.h>

// The required link libraries 
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mf")
#pragma comment(lib, "mfuuid")
#pragma comment(lib, "mfreadwrite")
#pragma comment(lib, "msdmo")
#pragma comment(lib, "strmiids")
#pragma comment(lib, "propsys")
//#pragma comment(lib, "d3d9")
//#pragma comment(lib, "dxva2")
using namespace Regards::OpenCL;
const MFTIME ONE_SECOND = 10000000; // One second

template <class T>
void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class CMFTEncodingPimp
{
public:
	CMFTEncodingPimp()
	{
	}


	~CMFTEncodingPimp()
	{
	}


	int Encode(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress,
	           CVideoOptionCompress* videoCompressOption, long time = -1);
	HRESULT WriteFrame(BYTE* buffer, DWORD streamIndex, LONGLONG timestamp, LONGLONG duration,
	                   CompressVideo* m_dlgProgress, CVideoOptionCompress* videoCompressOption);
	HRESULT WriteAudioBuffer(BYTE* buffer, size_t bufferSize, LONGLONG timestamp, LONGLONG duration);
	COpenCVStabilization* openCVStabilization = nullptr;

	IMFSourceReaderEx* m_pSourceReader{nullptr};
	/*
	IDirect3DDeviceManager9* m_pD3D9DeviceManager{ nullptr };
	IDirect3D9Ex* m_pD3D9Ex{ nullptr };
	IDirect3DDevice9Ex* m_pD3D9Device{ nullptr };
	IDirect3DTexture9* m_pD3D9Texture{ nullptr };
	IDirect3DSurface9* m_pD3D9Surface{ nullptr };
	*/
	unsigned int m_iResetToken{0};

	IMFSinkWriter* m_pWriter;

	DWORD m_readVideoStreamIndex;
	DWORD m_readAudioStreamIndex;

	DWORD m_writeVideoStreamIndex;
	DWORD m_writeAudioStreamIndex;

	//video metadata
	LONGLONG mediaDuration{0};
	UINT32 frameDurationNum{0}, frameDurationDenom{0};
	UINT32 width{0}, height{0};

	//audio metadata
	UINT32 audioChannels{0};
	UINT32 audioSamplesPerSecond{0};
	UINT32 audioAvgBitrate{0};
	UINT32 audioBitsPerSample{0};
	UINT32 audioBlockAlign{0};
	UINT32 audioAvgBytesPerSecond{0};
	UINT32 audioSamplesPerBlock{0};
	UINT32 audioValidBitsPerSample{0};

	UINT32 VIDEO_BIT_RATE = 10000000;
	GUID VIDEO_INPUT_FORMAT = MFVideoFormat_RGB32;
	GUID VIDEO_ENCODING_FORMAT = MFVideoFormat_H264;
	GUID AUDIO_ENCODING_FORMAT = MFAudioFormat_AAC;

	cv::Mat bitmap;
};


static class MFSingleton
{
public:
	MFSingleton()
	{
	}

	~MFSingleton()
	{
		/*
		if (s_pD3D9Ex != NULL)
		{
		    ULONG refCount = s_pD3D9Ex->Release();
		    if (refCount > 0)
		    {
		        std::cout << "Released Direct3D interface but ref cout > 0" << std::endl;
		    }

		    s_pD3D9Ex = NULL;
		}
		*/

		if (s_bMFInitialized)
		{
			HRESULT hr = MFShutdown();
			if (FAILED(hr))
			{
				std::cout << "Failed to shut down Media Foundation" << std::endl;
			}
			s_bMFInitialized = false;
		}

		if (s_bCOMInitialized)
		{
			CoUninitialize();
			s_bCOMInitialized = false;
		}
	}

	static void Init()
	{
		init();
	}

	/*
	static IDirect3D9Ex* getD3D9Ex()
	{
	    init();
	    return s_pD3D9Ex;
	}
	*/

private:
	//static IDirect3D9Ex* s_pD3D9Ex;
	static bool s_bCOMInitialized;
	static bool s_bMFInitialized;
	static bool s_bNVAPIInitialized;

	static void init()
	{
		if (!s_bCOMInitialized)
		{
			// Initialize COM
			HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
			if (hr == S_FALSE)
			{
				std::cout << "COM already initialized on this thread." << std::endl;
			}
			else if (hr != S_OK)
			{
				throw std::runtime_error("Failed to initialize COM.");
			}
			s_bCOMInitialized = true;
		}

		if (!s_bMFInitialized)
		{
			// Initialize Media Foundation
			HRESULT hr = MFStartup(MF_SDK_VERSION);
			if (FAILED(hr))
			{
				throw std::runtime_error("Failed to initialize Media Foundation");
			}
			s_bMFInitialized = true;
		}

		/*
		if (s_pD3D9Ex == NULL)
		{
		    // Initialize D3D9
		    HRESULT hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &s_pD3D9Ex);
		    if (FAILED(hr))
		    {
		        throw std::runtime_error("Failed to initialize Direct3D");
		    }
		}
		*/
	}
};


// Create global scope instance of MFSingleton to ensure startup and shutdown occurs once
// Do not use this class anywhere else!
static MFSingleton g_MFSingleton;

struct MFBufferAccess
{
	~MFBufferAccess()
	{
		HRESULT hr = m_pBuffer->Unlock();
		if (hr != S_OK)
		{
			SafeRelease(&m_pBuffer);
			std::cout << "Unable to unlock audio buffer" << std::endl;
		}
		SafeRelease(&m_pBuffer);
	}

	MFBufferAccess(IMFSample* pSample)
	{
		DWORD numBuffers;
		HRESULT hr = pSample->GetBufferCount(&numBuffers);
		if (hr != S_OK)
		{
			std::cout << "Unable to query audio buffer count" << std::endl;
		}

		if (numBuffers > 1)
		{
			hr = pSample->ConvertToContiguousBuffer(&m_pBuffer);
		}
		else
		{
			hr = pSample->GetBufferByIndex(0, &m_pBuffer);
		}
		if (hr != S_OK)
		{
			SafeRelease(&m_pBuffer);
			std::cout << "Unable to fetch audio buffer" << std::endl;
		}

		hr = m_pBuffer->Lock(&data, &maxSize, &size);
		if (hr != S_OK)
		{
			SafeRelease(&m_pBuffer);
			std::cout << "Unable to lock audio buffer for reading" << std::endl;
		}
	}

	BYTE* data{nullptr};
	DWORD size{0};
	DWORD maxSize{0};

private:
	IMFMediaBuffer* m_pBuffer{nullptr};
};

//IDirect3D9Ex* MFSingleton::s_pD3D9Ex = NULL;
bool MFSingleton::s_bCOMInitialized = false;
bool MFSingleton::s_bMFInitialized = false;
bool MFSingleton::s_bNVAPIInitialized = false;


CMFTEncoding::CMFTEncoding()
{
	pimpl = new CMFTEncodingPimp();
}

CMFTEncoding::~CMFTEncoding()
{
	delete pimpl;
}

cv::Mat CMFTEncoding::GetFrameOutput()
{
	return pimpl->bitmap.clone();
}


int CMFTEncoding::EncodeOneFrame(CompressVideo* m_dlgProgress, const wxString& input, const wxString& output,
                                 const long& time, CVideoOptionCompress* videoCompressOption)
{
	return pimpl->Encode(input, output, m_dlgProgress, videoCompressOption, time);
}


int CMFTEncoding::EncodeFile(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress,
                             CVideoOptionCompress* videoCompressOption)
{
	return pimpl->Encode(input, output, m_dlgProgress, videoCompressOption);
}


HRESULT CMFTEncodingPimp::WriteFrame(
	BYTE* buffer,
	DWORD streamIndex,
	LONGLONG timestamp,
	LONGLONG duration, CompressVideo* m_dlgProgress, CVideoOptionCompress* videoCompressOption
)
{
	COpenCLEffectVideo openclEffectVideo;
	IMFSample* pSample = nullptr;
	IMFMediaBuffer* pBuffer = nullptr;
	BYTE* pData = nullptr;

	UINT32 numPixels = width * height;
	DWORD cbWidth = 4 * width;
	DWORD cbBuffer = cbWidth * height;

	// Create a new memory buffer.
	HRESULT hr = MFCreateMemoryBuffer(cbBuffer, &pBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to initialize sink writer: failed to create memory buffer");
	}

	// Lock the buffer and copy the video frame to the buffer.
	hr = pBuffer->Lock(&pData, nullptr, nullptr);

	if (SUCCEEDED(hr))
	{
		hr = MFCopyImage(
			pData, // Destination buffer.
			cbWidth, // Destination stride.
			buffer, // First row in source image.
			cbWidth, // Source stride.
			cbWidth, // Image width in bytes.
			height // Image height in pixels.
		);
	}


	cv::Mat mat(height, width, CV_8UC4, pData);
	openclEffectVideo.SetMatrix(mat);

	bool stabilizeFrame = videoCompressOption->videoEffectParameter.stabilizeVideo;
	bool correctedContrast = videoCompressOption->videoEffectParameter.autoConstrast;

	if (stabilizeFrame && openCVStabilization == nullptr)
		openCVStabilization = new COpenCVStabilization(videoCompressOption->videoEffectParameter.stabilizeImageBuffere);


	if (stabilizeFrame || correctedContrast)
	{
		openclEffectVideo.ApplyOpenCVEffect(&videoCompressOption->videoEffectParameter, openCVStabilization);
	}

	openclEffectVideo.ApplyVideoEffect(&videoCompressOption->videoEffectParameter);

	bitmap = openclEffectVideo.GetMatrix();

	memcpy(pData, mat.data, cbWidth * height);

	if (m_dlgProgress != nullptr)
	{
		m_dlgProgress->SetBitmap(bitmap);
		char txtduration[255];
		double percent = timestamp / duration;
		double pos = timestamp / 1000;
		double sec = duration / 1000;

		sprintf(txtduration, "Progress : %d percent - Total Second : %d / %d", static_cast<int>(percent * 100),
		        static_cast<int>(pos), static_cast<int>(sec));

		m_dlgProgress->SetPos(static_cast<int>(duration), static_cast<int>(sec));
		m_dlgProgress->SetTextProgression(txtduration);
	}

	if (pBuffer)
	{
		pBuffer->Unlock();
	}

	// Set the data length of the buffer.
	if (SUCCEEDED(hr))
	{
		hr = pBuffer->SetCurrentLength(cbBuffer);
	}

	// Create a media sample and add the buffer to the sample.
	if (SUCCEEDED(hr))
	{
		hr = MFCreateSample(&pSample);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSample->AddBuffer(pBuffer);
	}

	// Set the time stamp and the duration.
	if (SUCCEEDED(hr))
	{
		hr = pSample->SetSampleTime(timestamp);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSample->SetSampleDuration(duration);
	}

	// Send the sample to the Sink Writer.
	if (SUCCEEDED(hr))
	{
		hr = m_pWriter->WriteSample(streamIndex, pSample);
	}

	if (!SUCCEEDED(hr))
	{
		std::cout << "Error writing video frame" << std::endl;
	}

	SafeRelease(&pBuffer);
	SafeRelease(&pSample);
	return hr;
}

HRESULT CMFTEncodingPimp::WriteAudioBuffer(BYTE* buffer, size_t bufferSize, LONGLONG timestamp, LONGLONG duration)
{
	HRESULT hr;
	IMFSample* pSample = nullptr;
	BYTE* pData = nullptr;

	// Create a new memory buffer.
	const DWORD cbBuffer = bufferSize;
	IMFMediaBuffer* aBuffer = nullptr;
	hr = MFCreateMemoryBuffer(cbBuffer, &aBuffer);

	// Lock the buffer and copy the video frame to the buffer.
	if (SUCCEEDED(hr))
	{
		hr = aBuffer->Lock(&pData, nullptr, nullptr);
	}
	if (SUCCEEDED(hr))
	{
		memcpy(pData, buffer, cbBuffer);
	}
	if (aBuffer)
	{
		aBuffer->Unlock();
	}
	// Set the data length of the buffer.
	if (SUCCEEDED(hr))
	{
		hr = aBuffer->SetCurrentLength(cbBuffer);
	}

	// Create a media sample and add the buffer to the sample.
	if (SUCCEEDED(hr))
	{
		hr = MFCreateSample(&pSample);
	}
	else
	{
		throw std::runtime_error("Error writing audio frame: unable to create sample");
	}

	if (SUCCEEDED(hr))
	{
		hr = pSample->AddBuffer(aBuffer);
	}

	// Set the sample time
	if (SUCCEEDED(hr))
	{
		hr = pSample->SetSampleTime(timestamp);
	}

	// Set the sample duration
	if (SUCCEEDED(hr))
	{
		hr = pSample->SetSampleDuration(duration);
	}

	// Send the sample to the Sink Writer.
	if (SUCCEEDED(hr))
	{
		hr = m_pWriter->WriteSample(m_writeAudioStreamIndex, pSample);
	}

	if (!SUCCEEDED(hr))
	{
		std::cout << "Error writing audio frame" << std::endl;
	}
	SafeRelease(&pSample);
	SafeRelease(&aBuffer);
	return hr;
}


int CMFTEncodingPimp::Encode(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress,
                             CVideoOptionCompress* videoCompressOption, long time)
{
	//--------------------------------
	// SETUP DEVICES
	//--------------------------------

	if (videoCompressOption != nullptr)
	{
		VIDEO_BIT_RATE = videoCompressOption->videoBitRate * 1024;
		if (videoCompressOption->videoCodec == "H264")
			VIDEO_ENCODING_FORMAT = MFVideoFormat_H264;
		else if (videoCompressOption->videoCodec == "H265")
			VIDEO_ENCODING_FORMAT = MFVideoFormat_H265;
		else if (videoCompressOption->videoCodec == "VP9")
			VIDEO_ENCODING_FORMAT = MFVideoFormat_VP90;
		else if (videoCompressOption->videoCodec == "VP8")
			VIDEO_ENCODING_FORMAT = MFVideoFormat_VP80;
		else if (videoCompressOption->videoCodec == "MPEG4")
			VIDEO_ENCODING_FORMAT = MFVideoFormat_MP4V;
		else if (videoCompressOption->videoCodec == "MPEG2")
			VIDEO_ENCODING_FORMAT = MFVideoFormat_MPG1;

		if (videoCompressOption->audioCodec == "AAC")
			AUDIO_ENCODING_FORMAT = MFAudioFormat_AAC;
		else if (videoCompressOption->audioCodec == "VORBIS")
			AUDIO_ENCODING_FORMAT = MFAudioFormat_Opus;
		else if (videoCompressOption->audioCodec == "MP3")
			AUDIO_ENCODING_FORMAT = MFAudioFormat_MP3;
		// UINT32 VIDEO_BIT_RATE = 10000000;
		//const GUID   VIDEO_INPUT_FORMAT = MFVideoFormat_RGB32;
		//  GUID   VIDEO_ENCODING_FORMAT = MFVideoFormat_H264;
	}


	//m_pD3D9Ex = MFSingleton::getD3D9Ex();
	MFSingleton::Init();

	IMFAttributes* pAttr = nullptr;

	//enable hardware decoding to convert yuv formats to RGB32
	HRESULT hr = MFCreateAttributes(&pAttr, 1);
	hr = pAttr->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, TRUE);

	if (hr != S_OK)
	{
		return 1;
	}

	if (SUCCEEDED(hr))
	{
		hr = pAttr->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, true);
	}

	if (SUCCEEDED(hr))
	{
		hr = pAttr->SetUINT32(MF_LOW_LATENCY, false);
	}

	IMFSourceReader* pReader = nullptr;
	std::string path = CConvertUtility::ConvertToStdString(input);
	hr = MFCreateSourceReaderFromURL(std::wstring(path.begin(), path.end()).c_str(), pAttr, &pReader);
	if (hr != S_OK)
	{
		SafeRelease(&pReader);
		return 1;
	}

	hr = pReader->QueryInterface<IMFSourceReaderEx>(&m_pSourceReader);
	if (hr != S_OK)
	{
		SafeRelease(&pReader);
		return 1;
	}
	SafeRelease(&pReader);

	DWORD dwStreamIndex = 0;
	IMFMediaType* pNativeType = nullptr;

	m_readVideoStreamIndex = MF_SOURCE_READER_INVALID_STREAM_INDEX;
	m_readAudioStreamIndex = MF_SOURCE_READER_INVALID_STREAM_INDEX;

	do
	{
		hr = m_pSourceReader->GetNativeMediaType(dwStreamIndex, 0, &pNativeType);
		if (hr == S_OK)
		{
			GUID majorType;
			hr = pNativeType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
			if (hr == S_OK)
			{
				if (m_readVideoStreamIndex == MF_SOURCE_READER_INVALID_STREAM_INDEX && majorType == MFMediaType_Video)
				{
					m_readVideoStreamIndex = dwStreamIndex;
				}
				else if (m_readAudioStreamIndex == MF_SOURCE_READER_INVALID_STREAM_INDEX && majorType ==
					MFMediaType_Audio)
				{
					m_readAudioStreamIndex = dwStreamIndex;
				}
			}
			SafeRelease(&pNativeType);
			++dwStreamIndex;
		}
	}
	while (hr == S_OK);

	SafeRelease(&pNativeType);

	hr = S_OK;

	if (hr != S_OK)
	{
		std::cout << "Couldn't get stream indices from file." << std::endl;
		return 1;
	}

	double totalTime = 0;

	if (m_readVideoStreamIndex != MF_SOURCE_READER_INVALID_STREAM_INDEX)
	{
		// read video metadata
		PROPVARIANT var;
		HRESULT hr = m_pSourceReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE,
		                                                       MF_PD_DURATION, &var);
		if (SUCCEEDED(hr))
		{
			hr = PropVariantToInt64(var, &mediaDuration);
			PropVariantClear(&var);
		}
		if (hr != S_OK)
		{
			SafeRelease(&m_pSourceReader);
			std::cout << "couldn't read media duration" << std::endl;
			return 1;
		}

		totalTime = static_cast<double>(mediaDuration) / 10000000.0;
		totalTime *= 1000;
		std::cout << "Video duration: " << (static_cast<double>(mediaDuration) / 10000000.0) << std::endl;

		IMFMediaType* pNativeType = nullptr;

		// Find the native format of the stream.
		hr = m_pSourceReader->GetNativeMediaType(m_readVideoStreamIndex, 0, &pNativeType);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = MFGetAttributeRatio(pNativeType, MF_MT_FRAME_RATE, &frameDurationNum, &frameDurationDenom);
		std::cout << "Video frame rate: " << frameDurationNum << " / " << frameDurationDenom << " (" << (static_cast<
			double>(frameDurationNum) / static_cast<double>(frameDurationDenom)) << " fps)" << std::endl;

		if (hr != S_OK)
		{
			SafeRelease(&m_pSourceReader);
			std::cout << "couldn't get frame duration from media" << std::endl;
			return 1;
		}

		hr = MFGetAttributeSize(pNativeType, MF_MT_FRAME_SIZE, &width, &height);
		std::cout << "Video width: " << width << " height: " << height << std::endl;
		SafeRelease(&pNativeType);

		if (hr != S_OK)
		{
			SafeRelease(&m_pSourceReader);
			std::cout << "couldn't get frame size from media." << std::endl;
			return 1;
		}
	}
	else
	{
		// For our pourposes, no video streams is fatal
		SafeRelease(&m_pSourceReader);
		std::cout << "media has no video stream" << std::endl;
		return 1;
	}

	if (m_readAudioStreamIndex != MF_SOURCE_READER_INVALID_STREAM_INDEX)
	{
		IMFMediaType* pNativeType = nullptr;

		// Find the native format of the stream.
		hr = m_pSourceReader->GetNativeMediaType(m_readAudioStreamIndex, 0, &pNativeType);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pNativeType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &audioChannels);
		std::cout << "Audio channels: " << audioChannels << std::endl;

		if (hr != S_OK)
		{
			std::cout << "couldn't read audio channel num from media." << std::endl;
			return 1;
		}

		hr = pNativeType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &audioSamplesPerSecond);
		if (FAILED(hr))
		{
			return hr;
		}
		std::cout << "Audio channels: " << audioSamplesPerSecond << std::endl;


		hr = pNativeType->GetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, &audioBlockAlign);
		if (SUCCEEDED(hr))
		{
			std::cout << "Audio block align: " << audioBlockAlign << std::endl;
		}

		hr = pNativeType->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &audioBitsPerSample);
		if (SUCCEEDED(hr))
		{
			std::cout << "Audio bits per sample: " << audioBitsPerSample << std::endl;
		}


		hr = pNativeType->GetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, &audioAvgBytesPerSecond);
		if (SUCCEEDED(hr))
		{
			std::cout << "Audio avg bytes per second: " << audioAvgBytesPerSecond << std::endl;
		}


		hr = pNativeType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_BLOCK, &audioSamplesPerBlock);
		if (SUCCEEDED(hr))
		{
			std::cout << "Audio samples per block: " << audioSamplesPerBlock << std::endl;
		}


		hr = pNativeType->GetUINT32(MF_MT_AUDIO_VALID_BITS_PER_SAMPLE, &audioValidBitsPerSample);
		if (SUCCEEDED(hr))
		{
			std::cout << "Audio valid bits per sample: " << audioValidBitsPerSample << std::endl;
		}

		SafeRelease(&pNativeType);
	}
	else
	{
		SafeRelease(&m_pSourceReader);
		std::cout << "No audio streams found." << std::endl;
		return 1;
	}

	// Configure the video stream

	pNativeType = nullptr;
	IMFMediaType* pType = nullptr;
	GUID majorType, subtype;

	// Find the native format of the stream.
	hr = m_pSourceReader->GetNativeMediaType(m_readVideoStreamIndex, 0, &pNativeType);
	if (FAILED(hr))
	{
		std::cout << "couldn't get native media type from video stream." << std::endl;
		return 1;
	}

	hr = pNativeType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	hr = MFCreateMediaType(&pType);

	if (majorType != MFMediaType_Video)
	{
		std::cout << "wrong type for video stream." << std::endl;
		return 1;
	}

	hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	const GUID videoSubtype = MFVideoFormat_RGB32;
	// Works on AMD and NVidia with HW Accel, AMD skips second or third frame replacing with duplicate of previous frame
	//const GUID videoSubtype = MFVideoFormat_ARGB32; // Works on NVidia with HW Accel, but not AMD
	//const GUID videoSubtype = MFVideoFormat_NV12; // fastest but has missing first frame on AMD, Dynamic range issues on NV without changing settings, color space issues everywhere
	hr = pType->SetGUID(MF_MT_SUBTYPE, videoSubtype);
	hr = m_pSourceReader->SetCurrentMediaType(m_readVideoStreamIndex, nullptr, pType);

	if (hr != S_OK)
	{
		SafeRelease(&m_pSourceReader);
		switch (hr)
		{
		case MF_E_INVALIDMEDIATYPE:
			std::cout <<
				"At least one decoder was found for the native stream type, but the type specified was rejected." <<
				std::endl;
			return 1;
			break;
		case MF_E_INVALIDREQUEST:
			std::cout << "One or more sample requests are still pending." << std::endl;
			return 1;
			break;
		case MF_E_INVALIDSTREAMNUMBER:
			std::cout << "The stream index parameter is invalid." << std::endl;
			return 1;
			break;
		case MF_E_TOPO_CODEC_NOT_FOUND:
			std::cout << "Could not find a decoder for the native stream type." << std::endl;
			return 1;
			break;
		default:
			std::cout << "Failed to configure stream." << std::endl;
			return 1;
			break;
		}
	}

	SafeRelease(&pNativeType);
	SafeRelease(&pType);

	pNativeType = nullptr;
	pType = nullptr;

	// Find the native format of the stream.
	hr = m_pSourceReader->GetNativeMediaType(m_readAudioStreamIndex, 0, &pNativeType);
	if (FAILED(hr))
	{
		std::cout << "failed to get native media type from source reader." << std::endl;
		return 1;
	}

	hr = pNativeType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	hr = MFCreateMediaType(&pType);
	if (majorType != MFMediaType_Audio)
	{
		std::cout << "wrong type for audio stream." << std::endl;
		return 1;
	}

	hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	hr = pType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	hr = m_pSourceReader->SetCurrentMediaType(m_readAudioStreamIndex, nullptr, pType);

	hr = m_pSourceReader->GetCurrentMediaType(m_readAudioStreamIndex, &pType);

	SafeRelease(&pNativeType);
	SafeRelease(&pType);

	if (hr != S_OK)
	{
		SafeRelease(&m_pSourceReader);
		switch (hr)
		{
		case MF_E_INVALIDMEDIATYPE:
			std::cout <<
				"At least one decoder was found for the native stream type, but the type specified was rejected." <<
				std::endl;
			return 1;
			break;
		case MF_E_INVALIDREQUEST:
			std::cout << "One or more sample requests are still pending." << std::endl;
			return 1;
			break;
		case MF_E_INVALIDSTREAMNUMBER:
			std::cout << "The stream index parameter is invalid." << std::endl;
			return 1;
			break;
		case MF_E_TOPO_CODEC_NOT_FOUND:
			std::cout << "Could not find a decoder for the native stream type." << std::endl;
			return 1;
			break;
		default:
			std::cout << "Failed to configure stream." << std::endl;
			return 1;
			break;
		}
	}

	//--------------------------------
	// CONFIGURE WRITER
	//--------------------------------

	{
		IMFMediaType* pMediaTypeOutV = nullptr;
		IMFMediaType* pMediaTypeOutA = nullptr;
		IMFMediaType* pMediaTypeInV = nullptr;
		IMFMediaType* pMediaTypeInA = nullptr;

		HRESULT hr = S_OK;
		IMFAttributes* pAttr = nullptr;

		// Create attributes
		hr = MFCreateAttributes(&pAttr, 1);
		if (SUCCEEDED(hr))
		{
			// Enable HW accelerated encoding
			hr = pAttr->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, true);
		}

		// Create the SinkWriter
		if (SUCCEEDED(hr))
		{
			std::string output_file = CConvertUtility::ConvertToStdString(output);
			hr = MFCreateSinkWriterFromURL(std::wstring(output_file.begin(), output_file.end()).c_str(), nullptr, pAttr,
			                               &m_pWriter);
		}

		//////////////////////////
		////WRITER - VIDEO OUTPUT
		//////////////////////////

		// Set the video output media type.
		hr = MFCreateMediaType(&pMediaTypeOutV);
		hr = pMediaTypeOutV->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		hr = pMediaTypeOutV->SetGUID(MF_MT_SUBTYPE, VIDEO_ENCODING_FORMAT);
		hr = pMediaTypeOutV->SetUINT32(MF_MT_AVG_BITRATE, VIDEO_BIT_RATE);
		hr = pMediaTypeOutV->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
		hr = MFSetAttributeSize(pMediaTypeOutV, MF_MT_FRAME_SIZE, width, height);
		// frame rate is the inverse of frame duration
		hr = MFSetAttributeRatio(pMediaTypeOutV, MF_MT_FRAME_RATE, frameDurationNum, frameDurationDenom);
		hr = MFSetAttributeRatio(pMediaTypeOutV, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
		hr = pMediaTypeOutV->SetUINT32(MF_MT_VIDEO_NOMINAL_RANGE, MFNominalRange_Normal);
		hr = m_pWriter->AddStream(pMediaTypeOutV, &m_writeVideoStreamIndex);
		if (FAILED(hr))
		{
			std::cout << "Couldn't add output video type to video writer." << std::endl;
			return 1;
		}

		///////////////////////
		////WRITER VIDEO INPUT
		///////////////////////

		hr = MFCreateMediaType(&pMediaTypeInV);
		hr = pMediaTypeInV->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
		hr = pMediaTypeInV->SetGUID(MF_MT_SUBTYPE, VIDEO_INPUT_FORMAT);
		hr = pMediaTypeInV->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
		hr = MFSetAttributeSize(pMediaTypeInV, MF_MT_FRAME_SIZE, width, height);
		// frame rate is the inverse of frame duration
		hr = MFSetAttributeRatio(pMediaTypeInV, MF_MT_FRAME_RATE, frameDurationNum, frameDurationDenom);
		hr = MFSetAttributeRatio(pMediaTypeInV, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
		hr = pMediaTypeInV->SetUINT32(MF_MT_DEFAULT_STRIDE, width * 4);
		hr = m_pWriter->SetInputMediaType(m_writeVideoStreamIndex, pMediaTypeInV, nullptr);
		if (FAILED(hr))
		{
			std::cout << "Couldn't add input video type to video writer." << std::endl;
			return 1;
		}

		//////////////////////////
		////WRITER - AUDIO OUTPUT
		//////////////////////////

		hr = MFCreateMediaType(&pMediaTypeOutA);
		hr = pMediaTypeOutA->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		hr = pMediaTypeOutA->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_AAC);
		hr = pMediaTypeOutA->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, audioChannels);
		hr = pMediaTypeOutA->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, audioSamplesPerSecond);
		hr = pMediaTypeOutA->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
		hr = pMediaTypeOutA->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, 24000);
		hr = m_pWriter->AddStream(pMediaTypeOutA, &m_writeAudioStreamIndex);
		if (FAILED(hr))
		{
			std::cout << "Couldn't add output video type to video writer." << std::endl;
			return 1;
		}
		//////////////////////////
		////WRITER - AUDIO INPUT
		//////////////////////////
		// Calculate derived values.

		UINT32 bitsPerSample = 8 * sizeof(int16_t);
		UINT32 blockAlign = audioChannels * bitsPerSample / 8;
		UINT32 bytesPerSecond = blockAlign * audioSamplesPerSecond;

		hr = MFCreateMediaType(&pMediaTypeInA);
		hr = pMediaTypeInA->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		hr = pMediaTypeInA->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		hr = pMediaTypeInA->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, audioChannels);
		hr = pMediaTypeInA->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, audioSamplesPerSecond);
		hr = pMediaTypeInA->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, blockAlign);
		hr = pMediaTypeInA->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, bytesPerSecond);
		hr = pMediaTypeInA->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, bitsPerSample);
		hr = m_pWriter->SetInputMediaType(m_writeAudioStreamIndex, pMediaTypeInA, nullptr);
		if (FAILED(hr))
		{
			std::cout << "Couldn't add input audio type to video writer." << std::endl;
			return 1;
		}

		SafeRelease(&pMediaTypeInV);
		SafeRelease(&pMediaTypeOutV);
		SafeRelease(&pMediaTypeInA);
		SafeRelease(&pMediaTypeOutA);
		SafeRelease(&pAttr);

		if (FAILED(hr))
		{
			std::cout << "Couldn't initialized video writer." << std::endl;
			return 1;
		}
	}

	//--------------------------------
	// READ MEADIA AND WRITE TO DISK
	//--------------------------------

	//set position to 0
	PROPVARIANT var;
	hr = InitPropVariantFromInt64(0, &var);
	if (SUCCEEDED(hr))
	{
		hr = m_pSourceReader->SetCurrentPosition(GUID_NULL, var);
		PropVariantClear(&var);
	}

	IMFSample* pSample = nullptr;
	DWORD streamIndex, flags;
	LONGLONG sampleTimeStamp;
	LONGLONG videoFrameTimeStamp;
	LONGLONG audioFrameTimeStamp;
	LONGLONG videoOffset{0};
	LONGLONG audioOffset{0};

	//Read the first video frame to get the videoOffset 
	hr = m_pSourceReader->ReadSample(
		m_readVideoStreamIndex, // Stream index.
		0, // Flags.
		&streamIndex, // Receives the actual stream index. 
		&flags, // Receives status flags.
		&videoOffset, // Receives the time stamp.
		&pSample // Receives the sample or NULL.
	);

	SafeRelease(&pSample);

	//set position to 0 again after reading first frame
	hr = InitPropVariantFromInt64(0, &var);
	if (SUCCEEDED(hr))
	{
		hr = m_pSourceReader->SetCurrentPosition(GUID_NULL, var);
		PropVariantClear(&var);
	}

	hr = m_pWriter->BeginWriting();

	size_t frame{0};

	if (time != -1)
	{
		//set position to 0 again after reading first frame
		LONGLONG time_position = time * ONE_SECOND;
		hr = InitPropVariantFromInt64(time_position, &var);
		if (SUCCEEDED(hr))
		{
			hr = m_pSourceReader->SetCurrentPosition(GUID_NULL, var);
			PropVariantClear(&var);
		}
	}

	int nbFrameEncode = 0;

	while (true)
	{
		auto frameStart = std::chrono::system_clock::now();

		hr = m_pSourceReader->ReadSample(
			MF_SOURCE_READER_ANY_STREAM, // Stream index.
			0, // Flags.
			&streamIndex, // Receives the actual stream index. 
			&flags, // Receives status flags.
			&sampleTimeStamp, // Receives the time stamp.
			&pSample // Receives the sample or NULL.
		);

		if (streamIndex != m_readVideoStreamIndex && streamIndex != m_readAudioStreamIndex)
		{
			SafeRelease(&pSample);
			continue;
		}

		if (!(flags & MF_SOURCE_READERF_ENDOFSTREAM))
		{
			if (streamIndex == m_readVideoStreamIndex)
			{
				frame++;
				//OFFSET THE VIDEO STREAM TO PRODUCE A COPY WHO'S STREAM STARTS AT 0
				videoFrameTimeStamp = sampleTimeStamp - videoOffset;
			}
			else if (streamIndex == m_readAudioStreamIndex)
			{
				audioFrameTimeStamp = sampleTimeStamp;
			}
		}

		if (FAILED(hr))
		{
			std::cout << "failed to read sample" << std::endl;
			break;
		}

		bool reachedEnd = (flags & MF_SOURCE_READERF_ENDOFSTREAM);
		if (reachedEnd || pSample == nullptr)
		{
			std::cout << "Reached end of video." << std::endl;
			SafeRelease(&pSample);
			break;
		}

		if (flags & MF_SOURCE_READERF_NEWSTREAM)
		{
			std::cout << L"New stream." << std::endl;
		}
		if (flags & MF_SOURCE_READERF_NATIVEMEDIATYPECHANGED)
		{
			std::cout << L"Native type changed." << std::endl;
		}
		if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
		{
			std::cout << L"Current type changed." << std::endl;
		}
		if (flags & MF_SOURCE_READERF_STREAMTICK)
		{
			std::cout << L"Stream tick." << std::endl;
		}

		if (flags & MF_SOURCE_READERF_NATIVEMEDIATYPECHANGED)
		{
			std::cout << "Stream format changed." << std::endl;
			return 1;
		}

		LONGLONG llDuration;
		hr = pSample->GetSampleDuration(&llDuration);
		if (hr != S_OK)
		{
			std::cout << "Unable to query sample duration" << std::endl;
			return false;
		}

		{
			MFBufferAccess access(pSample);

			std::cout << "Writing " << (streamIndex == m_readAudioStreamIndex ? "Audio" : "Video") << " sample @ " <<
				sampleTimeStamp << " frame: " << (frame) << " duration: " << llDuration / 10000.0 << "ms" << std::endl;
			std::cout << "\tbuffer size: " << access.size << " maxSize: " << access.maxSize << std::endl;

			if (streamIndex == m_readVideoStreamIndex)
			{
				WriteFrame(access.data, m_writeVideoStreamIndex, (videoFrameTimeStamp / ONE_SECOND) * 1000, totalTime,
				           m_dlgProgress, videoCompressOption);
				nbFrameEncode++;
			}
			else if (streamIndex == m_readAudioStreamIndex)
			{
				/*
				// UNCOMMENT THIS BLOCK TO FIX THE AUDIO OFFSET SYNC ISSUE
				// Not sure how many blocks i need or why i need them??
				static bool stored{ false };
				if (!stored)
				{
				    size_t numberOfSilenceBlocks = 2; //how to derive how many a file needs!?  seems aribrary
				    size_t samples = 1024 * numberOfSilenceBlocks;
				    audioOffset = samples * 10000000 / audioSamplesPerSecond;
				    std::vector<uint8_t> silence(samples * (LONGLONG)audioChannels * sizeof(int16_t), 0);
				    WriteAudioBuffer(silence.data(), silence.size(), audioFrameTimeStamp, audioOffset);
				}
				*/

				LONGLONG audioTime = audioFrameTimeStamp + audioOffset;
				WriteAudioBuffer(access.data, access.size, audioTime, llDuration);
			}
			else
			{
				std::cout << "Read unknown sample from stream: " << streamIndex;
			}
		}

		SafeRelease(&pSample);

		if (nbFrameEncode == 30 && time != -1)
			break;
	}

	SafeRelease(&pSample);
	m_pWriter->Finalize();
	SafeRelease(&m_pWriter);

	return 0;
}

#endif
