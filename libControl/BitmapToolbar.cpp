#include <header.h>
#include "BitmapToolbar.h"
#include <LibResource.h>
#include <ConfigParam.h>
using namespace Regards::Control;

#define WM_EMAIL 4
#define WM_IMPRIMER 5
#define WM_CROP 6
#define IDM_SETSHRINK 7
#define WM_EXPORT 8



/*
void CBitmapToolbar::SetBitmapDisplayPt(Regards::Control::CBitmapWndViewer * bitmapWindow)
{
	this->bitmapWindow = bitmapWindow;
}
*/

CBitmapToolbar::CBitmapToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;
	slide = nullptr;
	this->parentId = viewerId;

	//wxString saveLibelle = CLibResource::LoadStringFromResource("LBLSAVE",1);// "Save";
	wxString exportLibelle = CLibResource::LoadStringFromResource("LBLEXPORT", 1);// "Save";
	wxString emailLibelle = CLibResource::LoadStringFromResource("LBLEMAIL",1);//"Emai";
	wxString printLibelle = CLibResource::LoadStringFromResource("LBLPRINT",1);//"Print";
	//wxString cropLibelle = CLibResource::LoadStringFromResource("LBLCROP",1);//"Crop";
	wxString shrinkLibelle = CLibResource::LoadStringFromResource("LBLSHRINK",1);//"Shrink Picture";
	wxString zoomOn = CLibResource::LoadStringFromResource("LBLZOOMON",1);// "Zoom On";
	wxString zoomOff = CLibResource::LoadStringFromResource("LBLZOOMOFF",1);// "Zoom Off";

	/*
	save = new CToolbarButton(themeToolbar.button);
	save->SetButtonResourceId("IDB_SAVE");
	save->SetCommandId(WM_SAVE);
	save->SetLibelleTooltip(saveLibelle);
	navElement.push_back(save);
	*/

	export_button = new CToolbarButton(themeToolbar.button);
	export_button->SetButtonResourceId("IDB_SAVE");
	export_button->SetCommandId(WM_EXPORT);
	export_button->SetLibelleTooltip(exportLibelle);
	navElement.push_back(export_button);

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
	CBitmapWndViewer * bitmapWindow = (CBitmapWndViewer *)this->FindWindowById(BITMAPWINDOWVIEWERID);
	
	if (bitmapWindow != nullptr)
		bitmapWindow->SetRatioPos(position);
}

void CBitmapToolbar::ZoomOn()
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->ZoomOn();
	}

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
    ChangeZoomInPos();

}

void CBitmapToolbar::ChangeZoomInPos()
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
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
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
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
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if (bitmapWindow != nullptr)
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
		ForceRefresh();
		//this->Update();
	}
}

void CBitmapToolbar::ShowExportButton()
{
  	if (export_button != nullptr)
	{
		if (!export_button->IsVisible())
		{
			export_button->SetVisible(true);
			Refresh();
		}
	}  
}

void CBitmapToolbar::HideExportButton()
{
  	if (export_button != nullptr)
	{
		if (export_button->IsVisible())
		{
			export_button->SetVisible(false);
			Refresh();
		}
	}  
}

void CBitmapToolbar::SlidePosChange(const int &position, const wxString &key)
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if(bitmapWindow != nullptr)
		bitmapWindow->SetZoomPosition(position);
}


void CBitmapToolbar::EventManager(const int &id)
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if (bitmapWindow != nullptr)
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

		case WM_EXPORT:
		{
			if (bitmapWindow != nullptr)
				bitmapWindow->ExportPicture();
			break;
		}
		/*
		case WM_SAVE:
			if (bitmapWindow != nullptr)
				bitmapWindow->SavePicture();
			break;
*/
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
}




