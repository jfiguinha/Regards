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
wxString CViewerController::LoadingNextPicture(const bool& loadPicture, LoadingMode mode)
{
    if (!windowMode)
        return filename;

    int numItem = 0;
    if (windowMode == WINDOW_EXPLORER && listPicture != nullptr)
    {
		switch (mode)
		{
			case Next:
				numItem = listPicture->ImageSuivante();
				break;
			case Previous:
				numItem = listPicture->ImagePrecedente();
				break;
			case First:
				numItem = listPicture->ImageDebut();
				break;
			case Last:
				numItem = listPicture->ImageFin();
				break;
		}
        filename = listPicture->GetFilename(numItem);
    }
    else if ((windowMode == WINDOW_VIEWER || windowMode == WINDOW_PICTURE) && thumbnailPicture != nullptr)
    {
        switch (mode)
        {
        case Next:
            numItem = thumbnailPicture->ImageSuivante();
            break;
        case Previous:
            numItem = thumbnailPicture->ImagePrecedente();
            break;
        case First:
            numItem = thumbnailPicture->ImageDebut();
            break;
        case Last:
            numItem = thumbnailPicture->ImageFin();
            break;
        }
        filename = thumbnailPicture->GetFilename(numItem);
    }
    else if (windowMode == WINDOW_FACE && listFace != nullptr)
    {
        switch (mode)
        {
        case Next:
            numItem = listFace->ImageSuivante();
            break;
        case Previous:
            numItem = listFace->ImagePrecedente();
            break;
        }
        filename = listFace->GetFilename(numItem);
    }

    if (!filename.empty() && loadPicture)
    {
        isNext = true;
        mediaLoader->LoadPicture(filename);
    }
}

wxString CViewerController::ImageSuivante(const bool& loadPicture)
{
    return LoadingNextPicture(loadPicture, Next);
}

wxString CViewerController::ImagePrecedente(const bool& loadPicture)
{
    return LoadingNextPicture(loadPicture, Previous);
}

wxString CViewerController::ImageFin(const bool& loadPicture)
{
    return LoadingNextPicture(loadPicture, Last);
}

wxString CViewerController::ImageDebut(const bool& loadPicture)
{
    return LoadingNextPicture(loadPicture, First);
}

// ─────────────────────────────────────────────
// Picture display
// ─────────────────────────────────────────────

void CViewerController::ShowPicture(CBitmapReturn* pictureData, const int& redraw)
{
    processLoadPicture = false;
    bool isPictureToShow = false;
    if(filename.empty())
        filename = pictureData->bitmap != nullptr ? pictureData->bitmap->GetFilename() : wxEmptyString;

	if (pictureData->bitmap != nullptr && filename == pictureData->bitmap->GetFilename())
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
