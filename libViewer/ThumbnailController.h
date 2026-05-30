#pragma once

namespace Regards::Viewer
{
    class CThumbnailViewerPicture;
    class CThumbnailViewerVideo;
    class CListPicture;
    class CListFace;
}

namespace Regards::Viewer
{
    class CThumbnailController
    {
    public:
        CThumbnailController(CThumbnailViewerPicture* thumbnailPicture,
                             CThumbnailViewerVideo*   thumbnailVideo,
                             CListPicture*            listPicture,
                             CListFace*               listFace);
        ~CThumbnailController() = default;

        // Refresh an individual thumbnail icon (from wxEVENT_ICONEUPDATE)
        void UpdateThumbnailIcone(wxCommandEvent& event);

        // Resize all thumbnail panels (from wxEVENT_ICONESIZEREFRESH)
        void UpdateThumbnailIconeSize(wxCommandEvent& event);

        // Refresh the thumbnail strip after a list change (from wxEVENT_REFRESHTHUMBNAIL)
        void OnRefreshThumbnail(wxCommandEvent& event);

    private:
        CThumbnailViewerPicture* thumbnailPicture = nullptr;
        CThumbnailViewerVideo*   thumbnailVideo   = nullptr;
        CListPicture*            listPicture      = nullptr;
        CListFace*               listFace         = nullptr;
    };
}
