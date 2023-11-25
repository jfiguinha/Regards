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

namespace Regards::Control
{
	class CCriteriaTreeWnd : public CTreeWithScrollbar
	{
	public:
		CCriteriaTreeWnd(wxWindow* parent, wxWindowID id, const int& mainWindowID, const CThemeTree& theme,
		                 const CThemeScrollBar& themeScroll);
		~CCriteriaTreeWnd(void) override;
		void SetFile(const wxString& filename);

	private:
		void ShowCalendar(wxCommandEvent& event);
		void ShowMap(wxCommandEvent& event);
		wxString GenerateUrl();
		void UpdateTreeData();
		void ShowKeyWord(wxCommandEvent& event);

		CFileGeolocation* fileGeolocalisation;
		CCriteriaTree* criteriaTree;
		CCriteriaTree* oldCriteriaTree;
		wxString filename;
		int numPhotoId;
		int mainWindowID;
	};
}
