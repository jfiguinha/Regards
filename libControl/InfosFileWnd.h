#pragma once
#include "InfosFile.h"
#include <ScrollbarWnd.h>
#include <WindowMain.h>
#include <thread>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
using namespace Regards::Control;
using namespace std;
class CTreeElementTriangle;
class CTreeElementTexte;

namespace Regards
{
	namespace Control
	{
        class CInfosFileWnd;
        
        class CThreadLoadInfos
        {
        public:
            CInfosFileWnd * panelInfos = nullptr;
            CInfosFile * infosFileWnd = nullptr;
            wxString filename;
            std::thread * threadLoadInfos = nullptr;
        };
        
		class CInfosFileWnd : public CWindowMain
		{
		public:

			CInfosFileWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & theme);
            ~CInfosFileWnd(void);
            void UpdateScreenRatio();
            void InfosUpdate(const wxString & filename);
            
		private:

            void Resize();
            void UpdateTreeInfosEvent(wxCommandEvent &event);
            static void GenerateTreeInfos(CThreadLoadInfos * threadInfos);
            
            CScrollbarWnd * InfosFileScroll = nullptr;
            CTreeWindow * treeWindow = nullptr;
            CInfosFile * infosFile = nullptr;
            CInfosFile * oldInfosFileControl = nullptr;
            wxString filename;
		};

	}
}