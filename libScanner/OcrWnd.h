#pragma once
#include <WindowMain.h>
#include <checktree.h>
#include <MouseUpdate.h>
#include "OcrLabelWnd.h"
using namespace Regards::Window;

class CRegardsBitmap;

namespace Regards
{
	namespace Scanner
	{
		class COcrWnd : public CWindowMain, public IMouseUpdate
		{
		public:
			COcrWnd(wxWindow* parent, wxWindowID id);
			~COcrWnd();
			CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput);
			void Init();
            static void tesseract_preprocess(wxString source_file, wxString out_file);
            static  void OcrToPDF(wxString bitmapFile, wxString outputFile, wxString language);
		private:
			void OnOcr(wxCommandEvent& event);
			void OnOcrPDF(wxCommandEvent& event);
			void OnSelChanged(wxCommandEvent& aEvent);
			void OnSelRead(wxCommandEvent& aEvent);

			wxPanel * CreateListTesseract(wxWindow * parent);
			void LoadOcrBoxFile(wxString boxfile);
		
			void Resize();
			//void GenerateLayerBitmap();
			void Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin);

			float XDrawingPosition(const float &m_lx, const long &m_lHScroll, const float &ratio);
			float YDrawingPosition(const float &m_ly, const long &m_lVScroll, const float &ratio);

			wxPanel * listOcr;
			COcrLabelWnd * ocrLabelWnd;
			wxButton * exportPdf;
			wxButton * ocrPdf;
			vector<wxString> listFile;
			wxChoice *choice;
			vector<BBoxText *> listRect;
			//wxBitmap bmpSelection;
			CRegardsBitmap * bitmapBackground;
		};
	}
}