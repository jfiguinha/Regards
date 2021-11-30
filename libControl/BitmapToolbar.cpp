#include <header.h>
#include "BitmapToolbar.h"
#include <LibResource.h>
#include "BitmapWndViewer.h"
#include "BitmapWnd3d.h"

using namespace Regards::Control;

constexpr auto WM_EMAIL = 4;
constexpr auto WM_IMPRIMER = 5;
constexpr auto IDM_SETSHRINK = 7;
constexpr auto WM_EXPORT = 8;

CBitmapToolbar::CBitmapToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar& theme,
                               const bool& vertical, const bool& exportPicture)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;
	slide = nullptr;
	this->parentId = viewerId;
	this->exportPicture = exportPicture;

	const wxString exportLibelle = CLibResource::LoadStringFromResource("LBLEXPORT", 1); // "Save";
	const wxString emailLibelle = CLibResource::LoadStringFromResource("LBLEMAIL", 1); //"Emai";
	const wxString printLibelle = CLibResource::LoadStringFromResource("LBLPRINT", 1); //"Print";
	const wxString shrinkLibelle = CLibResource::LoadStringFromResource("LBLSHRINK", 1); //"Shrink Picture";
	const wxString zoomOn = CLibResource::LoadStringFromResource("LBLZOOMON", 1); // "Zoom On";
	const wxString zoomOff = CLibResource::LoadStringFromResource("LBLZOOMOFF", 1); // "Zoom Off";

	/*
	export_button = new CToolbarButton(themeToolbar.button);
	export_button->SetButtonResourceId("IDB_SAVE");
	export_button->SetCommandId(WM_EXPORT);
	export_button->SetLibelleTooltip(exportLibelle);
	navElement.push_back(export_button);
	*/
	email = new CToolbarButton(themeToolbar.button);
	email->SetButtonResourceId("IDB_EMAIL");
	email->SetCommandId(WM_EMAIL);
	email->SetLibelleTooltip(emailLibelle);
	navElement.push_back(email);

	auto printer = new CToolbarButton(themeToolbar.button);
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

CBitmapToolbar::~CBitmapToolbar()
{
}

void CBitmapToolbar::ZoomPos(const int& position)
{
	auto bitmapWindow = static_cast<CBitmapWnd3D*>(this->FindWindowById(BITMAPWINDOWVIEWERID));
	if (bitmapWindow != nullptr)
	{
		CBitmapWndViewer* wndViewer =(CBitmapWndViewer*)bitmapWindow->GetWndPt();
		if(wndViewer != nullptr)
			wndViewer->SetRatioPos(position);
	}
		
}

void CBitmapToolbar::ZoomOn()
{
	auto bitmapWindow = dynamic_cast<CBitmapWnd3D*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		CBitmapWndViewer* wndViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		if (wndViewer != nullptr)
			wndViewer->ZoomOn();
	}

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
	ChangeZoomInPos();
}

void CBitmapToolbar::ChangeZoomInPos()
{
	auto bitmapWindow = static_cast<CBitmapWnd3D*>(this->FindWindowById(parentId));
	if (slide != nullptr && bitmapWindow != nullptr)
	{
		CBitmapWndViewer* wndViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		if (wndViewer != nullptr)
		{
			int dwPos = wndViewer->GetPosRatio();
			dwPos++;
			if (dwPos >= slide->GetNbValue())
				dwPos = slide->GetNbValue() - 1;
			SetTrackBarPosition(dwPos);
		}


	}
}

void CBitmapToolbar::ChangeZoomOutPos()
{
	auto bitmapWindow = static_cast<CBitmapWnd3D*>(this->FindWindowById(parentId));
	if (slide != nullptr && bitmapWindow != nullptr)
	{
		CBitmapWndViewer* wndViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		if (wndViewer != nullptr)
		{
			int dwPos = wndViewer->GetPosRatio();
			dwPos--;
			if (dwPos < 0)
				dwPos = 0;
			SetTrackBarPosition(dwPos);
		}
	}
}


void CBitmapToolbar::ZoomOut()
{
	auto bitmapWindow = static_cast<CBitmapWnd3D*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		CBitmapWndViewer* wndViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		if (wndViewer != nullptr)
			wndViewer->ZoomOut();
	}
	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
	ChangeZoomOutPos();
}

void CBitmapToolbar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CBitmapToolbar::ClickButton(const int& id)
{
}

void CBitmapToolbar::SetTrackBarPosition(const int& iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		Refresh();

	}
}

void CBitmapToolbar::SlidePosChange(const int& position, const wxString& key)
{
	auto bitmapWindow = static_cast<CBitmapWnd3D*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		CBitmapWndViewer* wndViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		if (wndViewer != nullptr)
			wndViewer->SetZoomPosition(position);
	}
}


void CBitmapToolbar::EventManager(const int& id)
{
	auto bitmapWindow = static_cast<CBitmapWnd3D*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		CBitmapWndViewer* wndViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
		if (wndViewer != nullptr)
		{
			switch (id)
			{
			case WM_IMPRIMER:
				wndViewer->PrintPicture();
				break;
			case IDM_SETSHRINK:
				wndViewer->ShrinkImage();
				break;

			case WM_EXPORT:
			{
				if (exportPicture)
					wndViewer->ExportPicture();
				else
					wndViewer->SavePicture();
				break;
			}
			case WM_EMAIL:
				wndViewer->SendEmail();
				break;

			case WM_ZOOMOUT:
				ZoomOut();
				break;
			case WM_ZOOMON:
				ZoomOn();
				break;
			default:;
			}
		}

	}
}
