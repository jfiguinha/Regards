#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>
#include <stdexcept>
#include "ffmpeg_application.h"

extern "C" {
#include "ffmpeg.h"
}

/*
std::string convertSecondsToHHMMSS(int value)
{
	std::string result;
	// compute h, m, s
	std::string h = std::to_string(value / 3600);
	std::string m = std::to_string((value % 3600) / 60);
	std::string s = std::to_string(value % 60);
	// add leading zero if needed
	std::string hh = std::string(2 - h.length(), '0') + h;
	std::string mm = std::string(2 - m.length(), '0') + m;
	std::string ss = std::string(2 - s.length(), '0') + s;
	// return mm:ss if hh is 00
	if (hh.compare("00") != 0)
	{
		result = hh + ':' + mm + ":" + ss;
	}
	else
	{
		result = mm + ":" + ss;
	}
	return result;
}
*/
void CFFmpegApp::ExitFunction(int x)
{
	throw x;
}

int CFFmpegApp::ProgressBarFunction(int x, void* progressWnd)
{
	if (progressWnd != nullptr)
	{
		auto dialog = static_cast<wxProgressDialog*>(progressWnd);
		if (false == dialog->Update(x, "Video Progress ..."))
			return 1;
	}

	return 0;
}

void CFFmpegApp::ExecuteFFmpeg()
{
	arrayOfCstrings = new char*[arrayOfStrings.size()];

	for (int i = 0; i < arrayOfStrings.size(); ++i)
		arrayOfCstrings[i] = (char*)arrayOfStrings[i].c_str();


	wxProgressDialog* dialog = nullptr;

	if (showProgressWindow)
	{
		dialog = new wxProgressDialog("FFmpeg Process", "Checking...", 100, nullptr,
			wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
	}

	try
	{
		void (*foo)(int);
		foo = &ExitFunction;

		int (*progress)(int, void*);
		progress = &ProgressBarFunction;

		ret = ExecuteFFMpegProgram(arrayOfStrings.size(), arrayOfCstrings, foo, progress, dialog);
	}
	catch (int e)
	{
		ret = e;
	}
	catch (...)
	{
	}

	if (dialog != nullptr)
		delete dialog;

	Cleanup(ret);
}

int CFFmpegApp::TestFFmpeg(const wxString& commandline)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back("d:\\video\\IMG_1293.MOV");
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("vp9");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("libvorbis");
	arrayOfStrings.push_back("d:\\video\\output.mkv");

	ExecuteFFmpeg();

	return 0;
}

void CFFmpegApp::Cleanup(int ret)
{
	ffmpeg_cleanup(ret);
	delete[] arrayOfCstrings;
}


int CFFmpegApp::CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension,
                          const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-stream_loop");
	arrayOfStrings.push_back("-1");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back(extension.ToStdString());
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile,
                                 const wxString& timeVideo, const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("0:v:0");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("1:a:0");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegCutVideo(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back(timeVideoIn.ToStdString());
	arrayOfStrings.push_back("-to");
	arrayOfStrings.push_back(timeVideoOut.ToStdString());
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractVideo(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn.ToStdString());
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut.ToStdString());
	}
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-an");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractAudio(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn.ToStdString());
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut.ToStdString());
	}
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}



int CFFmpegApp::ExecuteFFmpegMuxVideoAudio(const wxString& inputVideoFile, const wxString& inputAudioFile, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

/*
int CFFmpegApp::ExecuteFFmpegAddRotateInfo(const wxString& inputVideoFile, const wxString& outputFile, const int& rotate)
{
	//ffmpeg -i 20200509_132328.mp4 -map_metadata 0 -metadata:s:v rotate=-90 -codec copy output.mp4
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-map_metadata");
	arrayOfStrings.push_back("0");
	arrayOfStrings.push_back("-metadata:s:v");
	arrayOfStrings.push_back("rotate=" + std::to_string(rotate));
	arrayOfStrings.push_back("-codec");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();
	return 0;
}
*/