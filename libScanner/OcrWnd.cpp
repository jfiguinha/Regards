#include <header.h>
#include "OcrWnd.h"
#include <wx/combobox.h>
#include <wx/gbsizer.h>
#include <FileUtility.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <ShowBitmap.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>

enum
{
	ID_BUT_OCR = 3000,
	ID_BUT_PREVIEW,
};

using namespace Regards::Scanner;
using namespace Regards::Control;
COcrWnd::COcrWnd(wxWindow* parent, wxWindowID id)
	: CWindowMain("OCR Window", parent, id)
{
	long treeStyle = wxTR_HAS_BUTTONS;

	treeStyle |= wxTR_HIDE_ROOT;

#ifdef __WXGTK20__
	treeStyle |= wxTR_NO_LINES;
#endif

	//if (style & wxDIRCTRL_EDIT_LABELS)
	//	treeStyle |= wxTR_EDIT_LABELS;

	treeStyle |= wxTR_MULTIPLE;

	//if ((style & wxDIRCTRL_3D_INTERNAL) == 0)
	//	treeStyle |= wxNO_BORDER;

	CThemeTree themeTree;
	listOcr = CreateListTesseract(this);
	treeCtrl = new wxCheckTree(this, wxID_ANY, wxDefaultPosition, wxSize(250, 200), treeStyle);
	treeCtrl->SetBackgroundColour(themeTree.bgColorOne);
	treeCtrl->SetForegroundColour(themeTree.bgColorBackground);
	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	hsizer->Add(listOcr, 1, wxEXPAND | wxALL, 5);
	hsizer->Add(treeCtrl, 2, wxEXPAND | wxALL, 5);

	Connect(ID_BUT_OCR, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcr));
	Connect(wxEVT_CHECKTREE_CHOICE, wxCommandEventHandler(COcrWnd::OnSelChanged), NULL, this);
}

COcrWnd::~COcrWnd()
{

}

void COcrWnd::GenerateLayerBitmap()
{

}

void COcrWnd::OnSelChanged(wxCommandEvent& aEvent)
{
	// Allow calling GetPath() in multiple selection from OnSelFilter
	if (treeCtrl->HasFlag(wxTR_MULTIPLE))
	{
		wxArrayTreeItemIds items;
		treeCtrl->GetSelections(items);
		if (items.size() > 0)
		{
			// return first string only
			wxTreeItemId treeid = items[0];

		}
	}
}

void COcrWnd::Resize()
{
	int width = this->GetWidth() - 5;
	int height = this->GetHeight();

	int panelListOcrH = listOcr->GetMinHeight();
	listOcr->SetSize(0, 0, width, panelListOcrH);
	treeCtrl->SetSize(0, panelListOcrH, width, height - panelListOcrH);
}

wxString COcrWnd::GetTempFile(wxString filename)
{
	wxString file;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
#else
	wxString tempFolder = documentPath + "/temp";
#endif
	if (!wxMkDir(tempFolder)) {
		// handle the error here
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\" + filename;
#else
		file = tempFolder + "/" + filename;
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);
	}
	return file;
}


void COcrWnd::OnOcr(wxCommandEvent& event)
{
	//Get select 
	int i = choice->GetSelection();
	wxString language = choice->GetStringSelection();

	CShowBitmap * showBitmap = (CShowBitmap *)wxWindow::FindWindowById(SHOWBITMAPVIEWERIDPDF);
	if (showBitmap != nullptr)
	{
		CRegardsBitmap * bitmap = nullptr;
		try
		{
			wxString resourcePath = CFileUtility::GetResourcesFolderPath();
			resourcePath = resourcePath + "\\tesseract";

			bitmap = showBitmap->GetBitmap(true);

			tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
			// Initialize tesseract-ocr with English, without specifying tessdata path
			if (api->Init(resourcePath, language)) {
				fprintf(stderr, "Could not initialize tesseract.\n");
				throw("Could not initialize tesseract.\n");
			}

			CLibPicture libPicture;
			wxString tempFile = GetTempFile("temp.bmp");
			CImageLoadingFormat loadingformat(false);
			loadingformat.SetPicture(bitmap);
			libPicture.SavePicture(tempFile, &loadingformat, 0, 0);

			wxTreeItemId rootId = treeCtrl->AddRoot("Text");

			wxString preprocess = GetTempFile("preprocess.bmp");

			tesseract_preprocess(tempFile.ToStdString(), preprocess.ToStdString());

			Pix *image = pixRead(preprocess.ToStdString().c_str());

			//api->Init(CurDir, "eng");
			api->SetPageSegMode(tesseract::PSM_AUTO_OSD); //PSM_SINGLE_BLOCK PSM_AUTO_OSD

			api->SetImage(image);

			// Open input image with leptonica library
			//Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
			//api->SetImage(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(),
			//	4, 4 * bitmap->GetBitmapWidth());

			Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
			printf("Found %d textline image components.\n", boxes->n);
			for (int i = 0; i < boxes->n; i++) {
				BOX* box = boxaGetBox(boxes, i, L_CLONE);
				api->SetRectangle(box->x, box->y, box->w, box->h);
				BBoxText * bboxText = new BBoxText();

				
				//char* ocrResult = api->GetUTF8Text();
				//int conf = api->MeanTextConf();
				//fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
				//	i, box->x, box->y, box->w, box->h, conf, ocrResult);
				bboxText->rect.x = box->x;
				bboxText->rect.y = box->y;
				bboxText->rect.width = box->w;
				bboxText->rect.height = box->h;
				bboxText->rect.x = box->x;
				bboxText->confidence = api->MeanTextConf();
				bboxText->label = api->GetUTF8Text();
				//wxTreeItemId childId = treeCtrl->AppendItem(rootId, bboxText.label);

				wxTreeItemId id = treeCtrl->AppendItem(rootId, bboxText->label, -1, -1, bboxText);
				bboxText->SetId(id);
				listRect.push_back(bboxText);
			}

			// Get OCR result
			char * outText = api->GetUTF8Text();
			printf("OCR output:\n%s", outText);

			// Destroy used object and release memory
			api->End();

			delete[] outText;
		}
		catch (...)
		{

		}
		if (bitmap != nullptr)
			delete bitmap;
	}

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
	resourcePath = resourcePath + "\\tesseract";

	wxArrayString files;

	wxDir::GetAllFiles(resourcePath, &files, wxEmptyString, wxDIR_FILES);

	for (wxString file : files)
	{
		wxFileName fullpath(file);
		listFile.push_back(file);
		choice->Append(fullpath.GetName());
	}

	choice->SetSelection(0);
	gsizer->Add(choice, wxGBPosition(row, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);

	wxButton *defBut = new wxButton(panel, ID_BUT_OCR, _("&OCR"));
	gsizer->Add(defBut, wxGBPosition(row, 2), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);


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

/*
string COcrWnd::tesseract_ocr(string preprocessed_file)
{
	char *outText;
	Pix *image = pixRead(preprocessed_file.c_str());
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();


	TCHAR CurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurDir);

	api->Init(CurDir, "eng");
	api->SetPageSegMode(tesseract::PSM_AUTO_OSD); //PSM_SINGLE_BLOCK PSM_AUTO_OSD
	
	api->SetImage(image);

	outText = api->GetUTF8Text();

	system("cls");

	string out(outText);
	return out;
}


void toClipboard(const std::string s) {
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}
*/