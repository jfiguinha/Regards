#pragma once
#include <WindowMain.h>
#include <checktree.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Scanner
	{
		class BBoxText : public wxTreeItemData
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
			void OnSelChanged(wxCommandEvent& aEvent);
			wxPanel * CreateListTesseract(wxWindow * parent);
			void tesseract_preprocess(string source_file, string out_file);
			wxString GetTempFile(wxString filename);
			void Resize();
			void GenerateLayerBitmap();


			wxPanel * listOcr;
			wxCheckTree * treeCtrl;
			
			vector<wxString> listFile;
			wxChoice *choice;
			vector<BBoxText *> listRect;
		};
	}
}