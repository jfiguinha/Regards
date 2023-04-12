#include <header.h>
#include <ffmpeg_transcoding.h>
#include "ConvertVideoFrame.h"
#include <MediaInfo.h>
#include <ffmpeg_application.h>
#include <ConvertUtility.h>
#include <wx/dir.h>
#include <wx/filename.h>
#ifndef wxHAS_IMAGES_IN_RESOURCES
#ifdef __WXGTK__
#include "../Resource/sample.xpm"
#else
#include "../../Resource/sample.xpm"
#endif
#endif



// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
CConvertVideoFrame::CConvertVideoFrame(const wxString& title, IMainInterface* mainInterface, const wxPoint& pos,
                             const wxSize& size,
                             long style) :
	wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{

}

CConvertVideoFrame::~CConvertVideoFrame()
{

}


void CConvertVideoFrame::OnEndDecompressFile(wxCommandEvent& event)
{
	wxString outputFile = "";
	int ret = event.GetInt();
	if (ffmpegEncoder != nullptr)
	{
		ffmpegEncoder->EndDecodeFile(ret);

		outputFile = ffmpegEncoder->GetOutputFilename();

		delete ffmpegEncoder;
		ffmpegEncoder = nullptr;
	}
	if (wxFileExists(tempAudioVideoFile))
		wxRemoveFile(tempAudioVideoFile);

	if (wxFileExists(tempVideoFile))
		wxRemoveFile(tempVideoFile);

	if (needToRemux)
	{
		if (wxFileExists(filepathVideo))
			wxRemoveFile(filepathVideo);

		if (isAudio)
		{
			if (wxFileExists(fileOut) && wxFileExists(fileOutAudio))
			{
				CFFmpegApp fmpegApp(false);
				try
				{
					fmpegApp.ExecuteFFmpegMuxVideoAudio(fileOut, fileOutAudio, filepathVideo);
				}
				catch (int e)
				{
					fmpegApp.Cleanup(e);
				}
			}
		}
		else
		{
			if (wxFileExists(fileOut) && wxFileExists(fileOutVideo))
			{
				CFFmpegApp fmpegApp(false);
				try
				{
					fmpegApp.ExecuteFFmpegMuxVideoAudio(fileOutVideo, fileOut, filepathVideo);
				}
				catch (int e)
				{
					fmpegApp.Cleanup(e);
				}
			}
		}

		//Cleanup
		if (wxFileExists(fileOutVideo))
			wxRemoveFile(fileOutVideo);

		if (wxFileExists(fileOutAudio))
			wxRemoveFile(fileOutAudio);

		if (wxFileExists(fileOut))
			wxRemoveFile(fileOut);
	}
	else
	{
		if (wxFileExists(fileOut))
			wxRemoveFile(fileOut);
	}
}


void CConvertVideoFrame::ExportVideo(const wxString& filename)
{
	if (!wxFileExists(filename))
		return;

	////CFFmpegApp ffmpeg;
	//ffmpeg.TestFFmpeg("");

	CMediaInfo metadata;
	int rotation = metadata.GetVideoRotation(filename);

	int ret = 0;
	//CVideoControlSoft* videoControlSoft = (CVideoControlSoft*)this->FindWindowById(VIDEOCONTROL);

	wxString filepath = "";
	if (filepath == "")
	{
		wxFileName videoFilename(filename);
		wxString savevideofile = CLibResource::LoadStringFromResource(L"LBLSAVEVIDEOFILE", 1);
		wxString filename_label = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);


		wxString filenameToSave = videoFilename.GetName();


		wxFileDialog saveFileDialog(nullptr, savevideofile, "", filenameToSave,
			"mp4 " + filename_label + " (*.mp4)|*.mp4|webm " + filename_label +
			" (*.webm)|*.webm|mov " + filename_label + " (*.mov)|*.mov|mkv " + filename_label +
			" (*.mkv)|*.mkv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return; // the user changed idea...
		filepath = saveFileDialog.GetPath();
		int index = saveFileDialog.GetFilterIndex();

		wxFileName file_path(filepath);
		wxString extension = file_path.GetExt();

		if (extension != "mp4" && extension != "webm" && extension != "mov" && extension != "mkv")
		{
			switch (index)
			{
			case 0:
				filepath += ".mp4";
				break;
			case 1:
				filepath += ".webm";
				break;
			case 2:
				filepath += ".mov";
				break;
			case 3:
				filepath += ".mkv";
				break;
			default:;
			}
		}
	}

	auto compressAudioVideoOption = new CompressionAudioVideoOption(this);

	compressAudioVideoOption->SetFile(filename, filepath);



	compressAudioVideoOption->ShowModal();
	wxString filename_in = filename;
	if (compressAudioVideoOption->IsOk())
	{
		if (ffmpegEncoder == nullptr)
		{
			auto videoCompressOption = new CVideoOptionCompress();
			compressAudioVideoOption->GetCompressionOption(videoCompressOption);

			if ((videoCompressOption->audioDirectCopy && videoCompressOption->videoDirectCopy) || (!videoCompressOption
				->audioDirectCopy && !videoCompressOption->videoDirectCopy))
			{
				needToRemux = false;

				if (videoCompressOption->startTime != 0 || videoCompressOption->endTime != 0)
				{
					if (wxFileExists(filename))
					{
						CFFmpegApp fmpegApp(false);
						try
						{
							wxFileName file_temp(filepath);
							fileOut = CFileUtility::GetTempFile("temp." + file_temp.GetExt(), file_temp.GetPath(),
								true);

							wxString timeInput = CConvertUtility::GetTimeLibelle(videoCompressOption->startTime);
							wxString timeOutput = CConvertUtility::GetTimeLibelle(videoCompressOption->endTime);
							fmpegApp.ExecuteFFmpegCutVideo(filename, timeInput, timeOutput, fileOut);

							filename_in = fileOut;
						}
						catch (int e)
						{
							fmpegApp.Cleanup(e);
							filename_in = fileOut;
							ret = -1;
						}
					}
					else
						ret = -1;
				}

				if (ret == 0)
				{
					if (wxFileExists(filename_in))
					{
						if (videoCompressOption->audioDirectCopy && videoCompressOption->videoDirectCopy)
						{
							if (wxFileExists(filepath))
								wxRemoveFile(filepath);
							wxCopyFile(filename_in, filepath);
						}
						else if (!videoCompressOption->audioDirectCopy && !videoCompressOption->videoDirectCopy)
						{
							if (wxFileExists(filepath))
								wxRemoveFile(filepath);
							wxString decoder = "";
							ffmpegEncoder = new CFFmpegTranscoding();
							ffmpegEncoder->EncodeFile(this, filename_in, filepath, videoCompressOption, rotation);
						}
					}
					else
						ret = -1;
				}
			}
			else
			{
				needToRemux = true;

				wxFileName file_temp(filepath);

				filepathVideo = filepath;

				fileOut = CFileUtility::GetTempFile("temp." + file_temp.GetExt(), file_temp.GetPath(), true);

				fileOutVideo = "";
				fileOutAudio = "";

				wxString timeInput = CConvertUtility::GetTimeLibelle(videoCompressOption->startTime);
				wxString timeOutput = CConvertUtility::GetTimeLibelle(videoCompressOption->endTime);

				if (wxFileExists(filename_in))
				{
					{
						CFFmpegApp fmpegApp(false);
						try
						{
							wxFileName file_temp(filepath);
							fileOutAudio = CFileUtility::GetTempFile("temp_audio." + file_temp.GetExt(),
								file_temp.GetPath(), true);
							fmpegApp.ExecuteFFmpegExtractAudio(filename_in, timeInput, timeOutput, fileOutAudio);
						}
						catch (int e)
						{
							fmpegApp.Cleanup(e);
						}
					}

					{
						CFFmpegApp fmpegApp(false);
						try
						{
							wxFileName file_temp(filepath);
							fileOutVideo = CFileUtility::GetTempFile("temp_video." + file_temp.GetExt(),
								file_temp.GetPath(), true);
							fmpegApp.ExecuteFFmpegExtractVideo(filename_in, timeInput, timeOutput, fileOutVideo);
						}
						catch (int e)
						{
							fmpegApp.Cleanup(e);
						}
					}
				}
				else
					ret = -1;

				if (ret == 0)
				{
					if (videoCompressOption->audioDirectCopy)
					{
						if (wxFileExists(fileOutVideo))
						{
							wxString decoder = "";
							ffmpegEncoder = new CFFmpegTranscoding();
							ffmpegEncoder->EncodeFile(this, fileOutVideo, fileOut, videoCompressOption, rotation);
							isAudio = true;
						}
						else
							ret = -1;
					}
					else
					{
						if (wxFileExists(fileOutAudio))
						{
							wxString decoder = "";
							ffmpegEncoder = new CFFmpegTranscoding();
							ffmpegEncoder->EncodeFile(this, fileOutAudio, fileOut, videoCompressOption, rotation);
							isAudio = false;
						}
						else
							ret = -1;
					}
				}
			}
		}
	}
	else if (!filenameOutput.empty())
	{
		wxCopyFile(filename, filepath);
		wxCommandEvent event(wxEVENT_ENDCOMPRESSION);
		event.SetInt(0);
		wxPostEvent(this, event);
	}


	if (ret < 0)
	{
		wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
		char message[255];
		av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, ret);
		wxMessageBox(message, errorConversion, wxICON_ERROR);
	}

	delete compressAudioVideoOption;
}
