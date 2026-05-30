#include <header.h>
#include "MusicController.h"
#include <ffplaycore.h>
#include <ConvertUtility.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>

using namespace Regards::Viewer;

CMusicController::CMusicController(wxWindow* eventSink)
    : eventSink(eventSink)
{
}

CMusicController::~CMusicController()
{
    if (ffmfc != nullptr)
        delete ffmfc;
}

void CMusicController::StartMusic()
{
    CRegardsConfigParam* config = CParamInit::getInstance();
    wxString musicDiaporama;

    if (config != nullptr)
        musicDiaporama = config->GetMusicDiaporama();

    if (ffmfc == nullptr && !musicDiaporama.empty() && wxFileExists(musicDiaporama))
        ffmfc = new CFFmfc(eventSink, wxNewId());

    if (!musicDiaporama.empty() && wxFileExists(musicDiaporama) && musicStop)
    {
        ffmfc->SetFile(nullptr, CConvertUtility::ConvertToStdString(musicDiaporama), "", false, 100);
        musicStop = false;
        ffmfc->SetTimePosition(musicPosition);
    }
}

void CMusicController::StopMusic()
{
    if (!musicStop && ffmfc != nullptr)
    {
        musicPosition = ffmfc->GetTimePosition();
        ffmfc->Quit();
        ffmfcQuit = true;
        musicStop  = true;
    }
}

void CMusicController::OnQuitAudio(wxCommandEvent& /*event*/)
{
    musicStop = true;
}

void CMusicController::OnStopAudio(wxCommandEvent& /*event*/)
{
    CRegardsConfigParam* config = CParamInit::getInstance();
    wxString musicDiaporama;
    musicStop = true;

    if (config != nullptr)
        musicDiaporama = config->GetMusicDiaporama();

    if (musicDiaporama.empty() && ffmfc != nullptr && !ffmfcQuit)
    {
        ffmfc->Quit();
        ffmfc->SetFile(nullptr, CConvertUtility::ConvertToStdString(musicDiaporama), "", false, 100);
        musicStop = false;
    }
}
