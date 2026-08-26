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
	this->parentId = viewerId;
	this->exportPicture = exportPicture;

	email = CreateButton("IDB_EMAIL", "LBLEMAIL", WM_EMAIL, false);
	printer = CreateButton("IDB_PRINTERPNG", "LBLPRINT", WM_IMPRIMER, false);
	shrink = CreateButton("IDB_SHRINK", "LBLSHRINK", IDM_SETSHRINK, false);
	moins = CreateButton("IDB_ZOOMMOINS", "LBLZOOMOFF", WM_ZOOMOUT, false);

	slide = std::make_unique<CToolbarSlide>(themeToolbar.slider, this);
	navElement.push_back(slide.get());

	plus = CreateButton("IDB_ZOOMPLUS", "LBLZOOMON", WM_ZOOMON, false);

}

void CBitmapToolbar::ZoomPos(const int& position)
{
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		auto wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
		if (wndViewer != nullptr)
			wndViewer->SetRatioPos(position);
	}
}

void CBitmapToolbar::ZoomOn()
{
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		auto wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
		if (wndViewer != nullptr)
			wndViewer->ZoomOn();
	}

	//SetTrackBarPosition(bitmapWindow->GetPosRatio());
	ChangeZoomInPos();
}

void CBitmapToolbar::ChangeZoomInPos()
{
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(this->FindWindowById(parentId));
	if (slide != nullptr && bitmapWindow != nullptr)
	{
		auto wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
		if (wndViewer != nullptr)
		{
			int dwPos = wndViewer->GetPosRatio();
			if (dwPos >= slide->GetNbValue())
				dwPos = slide->GetNbValue() - 1;
			SetTrackBarPosition(dwPos);
		}
	}
}

void CBitmapToolbar::ChangeZoomOutPos()
{
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(this->FindWindowById(parentId));
	if (slide != nullptr && bitmapWindow != nullptr)
	{
		auto wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
		if (wndViewer != nullptr)
		{
			int dwPos = wndViewer->GetPosRatio();
			if (dwPos < 0)
				dwPos = 0;
			SetTrackBarPosition(dwPos);
		}
	}
}


void CBitmapToolbar::ZoomOut()
{
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		auto wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
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
		needToRefresh = true;
	}
}

void CBitmapToolbar::SlidePosChange(const int& position, const wxString& key)
{
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		auto wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
		if (wndViewer != nullptr)
			wndViewer->SetZoomPosition(position);
	}
}


void CBitmapToolbar::EventManager(const int& id)
{
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(this->FindWindowById(parentId));
	if (bitmapWindow != nullptr)
	{
		auto wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
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
			default: ;
			}
		}
	}
}
