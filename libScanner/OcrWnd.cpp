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
	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	hsizer->Add(CreateListTesseract(this), 1, wxEXPAND | wxALL, 5);


	Connect(ID_BUT_OCR, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcr));
}

COcrWnd::~COcrWnd()
{

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

			// Open input image with leptonica library
			//Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
			api->SetImage(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(),
				4, 4 * bitmap->GetBitmapWidth());

			Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
			printf("Found %d textline image components.\n", boxes->n);
			for (int i = 0; i < boxes->n; i++) {
				BOX* box = boxaGetBox(boxes, i, L_CLONE);
				api->SetRectangle(box->x, box->y, box->w, box->h);
				BBoxText bboxText;


				//char* ocrResult = api->GetUTF8Text();
				//int conf = api->MeanTextConf();
				//fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
				//	i, box->x, box->y, box->w, box->h, conf, ocrResult);

				bboxText.rect.x = box->x;
				bboxText.rect.y = box->y;
				bboxText.rect.width = box->w;
				bboxText.rect.height = box->h;
				bboxText.rect.x = box->x;
				bboxText.confidence = api->MeanTextConf();
				bboxText.label = api->GetUTF8Text();

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
	gsizer->Add(defBut, wxGBPosition(row + 1, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	//choiceValue.id = choice->GetId();
	//Connect(choice->GetId(), wxEVT_CHOICE, wxCommandEventHandler(wxScanSaneAcquireDialog::OnChoiceSelected));

	panel->SetSizer(sizer);
	sizer->SetSizeHints(panel);
	return panel;
}