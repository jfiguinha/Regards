#pragma once
#include "InfosToolbar.h"
#include <ToolbarInterface.h>
#include <InfosFileWnd.h>
#include <CriteriaTreeWnd.h>
#include <wx/webview.h>
#include <string>
#include <thread>   
#include <WindowMain.h>
#include <FileGeolocation.h>
#include <wx/animate.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::Internet;


namespace Regards
{
	namespace Explorer
	{
		class CPanelInfosWnd : public CWindowMain, public CToolbarInterface
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID id, CFileGeolocation * fileGeolocalisation);
			~CPanelInfosWnd();
			void ShowInfos();
			void ShowMap();
            void ShowCriteria();
			void SetFile(const wxString &filename);
            void UpdateScreenRatio();
            void UpdateData();
            void StartLoadingPicture(wxCommandEvent &event);
            void StopLoadingPicture(wxCommandEvent &event);
            
		protected:

            void OnPaint(wxPaintEvent& event);
            
			void LoadInfo();
			void DisplayURL(const wxString &url);
			void ClickShowButton(const int &id);

			void Resize();

			void ShowFiltre(const wxString &title);
			void HideAllWindow();
			
			void InfosUpdate();
			wxString MapsUpdate();
            
            wxAnimationCtrl * m_animationCtrl = nullptr;
            CCriteriaTreeWnd * criteriaTreeWnd = nullptr;
			CInfosFileWnd * infosFileWnd = nullptr;
			wxWebView * webBrowser = nullptr;
			CInfosToolbar * infosToolbar = nullptr;
			CFileGeolocation * fileGeolocalisation = nullptr;
            wxString url = "http://www.google.fr";
            wxString filename = "";
			int windowVisible;
            CThemeBitmapWindow themeBitmap;
		};
	}
}
