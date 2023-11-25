#pragma once


#ifdef WIN32
#include <Windows.h>
#include <stdint.h>

#ifdef DLLFFMPEG_EXPORTS
#define DLLFFMPEG_API __declspec(dllexport)
#else
#define DLLFFMPEG_API __declspec(dllimport)
#endif

#else

#include <stdint.h>
#include <stdlib.h>
#define DLLFFMPEG_API extern "C"

#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <string>
#include <vector>

using namespace std;

class DLLFFMPEG_API CFFmpegApp
{
public:
	CFFmpegApp(const bool showProgressWindow = true)
	{
		this->showProgressWindow = showProgressWindow;
	};

	~CFFmpegApp()
	{
	};

	static int ProgressBarFunction(int x, void* progressWnd);
	static void ExitFunction(int x);

	void Cleanup(int x);
	int TestFFmpeg(const wxString& commandline);
	int CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension,
		const wxString& outputFile);
	int ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile, const wxString& timeVideo,
		const wxString& outputFile);
	int ExecuteFFmpegCutVideo(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut,
		const wxString& outputFile);
	int ExecuteFFmpegAddRotateInfo(const wxString& inputVideoFile, const wxString& outputFile, const int& rotate);
	int ExecuteFFmpegExtractVideo(const wxString& inputVideoFile, const wxString& timeVideoIn,
		const wxString& timeVideoOut, const wxString& outputFile);
	int ExecuteFFmpegExtractAudio(const wxString& inputVideoFile, const wxString& timeVideoIn,
		const wxString& timeVideoOut, const wxString& outputFile);
	int ExecuteFFmpegMuxVideoAudio(const wxString& inputVideoFile, const wxString& inputAudioFile,
		const wxString& outputFile);

private:
	void ExecuteFFmpeg();
	bool showProgressWindow = true;
	int ret = 0;
	//int i = 0;
	char** arrayOfCstrings;
	std::vector<std::string> arrayOfStrings;
};


#ifdef __cplusplus
}
#endif

