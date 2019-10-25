#pragma once
#include <WindowMain.h>
#include <checktree.h>
#include <MouseUpdate.h>
using namespace Regards::Window;

class CRegardsBitmap;

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
			bool selected;
		};

		class COcrWnd : public CWindowMain, public IMouseUpdate
		{
		public:
			COcrWnd(wxWindow* parent, wxWindowID id);
			~COcrWnd();
			CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput);
			void Init();
            void tesseract_preprocess(string source_file, string out_file);
            void OcrToPDF(const wxString &bitmapFile, wxString outputFile);
		private:
			void OnOcr(wxCommandEvent& event);
			void OnOcrPDF(wxCommandEvent& event);
			void OnSelChanged(wxCommandEvent& aEvent);
			wxPanel * CreateListTesseract(wxWindow * parent);
			
		
			void Resize();
			//void GenerateLayerBitmap();
			void Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin);

			float XDrawingPosition(const float &m_lx, const long &m_lHScroll, const float &ratio);
			float YDrawingPosition(const float &m_ly, const long &m_lVScroll, const float &ratio);

			wxPanel * listOcr;
			wxCheckTree * treeCtrl;
			wxButton * exportPdf;
			vector<wxString> listFile;
			wxChoice *choice;
			vector<BBoxText *> listRect;
			//wxBitmap bmpSelection;
			CRegardsBitmap * bitmapBackground;
		};
	}
}