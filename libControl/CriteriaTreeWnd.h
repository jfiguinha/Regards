#pragma once
#include "CriteriaTree.h"
#include <TreeWithScrollbar.h>
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
		class CCriteriaTreeWnd : public CTreeWithScrollbar
		{
		public:

			CCriteriaTreeWnd(wxWindow* parent, wxWindowID id, const int &mainWindowID, CFileGeolocation * fileGeolocalisation, const CThemeTree & theme,const CThemeScrollBar & themeScroll);
            ~CCriteriaTreeWnd(void);
            void SetFile(const wxString & filename);
            
		private:

            void ShowCalendar(wxCommandEvent &event);
            void ShowMap(wxCommandEvent &event);
            wxString GenerateUrl();
            void UpdateTreeData();

            CCriteriaTree * criteriaTree;
            CCriteriaTree * oldCriteriaTree;
            CFileGeolocation * fileGeolocalisation;
            wxString filename;
            int numPhotoId;
            int mainWindowID;
		};

	}
}