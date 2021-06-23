#pragma once

#include <Splitter.h>
using namespace std;
using namespace Regards::Window;

namespace Regards
{

    namespace Control
    {
        class CCriteriaTreeWnd;
    };
    
    using namespace Regards::Control;
    
    namespace Viewer
	{
		class CKeywordWndToolbar;

		class CCriteriaWindow : public CSplitter
		{
		public:
			CCriteriaWindow(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, const bool &horizontal = true);
			~CCriteriaWindow();
            void SetFile(const wxString &filename);
			void UpdateScreenRatio();


		private:

			CCriteriaTreeWnd * criteriaTreeWnd;
			CKeywordWndToolbar * keywordWnd = nullptr;
            wxString url = "";
            wxString filename = "";

		};
	}
}