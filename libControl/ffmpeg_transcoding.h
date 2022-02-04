#pragma once
#include <OpenCLContext.h>
using namespace Regards::OpenCL;
class CompressVideo;
class CVideoOptionCompress;
class CRegardsBitmap;

class CFFmpegTranscoding
{
public:
	CFFmpegTranscoding();
	~CFFmpegTranscoding();
	int EncodeFile(wxWindow* mainWindow, const wxString& input, const wxString& output, CVideoOptionCompress* videoCompressOption, int rotation);
	int EncodeFrame(const wxString& input, const wxString& output, const int& position, CVideoOptionCompress* videoCompressOption);
	int EndDecodeFile(const int& returnValue);
	wxString GetOutputFilename();
protected:
	static void EncodeFileThread(void* data);
	wxString input;
	wxString output;
	std::thread* encode_thread;
	CompressVideo* m_dlgProgress;
	wxWindow* mainWindow;
	CVideoOptionCompress* videoCompressOption;
};