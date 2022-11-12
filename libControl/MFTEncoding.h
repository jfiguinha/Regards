#pragma once
#ifdef WIN32
#include "VideoCompressOption.h"
#include <map>
#include <wx/mstream.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <OpenCLEffectVideo.h>
#include <OpenCLParameter.h>
#include <RegardsBitmap.h>
#include <VideoStabilization.h>

namespace Regards
{
	namespace Video
	{
		class CVideoPlayer;
	}
}


using namespace Regards::OpenCV;
class CRegardsBitmap;
class CompressVideo;
class CFiltreEffet;
class CffmpegToBitmap;

namespace cv
{
	class VideoCapture;
}

class CMFTEncoding
{
public:


	CRegardsBitmap* GetFrameOutput();

	CMFTEncoding();
	~CMFTEncoding();

	int EncodeOneFrame(CompressVideo* m_dlgProgress, const wxString& input, const wxString& output, const long& time, CVideoOptionCompress* videoCompressOption);
	int EncodeFile(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress, CVideoOptionCompress* videoCompressOption);


private:
	int Encode(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress, CVideoOptionCompress* videoCompressOption, long time =  -1);
	HRESULT WriteFrame(BYTE* buffer, DWORD streamIndex, LONGLONG timestamp, LONGLONG duration, CompressVideo* m_dlgProgress, CVideoOptionCompress* videoCompressOption);
	HRESULT WriteAudioBuffer(BYTE* buffer, size_t bufferSize, LONGLONG timestamp, LONGLONG duration);
	Regards::OpenCV::COpenCVStabilization* openCVStabilization = nullptr;
};

#endif