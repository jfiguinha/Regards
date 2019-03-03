#include <header.h>
#include "BitmapToolbar.h"
#include <LibResource.h>
#include <ParamInit.h>
#include <ConfigParam.h>
using namespace Regards::Control;

#define WM_SAVE 3
#define WM_EMAIL 4
#define WM_IMPRIMER 5
#define WM_CROP 6
#define IDM_SETSHRINK 7




void CBitmapToolbar::SetBitmapDisplayPt(Regards::Control::CBitmapWndViewer * bitmapWindow)
{
	this->bitmapWindow = bitmapWindow;
}

CBitmapToolbar::CBitmapToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme)
	: CToolbarWindow(parent, id, theme)
{
	themeToolbar = theme;
	slide = nullptr;
	

	wxString saveLibelle = CLibResource::LoadStringFromResource("LBLSAVE",1);// "Save";
	wxString emailLibelle = CLibResource::LoadStringFromResource("LBLEMAIL",1);//"Emai";
	wxString printLibelle = CLibResource::LoadStringFromResource("LBLPRINT",1);//"Print";
	//wxString cropLibelle = CLibResource::LoadStringFromResource("LBLCROP",1);//"Crop";
	wxString shrinkLibelle = CLibResource::LoadStringFromResource("LBLSHRINK",1);//"Shrink Picture";
	wxString zoomOn = CLibResource::LoadStringFromResource("LBLZOOMON",1);// "Zoom On";
	wxString zoomOff = CLibResource::LoadStringFromResource("LBLZOOMOFF",1);// "Zoom Off";

	save = new CToolbarButton(themeToolbar.button);
	save->SetButtonResourceId("IDB_SAVE");
	save->SetCommandId(WM_SAVE);
	save->SetLibelleTooltip(saveLibelle);
	navElement.push_back(save);

	email = new CToolbarButton(themeToolbar.button);
	email->SetButtonResourceId("IDB_EMAIL");
	email->SetCommandId(WM_EMAIL);
	email->SetLibelleTooltip(emailLibelle);
	navElement.push_back(email);

	CToolbarButton * printer = new CToolbarButton(themeToolbar.button);
	printer->SetButtonResourceId("IDB_PRINTERPNG");
	printer->SetCommandId(WM_IMPRIMER);
	printer->SetLibelleTooltip(printLibelle);
	navElement.push_back(printer);

    /*
	crop = new CToolbarButton(themeToolbar.button);
	crop->SetButtonResourceId("IDB_CROP");
	crop->SetCommandId(WM_CROP);
	crop->SetLibelleTooltip(cropLibelle);
	navElement.push_back(crop);
    */

	CToolbarButton * shrink = new CToolbarButton(themeToolbar.button);
	shrink->SetButtonResourceId("IDB_SHRINK");
	shrink->SetCommandId(IDM_SETSHRINK);
	shrink->SetLibelleTooltip(shrinkLibelle);
	navElement.push_back(shrink);

	CToolbarButton * moins = new CToolbarButton(themeToolbar.button);
	moins->SetButtonResourceId("IDB_ZOOMMOINS");
	moins->SetCommandId(WM_ZOOMOUT);
	moins->SetLibelleTooltip(zoomOff);
	navElement.push_back(moins);

	slide = new CToolbarSlide(themeToolbar.slider, this);
	navElement.push_back(slide);

	CToolbarButton * plus = new CToolbarButton(themeToolbar.button);
	plus->SetButtonResourceId("IDB_ZOOMPLUS");
	plus->SetCommandId(WM_ZOOMON);
	plus->SetLibelleTooltip(zoomOn);
	navElement.push_back(plus);
}

CBitmapToolbar::~CBitmapToolbar()
{
}

void CBitmapToolbar::ZoomPos(const int &position)
{
	if (bitmapWindow != nullptr)
		bitmapWindow->SetRatioPos(position);
}

void CBitmapToolbar::ZoomOn()
{
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->ZoomOn();
	}

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
    ChangeZoomInPos();

}

void CBitmapToolbar::ChangeZoomInPos()
{
    if (slide != nullptr && bitmapWindow != nullptr)
    {
        int dwPos = bitmapWindow->GetPosRatio();
        dwPos++;
        if (dwPos >= slide->GetNbValue())
            dwPos = slide->GetNbValue() - 1;
        SetTrackBarPosition(dwPos);
    }
}

void CBitmapToolbar::ChangeZoomOutPos()
{
    if (slide != nullptr && bitmapWindow != nullptr)
    {
        int dwPos = bitmapWindow->GetPosRatio();
        dwPos--;
        if (dwPos < 0)
            dwPos = 0;
        SetTrackBarPosition(dwPos);
    }
}


void CBitmapToolbar::ZoomOut()
{
	bitmapWindow->ZoomOut();

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
    ChangeZoomOutPos();

}

void CBitmapToolbar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CBitmapToolbar::ClickButton(const int &id)
{

}

void CBitmapToolbar::SetTrackBarPosition(const int &iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		this->FastRefresh(this);
		//this->Update();
	}
}


void CBitmapToolbar::ShowSaveButton()
{
	if (save != nullptr)
	{
		if (!save->IsVisible())
		{
			save->SetVisible(true);
			this->FastRefresh(this);
		}
	}
}
void CBitmapToolbar::HideSaveButton()
{
	if (save != nullptr)
	{
		if (save->IsVisible())
		{
			save->SetVisible(false);
			this->FastRefresh(this);
		}
	}
}

void CBitmapToolbar::SlidePosChange(const int &position, const wxString &key)
{
	bitmapWindow->SetZoomPosition(position);
}


void CBitmapToolbar::EventManager(const int &id)
{
	switch (id)
	{
	case WM_IMPRIMER:
		if (bitmapWindow != nullptr)
			bitmapWindow->PrintPicture();
		break;
	case IDM_SETSHRINK:
		if (bitmapWindow != nullptr)
			bitmapWindow->ShrinkImage();
		break;
	case WM_SAVE:
		if (bitmapWindow != nullptr)
			bitmapWindow->SavePicture();
		break;

	case WM_EMAIL:
		if (bitmapWindow != nullptr)
			bitmapWindow->SendEmail();
		break;
            
	case WM_ZOOMOUT:
		ZoomOut();
		break;
	case WM_ZOOMON:
		ZoomOn();
		break;
	}
}




