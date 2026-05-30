#pragma once

// Data returned from the async picture-loading thread
class CImageLoadingFormat;

namespace Regards::Viewer
{
    class CThumbnailViewerPicture;
    class CListPicture;
    class CListFace;
    class CMediaLoader;
    class CPreviewWnd;


    class CBitmapReturn
    {
    public:
        CBitmapReturn() : bitmap(nullptr), isThumbnail(false), myThread(nullptr) {}
        CImageLoadingFormat* bitmap;
        bool   isThumbnail;
        std::thread* myThread;
    };

    class CViewerController
    {
    public:
        CViewerController(wxWindow*                parent,
                          CThumbnailViewerPicture* thumbnailPicture,
                          CListPicture*            listPicture,
                          CListFace*               listFace,
                          CPreviewWnd*             previewWindow,
                          CMediaLoader*            mediaLoader);
        ~CViewerController() = default;

        wxString ImageSuivante(const bool& loadPicture = true);
        wxString ImagePrecedente(const bool& loadPicture = true);
        wxString ImageFin(const bool& loadPicture = true);
        wxString ImageDebut(const bool& loadPicture = true);

        wxString GetFilename() const { return filename; }
        void     SetFilename(const wxString& f) { filename = f; }

        int  GetNbElement() const;

        // Receives the loaded bitmap from the thread (EVENT_SHOWPICTURE)
        void OnShowPicture(wxCommandEvent& event);

        // Navigation events forwarded from CCentralWindow
        void OnPictureNext(wxCommandEvent& event)     { ImageSuivante(); }
        void OnPicturePrevious(wxCommandEvent& event) { ImagePrecedente(); }
        void OnPictureFirst(wxCommandEvent& event)    { ImageDebut(); }
        void OnPictureLast(wxCommandEvent& event)     { ImageFin(); }

        // Direction hint used by transition animations
        bool IsNext() const { return isNext; }

        // Called from MediaLoader when a picture is ready to display
        void ShowPicture(CBitmapReturn* pictureData, const int& redraw);

    private:
        int GetPhotoId(const wxString& filename) const;

        wxWindow*                parent           = nullptr;
        CThumbnailViewerPicture* thumbnailPicture = nullptr;
        CListPicture*            listPicture      = nullptr;
        CListFace*               listFace         = nullptr;
        CPreviewWnd*             previewWindow    = nullptr;
        CMediaLoader*            mediaLoader      = nullptr;

        wxString filename;
        bool     isNext = false;

        // windowMode is read from MediaLoader (shared reference)
        int& windowMode;
        bool& isDiaporama;
        bool& isPicture;
        bool& isAnimation;
        bool& isVideo;
        bool& processLoadPicture;
    };
}
