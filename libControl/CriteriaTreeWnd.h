#pragma once
#include "CriteriaTree.h"
#include <ScrollbarWnd.h>
#include <WindowMain.h>
#include <FileGeolocation.h>
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace std;
class CTreeElementTriangle;
class CTreeElementTexte;

namespace Regards
{
	namespace Control
	{
		class CCriteriaTreeWnd : public CWindowMain
		{
		public:

			CCriteriaTreeWnd(wxWindow* parent, wxWindowID id, const int &mainWindowID, CFileGeolocation * fileGeolocalisation, const CThemeTree & theme,const CThemeScrollBar & themeScroll);
            ~CCriteriaTreeWnd(void);
            void UpdateScreenRatio();
            void SetFile(const wxString & filename);
            
		private:

            void Resize();
            void ShowCalendar(wxCommandEvent &event);
            void ShowMap(wxCommandEvent &event);
            wxString GenerateUrl();
            void UpdateTreeData();
            
            CScrollbarWnd * InfosFileScroll;
            CTreeWindow * treeWindow;
            CCriteriaTree * criteriaTree;
            CCriteriaTree * oldCriteriaTree;
            CFileGeolocation * fileGeolocalisation;
            wxString filename;
            int numPhotoId;
            int mainWindowID;
		};

	}
}