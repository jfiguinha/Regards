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
	// Make sure the worker thread is not still touching member state (or the
	// progress dialog) when the frame gets destroyed. This can block until the
	// in-flight encode finishes; there is no cancellation flag yet.
	if (m_encodeThread.joinable())
		m_encodeThread.join();

	RemoveIfExists(fileOut);
	RemoveIfExists(fileOutAudio);
	RemoveIfExists(fileOutVideo);

	exit(0);
}

void CVideoConverterFrame::EncodeFile(CVideoOptionCompress* videoCompressOption, const wxString& input, const wxString& output, int rotation, std::function<void(int)> onComplete)
{
	// If a previous encode thread is still lingering (shouldn't normally happen
	// since callers only start a new one from the completion callback), don't
	// leak/orphan it.
	if (m_encodeThread.joinable())
		m_encodeThread.join();

	m_dlgProgress = std::make_unique<CompressVideo>(nullptr, rotation);
	m_dlgProgress->SetFocus();  // focus on my window
	m_dlgProgress->Raise();  // bring window to front
	m_dlgProgress->Show();

	CompressVideo* progressDlg = m_dlgProgress.get();

	m_encodeThread = std::thread([this, videoCompressOption, input, output, progressDlg, onComplete]()
		{
			std::unique_ptr<COpenCLContext> openCLContext = std::make_unique<COpenCLContext>();
			openCLContext->CreateDefaultOpenCLContext();

			CFFmpegTranscoding ffmpegtranscoding(openCLContext.get());

			int ret = ffmpegtranscoding.EncodeFile(input, output, progressDlg, videoCompressOption);

			if (ret < 0)
			{
				wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
				wxMessageBox(FormatFFmpegError(ret), errorConversion, wxICON_ERROR);
			}

			if (m_dlgProgress)
				m_dlgProgress->Close();

			if (ret == 0)
			{
				if (m_dlgProgress->IsOk())
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
			}

			bool result = (ret == 0);

			if (needToRemux)
			{
				RemoveIfExists(fileOutputPath);

				if (isAudio && wxFileExists(fileOut) && wxFileExists(fileOutAudio))
					result = Regards::Media::ExecuteFFmpegMuxVideoAudio(fileOut.utf8_string(), fileOutAudio.utf8_string(), fileOutputPath.utf8_string());
				else if (wxFileExists(fileOut) && wxFileExists(fileOutVideo))
					result = Regards::Media::ExecuteFFmpegMuxVideoAudio(fileOutVideo.utf8_string(), fileOut.utf8_string(), fileOutputPath.utf8_string());
				else
					result = false;

				// Cleanup - built fresh every call (was previously a `static`
				// array initialized only once from stale member values).
				const wxString filesToClean[] = { fileOutVideo, fileOutAudio, fileOut };
				for (const auto& filepath : filesToClean)
					RemoveIfExists(filepath);
			}
			else
			{
				RemoveIfExists(fileOut);
			}
		});
}

void CVideoConverterFrame::ExportVideo(const wxString& fileIn)
{
	CMediaInfo metadata;
	CLibPicture libPicture;
	fileOut = "";
	wxString filename = fileIn;

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

	if ((videoCompressOption->audioDirectCopy && videoCompressOption->videoDirectCopy) ||
		(!videoCompressOption->audioDirectCopy && !videoCompressOption->videoDirectCopy))
	{
		bool result = Regards::Media::ExecuteFFmpegCutVideo(filename.utf8_string(), timeInput.utf8_string(), timeOutput.utf8_string(), fileOut.utf8_string());

		if (!result)
		{
			reportExtractionFailure();
			return;
		}

		if (videoCompressOption->audioDirectCopy && videoCompressOption->videoDirectCopy)
		{
			// Pure remux, no encoding needed: stays synchronous.
			RemoveIfExists(fileOutputPath);
			wxCopyFile(fileOut, fileOutputPath);
			RemoveIfExists(fileOut);
			needToRemux = false;
			ExitApplication();
			return;
		}
		else
		{
			RemoveIfExists(fileOutputPath);
			needToRemux = false;
			EncodeFile(videoCompressOption, fileOut, fileOutputPath, rotation, onEncodeComplete);
			return;
		}
	}
	else
	{
		if (videoCompressOption->audioDirectCopy)
		{
			fileOutVideo = CFileUtility::GetTempFile("temp_video." + file_temp.GetExt(), true);
			bool result = Regards::Media::ExecuteFFmpegExtractVideo(filename.utf8_string(), timeInput.utf8_string(), timeOutput.utf8_string(), fileOutVideo.utf8_string());

			if (!result || !wxFileExists(fileOutVideo))
			{
				reportExtractionFailure();
				return;
			}

			isAudio = true;
			needToRemux = true;
			EncodeFile(videoCompressOption, fileOutVideo, fileOut, rotation, onEncodeComplete);
			return;
		}
		else if (videoCompressOption->videoDirectCopy)
		{
			fileOutAudio = CFileUtility::GetTempFile("temp_audio." + file_temp.GetExt(), true);
			bool result = Regards::Media::ExecuteFFmpegExtractAudio(filename.utf8_string(), timeInput.utf8_string(), timeOutput.utf8_string(), fileOutAudio.utf8_string());

			if (!result || !wxFileExists(fileOutAudio))
			{
				reportExtractionFailure();
				return;
			}

			isAudio = false;
			needToRemux = true;
			EncodeFile(videoCompressOption, fileOutAudio, fileOut, rotation, onEncodeComplete);
			return;
		}
	}

	// Neither branch matched (shouldn't happen given the if/else above, but
	// keep a safe fallback instead of silently doing nothing).
	ExitApplication();
}
