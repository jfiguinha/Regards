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
			void tesseract_preprocess(string source_file, string out_file);
			wxString GetTempFile(wxString filename);

			vector<wxString> listFile;
			wxChoice *choice;
			vector<BBoxText> listRect;
		};
	}
}