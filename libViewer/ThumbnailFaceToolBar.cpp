#include <header.h>
#ifndef __NOFACE_DETECTION__
#include "ThumbnailFaceToolBar.h"
#include <ToolbarSlide.h>
#include <LibResource.h>
#include "ListFace.h"
#include <window_id.h>
using namespace Regards::Viewer;

#define WM_ADD 1023


CThumbnailFaceToolBar::CThumbnailFaceToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
                                             const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;

	add = CreateButton(L"IDB_PLUS", L"LBLFACEADD", WM_ADD, false);
	copy = CreateButton(L"IDB_MULTIPLESELECT", L"LBLCOPY", WM_COPY, false);
	moins = CreateButton(L"IDB_ZOOMMOINS", L"LBLZOOMOFF", WM_ZOOMOUT, false);

	slide = std::make_unique<CToolbarSlide>(themeToolbar.slider, this);
	navElement.push_back(slide.get());

	plus = CreateButton(L"IDB_ZOOMPLUS", L"LBLZOOMON", WM_ZOOMON, false);
	refreshButton = CreateButton(L"IDB_FOLDER_REFRESH", L"LBLREFRESHTHUMBNAIL", WM_REFRESH, false);

}


bool CThumbnailFaceToolBar::ZoomOn()
{
	if (slide != nullptr)
	{
		int dwPos = slide->GetPosition();
		dwPos++;
		if (dwPos >= slide->GetNbValue())
		{
			return false;
		}
		SetTrackBarPosition(dwPos);
		needToRefresh = true;
	}
	return true;
}

bool CThumbnailFaceToolBar::ZoomOff()
{
	if (slide != nullptr)
	{
		int dwPos = slide->GetPosition();
		dwPos--;
		if (dwPos < 0)
		{
			return false;
		}
		SetTrackBarPosition(dwPos);
		needToRefresh = true;
	}
	return true;
}

void CThumbnailFaceToolBar::SetTabValue(vector<int> value)
{
	if (slide != nullptr)
		slide->SetTabValue(value);
}

void CThumbnailFaceToolBar::SetTrackBarPosition(const int& iPos)
{
	int positionTrackBar = iPos;
	if (slide != nullptr)
	{
		slide->SetPosition(positionTrackBar);
		needToRefresh = true;
	}
}

void CThumbnailFaceToolBar::SlidePosChange(const int& position, const wxString& key)
{
	auto listFace = static_cast<CListFace*>(this->FindWindowById(LISTFACEID));
	if (listFace != nullptr)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILZOOMPOSITION);
		evt.SetExtraLong(position);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailFaceToolBar::ZoomPos(const int& position)
{
	auto listFace = static_cast<CListFace*>(this->FindWindowById(LISTFACEID));
	if (listFace != nullptr)
	{
		wxCommandEvent evt(wxEVENT_THUMBNAILZOOMPOSITION);
		evt.SetExtraLong(position);
		listFace->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CThumbnailFaceToolBar::EventManager(const int& id)
{
	auto listFace = static_cast<CListFace*>(this->FindWindowById(LISTFACEID));
	if (listFace != nullptr)
	{
		switch (id)
		{
		case WM_ADD:
		{
			auto windowMain = static_cast<CWindowMain*>(this->FindWindowById(MAINVIEWERWINDOWID));
			if (windowMain != nullptr)
			{
				wxCommandEvent evt(wxEVENT_FACEADD);
				vector<int> faceId = listFace->GetFaceSelectID();
				if (faceId.size() > 0)
				{
					vector<int>* data = new vector<int>();
					*data = faceId;
					evt.SetClientData(data);
					windowMain->GetEventHandler()->AddPendingEvent(evt);
				}
				else
				{
					wxString labelInformations = CLibResource::LoadStringFromResource(
						L"labelInformations", 1);
					wxString notCompatibleFormat = "Please select a face";
					wxMessageBox(notCompatibleFormat, labelInformations,
						wxICON_INFORMATION);
				}
			}

		}
		break;

		case WM_COPY:
			{
				if (listFace != nullptr)
				{
					wxCommandEvent evt(wxEVENT_THUMBNAILMOVE);
					listFace->GetEventHandler()->AddPendingEvent(evt);
				}
			}
			break;

		case WM_ZOOMON:
			{
				if (ZoomOn() && listFace != nullptr)
				{
					wxCommandEvent evt(wxEVENT_THUMBNAILZOOMON);
					listFace->GetEventHandler()->AddPendingEvent(evt);
				}
			}

			break;

		case WM_ZOOMOUT:
			{
				if (ZoomOff() && listFace != nullptr)
				{
					wxCommandEvent evt(wxEVENT_THUMBNAILZOOMOFF);
					listFace->GetEventHandler()->AddPendingEvent(evt);
				}
			}

			break;
		case WM_REFRESH:
			if (listFace != nullptr)
			{
				wxCommandEvent evt(wxEVENT_THUMBNAILREFRESHFACE);
				listFace->GetEventHandler()->AddPendingEvent(evt);
			}
			break;
		}
	}
}

#endif
