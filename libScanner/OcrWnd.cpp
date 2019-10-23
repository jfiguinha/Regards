#include <header.h>
#include "OcrWnd.h"
#include <tesseract/ocrclass.h>
#include <wx/combobox.h>
#include <wx/gbsizer.h>
#include <FileUtility.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <ShowBitmap.h>
#include <tesseract/baseapi.h>
#include <tesseract/renderer.h>
#include <leptonica/allheaders.h>

#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <directoryctrl.h>
#include <FiltreEffet.h>
#include <FilterData.h>
#include <wx/progdlg.h>
#include "ExportOcr.h"
enum
{
	ID_BUT_OCR = 3000,
	ID_BUT_OCRPDF = ID_BUT_OCR + 1,
	ID_BUT_PREVIEW,
};

using namespace Regards::Scanner;
using namespace Regards::Control;

COcrWnd::COcrWnd(wxWindow* parent, wxWindowID id)
	: CWindowMain("OCR Window", parent, id)
{
	//bitmapBackground = nullptr;
	CThemeTree themeTree;
	listOcr = CreateListTesseract(this);

	long style = wxDIRCTRL_DEFAULT_STYLE;
	long treeStyle = wxTR_HAS_BUTTONS;

	treeStyle |= wxTR_HIDE_ROOT;

#ifdef __WXGTK20__
	treeStyle |= wxTR_NO_LINES;
#endif

	if (style & wxDIRCTRL_EDIT_LABELS)
		treeStyle |= wxTR_EDIT_LABELS;

	if (style & wxDIRCTRL_MULTIPLE)
		treeStyle |= wxTR_MULTIPLE;

	if ((style & wxDIRCTRL_3D_INTERNAL) == 0)
		treeStyle |= wxNO_BORDER;

	treeCtrl = new wxCheckTree(this, wxID_ANY, wxDefaultPosition, wxSize(250, 200), treeStyle);
	treeCtrl->SetBackgroundColour(themeTree.bgColorOne);
	treeCtrl->SetForegroundColour(themeTree.bgColorBackground);
	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	hsizer->Add(listOcr, 1, wxEXPAND | wxALL, 5);
	hsizer->Add(treeCtrl, 2, wxEXPAND | wxALL, 5);


	
	Connect(ID_BUT_OCR, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcr));
	Connect(ID_BUT_OCRPDF, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcrPDF));
	Connect(wxEVT_CHECKTREE_CHOICE, wxCommandEventHandler(COcrWnd::OnSelChanged), NULL, this);


}

void COcrWnd::Init()
{
	exportPdf->Enable(false);
	treeCtrl->DeleteAllItems();
	listRect.clear();
}

COcrWnd::~COcrWnd()
{

	CBitmapWndViewer * showBitmap = (CBitmapWndViewer *)wxWindow::FindWindowById(BITMAPWINDOWVIEWERIDPDF);
	if (showBitmap != nullptr)
	{
		showBitmap->RemoveListener();
	}
}

CImageLoadingFormat * COcrWnd::ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessin)
{
	return nullptr;
}

float COcrWnd::XDrawingPosition(const float &m_lx, const long &m_lHScroll, const float &ratio)
{
	int x = (m_lx * ratio) - m_lHScroll;
	return x;
}

float COcrWnd::YDrawingPosition(const float &m_ly, const long &m_lVScroll, const float &ratio)
{
	int y = (m_ly * ratio) - m_lVScroll;
	return y;
}

void COcrWnd::Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin)
{
	int hpos = bitmapViewer->GetHPos();
	int vpos = bitmapViewer->GetVPos();

	for (BBoxText * bbox : listRect)
	{
		if (bbox->selected)
		{
			wxRect rcTemp;
			rcTemp.x = XDrawingPosition(bbox->rect.x, hpos, bitmapViewer->GetRatio());
			rcTemp.width = bbox->rect.width * bitmapViewer->GetRatio();
			rcTemp.y = YDrawingPosition(bbox->rect.y, vpos, bitmapViewer->GetRatio());
			rcTemp.height = bbox->rect.height * bitmapViewer->GetRatio();

			CDraw::DessinerRectangleVide(dc, 2, rcTemp, wxColor(0, 0, 255, 0));
		}
	}
}

void COcrWnd::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessin, int & widthOutput, int & heightOutput)
{
	wxImage image = filtreEffet->GetwxImage();
	wxBitmap bitmap = wxBitmap(image);
	wxMemoryDC dc;
	dc.SelectObject(bitmap);
	wxRect rc(0, 0, image.GetWidth(), image.GetHeight());
	wxImage render = filtreEffet->GetwxImage();
	
	Drawing(&dc, bitmapViewer, dessin);

	dc.SelectObject(wxNullBitmap);
	CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
	wxImage * local_image = new wxImage(bitmap.ConvertToImage());
	imageLoad->SetPicture(local_image);
	filtreEffet->SetBitmap(imageLoad);
}

void COcrWnd::OnSelChanged(wxCommandEvent& aEvent)
{
	// Allow calling GetPath() in multiple selection from OnSelFilter
	//if (treeCtrl->HasFlag(wxTR_MULTIPLE))
	//{
	wxArrayTreeItemIds items;
	treeCtrl->GetSelections(items);
	for (int i = 0;i < items.size();i++)
	{
		// return first string only
		wxTreeItemId treeid = items[i];
		BBoxText * bboxText = (BBoxText *)treeCtrl->GetItemData(treeid);
		if(bboxText != nullptr)
			bboxText->selected = !bboxText->selected;
	}
	//}

	//GenerateLayerBitmap();
	CBitmapWndViewer * viewer = (CBitmapWndViewer *)wxWindow::FindWindowById(BITMAPWINDOWVIEWERIDPDF);
	if (viewer != nullptr)
		viewer->Refresh();
}

void COcrWnd::Resize()
{
	int width = this->GetWidth() - 5;
	int height = this->GetHeight();

	int panelListOcrH = listOcr->GetMinHeight();
	listOcr->SetSize(0, 0, width, panelListOcrH);
	treeCtrl->SetSize(0, panelListOcrH, width, height - panelListOcrH);
}

wxString COcrWnd::GetTempFile(wxString filename, const bool &removeFile)
{
	wxString file;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder,  wxS_DIR_DEFAULT)) {
#endif
	
		// handle the error here
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\" + filename;
#else
		file = tempFolder + "/" + filename;
#endif

		if (removeFile)
		{
			if (wxFileExists(file))
				wxRemoveFile(file);
		}
	}
	return file;
}


void COcrWnd::OnOcrPDF(wxCommandEvent& event)
{
	int timeout_ms = 5000;
	const char* retry_config = nullptr;
	bool textonly = false;
	int jpg_quality = 92;

	//Get select 
	int i = choice->GetSelection();
	wxString language = choice->GetStringSelection();

	wxString preprocess = GetTempFile("preprocess.bmp", false);

	wxFileDialog saveFileDialog(this, _("Export OCR to ... "), "", "", "PDF files (*.pdf)|*.pdf | TXT files (*.txt)|*.txt | boxfile files (*.boxfile) | *.boxfile | hOcr files (*.hocr) | *.hocr", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	wxString newfilename = saveFileDialog.GetPath();
	wxFileName fullpath(newfilename);
	wxString extension = fullpath.GetExt();

	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	resourcePath = resourcePath + "\\tessdata";

	i = 0;
	char * args[8];
	args[i++] = new char[255];
	args[i] = new char[255];
	strcpy(args[i++], preprocess);
	args[i] = new char[255];
	strcpy(args[i++], newfilename);
	args[i] = new char[255];
	strcpy(args[i++], "-l");
	args[i] = new char[255];
	strcpy(args[i++], language);
	args[i] = new char[255];
	strcpy(args[i++], "--tessdata-dir");
	args[i] = new char[255]; 
	strcpy(args[i++], resourcePath);
	args[i] = new char[255];
	strcpy(args[i++], extension);
	wxString error = "";
	int failed = CExportOcr::ExportOcr(8, args, error);
	
	for (int i = 0; i < 8; i++)
		delete[] args[i];
}



void COcrWnd::OnOcr(wxCommandEvent& event)
{
	CBitmapWndViewer * viewer = (CBitmapWndViewer *)wxWindow::FindWindowById(BITMAPWINDOWVIEWERIDPDF);
	if (viewer != nullptr)
	{
		viewer->SetListener(this);
		viewer->SetBitmapPreviewEffect(IDM_OCR);
	}

	//Get select 
	int i = choice->GetSelection();
	wxString language = choice->GetStringSelection();

	CShowBitmap * showBitmap = (CShowBitmap *)wxWindow::FindWindowById(SHOWBITMAPVIEWERIDPDF);
	if (showBitmap != nullptr)
	{
		try
		{
			wxString resourcePath = CFileUtility::GetResourcesFolderPath();
			resourcePath = resourcePath + "\\tessdata";

			ETEXT_DESC *monitor = new ETEXT_DESC();

			CRegardsBitmap * bitmapBackground = showBitmap->GetBitmap(true);

			// Initialize tesseract-ocr with English, without specifying tessdata path
			if (CExportOcr::api.Init(resourcePath, language)) {
				fprintf(stderr, "Could not initialize tesseract.\n");
				throw("Could not initialize tesseract.\n");
			}

			CLibPicture libPicture;
			wxString tempFile = GetTempFile("temp.bmp");
			CImageLoadingFormat loadingformat(false);
			loadingformat.SetPicture(bitmapBackground);
			libPicture.SavePicture(tempFile, &loadingformat, 0, 0);

			wxTreeItemId rootId = treeCtrl->AddRoot("Text");

			wxString preprocess = GetTempFile("preprocess.bmp");

			tesseract_preprocess(tempFile.ToStdString(), preprocess.ToStdString());

			Pix *image = pixRead(preprocess.ToStdString().c_str());

			//api->Init(CurDir, "eng");
			CExportOcr::api.SetPageSegMode(tesseract::PSM_AUTO_OSD); //PSM_SINGLE_BLOCK PSM_AUTO_OSD

			CExportOcr::api.SetImage(image);

			/*
			int page = 0;

			std::thread t1(CExportOcr::ocrProcess, &CExportOcr::api, monitor);
			std::thread t2(CExportOcr::monitorProgress, monitor, page);
			t1.join();
			t2.join();
			*/
			// Open input image with leptonica library
			//Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
			//api.SetImage(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(),
			//	4, 4 * bitmap->GetBitmapWidth());

			Boxa* boxes = CExportOcr::api.GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
			printf("Found %d textline image components.\n", boxes->n);
			for (int i = 0; i < boxes->n; i++) {
				BOX* box = boxaGetBox(boxes, i, L_CLONE);
				CExportOcr::api.SetRectangle(box->x, box->y, box->w, box->h);
				BBoxText * bboxText = new BBoxText();

				bboxText->rect.x = box->x;
				bboxText->rect.y = box->y;
				bboxText->rect.width = box->w;
				bboxText->rect.height = box->h;
				bboxText->rect.x = box->x;
				bboxText->confidence = CExportOcr::api.MeanTextConf();
				bboxText->label = wxString::FromUTF8(CExportOcr::api.GetUTF8Text());
				bboxText->selected = true;
				//wxTreeItemId childId = treeCtrl->AppendItem(rootId, bboxText.label);

				wxTreeItemId treeid = treeCtrl->AppendItem(rootId, bboxText->label, -1, -1, bboxText);


				treeCtrl->SetItemHasChildren(treeid);
				treeCtrl->MakeCheckable(treeid, true);


				bboxText->SetId(treeid);
				listRect.push_back(bboxText);
			}

			// Get OCR result
			//char * outText = api.GetUTF8Text();
			//printf("OCR output:\n%s", outText);

			// Destroy used object and release memory
			CExportOcr::api.End();

			exportPdf->Enable(true);

			//delete[] outText;

			if (bitmapBackground != nullptr)
				delete bitmapBackground;
		}
		catch (...)
		{

		}
	}
	//GenerateLayerBitmap();
	viewer->Refresh();
}

wxPanel * COcrWnd::CreateListTesseract(wxWindow * parent)
{
	wxPanel *panel = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxGridBagSizer *gsizer = NULL;

	wxStaticBox *sbox = new wxStaticBox(panel, wxID_ANY, wxString("Language : "));
	wxStaticBoxSizer *sbsizer = new wxStaticBoxSizer(sbox, wxVERTICAL);
	gsizer = new wxGridBagSizer(5, 5);
	sbsizer->Add(gsizer, 0, wxEXPAND | wxALL, 5);
	sizer->Add(sbsizer, 0, wxEXPAND | wxALL, 5);

	unsigned int row = gsizer->GetEffectiveRowsCount();

	gsizer->Add(new wxStaticText(panel, wxID_ANY, wxString("Language") + _T(":")),
		wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);

	choice = new wxChoice(panel, wxID_ANY);
	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	resourcePath = resourcePath + "\\tessdata";

	wxArrayString files;

	wxDir::GetAllFiles(resourcePath, &files, _T("*.traineddata"), wxDIR_FILES);

	for (wxString file : files)
	{
		wxFileName fullpath(file);
		listFile.push_back(file);
		choice->Append(fullpath.GetName());
	}

	choice->SetSelection(0);
	gsizer->Add(choice, wxGBPosition(row, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);

	exportPdf = new wxButton(panel, ID_BUT_OCRPDF, _("&Export To File ..."));
	gsizer->Add(new wxButton(panel, ID_BUT_OCR, _("&OCR")), wxGBPosition(row, 2), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	gsizer->Add(exportPdf, wxGBPosition(row, 3), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	exportPdf->Enable(false);
	panel->SetSizer(sizer);
	sizer->SetSizeHints(panel);
	return panel;
}


void COcrWnd::tesseract_preprocess(string source_file, string out_file) {

	/*
	char tempPath[128];
	GetTempPathA(128, tempPath);
	strcat_s(tempPath, "preprocess_ocr.bmp");

	char preprocessed_file[MAX_PATH];
	strcpy_s(preprocessed_file, tempPath);
	*/

	BOOL perform_negate = TRUE;
	l_float32 dark_bg_threshold = 0.5f; // From 0.0 to 1.0, with 0 being all white and 1 being all black 

	int perform_scale = 0;
	l_float32 scale_factor = 3.5f;

	int perform_unsharp_mask = 1;
	l_int32 usm_halfwidth = 5;
	l_float32 usm_fract = 2.5f;

	int perform_otsu_binarize = 1;
	l_int32 otsu_sx = 2000;
	l_int32 otsu_sy = 2000;
	l_int32 otsu_smoothx = 0;
	l_int32 otsu_smoothy = 0;
	l_float32 otsu_scorefract = 0.0f;


	l_int32 status = 1;
	l_float32 border_avg = 0.0f;
	PIX *pixs = NULL;
	char *ext = NULL;


	// Read in source image 
	pixs = pixRead(source_file.c_str());


	// Convert to grayscale 
	pixs = pixConvertRGBToGray(pixs, 0.0f, 0.0f, 0.0f);


	if (perform_negate)
	{
		PIX *otsu_pixs = NULL;

		status = pixOtsuAdaptiveThreshold(pixs, otsu_sx, otsu_sy, otsu_smoothx, otsu_smoothy, otsu_scorefract, NULL, &otsu_pixs);


		// Get the average intensity of the border pixels,
		//with average of 0.0 being completely white and 1.0 being completely black. 
		border_avg = pixAverageOnLine(otsu_pixs, 0, 0, otsu_pixs->w - 1, 0, 1);                               // Top 
		border_avg += pixAverageOnLine(otsu_pixs, 0, otsu_pixs->h - 1, otsu_pixs->w - 1, otsu_pixs->h - 1, 1); // Bottom 
		border_avg += pixAverageOnLine(otsu_pixs, 0, 0, 0, otsu_pixs->h - 1, 1);                               // Left 
		border_avg += pixAverageOnLine(otsu_pixs, otsu_pixs->w - 1, 0, otsu_pixs->w - 1, otsu_pixs->h - 1, 1); // Right 
		border_avg /= 4.0f;

		pixDestroy(&otsu_pixs);

		// If background is dark 
		if (border_avg > dark_bg_threshold)
		{
			// Negate image 
			pixInvert(pixs, pixs);

		}
	}

	if (perform_scale)
	{
		// Scale the image (linear interpolation) 
		pixs = pixScaleGrayLI(pixs, scale_factor, scale_factor);
	}

	if (perform_unsharp_mask)
	{
		// Apply unsharp mask 
		pixs = pixUnsharpMaskingGray(pixs, usm_halfwidth, usm_fract);
	}

	if (perform_otsu_binarize)
	{
		// Binarize 
		status = pixOtsuAdaptiveThreshold(pixs, otsu_sx, otsu_sy, otsu_smoothx, otsu_smoothy, otsu_scorefract, NULL, &pixs);
	}


	// Write the image to file 
	status = pixWriteImpliedFormat(out_file.c_str(), pixs, 0, 0);

}
