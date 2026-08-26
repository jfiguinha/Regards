#include <header.h>
#include "PrintService.h"
#include <BitmapPrintout.h>
#include <PrintEngine.h>
#include <LibResource.h>
#include <wx/printdlg.h>

#ifdef __WXMAC__
#include <wx/osx/printdlg.h>
#endif

using namespace Regards::Viewer;
using namespace Regards::Control;
using namespace Regards::Print;


CPrintService::CPrintService(wxFrame* parent)
    : parent_(parent)
{
}

void CPrintService::ShowImagePreview(const wxString &filename)
{
    Regards::Control::CBitmapPrintout* bitmapPrintOut_first = new CBitmapPrintout(filename);
    Regards::Control::CBitmapPrintout* bitmapPrintOut_second = new CBitmapPrintout(filename);
    GeneratePreviewFrame(bitmapPrintOut_first, bitmapPrintOut_second);
}

void CPrintService::GeneratePreviewFrame(Regards::Control::CBitmapPrintout* bitmapPrintOut_first, Regards::Control::CBitmapPrintout* bitmapPrintOut_second)
{
    wxPrintData* printData = CPrintEngine::GetPrintData();

    if (!printData)
    {
        wxLogError("Invalid print data");
        return;
    }

    wxPrintDialogData dialogData(*printData);

    wxPrintPreview* preview =
        new wxPrintPreview(
            bitmapPrintOut_first,
            bitmapPrintOut_second,
            &dialogData);

    if (!preview || !preview->IsOk())
    {
        delete preview;
        wxLogError(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"));
        return;
    }

    OpenPreviewFrame(preview);
}

// Aperçu impression à partir d'un CImageLoadingFormat copié (PrintImagePreview original).
void CPrintService::ShowMatrixPreview(cv::Mat& picture)
{
    Regards::Control::CBitmapPrintout* bitmapPrintOut_first = new CBitmapPrintout(picture);
    Regards::Control::CBitmapPrintout* bitmapPrintOut_second = new CBitmapPrintout(picture);
    GeneratePreviewFrame(bitmapPrintOut_first, bitmapPrintOut_second);
}


void CPrintService::ShowPageSetup()
{
    wxPrintData* printData         = CPrintEngine::GetPrintData();
    wxPageSetupDialogData* setupData = CPrintEngine::GetPageSetupDialogData();
    (*setupData) = *printData;

    wxPageSetupDialog dialog(parent_, setupData);
    dialog.ShowModal();

    (*printData)  = dialog.GetPageSetupDialogData().GetPrintData();
    (*setupData)  = dialog.GetPageSetupDialogData();
}

#ifdef __WXMAC__
void CPrintService::ShowPageMargins()
{
    wxPrintData* printData         = CPrintEngine::GetPrintData();
    wxPageSetupDialogData* setupData = CPrintEngine::GetPageSetupDialogData();
    (*setupData) = *printData;

    wxMacPageMarginsDialog dialog(parent_, setupData);
    dialog.ShowModal();

    (*printData)  = dialog.GetPageSetupDialogData().GetPrintData();
    (*setupData)  = dialog.GetPageSetupDialogData();
}
#endif

void CPrintService::PreviewFile(const wxString& filename)
{
    if (filename.IsEmpty())
        return;

    ShowImagePreview(filename);
}

void CPrintService::OpenPreviewFrame(wxPrintPreview* preview)
{
    const wxString label = CLibResource::LoadStringFromResource(L"PicturePrintPreview", 1);
    auto * frame = new wxPreviewFrame(preview, parent_, label, wxPoint(100, 100), wxSize(1000, 800));
    frame->Centre(wxBOTH);
    frame->Show();
}
