#include <header.h>
#include "PrintService.h"
#include <BitmapPrintout.h>
#include <PrintEngine.h>
#include <LibResource.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <wx/printdlg.h>

#if wxUSE_POSTSCRIPT
#include <wx/generic/printps.h>
#include <wx/generic/prntdlgg.h>
#endif

#ifdef __WXMAC__
#include <wx/osx/printdlg.h>
#endif

using namespace Regards::Viewer;
using namespace Regards::Control;
using namespace Regards::Print;
using namespace Regards::Picture;

CPrintService::CPrintService(wxWindow* parent)
    : parent_(parent)
{
}

void CPrintService::ShowMatrixPreview(CImageLoadingFormat* image)
{
    wxPrintData* printData = CPrintEngine::GetPrintData();
    wxPrintDialogData dialogData(*printData);

    cv::Mat bitmapPreview = image->GetMatrix().getMat();
    auto* preview = new wxPrintPreview(
        new CBitmapPrintout(image),
        new CBitmapPrintout(bitmapPreview),
        &dialogData);

    if (!preview->IsOk())
    {
        delete preview;
        wxLogError(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"));
        return;
    }

    OpenPreviewFrame(preview);
}

void CPrintService::ShowImagePreview(CImageLoadingFormat* image)
{
    wxPrintData* printData = CPrintEngine::GetPrintData();
    wxPrintDialogData dialogData(*printData);

    auto* bitmapCopy = new CImageLoadingFormat();
    *bitmapCopy = *image;

    auto* preview = new wxPrintPreview(
        new CBitmapPrintout(image),
        new CBitmapPrintout(bitmapCopy),
        &dialogData);

    if (!preview->IsOk())
    {
        delete preview;
        wxLogError(wxT("There was a problem previewing.\nPerhaps your current printer is not set correctly?"));
        return;
    }

    OpenPreviewFrame(preview);
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

void CPrintService::PrintFile(const wxString& filename)
{
    if (filename.IsEmpty())
        return;

    CLibPicture libPicture;
    CImageLoadingFormat* image = libPicture.LoadPicture(filename);
    if (image)
        ShowMatrixPreview(image);
}

void CPrintService::OpenPreviewFrame(wxPrintPreview* preview)
{
    const wxString label = CLibResource::LoadStringFromResource(L"PicturePrintPreview", 1);
    auto* frame = new wxPreviewFrame(preview, parent_, label, wxPoint(100, 100), wxSize(600, 650));
    frame->Centre(wxBOTH);
    frame->InitializeWithModality(modality_);
    frame->Show();
}
