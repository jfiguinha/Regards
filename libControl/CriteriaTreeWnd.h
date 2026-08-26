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
		CCriteriaTreeWnd(wxWindow* parent,
			wxWindowID id,
			wxWindowID mainWindowID,
			const CThemeTree& theme,
			const CThemeScrollBar& themeScroll);

		~CCriteriaTreeWnd() override = default;

		void SetFile(const wxString& filename);

	private:
		void ShowCalendar(wxCommandEvent& event);
		void ShowMap(wxCommandEvent& event);
		void ShowKeyWord(wxCommandEvent& event);

		[[nodiscard]] wxString GenerateUrl() const;
		void UpdateTreeData();
		void NotifyCriteriaChanged() const;
		void NotifyUrlChanged() const;
		void UpdateGpsWindow(int messageId) const;
		void ReleasePhotoCriteria(wxCommandEvent& event) const;

		std::unique_ptr<CFileGeolocation> fileGeolocalisation;
		std::unique_ptr<CCriteriaTree> oldCriteriaTree;
		wxString filename;
		int numPhotoId = 0;
		wxWindowID mainWindowID = wxID_ANY;
	};
}
