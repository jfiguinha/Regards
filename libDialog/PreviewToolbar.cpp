#include <header.h>
#include "PreviewToolbar.h"
#include <LibResource.h>
#include <ConfigParam.h>
using namespace Regards::Control;

#define WM_EMAIL 4
#define WM_IMPRIMER 5
#define WM_CROP 6
#define IDM_SETSHRINK 7
#define WM_EXPORT 8



CPreviewToolbar::CPreviewToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;
	slide = nullptr;
	this->parentId = viewerId;
	wxString zoomOn = CLibResource::LoadStringFromResource("LBLZOOMON",1);// "Zoom On";
	wxString zoomOff = CLibResource::LoadStringFromResource("LBLZOOMOFF",1);// "Zoom Off";
	wxString shrinkLibelle = CLibResource::LoadStringFromResource("LBLSHRINK", 1);//"Shrink Picture";
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

CPreviewToolbar::~CPreviewToolbar()
{
}

void CPreviewToolbar::ZoomPos(const int &position)
{
	CBitmapWndViewer * bitmapWindow = (CBitmapWndViewer *)this->FindWindowById(BITMAPWINDOWVIEWERID);
	
	if (bitmapWindow != nullptr)
		bitmapWindow->SetRatioPos(position);
}

void CPreviewToolbar::ZoomOn()
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->ZoomOn();
	}

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
    ChangeZoomInPos();

}

void CPreviewToolbar::ChangeZoomInPos()
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

void CPreviewToolbar::ChangeZoomOutPos()
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


void CPreviewToolbar::ZoomOut()
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if (bitmapWindow != nullptr)
		bitmapWindow->ZoomOut();

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
    ChangeZoomOutPos();

}

void CPreviewToolbar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CPreviewToolbar::ClickButton(const int &id)
{

}

void CPreviewToolbar::SetTrackBarPosition(const int &iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		ForceRefresh();
		//this->Update();
	}
}

void CPreviewToolbar::SlidePosChange(const int &position, const wxString &key)
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if(bitmapWindow != nullptr)
		bitmapWindow->SetZoomPosition(position);
}


void CPreviewToolbar::EventManager(const int &id)
{
	CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);
	if (bitmapWindow != nullptr)
	{
		switch (id)
		{
		case IDM_SETSHRINK:
			if (bitmapWindow != nullptr)
				bitmapWindow->ShrinkImage();
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




