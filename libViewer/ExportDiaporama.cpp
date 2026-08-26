#include <header.h>
#include "ExportDiaporama.h"
#include <SqlFindPhotos.h>
#include <LibResource.h>
#include <FileUtility.h>
#include <wx/filename.h>
#include <ThumbnailVideoExport.h>
#include <ConvertUtility.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <CompressionAudioVideoOption.h>
#include <MainParam.h>
#include <MainParamInit.h>
#include <MediaExtractor.h>
using namespace std;
using namespace Regards::Sqlite;
using namespace Regards::Viewer;


// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------
namespace {

    void RemoveIfExists(const wxString& path)
    {
        if (wxFileExists(path))
            wxRemoveFile(path);
    }

} // namespace


// ---------------------------------------------------------------------------
// OnExportDiaporama — Build and export a slideshow video
// ---------------------------------------------------------------------------
void CExportDiaporama::OnExportDiaporama(wxWindow* parent)
{
    // Gather photo list
    vector<wxString> list;
    {
        CSqlFindPhotos sqlFindPhotos;
        sqlFindPhotos.SearchPhotos(&list);
    }

    CRegardsConfigParam* config = CParamInit::getInstance();
    auto* viewerParam = CMainParamInit::getInstance();

    const int timeDelai = viewerParam->GetDelaiDiaporamaOption();
    int numEffect = config ? config->GetDiaporamaTransitionEffect() : 0;

    // Effect IDs below 400 are in the "slide" range; map them into the
    // transition range expected by the generator.
    if (numEffect < 400)
        numEffect += 400;

    // Prompt for output path
    const wxString saveLabel = CLibResource::LoadStringFromResource(L"LBLSAVEVIDEOFILE", 1);
    const wxString filenameLabel = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);

    wxFileDialog saveFileDialog(nullptr, saveLabel, "", filenameLabel,
        "mp4 " + filenameLabel + " (*.mp4)|*.mp4",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    saveFileDialog.SetDirectory(CFileUtility::GetDocumentFolderPath());

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString filepath = saveFileDialog.GetPath();
    if (wxFileName(filepath).GetExt() != "mp4")
        filepath += ".mp4";

    // Generate the silent slideshow video
    wxString m_tempVideoFile = CFileUtility::GetTempFile("thumbnail.mp4");
    wxString m_tempAudioVideoFile = CFileUtility::GetTempFile("thumbnail_audio.mp4");

    const int time_movie = CThumbnailVideoExport::GenerateVideoFromList(
        m_tempVideoFile, list, timeDelai, 30, 1920, 1080, numEffect);

    if (time_movie == 0)
    {
        wxMessageBox("Unable to generate diaporama movie!",
            CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1),
            wxICON_ERROR);
        RemoveIfExists(m_tempVideoFile);
        return;
    }

    RemoveIfExists(filepath);

    // Optionally mix in background music
    const wxString musicPath = config ? config->GetMusicDiaporama() : wxString{};

    if (!musicPath.IsEmpty() && wxFileExists(musicPath))
    {
        const wxString ext = wxFileName(musicPath).GetExt();
        const wxString movieTimeStr = CConvertUtility::GetTimeLibelle(time_movie);
        const wxString tempAudio = CFileUtility::GetTempFile("audio." + ext, true);

        bool result = Regards::Media::CreateLoopedAudio(musicPath.utf8_string(), tempAudio.utf8_string(), movieTimeStr.utf8_string());
        if (result)
        {
            result = Regards::Media::ExecuteFFmpegMuxVideoAudio(m_tempVideoFile.utf8_string(), tempAudio.utf8_string(), m_tempAudioVideoFile.utf8_string());
        }
        
        RemoveIfExists(tempAudio);
        RemoveIfExists(m_tempVideoFile);       // FIX: was never deleted in original
        wxRenameFile(m_tempAudioVideoFile, filepath);
    }
    else
    {
        RemoveIfExists(m_tempAudioVideoFile);
        wxRenameFile(m_tempVideoFile, filepath);
    }

    wxMessageBox("Diaporama movie created",
        CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1),
        wxICON_INFORMATION);
}

