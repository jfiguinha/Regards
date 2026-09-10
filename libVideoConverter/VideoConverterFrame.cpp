#include <header.h>
#include "VideoConverterFrame.h"
#include <CompressionAudioVideoOption.h>
#include <VideoCompressOption.h>
#include <wx/filename.h>
#include <MediaInfo.h>
#include <MediaExtractor.h>
#include <ConvertUtility.h>
#include <FileUtility.h>
#include <LibResource.h>
#include <libPicture.h>
#include <ConfigRegards.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include "FFmpegTranscoding.h"
#include <SliderVideoSelection.h>
#include <AudioEncoder.h>
#include <wx/progdlg.h>
#include <wx/evtloop.h>
#include <wx/dir.h>
using namespace Regards::Picture;

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------
namespace {

	void RemoveIfExists(const wxString& path)
	{
		if (wxFileExists(path))
			wxRemoveFile(path);
	}

	wxString FormatFFmpegError(int errnum)
	{
		char message[255];
		av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, errnum);
		return wxString(message);
	}

} // namespace

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
CVideoConverterFrame::CVideoConverterFrame(IVideoConverterInterface* videoInterface)
{
	this->videoInterface = videoInterface;

	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
		regardsParam->SetInterpolationType(1);

}

CVideoConverterFrame::~CVideoConverterFrame()
{
	// Make sure the worker thread is not still touching member state (or the
	// progress dialog) when the frame gets destroyed. This can block until the
	// in-flight encode finishes; there is no cancellation flag yet.
	if (m_encodeThread.joinable())
		m_encodeThread.join();

	RemoveIfExists(fileOut);
	RemoveIfExists(fileOutAudio);
	RemoveIfExists(fileOutVideo);
}


wxString CVideoConverterFrame::SelectFile()
{
	wxFileDialog openFileDialog(nullptr, _("Open video file"), "", "",
		"mp4 files (*.mp4)|*.mp4", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	wxString documentPath = CFileUtility::GetDocumentFolderPath();
	openFileDialog.SetDirectory(documentPath);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return "";


	return openFileDialog.GetPath();
}

wxString CVideoConverterFrame::SelectOutputFile(wxString& filename)
{
	wxString filepath;
	wxFileName videoFilename(filename);
	wxString savevideofile = CLibResource::LoadStringFromResource(L"LBLSAVEVIDEOFILE", 1);
	wxString filename_label = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);


	wxString filenameToSave = videoFilename.GetName();


	wxFileDialog saveFileDialog(nullptr, savevideofile, "", filenameToSave,
		"mp4 " + filename_label + " (*.mp4)|*.mp4|webm " + filename_label +
		" (*.webm)|*.webm|mov " + filename_label + " (*.mov)|*.mov|mkv " + filename_label +
		" (*.mkv)|*.mkv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	wxString documentPath = CFileUtility::GetDocumentFolderPath();
	saveFileDialog.SetDirectory(documentPath);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return ""; // the user changed idea...
	}

	filepath = saveFileDialog.GetPath();
	int index = saveFileDialog.GetFilterIndex();

	const wxString ext = wxFileName(filepath).GetExt();
	if (ext != "mp4" && ext != "webm" && ext != "mov" && ext != "mkv")
	{
		static const wxString kExts[] = { "mp4", "webm", "mov", "mkv" };
		filepath += "." + kExts[std::min(index, 3)];
	}
	return filepath;
}

void CVideoConverterFrame::ExitApplication()
{
	wxString dirPath = CFileUtility::GetDocumentFolderPathWithFilename("temp");
	wxArrayString files;

	// Grab all files inside the folder (false = do not look inside subfolders)
	wxDir::GetAllFiles(dirPath, &files, wxEmptyString, wxDIR_FILES);

	for (size_t i = 0; i < files.GetCount(); ++i) {
		wxRemoveFile(files[i]);
	}


	videoInterface->Close();

	exit(0);
}

int CVideoConverterFrame::EncodeAudioSample(CVideoOptionCompress* videoCompressOption, const wxString& input, const wxString& output) {
	AudioEncoder encoder;
	AudioEncoderOptions options;

	if (videoCompressOption->audioCodec == "AAC")
		options.codec = AudioCodec::AAC;
	else if (videoCompressOption->audioCodec == "MP3")
		options.codec = AudioCodec::MP3;
	else
		options.codec = AudioCodec::VORBIS;

	if (videoCompressOption->audioBitRate > 0) {
		options.mode = EncodingMode::Bitrate;
		options.bitrateKbps = videoCompressOption->audioBitRate;
	}
	else {
		options.mode = EncodingMode::Quality;
		options.quality = videoCompressOption->audioQuality;
	}

	std::string strInput = CConvertUtility::ConvertToStdString(input);
	std::string strOutput = CConvertUtility::ConvertToStdString(output);

	std::atomic<bool> isFinished(false);
	std::atomic<bool> cancelRequested(false);
	std::atomic<int> progressPercent(0);
	std::atomic<int> currentSeconds(0);
	std::atomic<int> totalSeconds(0);
	int result = 0;

	// Dialogue modale
	wxProgressDialog* audioProgressDlg = new wxProgressDialog(
		"Encoding Audio",
		"Starting encoding...",
		100,
		nullptr,
		wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME
	);

	std::thread workerThread([&]() {
		result = encoder.EncodeAudioOnly(strInput, strOutput, options,
			[&](double curSec, double totSec) -> bool {
				if (cancelRequested.load()) return false;

				currentSeconds.store(static_cast<int>(curSec));
				totalSeconds.store(static_cast<int>(totSec));

				if (totSec > 0.0) {
					int percent = static_cast<int>((curSec / totSec) * 100.0);
					progressPercent.store(percent > 100 ? 100 : percent);
				}
				return true;
			}
		);
		isFinished.store(true);
		});

	// Boucle événementielle locale
	while (!isFinished.load())
	{
		if (wxEventLoopBase::GetActive()) {
			wxEventLoopBase::GetActive()->DispatchTimeout(30);
			if (wxTheApp) {
				wxTheApp->ProcessPendingEvents();
			}
		}
		else {
			wxMilliSleep(30);
		}

		if (audioProgressDlg) {
			wxString msg = wxString::Format("Processing: %d / %d seconds", currentSeconds.load(), totalSeconds.load());
			if (!audioProgressDlg->Update(progressPercent.load(), msg)) {
				cancelRequested.store(true);
			}
		}
	}

	if (workerThread.joinable()) {
		workerThread.join();
	}

	// =========================================================================
	//  CORRECTION ABSOLUE POUR FERMER ET EFFACER LE DIALOGUE MODAL
	// =========================================================================
	if (audioProgressDlg) {
		// 1. Forcer l'atteinte des 100 % (indispensable sous wxWidgets pour casser le verrou modal)
		audioProgressDlg->Update(100, "Encoding completed!");

		// 2. Dissocier de l'affichage natif
		audioProgressDlg->Hide();

		// 3. Libérer la mémoire et fermer la fenêtre
		audioProgressDlg->Destroy();
		audioProgressDlg = nullptr;
	}

	// 4. Vider une ultime fois la pile d'événements OS pour purger le "fantôme" graphique de l'écran
	if (wxTheApp) {
		wxTheApp->ProcessPendingEvents();
	}

	// Permet de forcer l'OS à redessiner immédiatement ce qui se trouvait sous la jauge
	wxYield();

	return result;
}


void CVideoConverterFrame::EncodeFile(CVideoOptionCompress* videoCompressOption, const wxString& input, const wxString& output, int rotation, std::function<void(int)> onComplete)
{
	// 1. S'assurer qu'aucun ancien thread ne tourne encore
	if (m_encodeThread.joinable())
		m_encodeThread.join();

	// 2. Initialisation et affichage de la fenêtre de progression (sur le thread principal)
	m_dlgProgress = std::make_unique<CompressVideo>(nullptr, rotation);
	m_dlgProgress->SetFocus();
	m_dlgProgress->Raise();
	m_dlgProgress->Show();

	CompressVideo* progressDlg = m_dlgProgress.get();

	// 3. Lancement du thread en tâche de fond
	m_encodeThread = std::thread([this, videoCompressOption, input, output, progressDlg, onComplete]()
		{
			auto openCLContext = std::make_unique<COpenCLContext>();
			openCLContext->CreateDefaultOpenCLContext();

			CFFmpegTranscoding ffmpegtranscoding(openCLContext.get());

			// Le traitement lourd de la vidéo s'exécute ici en arrière-plan
			int ret = ffmpegtranscoding.EncodeFile(input, output, progressDlg, videoCompressOption);

			// 4. Utilisation exclusive de CallAfter pour renvoyer les actions graphiques sur le thread UI
			wxTheApp->CallAfter([this, ret, onComplete]()
				{
					if (ret < 0)
					{
						wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
						wxMessageBox(FormatFFmpegError(ret), errorConversion, wxICON_ERROR);
					}

					// Fermeture sécurisée de la boîte de progression vidéo
					bool wasProgressOk = false;
					if (m_dlgProgress) {
						wasProgressOk = m_dlgProgress->IsOk();
						m_dlgProgress->Close();
					}

					// Exécution du multiplexage final (Muxing Audio + Vidéo)
					//RemoveIfExists(fileOutputPath);
					bool muxResult = Regards::Media::ExecuteFFmpegMuxVideoAudio(
						fileOutVideo.utf8_string(),
						fileOutAudio.utf8_string(),
						fileOutputPath.utf8_string()
					);

					// Nettoyage des fichiers temporaires intermédiaires
					//const wxString filesToClean[] = { fileOutVideo, fileOutAudio };
					//for (const auto& filepath : filesToClean)
					//	RemoveIfExists(filepath);

					// Notification de fin à l'utilisateur
					wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
					if (wasProgressOk)
					{
						wxString filecompleted = CLibResource::LoadStringFromResource("LBLFILEENCODINGCOMPLETED", 1);
						wxMessageBox(filecompleted, infos);
					}
					else
					{
						wxMessageBox("File encoding has been interrupted", infos);
					}
					

					// Appel du callback de complétio

					// 5. C'est uniquement ICI, quand TOUT est fini, qu'on ferme proprement l'application
					ExitApplication();

					/*
					if (onComplete) {
						onComplete(ret);
					}*/
				});
		});

	
}


void CVideoConverterFrame::ExportVideo(const wxString& fileIn)
{
	CMediaInfo metadata;
	CLibPicture libPicture;
	fileOut = "";
	filename = fileIn;

	

	wxString dirPath = CFileUtility::GetDocumentFolderPathWithFilename("temp");
	wxArrayString files;

	// Grab all files inside the folder (false = do not look inside subfolders)
	wxDir::GetAllFiles(dirPath, &files, wxEmptyString, wxDIR_FILES);

	for (size_t i = 0; i < files.GetCount(); ++i) {
		wxRemoveFile(files[i]);
	}


	if (!wxFileExists(filename))
		filename = SelectFile();

	if (filename.empty() || !libPicture.TestIsVideo(filename))
	{
		ExitApplication();
		return;
	}

	int rotation = metadata.GetVideoRotation(filename);
	fileOutputPath = SelectOutputFile(filename);
	if (fileOutputPath.empty())
	{
		ExitApplication();
		return;
	}

	m_compressAudioVideoOption = std::make_unique<CompressionAudioVideoOption>();
	m_compressAudioVideoOption->SetFile(filename, fileOutputPath);
	m_compressAudioVideoOption->ShowModal();

	if (!m_compressAudioVideoOption->IsOk())
	{
		ExitApplication();
		return;
	}

	auto videoCompressOption = m_compressAudioVideoOption->GetVideoCompressionPt();

	wxFileName file_temp(fileOutputPath);
	fileOut = CFileUtility::GetTempFile("temp." + file_temp.GetExt(), true);
	fileOut_cut = CFileUtility::GetTempFile("temp_cut." + file_temp.GetExt(), true);

	wxString timeInput = "00:00:00";
	wxString timeOutput = "00:00:00";
	if (videoCompressOption->startTime != 0 || videoCompressOption->endTime != 0)
	{
		timeInput = CConvertUtility::GetTimeLibelle(videoCompressOption->startTime);
		timeOutput = CConvertUtility::GetTimeLibelle(videoCompressOption->endTime);
	}

	// Called once the (possibly async) encode step has finished, whatever
	// path got us there. Mirrors the error-reporting that used to sit at the
	// bottom of the old synchronous ExportVideo.
	auto onEncodeComplete = [this](int ret)
		{
			if (ret != 0 && !m_compressAudioVideoOption->IsCancel())
			{
				wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
				wxMessageBox(FormatFFmpegError(ret), errorConversion, wxICON_ERROR);
			}
			ExitApplication();
		};

	auto reportExtractionFailure = [this]()
		{
			if (!m_compressAudioVideoOption->IsCancel())
			{
				wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
				wxMessageBox(errorConversion, errorConversion, wxICON_ERROR);
			}
			ExitApplication();
		};


	bool result = true;

	if (timeInput == "00:00:00" && timeOutput == "00:00:00")
		fileOut_cut = filename;
	else
		result = Regards::Media::ExecuteFFmpegCutVideo(filename.utf8_string(), timeInput.utf8_string(), timeOutput.utf8_string(), fileOut_cut.utf8_string());

	if (!result)
	{
		reportExtractionFailure();
		return;
	}


	if (videoCompressOption->audioDirectCopy && videoCompressOption->videoDirectCopy)
	{

		// Pure remux, no encoding needed: stays synchronous.
		RemoveIfExists(fileOutputPath);
		wxCopyFile(fileOut_cut, fileOutputPath);
		if (fileOut_cut != filename)
		{
			RemoveIfExists(fileOut_cut);
		}
		needToRemux = false;

		wxString filecompleted = CLibResource::LoadStringFromResource("LBLFILEENCODINGCOMPLETED", 1);
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox(filecompleted, infos);

		ExitApplication();
		return;
	}

	
	fileOutVideo = CFileUtility::GetTempFile("temp_video." + file_temp.GetExt(), true);
	fileOutAudio = CFileUtility::GetTempFile("temp_audio." + file_temp.GetExt(), true);
	RemoveIfExists(fileOutAudio);
	RemoveIfExists(fileOutVideo);

	if (videoCompressOption->audioDirectCopy)
	{
		//Compress Video and Copy Audio
		bool result = Regards::Media::ExecuteFFmpegExtractAudio(fileOut_cut.utf8_string(), fileOutAudio.utf8_string());
		if (result == false || !wxFileExists(fileOutAudio))
		{
			reportExtractionFailure();
			return;
		}

		isAudio = true;
		needToRemux = true;
		EncodeFile(videoCompressOption, fileOut_cut.utf8_string(), fileOutVideo, rotation, onEncodeComplete);
		return;
	}

	if (videoCompressOption->videoDirectCopy)
	{
		bool result = Regards::Media::ExecuteFFmpegExtractVideo(fileOut_cut.utf8_string(), fileOutVideo.utf8_string());

		if (!result || !wxFileExists(fileOutVideo))
		{
			reportExtractionFailure();
			return;
		}

		EncodeAudioSample(videoCompressOption, fileOut_cut.utf8_string(), fileOutAudio);

		if (wxFileExists(fileOutVideo) && wxFileExists(fileOutAudio))
			result = Regards::Media::ExecuteFFmpegMuxVideoAudio(fileOutVideo.utf8_string(), fileOutAudio.utf8_string(), fileOutputPath.utf8_string());

		if (result)
		{
			wxString filecompleted = CLibResource::LoadStringFromResource("LBLFILEENCODINGCOMPLETED", 1);
			wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
			wxMessageBox(filecompleted, infos);
		}
		else
		{
			wxString filecompleted = "File encoding has been interrupted";
			wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
			wxMessageBox(filecompleted, infos);
		}

		// Neither branch matched (shouldn't happen given the if/else above, but
		// keep a safe fallback instead of silently doing nothing).
		ExitApplication();
		return;
	}


	wxString fileOutAudio_encode = CFileUtility::GetTempFile("temp_audio_enc." + file_temp.GetExt(), true);
	wxString fileOutVideo_encode = CFileUtility::GetTempFile("temp_video_enc." + file_temp.GetExt(), true);

	RemoveIfExists(fileOutVideo_encode);
	RemoveIfExists(fileOutAudio_encode);
	//Other Case
	result = Regards::Media::ExecuteFFmpegExtractAudio(fileOut_cut.utf8_string(), fileOutAudio_encode.utf8_string());
	if (result == false || !wxFileExists(fileOutAudio_encode))
	{
		reportExtractionFailure();
		return;
	}
	result = Regards::Media::ExecuteFFmpegExtractVideo(fileOut_cut.utf8_string(), fileOutVideo_encode.utf8_string());
	if (result == false || !wxFileExists(fileOutVideo_encode))
	{
		reportExtractionFailure();
		return;
	}

	EncodeAudioSample(videoCompressOption, fileOutAudio_encode.utf8_string(), fileOutAudio);
	isAudio = true;
	needToRemux = true;
	EncodeFile(videoCompressOption, fileOutVideo_encode.utf8_string(), fileOutVideo, rotation, onEncodeComplete);

	return;

}
