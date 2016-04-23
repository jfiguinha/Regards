#pragma once
#include "InfosToolbar.h"
#include <ToolbarInterface.h>
#include <InfosFile.h>
#include <ScrollbarWnd.h>
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
        class CPanelInfosWnd;
        
        class CThreadLoadInfos
        {
        public:
            CPanelInfosWnd * panelInfos = nullptr;
            CInfosFile * infosFileWnd = nullptr;
            wxString filename;
            std::thread * threadLoadInfos = nullptr;
        };
        
		class CPanelInfosWnd : public CWindowMain, public CToolbarInterface
		{
		public:
			CPanelInfosWnd(wxWindow* parent, wxWindowID id, CFileGeolocation * fileGeolocalisation);
			~CPanelInfosWnd();
			void ShowInfos();
			void ShowMap();
			void SetFile(const wxString &filename);
            void UpdateScreenRatio();
            
            void StartLoadingPicture(wxWindow * window);
            void StopLoadingPicture(wxWindow * window);
            
		protected:

            static void GenerateTreeInfos(CThreadLoadInfos * threadInfos);
            void UpdateTreeInfosEvent(wxCommandEvent &event);
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

			CScrollbarWnd * InfosFileScroll = nullptr;
			CTreeWindow * treeWindow = nullptr;
			CInfosFile * infosFile = nullptr;
			wxWebView * webBrowser = nullptr;

            //wxWindow * emptyWindow = nullptr;
			CInfosFile * oldInfosFileControl = nullptr;
			CInfosToolbar * infosToolbar = nullptr;
			CFileGeolocation * fileGeolocalisation = nullptr;
            wxString url = "http://www.google.fr";
            wxString filename = "";
			int windowVisible;
            CThemeBitmapWindow themeBitmap;
		};
	}
}
