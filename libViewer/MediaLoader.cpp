#include <header.h>
#include "MediaLoader.h"
#include "MusicController.h"
#include "ThumbnailViewerPicture.h"
#include "ThumbnailViewerVideo.h"
#include "PreviewWnd.h"
#include "PanelInfosWnd.h"
#include "AnimationToolbar.h"
#include <ImageLoadingFormat.h>
#include <libPicture.h>
#include <LibResource.h>
#include <SqlPhotos.h>
#include <SqlThumbnail.h>
#include <ImageVideoThumbnail.h>
#include <ShowElement.h>
#include <picture_id.h>
#include <ThumbnailMessage.h>
#include "ViewerController.h"   // for CBitmapReturn & EVENT_SHOWPICTURE

#define DELAY_ANIMATION 20

extern bool     firstElementToShow;
extern int      numElementToLoad;
extern wxImage  defaultPicture;

using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

// ─────────────────────────────────────────────────────────────────────────────
// Construction / destruction
// ─────────────────────────────────────────────────────────────────────────────

CMediaLoader::CMediaLoader(wxWindow*                parent,
                           CPreviewWnd*             previewWindow,
                           CPanelInfosWnd*          panelInfosWindow,
                           CThumbnailViewerPicture* thumbnailPicture,
                           CThumbnailViewerVideo*   thumbnailVideo,
                           CMusicController*        musicController)
    : parent(parent)
    , previewWindow(previewWindow)
    , panelInfosWindow(panelInfosWindow)
    , thumbnailPicture(thumbnailPicture)
    , thumbnailVideo(thumbnailVideo)
    , musicController(musicController)
{
    animationTimer = new wxTimer(parent, wxTIMER_ANIMATION);
}

CMediaLoader::~CMediaLoader()
{
    if (animationTimer->IsRunning())
        animationTimer->Stop();
    delete animationTimer;

    for (auto* img : videoThumbnail)
        delete img;
    videoThumbnail.clear();
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

int CMediaLoader::GetPhotoId(const wxString& filename) const
{
    CSqlPhotos photo;
    return photo.GetPhotoId(filename);
}

void CMediaLoader::StartLoadingPicture()
{
    wxWindow* bitmapWindow = parent->FindWindowById(THUMBNAILVIEWERPICTURE);
    if (bitmapWindow != nullptr)
    {
        wxCommandEvent evt(wxEVENT_ONSTARTLOADINGPICTURE);
        evt.SetExtraLong(GetPhotoId(filename));
        bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
    }
}

void CMediaLoader::StopLoadingPicture()
{
    wxWindow* bitmapWindow = parent->FindWindowById(THUMBNAILVIEWERPICTURE);
    if (bitmapWindow != nullptr)
    {
        wxCommandEvent evt(wxEVENT_ONSTOPLOADINGPICTURE);
        bitmapWindow->GetEventHandler()->AddPendingEvent(evt);
    }
}

void CMediaLoader::SetPanelInfos(const bool& isThumbnail)
{
    if (panelInfosWindow->GetFilename() != filename)
    {
        if (isVideo)
            panelInfosWindow->SetVideoFile(filename);
        else
            panelInfosWindow->SetBitmapFile(filename, isThumbnail);
        panelInfosWindow->Refresh();
        panelInfosWindow->Update();
    }
    else if (!isThumbnail)
    {
        panelInfosWindow->SetBitmapFile(filename, isThumbnail);
        panelInfosWindow->Refresh();
        panelInfosWindow->Update();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Main load dispatch
// ─────────────────────────────────────────────────────────────────────────────

int CMediaLoader::LoadPicture(const wxString& newFilename, const bool& refresh)
{
    if (numElementToLoad != 0)
        firstElementToShow = false;

    // Broadcast dimensions to main window status bar
    if (wxFileExists(newFilename))
    {
        CLibPicture libPicture;
        int width = 0, height = 0, rotation = 0;
        libPicture.GetPictureDimensions(newFilename, width, height, rotation);
        auto* pictureInfos = new CPictureInfosMessage();
        pictureInfos->filename = newFilename;
        pictureInfos->infos    = std::to_string(width) + "x" + std::to_string(height);

        wxWindow* mainWindow = parent->FindWindowById(MAINVIEWERWINDOWID);
        if (mainWindow != nullptr)
        {
            wxCommandEvent evt(wxEVENT_INFOS);
            evt.SetClientData(pictureInfos);
            mainWindow->GetEventHandler()->AddPendingEvent(evt);
        }
    }

    if (newFilename != this->filename || refresh)
    {
        this->filename = newFilename;

        if (stopVideo)
            return 0;

        if (videoStart)
        {
            loadPicture = true;
            if (previewWindow != nullptr && !stopVideo)
            {
                stopVideo = true;
                previewWindow->StopVideo();
            }
            return 1;
        }
        loadPicture = false;

        bool errorPhoto        = false;
        bool needToLoadPicture = false;
        wxString pictureToLoad = newFilename;

        if (wxFileExists(newFilename))
        {
            CLibPicture libPicture;
            isVideo = false;

            if (libPicture.TestIsVideo(newFilename))
            {
                const bool isVideoValid = libPicture.TestIsVideoValid(newFilename);
                isVideo = true;

                if (isVideoValid)
                {
                    if (musicController != nullptr)
                        musicController->StopMusic();
                    SetVideo(newFilename);
                }
                else
                {
                    errorPhoto = true;
                }
            }
            else if (libPicture.TestIsAnimation(newFilename))
            {
                SetAnimation(newFilename);

                if (isDiaporama)
                {
                    wxWindow* mainWindow = parent->FindWindowById(MAINVIEWERWINDOWID);
                    wxCommandEvent evt(wxTIMER_DIAPORAMATIMERSTART);
                    mainWindow->GetEventHandler()->AddPendingEvent(evt);
                }
            }
            else
            {
                StartLoadingPicture();
                needToLoadPicture = true;
            }
        }
        else
        {
            errorPhoto = true;
        }

        if (errorPhoto)
        {
            pictureToLoad     = CLibResource::GetPhotoCancel();
            needToLoadPicture = true;
            StartLoadingPicture();
        }

        if (needToLoadPicture)
        {
            isPicture = true;

            // Try thumbnail first (instant display while full image loads)
            if (!isDiaporama)
            {
                CSqlThumbnail sqlThumbnail;
                CImageLoadingFormat* _loadingPicture = sqlThumbnail.GetPictureThumbnail(pictureToLoad);
                if (_loadingPicture != nullptr && _loadingPicture->IsOk())
                {
                    _loadingPicture->SetFilename(pictureToLoad);
                    auto* bitmapReturn       = new CBitmapReturn();
                    bitmapReturn->myThread   = nullptr;
                    bitmapReturn->isThumbnail = true;
                    bitmapReturn->bitmap     = _loadingPicture;
                    auto* event              = new wxCommandEvent(EVENT_SHOWPICTURE);
                    event->SetClientData(bitmapReturn);
                    event->SetInt(processLoadPicture ? 1 : 0);
                    wxQueueEvent(parent, event);

                    wxWindow* mainWindow = parent->FindWindowById(FRAMEVIEWER_ID);
                    wxCommandEvent evt(wxEVENT_PICTUREENDLOADING);
                    mainWindow->GetEventHandler()->AddPendingEvent(evt);
                }
            }

            // Start async full-resolution load
            if (!processLoadPicture)
            {
                auto* pictureData       = new CThreadPictureData();
                pictureData->mainWindow = parent;
                pictureData->picture    = pictureToLoad;
                pictureData->isVisible  = true;
                pictureData->isDiaporama = isDiaporama;
                auto* threadLoad        = new std::thread(LoadingNewPicture, pictureData);
                pictureData->myThread   = threadLoad;
                processLoadPicture      = true;
            }
        }
    }

    // Sync active item in all list views
    if (thumbnailPicture != nullptr)
        thumbnailPicture->SetActifItem(GetPhotoId(this->filename), true);

    numElementToLoad++;

    auto* event = new wxCommandEvent(wxEVENT_ENDLOADPICTURE);
    event->SetClientData(new wxString(this->filename));
    wxQueueEvent(parent, event);

    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// Media setters
// ─────────────────────────────────────────────────────────────────────────────

void CMediaLoader::SetVideo(const wxString& path)
{
    StopAnimation();
    filename    = path;
    isVideo     = true;
    isAnimation = false;
    isPicture   = false;

    if (thumbnailVideo->GetFilename() != filename)
    {
        int duration = CLibPicture::GetVideoDuration(filename);
        thumbnailVideo->SetFile(filename, (duration > 20) ? 20 : duration);
    }

    if (previewWindow != nullptr)
        previewWindow->SetVideo(path);

    SetPanelInfos(false);
}

bool CMediaLoader::SetAnimation(const wxString& filename)
{
    StopAnimation();

    CLibPicture libPicture;
    const bool refresh = !isAnimation;

    isVideo     = false;
    isAnimation = true;
    isPicture   = false;
    oldAnimationPosition = -1;
    oldFilename = L"";

    nbThumbnail = libPicture.GetNbImage(filename);
    if (nbThumbnail > 0)
        thumbnailVideo->SetFile(filename, nbThumbnail);

    for (auto* img : videoThumbnail)
        delete img;
    videoThumbnail.clear();

    const int iFormat = libPicture.TestImageFormat(filename);
    if (iFormat != TIFF && iFormat != PDF)
        videoThumbnail = libPicture.LoadAllVideoThumbnail(filename, false, false);

    animationPosition = 0;
    LoadAnimationBitmap(filename, 0);

    SetPanelInfos(false);

    if (previewWindow != nullptr)
    {
        wxCommandEvent evt1(wxEVENT_SHOWSAVEBUTTON);
        previewWindow->GetEventHandler()->AddPendingEvent(evt1);
        wxCommandEvent evt2(wxEVENT_ANIMATIONSTART);
        previewWindow->GetEventHandler()->AddPendingEvent(evt2);
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Animation
// ─────────────────────────────────────────────────────────────────────────────

void CMediaLoader::StartAnimation()
{
    animationPosition = 0;
    LoadAnimationBitmap(filename, 0);
    animationTimer->Start(DELAY_ANIMATION, wxTIMER_ONE_SHOT);
}

void CMediaLoader::StopAnimation()
{
    animationPosition = 0;
    if (animationTimer->IsRunning())
        animationTimer->Stop();
}

void CMediaLoader::LoadAnimationBitmap(const wxString& filename, const int& numFrame)
{
    if (numFrame == oldAnimationPosition && filename == oldFilename)
        return;

    oldFilename          = filename;
    oldAnimationPosition = numFrame;

    bool isSetImage = false;
    CImageLoadingFormat* image = nullptr;

    if (numFrame >= 0 && numFrame < nbThumbnail)
    {
        CLibPicture libPicture;
        const int iFormat = libPicture.TestImageFormat(filename);
        if (iFormat != TIFF && iFormat != PDF)
        {
            if ((int)videoThumbnail.size() > numFrame)
            {
                CImageVideoThumbnail* thumbnail = videoThumbnail.at(numFrame);
                if (thumbnail != nullptr)
                {
                    image = new CImageLoadingFormat();
                    image->SetPicture(thumbnail->image);
                    image->SetFilename(thumbnail->filename);
                }
            }
        }
        else
        {
            image = libPicture.LoadPicture(filename, false, numFrame);
        }
    }

    if (previewWindow != nullptr)
        previewWindow->IsNextPicture(isNext);

    if (image != nullptr && image->IsOk())
    {
        isVideo = false;
        if (isDiaporama)
        {
            if (previewWindow != nullptr)
                if (previewWindow->SetBitmap(image, false, true))
                    isSetImage = true;
        }
        else
        {
            if (previewWindow != nullptr)
            {
                previewWindow->HideValidationToolbar();
                if (previewWindow->SetBitmap(image, false, true))
                {
                    isSetImage = true;
                    SetPanelInfos(false);
                }
            }
        }
    }

    if (!isSetImage && image != nullptr)
    {
        delete image;
        image = nullptr;
    }
}

void CMediaLoader::OnTimerAnimation(wxTimerEvent& /*event*/)
{
    if (thumbnailVideo != nullptr)
        thumbnailVideo->SetVideoPosition(animationPosition);

    LoadAnimationBitmap(filename, animationPosition);
    animationPosition++;

    if (animationPosition < nbThumbnail)
    {
        uint32_t delay = 20;
        CLibPicture libPicture;
        const int iFormat = libPicture.TestImageFormat(filename);
        if (iFormat != TIFF && iFormat != PDF)
        {
            if ((int)videoThumbnail.size() > animationPosition)
                delay = videoThumbnail.at(animationPosition)->delay;
        }
        else
        {
            delay = libPicture.GetFrameDelay(filename);
        }
        animationTimer->Start(delay, wxTIMER_ONE_SHOT);
    }
    else
    {
        auto* animationToolbar = dynamic_cast<CAnimationToolbar*>(
            parent->FindWindowById(ANIMATIONTOOLBARWINDOWID));
        if (animationToolbar != nullptr)
            animationToolbar->AnimationStop();

        wxCommandEvent evt(wxEVT_ANIMATIONTIMERSTOP);
        parent->GetEventHandler()->AddPendingEvent(evt);
    }
}

void CMediaLoader::AnimationPictureNext()
{
    animationPosition++;
    if (animationPosition >= nbThumbnail)
        animationPosition = nbThumbnail - 1;
    if (thumbnailVideo != nullptr)
        thumbnailVideo->SetVideoPosition(animationPosition);
    LoadAnimationBitmap(filename, animationPosition);
}

void CMediaLoader::AnimationPicturePrevious()
{
    animationPosition--;
    if (animationPosition < 0)
        animationPosition = 0;
    if (thumbnailVideo != nullptr)
        thumbnailVideo->SetVideoPosition(animationPosition);
    LoadAnimationBitmap(filename, animationPosition);
}

void CMediaLoader::AnimationSetPosition(wxCommandEvent& event)
{
    const auto videoTime = static_cast<int64_t*>(event.GetClientData());
    if (thumbnailVideo != nullptr && videoTime != nullptr)
        thumbnailVideo->SetVideoPosition(*videoTime);
    if (videoTime != nullptr)
        delete videoTime;
}

void CMediaLoader::SetVideoPos(wxCommandEvent& event)
{
    int64_t pos = event.GetExtraLong();
    if (thumbnailVideo != nullptr)
        thumbnailVideo->SetVideoPosition(pos);
}

void CMediaLoader::SetPosition(const long& timePosition)
{
    if (isVideo)
    {
        auto* showVideoWindow = static_cast<CShowElement*>(parent->FindWindowById(SHOWBITMAPVIEWERID));
#ifdef FFMPEG
        if (showVideoWindow != nullptr)
            showVideoWindow->SetPosition(timePosition);
#else
        if (showVideoWindow != nullptr)
            showVideoWindow->SetPosition(timePosition * ONE_MSEC);
#endif
    }
    else
    {
        animationPosition = timePosition;
        LoadAnimationBitmap(filename, timePosition);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Video lifecycle
// ─────────────────────────────────────────────────────────────────────────────

void CMediaLoader::OnVideoEnd(wxCommandEvent& /*event*/)
{
    stopVideo  = false;
    videoStart = false;

    if (loadPicture)
    {
        LoadPicture(filename, true);
    }
    else if (isDiaporama)
    {
        // Delegate navigation to ViewerController via event
        wxCommandEvent evt(wxEVENT_PICTURENEXT);
        parent->GetEventHandler()->AddPendingEvent(evt);
    }

    loadPicture = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Static thread entry
// ─────────────────────────────────────────────────────────────────────────────

void CMediaLoader::LoadingNewPicture(CThreadPictureData* pictureData)
{
    CLibPicture libPicture;
    CImageLoadingFormat* bitmap = libPicture.LoadPicture(pictureData->picture);
    if (bitmap == nullptr)
        bitmap = new CImageLoadingFormat();

    if (bitmap->GetWidth() == 0 || bitmap->GetHeight() == 0)
        bitmap->SetPicture(defaultPicture);

    bitmap->SetFilename(pictureData->picture);

    auto* bitmapReturn       = new CBitmapReturn();
    bitmapReturn->myThread   = nullptr;
    bitmapReturn->isThumbnail = false;
    bitmapReturn->bitmap     = bitmap;

    auto* event = new wxCommandEvent(EVENT_SHOWPICTURE);
    event->SetClientData(bitmapReturn);
    event->SetInt(0);
    wxQueueEvent(pictureData->mainWindow, event);
}
