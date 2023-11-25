#include "header.h"
#include "ffmpeg_transcoding.h"
#include "FFmpegTranscodingPimpl.h"
#include <CompressVideo.h>
#include "ffmpegToBitmap.h"
#include "VideoCompressOption.h"
#include <window_id.h>
#include <LibResource.h>
#ifdef WIN32_MFT
#include "MFTEncoding.h"
#endif


CFFmpegTranscoding::CFFmpegTranscoding() :
	encode_thread(nullptr),
	m_dlgProgress(nullptr),
	mainWindow(nullptr),
	videoCompressOption(nullptr)
{
}

CFFmpegTranscoding::~CFFmpegTranscoding()
{
}

wxString CFFmpegTranscoding::GetOutputFilename()
{
	return output;
}

int CFFmpegTranscoding::EncodeFrame(const wxString& input, const wxString& output, const int& position,
                                    CVideoOptionCompress* videoCompressOption)
{
#ifdef WIN32_MFT
	CMFTEncoding mftEncoding;
	int ret = mftEncoding.EncodeOneFrame(nullptr, input, output, position, videoCompressOption);
	if (!mftEncoding.GetFrameOutput().empty())
	{
		data = mftEncoding.GetFrameOutput();
	}
#else
	CFFmpegTranscodingPimpl ffmpegtranscoding;
	int ret = ffmpegtranscoding.EncodeOneFrame(nullptr, input, output, position, videoCompressOption);
	if (!ffmpegtranscoding.GetFrameOutput().empty())
	{
		data = ffmpegtranscoding.GetFrameOutput();
		data_withouteffect = ffmpegtranscoding.GetFrameOutputWithOutEffect();
	}
#endif
	return 0;
}

cv::Mat CFFmpegTranscoding::GetFrameOutput()
{
	cv::Mat bitmap;
#ifdef WIN32_MFT
	data.copyTo(bitmap);
	//cv::flip(bitmap, bitmap, 0);
#else
	data.copyTo(bitmap);
#endif
	return bitmap;
}

cv::Mat CFFmpegTranscoding::GetFrameOutputWithOutEffect()
{
	cv::Mat bitmap;
#ifdef WIN32_MFT
	data_withouteffect.copyTo(bitmap);
	//cv::flip(bitmap, bitmap, 0);
#else
	data_withouteffect.copyTo(bitmap);
#endif
	return bitmap;
}

void CFFmpegTranscoding::EncodeFileThread(void* data)
{
	auto ffmpeg_encoding = static_cast<CFFmpegTranscoding*>(data);

#ifdef WIN32_MFT
	CMFTEncoding mftEncoding;
	int ret = mftEncoding.EncodeFile(ffmpeg_encoding->input, ffmpeg_encoding->output,
		ffmpeg_encoding->m_dlgProgress, ffmpeg_encoding->videoCompressOption);
	if (ret < 0)
	{
		wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
		char message[255];
		av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, ret);
		wxMessageBox(message, errorConversion, wxICON_ERROR);
	}
#else

	CFFmpegTranscodingPimpl ffmpegtranscoding;

	int ret = ffmpegtranscoding.EncodeFile(ffmpeg_encoding->input, ffmpeg_encoding->output,
	                                       ffmpeg_encoding->m_dlgProgress, ffmpeg_encoding->videoCompressOption);
	if (ret < 0)
	{
		wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);

		char message[255];
		av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, ret);
		wxMessageBox(message, errorConversion, wxICON_ERROR);
	}
#endif
	wxCommandEvent event(wxEVENT_ENDCOMPRESSION);
	event.SetInt(ret);
	wxPostEvent(ffmpeg_encoding->mainWindow, event);
}

int CFFmpegTranscoding::EndDecodeFile(const int& returnValue)
{
	m_dlgProgress->Close();
	encode_thread->join();

	wxSleep(1);

	delete encode_thread;

	delete m_dlgProgress;

	delete videoCompressOption;

	if (returnValue == 0)
	{
		wxString filecompleted = CLibResource::LoadStringFromResource("LBLFILEENCODINGCOMPLETED", 1);
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox(filecompleted, infos);
	}
	return 0;
}

int CFFmpegTranscoding::EncodeFile(wxWindow* mainWindow, const wxString& input, const wxString& output,
                                   CVideoOptionCompress* videoCompressOption, int rotation)
{
	this->mainWindow = mainWindow;
	this->input = input;
	this->output = output;
	this->videoCompressOption = videoCompressOption;

	m_dlgProgress = new CompressVideo(nullptr, rotation);
	m_dlgProgress->Show();
	encode_thread = new std::thread(EncodeFileThread, this);
	return 0;
}
