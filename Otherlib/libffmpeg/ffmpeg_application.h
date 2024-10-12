#pragma once
#include <string>
#include <vector>
#include <wx/filefn.h>
using namespace std;

class CFFmpegApp
{
public:
	CFFmpegApp(const bool showProgressWindow = true);

	~CFFmpegApp()
	{
	};


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


	static std::vector<wxString> GetHardwareList();

private:
    static int StartApp(void * arg);
	void ExecuteFFmpeg();
	bool showProgressWindow = true;
	int ret = 0;
	std::vector<wxString> arrayOfStrings;
};
