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
extern "C"
{
}

CFFmpegTranscoding::CFFmpegTranscoding(const wxString & acceleratorHardware, COpenCLEngine * openCLEngine)
{
	pimpl = new CFFmpegTranscodingPimpl(openCLEngine, acceleratorHardware);
}

CFFmpegTranscoding::~CFFmpegTranscoding()
{
	delete pimpl;
}



void CFFmpegTranscoding::EncodeFileThread(void * data)
{
	CFFmpegTranscoding * ffmpeg_encoding = (CFFmpegTranscoding *)data;
	
	int ret = ffmpeg_encoding->pimpl->EncodeFile(ffmpeg_encoding->input, ffmpeg_encoding->output, ffmpeg_encoding->m_dlgProgress, ffmpeg_encoding->videoCompressOption);
	if (ret < 0)
	{
		wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
		
		char message[255];
		av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, ret);
		wxMessageBox(message, errorConversion, wxICON_ERROR);
		//printf(message);
		//wxString message = av_err2str(ret);
	}
	ffmpeg_encoding->pimpl->ProcessEnd();
	wxCommandEvent event(wxEVENT_ENDCOMPRESSION);
	event.SetInt(ret);
	wxPostEvent(ffmpeg_encoding->mainWindow, event);
	
}

int CFFmpegTranscoding::EndDecodeFile(const int & returnValue)
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

int CFFmpegTranscoding::EncodeFile(wxWindow * mainWindow, const wxString & input, const wxString & output, CVideoOptionCompress * videoCompressOption)
{
	this->mainWindow = mainWindow;
	this->input = input;
	this->output = output;
	this->videoCompressOption = videoCompressOption;
	ListOfEncoder();
	m_dlgProgress = new CompressVideo(nullptr);
	m_dlgProgress->Show();
	encode_thread = new std::thread(EncodeFileThread, this);
	return 0;
}

vector<wxString> CFFmpegTranscoding::ListOfEncoder()
{
	vector<AVCodecID> listCodecId;
	vector<wxString> listOfEncoder;
	const AVCodec *current_codec = nullptr;
	void *i = 0;
	while ((current_codec = av_codec_iterate(&i))) {
		if (av_codec_is_encoder(current_codec)) 
		{
			listCodecId.push_back(current_codec->id);
			listOfEncoder.push_back(current_codec->long_name);
		}
	}
	return listOfEncoder;
}
