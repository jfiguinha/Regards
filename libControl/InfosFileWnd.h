#pragma once
#include "InfosFile.h"
#include <ScrollbarWnd.h>
#include <WindowMain.h>
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
			CThreadLoadInfos()
			{
				panelInfos = nullptr;
				infosFileWnd = nullptr;
				threadLoadInfos = nullptr;
			}

            CInfosFileWnd * panelInfos;
            CInfosFile * infosFileWnd;
            wxString filename;
            thread * threadLoadInfos;
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
            
            CScrollbarWnd * InfosFileScroll;
            CTreeWindow * treeWindow;
            CInfosFile * infosFile;
            CInfosFile * oldInfosFileControl;
            wxString filename;
		};

	}
}