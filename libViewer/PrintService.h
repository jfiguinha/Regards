#pragma once
#include <wx/print.h>
class CImageLoadingFormat;

namespace Regards
{
    namespace Control
    {
        class CBitmapPrintout;
    }
}
namespace Regards::Viewer
{
    // Encapsule toute la logique d'impression et d'aperçu.
    class CPrintService
    {
    public:
        explicit CPrintService(wxFrame* parent);

        // Aperçu impression à partir d'un CImageLoadingFormat copié (PrintImagePreview original).
        void ShowImagePreview(const wxString& filename);


        // Aperçu impression à partir d'un CImageLoadingFormat copié (PrintImagePreview original).
        void ShowMatrixPreview(cv::Mat& picture);

        // Dialogue de mise en page.
        void ShowPageSetup();

#ifdef __WXMAC__
        // Dialogue des marges (macOS uniquement).
        void ShowPageMargins();
#endif

        // Lance directement l'impression du fichier donné.
        void PreviewFile(const wxString& filename);

    private:
        void OpenPreviewFrame(wxPrintPreview* preview);
        void GeneratePreviewFrame(Regards::Control::CBitmapPrintout* bitmapPrintOut_first, Regards::Control::CBitmapPrintout* bitmapPrintOut_second);
        wxFrame * parent_   = nullptr;
        wxPreviewFrameModalityKind modality_ = wxPreviewFrame_AppModal;
    };
}
