#include <header.h>
#include "ExportDiaporama.h"
#include "ffmpeg_application.h"
#include <SqlFindPhotos.h>
#include <LibResource.h>
#include <FileUtility.h>
#include <wx/filename.h>
#include <ThumbnailVideoExport.h>
#include <ConvertUtility.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <ffmpeg_transcoding.h>
#include <window_id.h>
#include <MediaInfo.h>
#include <CompressionAudioVideoOption.h>
#include <MainParam.h>
#include <MainParamInit.h>
#include <VideoCompressOption.h>
extern "C"
{
#include <libavutil/error.h>
}
using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Viewer;

CExportDiaporama::CExportDiaporama(wxWindow* parent)
{
	this->parent = parent;
	tempVideoFile = "";
	tempAudioVideoFile = "";
	fileOut = "";
	filepathVideo = "";
	fileOutVideo = "";
	fileOutAudio = "";
	isAudio = false;
	needToRemux = false;
	ffmpegEncoder = nullptr;
}

void CExportDiaporama::OnExportDiaporama()
{
	wxString filepath = "";
	int numEffect = 0;
	CSqlFindPhotos sqlFindPhotos;
	vector<wxString> list;
	sqlFindPhotos.SearchPhotos(&list);

	tempVideoFile = CFileUtility::GetTempFile("thumbnail.mp4");
	tempAudioVideoFile = CFileUtility::GetTempFile("thumbnail_audio.mp4");

	CRegardsConfigParam* config = CParamInit::getInstance();

	auto viewerParam = CMainParamInit::getInstance();
	int timeDelai = viewerParam->GetDelaiDiaporamaOption();
	if (config != nullptr)
		numEffect = config->GetDiaporamaTransitionEffect();

	if (numEffect < 400)
		numEffect += 400;

	{
		wxString savevideofile = CLibResource::LoadStringFromResource(L"LBLSAVEVIDEOFILE", 1);
		wxString filename = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);

		wxFileDialog saveFileDialog(nullptr, savevideofile, "", filename,
			"mp4 " + filename + " (*.mp4)|*.mp4", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return; // the user changed idea...

		filepath = saveFileDialog.GetPath();
		wxFileName file_path(filepath);
		wxString extension = file_path.GetExt();
		if (extension != "mp4")
		{
			filepath.append(".mp4");
		}
	}

	int time_movie = CThumbnailVideoExport::GenerateVideoFromList(tempVideoFile, list, timeDelai, 30, 1920, 1080,
		numEffect);

	if (time_movie == 0)
	{
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox("Unable to generate diaporama movie !", infos, wxICON_ERROR);
	}
	else
	{
		if (wxFileExists(filepath))
		{
#ifdef WIN32
			std::remove(filepath);
#else
			wxRemoveFile(filepath);
#endif
		}

		wxString musicDiaporama = config->GetMusicDiaporama();

		if (!musicDiaporama.IsEmpty() && wxFileExists(musicDiaporama))
		{
			wxFileName file_path(musicDiaporama);
			wxString extension = file_path.GetExt();
			wxString movieTimeString = CConvertUtility::GetTimeLibelle(time_movie);
			wxString tempAudio = CFileUtility::GetTempFile("audio." + extension);
			{
				CFFmpegApp fmpegApp;
				try
				{
					fmpegApp.CropAudio(musicDiaporama, movieTimeString, extension, tempAudio);
				}
				catch (int e)
				{

				}
			}
			{
				CFFmpegApp fmpegApp;
				try
				{
					fmpegApp.ExecuteFFmpegApp(tempVideoFile, tempAudio, movieTimeString, tempAudioVideoFile);
				}
				catch (int e)
				{

				}
			}

			if (wxFileExists(tempAudio))
			{
#ifdef WIN32
				std::remove(tempAudio);
#else
				wxRemoveFile(tempAudio);
#endif
			}

			rename(tempAudioVideoFile, filepath);
		}
		else
			rename(tempVideoFile, filepath);

		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox("Diaporama movie create", infos, wxICON_INFORMATION);
	}
}


void CExportDiaporama::ExportVideo(const wxString& filename)
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

		wxWindow* videoWindow = parent->FindWindowById(SHOWBITMAPVIEWERID);
		if (videoWindow != nullptr)
		{
			wxCommandEvent event(wxEVENT_PAUSEMOVIE);
			wxPostEvent(videoWindow, event);
		}

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

	auto compressAudioVideoOption = new CompressionAudioVideoOption(parent);

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
							{
#ifdef WIN32
								std::remove(filepath);
#else
								wxRemoveFile(filepath);
#endif
							}
							wxCopyFile(filename_in, filepath);
						}
						else if (!videoCompressOption->audioDirectCopy && !videoCompressOption->videoDirectCopy)
						{
							if (wxFileExists(filepath))
							{
#ifdef WIN32
								std::remove(filepath);
#else
								wxRemoveFile(filepath);
#endif
							}
							wxString decoder = "";
							ffmpegEncoder = new CFFmpegTranscoding();
							ffmpegEncoder->EncodeFile(parent, filename_in, filepath, videoCompressOption, rotation);
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
							ffmpegEncoder->EncodeFile(parent, fileOutVideo, fileOut, videoCompressOption, rotation);
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
							ffmpegEncoder->EncodeFile(parent, fileOutAudio, fileOut, videoCompressOption, rotation);
							isAudio = false;
						}
						else
							ret = -1;
					}
				}
			}
		}
	}
	else if (!filepath.empty())
	{
		wxCopyFile(filename, filepath);
		wxCommandEvent event(wxEVENT_ENDCOMPRESSION);
		event.SetInt(0);
		wxPostEvent(parent, event);
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

void CExportDiaporama::OnEndDecompressFile(int ret)
{
	wxString outputFile = "";

	if (ffmpegEncoder != nullptr)
	{
		ffmpegEncoder->EndDecodeFile(ret);

		outputFile = ffmpegEncoder->GetOutputFilename();

		delete ffmpegEncoder;
		ffmpegEncoder = nullptr;
	}
	if (wxFileExists(tempAudioVideoFile))
	{
#ifdef WIN32
		std::remove(tempAudioVideoFile);
#else
		wxRemoveFile(tempAudioVideoFile);
#endif
	}

	if (wxFileExists(tempVideoFile))
	{
#ifdef WIN32
		std::remove(tempVideoFile);
#else
		wxRemoveFile(tempVideoFile);
#endif
	}

	if (needToRemux)
	{
		if (wxFileExists(filepathVideo))
		{
#ifdef WIN32
			std::remove(filepathVideo);
#else
			wxRemoveFile(filepathVideo);
#endif
		}

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

				}
			}
		}

		//Cleanup
		if (wxFileExists(fileOutVideo))
		{
#ifdef WIN32
			std::remove(fileOutVideo);
#else
			wxRemoveFile(fileOutVideo);
#endif
		}


		if (wxFileExists(fileOutAudio))
		{
#ifdef WIN32
			std::remove(fileOutAudio);
#else
			wxRemoveFile(fileOutAudio);
#endif
		}


		if (wxFileExists(fileOut))
		{
#ifdef WIN32
			std::remove(fileOut);
#else
			wxRemoveFile(fileOut);
#endif
		}

	}
	else
	{
		if (wxFileExists(fileOut))
		{
#ifdef WIN32
			std::remove(fileOut);
#else
			wxRemoveFile(fileOut);
#endif
		}

	}

	cv::ocl::OpenCLExecutionContext::getCurrent().bind();
}