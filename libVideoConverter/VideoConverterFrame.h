#pragma once
#include <MainInterface.h>
#include <OpenCLContext.h>
#include <CompressVideo.h>
#include <functional>
#include <thread>
#include <wx/progdlg.h>
class CFFmpegTranscoding;
class CVideoOptionCompress;
class CompressionAudioVideoOption;

// Define a new frame type: this is going to be our main frame
class CVideoConverterFrame
{
public:
	// ctor(s)
	CVideoConverterFrame(IVideoConverterInterface* videoInterface);
	~CVideoConverterFrame();
	void ExportVideo(const wxString& filename);


private:

	// Permet de gérer la progression audio asynchrone de manière thread-safe
	std::thread m_audioThread;
	std::atomic<bool> m_audioCancelRequested{ false };
	wxProgressDialog* m_dlgAudioProgress = nullptr;

	int EncodeAudioSample(CVideoOptionCompress* videoCompressOption, const wxString& input, const wxString& output);
	void OnCloseWindow(wxCloseEvent& event);
	wxString SelectOutputFile(wxString& filename);
	void ExitApplication();
	wxString SelectFile();

	// Launches the FFmpeg encode on a worker thread. onComplete is invoked on the
	// UI thread (via CallAfter) once encoding, remuxing and cleanup are done.
	// onComplete receives 0 on success, or a negative ffmpeg error code on failure.
	void EncodeFile(CVideoOptionCompress* videoCompressOption, const wxString& input, const wxString& output, int rotation, std::function<void(int)> onComplete);

	IVideoConverterInterface* videoInterface;

	wxString fileOut = "";
	wxString fileOutAudio = "";
	wxString fileOutVideo = "";
	wxString fileOutputPath = "";

	wxString fileOut_cut = "";
	wxString fileOutAudio_encode = "";
	wxString fileOutVideo_encode = "";
	wxString filename = "";
	bool needToRemux = false;
	bool isAudio = false;
	bool init = false;

	// Kept alive as members: their lifetime must now span the async encode,
	// not just the synchronous call that used to create them.
	std::unique_ptr<CompressVideo> m_dlgProgress;
	std::unique_ptr<CompressionAudioVideoOption> m_compressAudioVideoOption;
	std::thread m_encodeThread;



	DECLARE_EVENT_TABLE()
};