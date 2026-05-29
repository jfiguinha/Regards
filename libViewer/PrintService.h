#pragma once
#include <wx/wx.h>
#include <wx/print.h>

class CImageLoadingFormat;

namespace Regards::Viewer
{
    // Encapsule toute la logique d'impression et d'aperçu.
    class CPrintService
    {
    public:
        explicit CPrintService(wxWindow* parent);

        // Aperçu impression à partir d'une matrice OpenCV (PrintPreview original).
        void ShowMatrixPreview(CImageLoadingFormat* image);

        // Aperçu impression à partir d'un CImageLoadingFormat copié (PrintImagePreview original).
        void ShowImagePreview(CImageLoadingFormat* image);

        // Dialogue de mise en page.
        void ShowPageSetup();

#ifdef __WXMAC__
        // Dialogue des marges (macOS uniquement).
        void ShowPageMargins();
#endif

        // Lance directement l'impression du fichier donné.
        void PrintFile(const wxString& filename);

        wxPreviewFrameModalityKind GetModality() const { return modality_; }

    private:
        void OpenPreviewFrame(wxPrintPreview* preview);

        wxWindow* parent_   = nullptr;
        wxPreviewFrameModalityKind modality_ = wxPreviewFrame_AppModal;
    };
}
