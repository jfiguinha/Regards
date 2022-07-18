#include "header.h"
#include "ffmpeg_transcoding.h"
#include "FFmpegTranscodingPimpl.h"
#include <CompressVideo.h>
#include "ffmpegToBitmap.h"
#include "VideoCompressOption.h"
#include <wx/progdlg.h>
#include <wx/filename.h>
#include <window_id.h>
#include <LibResource.h>
#include "FFmpegTranscodingPimpl.h"

CFFmpegTranscoding::CFFmpegTranscoding() :
	encode_thread(nullptr),
	m_dlgProgress(nullptr),
	mainWindow(nullptr),
	videoCompressOption(nullptr)
{
}

CFFmpegTranscoding::~CFFmpegTranscoding()
{

}

wxString CFFmpegTranscoding::GetOutputFilename()
{

	return output;
}

int CFFmpegTranscoding::EncodeFrame(const wxString& input, const wxString& output, const int& position, CVideoOptionCompress* videoCompressOption)
{
	CFFmpegTranscodingPimpl ffmpegtranscoding;
	int ret =  ffmpegtranscoding.EncodeOneFrame(nullptr, input, output, position, videoCompressOption);
	if (ffmpegtranscoding.GetFrameOutput() != nullptr)
	{
		data = ffmpegtranscoding.GetFrameOutput()->GetMatrix();
	}
	return 0;
}

void CFFmpegTranscoding::GetFrameOutput(CRegardsBitmap * & bitmap)
{
	if (bitmap != nullptr)
	{
		bitmap->SetMatrix(data);
	}
}

void CFFmpegTranscoding::EncodeFileThread(void* data)
{
	auto ffmpeg_encoding = static_cast<CFFmpegTranscoding*>(data);

	CFFmpegTranscodingPimpl ffmpegtranscoding;

	int ret = ffmpegtranscoding.EncodeFile(ffmpeg_encoding->input, ffmpeg_encoding->output,
	                                             ffmpeg_encoding->m_dlgProgress, ffmpeg_encoding->videoCompressOption);
	if (ret < 0)
	{
		wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);

		char message[255];
		av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, ret);
		wxMessageBox(message, errorConversion, wxICON_ERROR);
	}

	wxCommandEvent event(wxEVENT_ENDCOMPRESSION);
	event.SetInt(ret);
	wxPostEvent(ffmpeg_encoding->mainWindow, event);
}

int CFFmpegTranscoding::EndDecodeFile(const int& returnValue)
{
	m_dlgProgress->Close();
	encode_thread->join();

	wxSleep(1);

	delete encode_thread;

	delete m_dlgProgress;

	delete videoCompressOption;

	if (returnValue == 0)
	{
		wxString filecompleted = CLibResource::LoadStringFromResource("LBLFILEENCODINGCOMPLETED", 1);
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox(filecompleted, infos);
	}
	return 0;
}

int CFFmpegTranscoding::EncodeFile(wxWindow* mainWindow, const wxString& input, const wxString& output,
                                   CVideoOptionCompress* videoCompressOption, int rotation)
{
	this->mainWindow = mainWindow;
	this->input = input;
	this->output = output;
	this->videoCompressOption = videoCompressOption;

	m_dlgProgress = new CompressVideo(nullptr, rotation);
	m_dlgProgress->Show();
	encode_thread = new std::thread(EncodeFileThread, this);
	return 0;
}
