#include <header.h>
#include "PicturePanel.h"
#include <RegardsBitmap.h>
#include <libPicture.h>
#include <OpenCVEffect.h>
#include <opencv2/core.hpp>
#include <wx/spinctrl.h>
using namespace Regards::Window;
using namespace Regards::Picture;
CPicturePanel::CPicturePanel(wxWindow* parent, wxWindowID id, const CThemeThumbnail& theme)
	: CWindowMain("CPicturePanel", parent, id)
{
    w = 0;
    h = 0;
    pictureOriginal = nullptr;
    histogram = nullptr;
    colorBack = theme.colorBack;
    CThemeFont themeFont = theme.themeIcone.font;
    colorFont = themeFont.GetColorFont();
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPicturePanel::OnPaint));
    image = nullptr;
    choice_control = new wxComboBox(this, -1, "", wxDefaultPosition, wxSize(100, -1));
    choice_control->AppendString("All channel");
    choice_control->AppendString("Blue");
    choice_control->AppendString("Green");
    choice_control->AppendString("Red");
    choice_control->Select(0);
    Connect(wxEVT_COMBOBOX, (wxObjectEventFunction)&CPicturePanel::OnChannelSelect);

    /*
    minValue = new wxSpinCtrl(this, -1, "Min : ", wxDefaultPosition, wxSize(100, -1));
    maxValue = new wxSpinCtrl(this, -1, "Max : ", wxDefaultPosition, wxSize(100, -1));
    maxValue->SetRange(0, 255);
    minValue->SetRange(0, 255);
    maxValue->SetValue(255);
    minValue->SetValue(0);
    Connect(wxEVT_SPINCTRL, (wxObjectEventFunction)&CPicturePanel::OnRangeSelect);

    pictureBackup = new CRegardsBitmap();
    */
}
/*
void CPicturePanel::OnRangeSelect(wxSpinEvent& evt)
{
    int min = minValue->GetValue();
    int max = maxValue->GetValue();

    pictureOriginal->SetBitmap(pictureBackup->GetPtBitmap(), pictureBackup->GetBitmapWidth(), pictureBackup->GetBitmapHeight());
    Regards::OpenCV::COpenCVEffect::NormalizeHistogram(pictureOriginal, channelSelect, min, max);
    pictureOriginal->SaveToBmp("d:\\test_histo.bmp");
    refreshPicture = true;
    this->Refresh();
}
*/
void CPicturePanel::CreateHistogram()
{

    wxSize size = this->GetSize();
    int neww = size.GetWidth();
    int newh = size.GetHeight();

    int chW = 0;
    int chH = 0;
    choice_control->GetSize(&chW, &chH);

    int width = neww - (marged * 2);
    int height = newh - chH - (marged * 4);

    if(histogram == nullptr)
        histogram = new CRegardsBitmap(width, height);

    if (width != histogram->GetBitmapWidth() || height != histogram->GetBitmapHeight())
    {
        delete histogram;
        histogram = new CRegardsBitmap(width, height);
    }
     
    if (refreshPicture)
    {
        Regards::OpenCV::COpenCVEffect::CalculateHistogram(pictureOriginal, histogram, channelSelect, colorBack, colorFont);
        this->image = CLibPicture::ConvertRegardsBitmapToWXImage(histogram);
    }

   // wxSize spinSize = minValue->GetSize();
   // minValue->SetPosition(wxPoint(marged, newh - spinSize.GetHeight() - marged));
   // maxValue->SetPosition(wxPoint(neww - spinSize.GetWidth() - marged, newh - spinSize.GetHeight() - marged));
}

void CPicturePanel::OnChannelSelect(wxCommandEvent& event)
{
    channelSelect = choice_control->GetSelection();
    refreshPicture = true;

    
    this->Refresh();
}

CPicturePanel::~CPicturePanel()
{
    if (pictureOriginal != nullptr)
        delete pictureOriginal;

    if (image != nullptr)
        delete image;

    if (histogram == nullptr)
        delete histogram;

    delete choice_control;
    //delete minValue;
   // delete maxValue;
   // delete pictureBackup;
}

void CPicturePanel::SetPictureToDisplay(CRegardsBitmap * picture)
{
    if (pictureOriginal != nullptr)
        delete pictureOriginal;

    pictureOriginal = picture;

   // pictureBackup->SetBitmap(pictureOriginal->GetPtBitmap(), pictureOriginal->GetBitmapWidth(), pictureOriginal->GetBitmapHeight());

    channelSelect = 0;
    choice_control->Select(0);

    refreshPicture = true;
    this->Refresh();

}

void CPicturePanel::UpdateScreenRatio()
{
    this->Refresh();
}

void CPicturePanel::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
    wxRect rc = GetWindowRect();
    FillRect(&dc, rc, colorBack);

	int neww, newh;
	dc.GetSize(&neww, &newh);

    int chW = 0;
    int chH = 0;
    choice_control->GetSize(&chW, &chH);

    int xPos = (neww - chW) / 2;
    choice_control->SetPosition(wxPoint(xPos, marged));

    CreateHistogram();

    if (image != nullptr)
    {
        dc.DrawBitmap(*image, marged, marged * 2 + chH, false);
    }


}