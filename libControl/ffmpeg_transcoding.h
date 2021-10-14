#pragma once
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;
class CFFmpegTranscodingPimpl;
class CompressVideo;
class CVideoOptionCompress;
class CRegardsBitmap;

class CFFmpegTranscoding
{
public:
	CFFmpegTranscoding(const wxString& acceleratorHardware, COpenCLContext* openclContext);
	~CFFmpegTranscoding();
	int EncodeFile(wxWindow* mainWindow, const wxString& input, const wxString& output,
	               CVideoOptionCompress* videoCompressOption);
	int EndDecodeFile(const int& returnValue);
	vector<wxString> ListOfEncoder();
protected:
	static void EncodeFileThread(void* data);
	wxString input;
	wxString output;
	std::thread* encode_thread;
	CFFmpegTranscodingPimpl* pimpl;
	std::mutex muEnding;
	bool ending = false;
	CompressVideo* m_dlgProgress;
	wxWindow* mainWindow;
	CVideoOptionCompress* videoCompressOption;
};
