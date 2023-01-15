#include <header.h>
#include "PreviewToolbar.h"
#include <LibResource.h>
#include <BitmapWndViewer.h>
#include <BitmapWnd3d.h>

using namespace Regards::Control;

#define WM_EMAIL 4
#define WM_IMPRIMER 5
#define WM_CROP 6
#define IDM_SETSHRINK 7
#define WM_EXPORT 8
#define IDM_OLDPICTURE 9
#define IDM_NEWPICTURE 10


CPreviewToolbar::CPreviewToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar& theme,
                                 const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;
	slide = nullptr;
	this->parentId = viewerId;
	wxString zoomOn = CLibResource::LoadStringFromResource("LBLZOOMON", 1); // "Zoom On";
	wxString zoomOff = CLibResource::LoadStringFromResource("LBLZOOMOFF", 1); // "Zoom Off";
	wxString shrinkLibelle = CLibResource::LoadStringFromResource("LBLSHRINK", 1); //"Shrink Picture";
	wxString oldPictureLable = CLibResource::LoadStringFromResource("LBLORIGINALLABEL", 1);
	wxString newPictureLable = CLibResource::LoadStringFromResource("LBLNEWLABEL", 1);

	auto newPicture = new CToolbarButton(themeToolbar.button);
	newPicture->SetButtonResourceId("IDB_NEWPICTURE");
	newPicture->SetCommandId(IDM_NEWPICTURE);
	newPicture->SetLibelleTooltip(newPictureLable);
	navElement.push_back(newPicture);

	auto oldPicture = new CToolbarButton(themeToolbar.button);
	oldPicture->SetButtonResourceId("IDB_OLDPICTURE");
	oldPicture->SetCommandId(IDM_OLDPICTURE);
	oldPicture->SetLibelleTooltip(oldPictureLable);
	navElement.push_back(oldPicture);

	auto shrink = new CToolbarButton(themeToolbar.button);
	shrink->SetButtonResourceId("IDB_SHRINK");
	shrink->SetCommandId(IDM_SETSHRINK);
	shrink->SetLibelleTooltip(shrinkLibelle);
	navElement.push_back(shrink);

	auto moins = new CToolbarButton(themeToolbar.button);
	moins->SetButtonResourceId("IDB_ZOOMMOINS");
	moins->SetCommandId(WM_ZOOMOUT);
	moins->SetLibelleTooltip(zoomOff);
	navElement.push_back(moins);

	slide = new CToolbarSlide(themeToolbar.slider, this);
	navElement.push_back(slide);

	auto plus = new CToolbarButton(themeToolbar.button);
	plus->SetButtonResourceId("IDB_ZOOMPLUS");
	plus->SetCommandId(WM_ZOOMON);
	plus->SetLibelleTooltip(zoomOn);
	navElement.push_back(plus);
}

CPreviewToolbar::~CPreviewToolbar()
{
}

void CPreviewToolbar::ZoomPos(const int& position)
{
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	//auto bitmapWindow = static_cast<CBitmapWndViewer*>(this->FindWindowById(BITMAPWINDOWVIEWERID));
	if (wndViewer != nullptr)
		wndViewer->SetRatioPos(position);
}

void CPreviewToolbar::ZoomOn()
{
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	//auto bitmapWindow = static_cast<CBitmapWndViewer*>(this->FindWindowById(BITMAPWINDOWVIEWERID));
	if (wndViewer != nullptr)
	{
		wndViewer->ZoomOn();
	}

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
	ChangeZoomInPos();
}

void CPreviewToolbar::ChangeZoomInPos()
{
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	if (slide != nullptr && wndViewer != nullptr)
	{
		int dwPos = wndViewer->GetPosRatio();
		dwPos++;
		if (dwPos >= slide->GetNbValue())
			dwPos = slide->GetNbValue() - 1;
		SetTrackBarPosition(dwPos);
	}
}

void CPreviewToolbar::ChangeZoomOutPos()
{
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	if (slide != nullptr && wndViewer != nullptr)
	{
		int dwPos = wndViewer->GetPosRatio();
		dwPos--;
		if (dwPos < 0)
			dwPos = 0;
		SetTrackBarPosition(dwPos);
	}
}


void CPreviewToolbar::ZoomOut()
{
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	if (wndViewer != nullptr)
		wndViewer->ZoomOut();

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
	ChangeZoomOutPos();
}

void CPreviewToolbar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CPreviewToolbar::ClickButton(const int& id)
{
}

void CPreviewToolbar::SetTrackBarPosition(const int& iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		needToRefresh = true;
	}
}

void CPreviewToolbar::SlidePosChange(const int& position, const wxString& key)
{
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}
	if (wndViewer != nullptr)
		wndViewer->SetZoomPosition(position);
}


void CPreviewToolbar::EventManager(const int& id)
{
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}
	if (wndViewer != nullptr)
	{
		switch (id)
		{
		case IDM_SETSHRINK:
			wndViewer->ShrinkImage();
			break;

		case WM_ZOOMOUT:
			ZoomOut();
			break;
		case WM_ZOOMON:
			ZoomOn();
			break;

		case IDM_OLDPICTURE:
			{
				wxCommandEvent evt(wxEVENT_SHOWORIGINAL);
				this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			}
			break;

		case IDM_NEWPICTURE:
			{
				wxCommandEvent evt(wxEVENT_SHOWNEW);
				this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			}
			break;
		default: ;
		}
	}
}
