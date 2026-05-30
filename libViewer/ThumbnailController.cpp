#include <header.h>
#include "ThumbnailController.h"
#include "ThumbnailViewerPicture.h"
#include "ThumbnailViewerVideo.h"
#include "ListPicture.h"
#include "ListFace.h"
#include <ThreadLoadingBitmap.h>
#include <ThumbnailFolder.h>
#include <ThumbnailFace.h>

using namespace Regards::Viewer;

CThumbnailController::CThumbnailController(CThumbnailViewerPicture* thumbnailPicture,
                                           CThumbnailViewerVideo*   thumbnailVideo,
                                           CListPicture*            listPicture,
                                           CListFace*               listFace)
    : thumbnailPicture(thumbnailPicture)
    , thumbnailVideo(thumbnailVideo)
    , listPicture(listPicture)
    , listFace(listFace)
{
}

void CThumbnailController::UpdateThumbnailIcone(wxCommandEvent& event)
{
    auto* threadLoadingBitmap = static_cast<CThreadLoadingBitmap*>(event.GetClientData());
    if (threadLoadingBitmap == nullptr)
        return;

    const long longWindow = threadLoadingBitmap->longWindow;

    auto refreshFolder = [&]()
    {
        if (listPicture != nullptr)
        {
            CThumbnailFolder* ptFolder = listPicture->GetPtThumbnailFolder();
            if (ptFolder->IsShown())
                ptFolder->Refresh();
        }
    };
    auto refreshFace = [&]()
    {
        if (listFace != nullptr)
        {
            CThumbnailFace* ptListFace = listFace->GetThumbnailFace();
            if (ptListFace->IsShown())
                ptListFace->Refresh();
        }
    };
    auto refreshPicture = [&]()
    {
        if (thumbnailPicture != nullptr && thumbnailPicture->IsShown())
            thumbnailPicture->Refresh();
    };
    auto refreshVideo = [&]()
    {
        if (thumbnailVideo != nullptr)
        {
            thumbnailVideo->UpdateVideoThumbnail(threadLoadingBitmap->filename);
            if (thumbnailVideo->IsShown())
                thumbnailVideo->Refresh();
        }
    };

    if (longWindow == 0)
    {
        refreshFolder();
        refreshFace();
        refreshPicture();
        refreshVideo();
    }
    else
    {
        switch (longWindow)
        {
        case THUMBNAILVIDEOWINDOW:
            refreshVideo();
            break;
        case LISTPICTUREID:
            refreshFolder();
            break;
        case LISTFACEID:
            refreshFace();
            break;
        case THUMBNAILVIEWERPICTURE:
            refreshPicture();
            if (thumbnailVideo != nullptr &&
                thumbnailVideo->GetFilename() == threadLoadingBitmap->filename)
                refreshVideo();
            break;
        default:
            break;
        }
    }

    // Type 30: notify the specific window directly
    if (threadLoadingBitmap->type == 30)
    {
        // The owning window must forward this if needed — we cannot call FindWindowById here.
        // CCentralWindow handles the dispatch after calling UpdateThumbnailIcone().
    }

    delete threadLoadingBitmap;
}

void CThumbnailController::UpdateThumbnailIconeSize(wxCommandEvent& /*event*/)
{
    // Panel resize is driven by WindowModeController which owns windowManager.
    // This slot intentionally left for subclass hook or direct windowManager call
    // delegated back from CCentralWindow.
}

void CThumbnailController::OnRefreshThumbnail(wxCommandEvent& /*event*/)
{
    if (thumbnailPicture != nullptr && thumbnailPicture->IsShown())
        thumbnailPicture->Refresh();
}
