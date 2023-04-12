#pragma once
#include <MainInterface.h>
class CFFmpegTranscoding;
// Define a new frame type: this is going to be our main frame
class CConvertVideoFrame : public wxFrame
{
public:
	// ctor(s)
	CConvertVideoFrame(const wxString &title, IMainInterface * mainInterface, const wxPoint &pos, const wxSize &size,
		long style = wxDEFAULT_FRAME_STYLE);

    ~CConvertVideoFrame();
	void ExportVideo(const wxString& filename);

private:
	void OnEndDecompressFile(wxCommandEvent& event);

	//int m_imageCount;
	IMainInterface * mainInterface;
	CFFmpegTranscoding* ffmpegEncoder = nullptr;
	wxString tempVideoFile = "";
	wxString tempAudioVideoFile = "";
	wxString fileOut = "";
	wxString fileOutAudio = "";
	wxString fileOutVideo = "";
	wxString filepathVideo = "";
	wxString firstFileToShow = "";
	bool needToRemux = false;
	bool isAudio = false;
	bool init = false;
	wxString oldRequest = "";
	DECLARE_EVENT_TABLE()
};
