#pragma once
#include <WindowMain.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Scanner
	{
		class BBoxText
		{
		public:
			wxRect rect;
			wxString label;
			int confidence;
		};

		class COcrWnd : public CWindowMain
		{
		public:
			COcrWnd(wxWindow* parent, wxWindowID id);
			~COcrWnd();

		private:
			void OnOcr(wxCommandEvent& event);
			wxPanel * CreateListTesseract(wxWindow * parent);

			vector<wxString> listFile;
			wxChoice *choice;
			vector<BBoxText> listRect;
		};
	}
}