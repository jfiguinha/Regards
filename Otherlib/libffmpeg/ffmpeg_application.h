#pragma once
#include <string>
#include <vector>
using namespace std;

class CFFmpegApp
{
public:
	CFFmpegApp() {};
	~CFFmpegApp() {};
	
	static void ProgressBarFunction(int x, void* progressWnd);
	static void ExitFunction(int x);

	void Cleanup(int x);
	int TestFFmpeg(const wxString& commandline);
	int CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension, const wxString& outputFile);
	int ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString &inputAudioFile, const wxString &timeVideo, const wxString &outputFile);

private:

	void ExecuteFFmpeg();

	int ret = 0;
	int i = 0;
	char** arrayOfCstrings;
	std::vector<std::string> arrayOfStrings;
};
