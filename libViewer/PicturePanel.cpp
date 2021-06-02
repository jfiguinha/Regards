#include <header.h>
#include "PicturePanel.h"
#include <RegardsBitmap.h>
#include <libPicture.h>
#include <OpenCVEffect.h>
#include <opencv2/core.hpp>
using namespace Regards::Window;
using namespace Regards::Picture;
CPicturePanel::CPicturePanel(wxWindow* parent, wxWindowID id, const CTheme& theme)
	: CWindowMain("CPicturePanel", parent, id)
{
    w = 0;
    h = 0;
    pictureOriginal = nullptr;
    histogram = nullptr;
    colorBack = theme.colorBack;
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPicturePanel::OnPaint));
    image = nullptr;
    choice_control = new wxComboBox(this, -1, "", wxDefaultPosition, wxSize(100, -1));
    choice_control->AppendString("All channel");
    choice_control->AppendString("Blue");
    choice_control->AppendString("Green");
    choice_control->AppendString("Red");
    choice_control->Select(0);
    Connect(wxEVT_COMBOBOX, (wxObjectEventFunction)&CPicturePanel::OnChannelSelect);
}

void CPicturePanel::CreateHistogram()
{
    if (histogram == nullptr)
        histogram = new CRegardsBitmap(512, 400);
    
    Regards::OpenCV::COpenCVEffect::CalculateHistogram(pictureOriginal, histogram, channelSelect, colorBack);
    this->image = CLibPicture::ConvertRegardsBitmapToWXImage(histogram);
}

void CPicturePanel::OnChannelSelect(wxCommandEvent& event)
{
    channelSelect = choice_control->GetSelection();
    CreateHistogram();
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
}

void CPicturePanel::SetPictureToDisplay(CRegardsBitmap * picture)
{
    if (pictureOriginal != nullptr)
        delete pictureOriginal;

    pictureOriginal = picture;

    channelSelect = 0;
    choice_control->Select(0);

    CreateHistogram();

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

    if (image != nullptr)
    {
        if ((neww != w || newh != h) || refreshPicture)
        {

            w = neww;
            h = newh;

            resized = wxBitmap(image->Scale(neww - (marged * 2), newh - chH - (marged * 4) /*, wxIMAGE_QUALITY_HIGH*/));

            dc.DrawBitmap(resized, marged, marged * 2 + chH, false);
        }
        else {
            dc.DrawBitmap(resized, marged, marged * 2 + chH, false);
        }
    }


}