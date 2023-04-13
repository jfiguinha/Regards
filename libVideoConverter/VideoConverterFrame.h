#pragma once
#include <MainInterface.h>
class CFFmpegTranscoding;

// Define a new frame type: this is going to be our main frame
class CVideoConverterFrame : public wxFrame
{
public:
	// ctor(s)
	CVideoConverterFrame(const wxString &title, const wxPoint &pos, const wxSize &size, IVideoConverterInterface * videoInterface,	wxString fileToOpen, long style = wxDEFAULT_FRAME_STYLE);
    ~CVideoConverterFrame();



private:
	void OnEndDecompressFile(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
	void ExportVideo(wxString filename);
	IVideoConverterInterface* videoInterface;

	//CompressionAudioVideoOption* compressAudioVideoOption = nullptr;
	CFFmpegTranscoding* ffmpegEncoder = nullptr;
	wxString fileOut = "";
	wxString fileOutAudio = "";
	wxString fileOutVideo = "";
	wxString filepathVideo = "";
	wxString firstFileToShow = "";
	bool needToRemux = false;
	bool isAudio = false;
	bool init = false;

	wxString tempVideoFile = "";
	wxString tempAudioVideoFile = "";
	DECLARE_EVENT_TABLE()
};
