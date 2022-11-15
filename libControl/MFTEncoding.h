#pragma once
#ifdef WIN32
#include "VideoCompressOption.h"
/*
#include <map>
#include <wx/mstream.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <OpenCLEffectVideo.h>
#include <OpenCLParameter.h>
#include <VideoStabilization.h>

class CompressVideo;
*/

class CMFTEncodingPimp;
class CompressVideo;
class CVideoOptionCompress;
class CRegardsBitmap;

class CMFTEncoding
{
public:


	CMFTEncoding();
	~CMFTEncoding();
	cv::Mat GetFrameOutput();
	int EncodeOneFrame(CompressVideo* m_dlgProgress, const wxString& input, const wxString& output, const long& time, CVideoOptionCompress* videoCompressOption);
	int EncodeFile(const wxString& input, const wxString& output, CompressVideo* m_dlgProgress, CVideoOptionCompress* videoCompressOption);


private:
	CMFTEncodingPimp* pimpl = nullptr;
	
};

#endif