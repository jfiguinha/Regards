#include <header.h>
#include "ViewerController.h"
#include "MediaLoader.h"
#include "ThumbnailViewerPicture.h"
#include "ListPicture.h"
#include "ListFace.h"
#include "PreviewWnd.h"
#include <SqlPhotos.h>
#include <ShowElement.h>
#include "window_mode_id.h"
#include "PanelInfosWnd.h"
#include <ImageLoadingFormat.h>
using namespace Regards::Viewer;

// ─────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────

CViewerController::CViewerController(wxWindow*                parent,
                                     CThumbnailViewerPicture* thumbnailPicture,
                                     CListPicture*            listPicture,
                                     CListFace*               listFace,
                                     CPreviewWnd*             previewWindow,
                                     CMediaLoader*            mediaLoader)
    : parent(parent)
    , thumbnailPicture(thumbnailPicture)
    , listPicture(listPicture)
    , listFace(listFace)
    , previewWindow(previewWindow)
    , mediaLoader(mediaLoader)
    , windowMode(mediaLoader->windowMode)
    , isDiaporama(mediaLoader->isDiaporama)
    , isPicture(mediaLoader->isPicture)
    , isAnimation(mediaLoader->isAnimation)
    , isVideo(mediaLoader->isVideo)
    , processLoadPicture(mediaLoader->processLoadPicture)
{
}

// ─────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────

int CViewerController::GetPhotoId(const wxString& filename) const
{
    CSqlPhotos photo;
    return photo.GetPhotoId(filename);
}

int CViewerController::GetNbElement() const
{
    if (thumbnailPicture != nullptr)
        return thumbnailPicture->GetNbElement();
    return 0;
}

// ─────────────────────────────────────────────
// Navigation
// ─────────────────────────────────────────────

wxString CViewerController::ImageSuivante(const bool& loadPicture)
{
    wxString localFilename;
    if (!windowMode)
        return localFilename;

    int numItem = 0;
    if (windowMode == WINDOW_EXPLORER && listPicture != nullptr)
    {
        numItem = listPicture->ImageSuivante();
        localFilename = listPicture->GetFilename(numItem);
    }
    else if ((windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE) && thumbnailPicture != nullptr)
    {
        numItem = thumbnailPicture->ImageSuivante();
        localFilename = thumbnailPicture->GetFilename(numItem);
    }
    else if (windowMode == WINDOW_FACE && listFace != nullptr)
    {
        numItem = listFace->ImageSuivante();
        localFilename = listFace->GetFilename(numItem);
    }

    if (!localFilename.empty() && loadPicture)
    {
        isNext = true;
        mediaLoader->LoadPicture(localFilename);
    }
    return localFilename;
}

wxString CViewerController::ImagePrecedente(const bool& loadPicture)
{
    wxString localFilename;
    if (!windowMode)
        return localFilename;

    int numItem = 0;
    if (windowMode == WINDOW_EXPLORER && listPicture != nullptr)
    {
        numItem = listPicture->ImagePrecedente();
        localFilename = listPicture->GetFilename(numItem);
    }
    else if ((windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE) && thumbnailPicture != nullptr)
    {
        numItem = thumbnailPicture->ImagePrecedente();
        localFilename = thumbnailPicture->GetFilename(numItem);
    }
    else if (windowMode == WINDOW_FACE && listFace != nullptr)
    {
        numItem = listFace->ImagePrecedente();
        localFilename = listFace->GetFilename(numItem);
    }

    if (!localFilename.empty() && loadPicture)
    {
        isNext = false;
        mediaLoader->LoadPicture(localFilename);
    }
    return localFilename;
}

wxString CViewerController::ImageFin(const bool& loadPicture)
{
    wxString localFilename;
    if (!windowMode)
        return localFilename;

    int numItem = 0;
    if (windowMode == WINDOW_EXPLORER && listPicture != nullptr)
    {
        numItem = listPicture->ImageFin();
        localFilename = listPicture->GetFilename(numItem);
    }
    else if ((windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE) && thumbnailPicture != nullptr)
    {
        numItem = thumbnailPicture->ImageFin();
        localFilename = thumbnailPicture->GetFilename(numItem);
    }

    if (!localFilename.empty() && loadPicture)
        mediaLoader->LoadPicture(localFilename);

    return localFilename;
}

wxString CViewerController::ImageDebut(const bool& loadPicture)
{
    wxString localFilename;
    if (!windowMode)
        return localFilename;

    int numItem = 0;
    if (windowMode == WINDOW_EXPLORER && listPicture != nullptr)
    {
        numItem = listPicture->ImageDebut();
        localFilename = listPicture->GetFilename(numItem);
    }
    else if ((windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE) && thumbnailPicture != nullptr)
    {
        numItem = thumbnailPicture->ImageDebut();
        localFilename = thumbnailPicture->GetFilename(numItem);
    }

    if (!localFilename.empty() && loadPicture)
        mediaLoader->LoadPicture(localFilename);

    return localFilename;
}

// ─────────────────────────────────────────────
// Picture display
// ─────────────────────────────────────────────

void CViewerController::ShowPicture(CBitmapReturn* pictureData, const int& redraw)
{
    processLoadPicture = false;
    bool isPictureToShow = false;

    if (pictureData->bitmap != nullptr &&
        filename == pictureData->bitmap->GetFilename())
        isPictureToShow = true;

    if (isPictureToShow)
    {
        if (pictureData->bitmap != nullptr && pictureData->bitmap->IsOk())
        {
            mediaLoader->StopAnimation();
            isAnimation = false;
            isPicture   = true;
            isVideo     = false;

            previewWindow->IsNextPicture(isNext);

            bool isSetImage = false;
            if (isDiaporama)
            {
                if (previewWindow->SetBitmap(pictureData->bitmap, pictureData->isThumbnail, false))
                {
                    isSetImage = true;
                    isVideo    = false;
                }
            }
            else
            {
                previewWindow->HideValidationToolbar();
                if (previewWindow->SetBitmap(pictureData->bitmap, pictureData->isThumbnail, false))
                {
                    isSetImage = true;
                    isVideo    = false;
                    mediaLoader->SetPanelInfos(pictureData->isThumbnail);
                }
            }

            if (!isSetImage && pictureData->bitmap != nullptr)
            {
                delete pictureData->bitmap;
                pictureData->bitmap = nullptr;
            }
        }

        if (!pictureData->isThumbnail)
            mediaLoader->StopLoadingPicture();
    }
    else
    {
        if (!pictureData->isThumbnail)
            mediaLoader->StopLoadingPicture();

        if (pictureData->bitmap != nullptr)
            delete pictureData->bitmap;
    }

    if (redraw)
        mediaLoader->LoadPicture(filename, true);
}

void CViewerController::OnShowPicture(wxCommandEvent& event)
{
    auto* pictureData = static_cast<CBitmapReturn*>(event.GetClientData());
    if (pictureData != nullptr)
    {
        if (isPicture)
        {
            ShowPicture(pictureData, event.GetInt());
        }
        else
        {
            delete pictureData->bitmap;
            pictureData->bitmap = nullptr;
        }

        if (pictureData->myThread != nullptr)
        {
            delete pictureData->myThread;
            pictureData->myThread = nullptr;
        }
        delete pictureData;
    }
}
