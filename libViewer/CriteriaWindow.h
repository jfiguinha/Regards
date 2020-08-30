#pragma once

#include <Splitter.h>
#include <wx/webview.h>
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

		class CCriteriaWindow : public CSplitter
		{
		public:
			CCriteriaWindow(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, const bool &horizontal = true);
			~CCriteriaWindow();
            void SetFile(const wxString &filename);
			void UpdateScreenRatio();


		private:

            void UpdateURL(wxCommandEvent& event);
            wxString MapsUpdate(const wxString &filename);

			CCriteriaTreeWnd * criteriaTreeWnd;
            wxWebView * webBrowser = nullptr;
            wxString url = "";
            wxString filename = "";

		};
	}
}