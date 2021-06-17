#pragma once
#include <string>
using namespace std;

class CFFmpegApp
{
public:
	CFFmpegApp() {};
	~CFFmpegApp() {};
	static int CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension, const wxString& outputFile);
	static int ExecuteFFmpegApp(const wxString& commandLine);
	static int ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString &inputAudioFile, const wxString &timeVideo, const wxString &outputFile);
};
