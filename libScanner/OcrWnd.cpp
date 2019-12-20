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
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "ScannerParam.h"
#include "ScannerParamInit.h"
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <directoryctrl.h>
#include <FiltreEffet.h>
#include <FilterData.h>
#include <wx/progdlg.h>
#include "ExportOcr.h"
#include <wx/filefn.h>
#include <ConvertUtility.h>
//#include "ExportOdt.h"
#ifdef __APPLE__
#undef fract1
#endif
#include <leptonica/allheaders.h>
enum
{
	ID_BUT_OCR = 3000,
	ID_BUT_OCRPDF = ID_BUT_OCR + 1,
	ID_BUT_EXPORT = ID_BUT_OCR + 2,
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

	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	CThemeScrollBar themeScroll;
	viewerTheme->GetScrollTheme(&themeScroll);

	viewerTheme->GetTreeTheme(&themeTree);
	ocrLabelWnd = new COcrLabelWnd(this, wxID_ANY, themeScroll, themeTree, this->GetId());
	ocrLabelWnd->Show(true);

	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	hsizer->Add(listOcr, 1, wxEXPAND | wxALL, 5);
	hsizer->Add(ocrLabelWnd, 2, wxEXPAND | wxALL, 5);


	
	Connect(ID_BUT_OCR, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcr));
	Connect(ID_BUT_OCRPDF, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcrPDF));
	//Connect(ID_BUT_EXPORT, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnExport));
	Connect(wxEVENT_CHECKTREE_CHOICE, wxCommandEventHandler(COcrWnd::OnSelChanged), NULL, this);
	Connect(wxEVENT_CHECKTREE_READ, wxCommandEventHandler(COcrWnd::OnSelRead), NULL, this);
	

}

void COcrWnd::OnExport(wxCommandEvent& event)
{
#ifdef WIN32
	CShowBitmap * showBitmap = (CShowBitmap *)wxWindow::FindWindowById(SHOWBITMAPVIEWERIDPDF);
	if (showBitmap != nullptr)
	{
		CRegardsBitmap * bitmapBackground = showBitmap->GetBitmap(true);
		bitmapBackground->VertFlipBuf();
		for (ChOcrElement * text : listRect)
		{
			if (text->itemClass == "ocr_line")
			{
				ChOcrElementLine * bboxText = (ChOcrElementLine *)text;
				CRgbaquad color = CRgbaquad(255, 255, 255);
				uint8_t * data = bitmapBackground->GetPtBitmap();
				int maxY = bboxText->rect.y + bboxText->rect.height;
				int maxX = bboxText->rect.x + bboxText->rect.width;
				for (int y = bboxText->rect.y; y < maxY; y++)
				{
					for (int x = bboxText->rect.x; x < maxX; x++)
					{
						bitmapBackground->SetColorValue(x, y, color);
					}
				}
			}
		}
		bitmapBackground->VertFlipBuf();
		bitmapBackground->SaveToBmp("c:\\developpement\\test.bmp");
		delete bitmapBackground;
	}
#endif
}

void COcrWnd::OnSelRead(wxCommandEvent& aEvent)
{
	int id = aEvent.GetInt();
	wxString label = "";

	for (ChOcrElement * text : listRect)
	{
		if (text->itemClass == "ocr_line")
		{
			ChOcrElementLine * bboxText = (ChOcrElementLine *)text;
			if (id == bboxText->id)
			{
				if (bboxText != nullptr)
					label = bboxText->label;
				break;
			}
		}
	}

	/*
	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
#ifdef WIN32
	resourcePath = resourcePath + "\\espeak";
	wxString language = choice->GetStringSelection();
	wxString link = resourcePath + "\\espeak-ng.exe \"" + label + "\" -v" + language.substr(0,2) + " --path=" + resourcePath;
	wxExecute(link);
#else
	resourcePath = resourcePath + "/espeak";
#endif    
	*/
}

void COcrWnd::Init()
{
	ocrPdf->Enable(true);
	exportPdf->Enable(false);
	ocrLabelWnd->Init();
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

	for (ChOcrElement * text : listRect)
	{
		if (text->itemClass == "ocr_line")
		{
			ChOcrElementLine * bbox = (ChOcrElementLine *)text;
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
	int id = aEvent.GetInt();
	int checked = aEvent.GetExtraLong();

	for (ChOcrElement * text : listRect)
	{
		if (text->itemClass == "ocr_line")
		{
			ChOcrElementLine * bboxText = (ChOcrElementLine *)text;
			if (id == bboxText->id)
			{
				if (bboxText != nullptr)
					bboxText->selected = checked;
				break;
			}
		}
	}
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
	ocrLabelWnd->SetSize(0, panelListOcrH, width, height - panelListOcrH);
}

void COcrWnd::OcrToPDF(wxString bitmapFile, wxString outputFile, wxString language)
{
 
	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
#ifdef WIN32
	resourcePath = resourcePath + "\\tessdata";
#else
	resourcePath = resourcePath + "/tessdata";
#endif    
	
	wxFileName fullpath(outputFile);
	wxString extension = fullpath.GetExt();    
    
	if (extension == "odt")
	{
		/*
		CExportOdt exportOdt;
		pageInfo->resolution = 72;
		bool value = exportOdt.Export(pageInfo, listRect, outputFile);
		*/
	}
	else
	{
		int i = 0;
		char * args[8];
		args[i++] = new char[255];
		args[i] = new char[255];
		strcpy(args[i++], bitmapFile);
		args[i] = new char[255];
		strcpy(args[i++], outputFile);
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


		wxRename(outputFile + "." + extension, outputFile);
	}
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

	wxString preprocess = CFileUtility::GetTempFile("preprocess.bmp", false);

	//wxFileDialog saveFileDialog(this, _("Export OCR to ... "), "", "", "PDF files (*.pdf)|*.pdf | TXT files (*.txt)|*.txt | boxfile files (*.boxfile) | *.boxfile | hOcr files (*.hocr) | *.hocr | Odt files (*.odt) | *.odt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	wxFileDialog saveFileDialog(this, _("Export OCR to ... "), "", "", "PDF files (*.pdf)|*.pdf | TXT files (*.txt)|*.txt | boxfile files (*.boxfile) | *.boxfile | hOcr files (*.hocr) | *.hocr", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	wxString newfilename = saveFileDialog.GetPath();


    OcrToPDF(preprocess, newfilename, language);
}

wxRect COcrWnd::FindRect(wxString ocr_title)
{
	wxRect rect;
	vector<wxString> listLabel = CConvertUtility::split(ocr_title, ';');
	for (wxString text : listLabel)
	{
		int pos = text.Find("bbox");
		if (pos != -1)
		{
			int sizebbox = 5;
			wxString coord = text.substr(pos + sizebbox, text.length() - pos + sizebbox);
			vector<wxString> listCoord = CConvertUtility::split(coord, ' ');
			rect.x = atoi(listCoord[0]);
			rect.y = atoi(listCoord[1]);
			rect.width = atoi(listCoord[2]) - rect.x;
			rect.height = atoi(listCoord[3]) - rect.y;
			break;
		}
	}
	return rect;
}


//////////////////////////////////////////////////////////////////////////////////////////
//Loading Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void COcrWnd::LoadOcrBoxFile(wxString boxfile)
{
	int id = 0;

	if (pageInfo != nullptr)
		delete pageInfo;

	for (ChOcrElement * element : listRect)
	{
		if (element != nullptr)
			delete element;
	}

	listRect.clear();

	//wxTreeItemId rootId = treeCtrl->AddRoot("Text");
	xml_document<> doc;
	// Read the xml file into a vector
	//const char * fichier = CConvertUtility::ConvertFromwxString(filename);
	ifstream theFile(boxfile.ToStdString());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	if (buffer.size() > 0)
	{
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);

		xml_node<> * root_node;
		//long nodeSize = 0;
		root_node = doc.first_node("html");
		xml_node<> * body_node = root_node->first_node("body");
		if (body_node != 0)
		{
			int numLigne = 0;
			xml_node<> * div_node = body_node->first_node("div");
			if (div_node != 0)
			{
				xml_attribute<char> * libelle = div_node->first_attribute("class");
				if (libelle != 0)
				{
					string ocr_libelle = libelle->value();
					if (ocr_libelle == "ocr_page")
					{
						pageInfo = new CPage();
						xml_attribute<> * libelleTitle = div_node->first_attribute("title");
						if (libelleTitle != 0)
						{
							wxString ocr_title = libelleTitle->value();
							pageInfo->rect = FindRect(ocr_title);
							pageInfo->resolution = 300;
						}
					}
				}
				xml_node<> * divchild_node = div_node->first_node("div");

				if (divchild_node != 0)
				{
					do
					{
						xml_node<> * p_node = divchild_node->first_node("p");
						if (p_node != 0)
						{
							xml_attribute<char> * libelle = p_node->first_attribute("class");
							if (libelle != 0)
							{
								string ocr_libelle = libelle->value();
								if (ocr_libelle == "ocr_par")
								{
									ChOcrElementPar * elementPar = new ChOcrElementPar();
									xml_attribute<> * libelleTitle = p_node->first_attribute("title");
									if (libelleTitle != 0)
									{
										wxString ocr_title = libelleTitle->value();
										elementPar->rect = FindRect(ocr_title);
										listRect.push_back(elementPar);
									}
								}
							}

							do
							{
								xml_node<> * spanligne_node = p_node->first_node("span");
								if (spanligne_node != 0)
								{
									do
									{
										xml_attribute<char> * libelle = spanligne_node->first_attribute("class");
										if (libelle != 0)
										{
											string ocr_libelle = libelle->value();
											if (ocr_libelle == "ocr_line" || ocr_libelle == "ocr_header")
											{
												//BBoxText * bboxText = new BBoxText();

												ChOcrElementLine * elementTitle = new ChOcrElementLine();
												xml_attribute<> * libelleTitle = spanligne_node->first_attribute("title");
												if (libelleTitle != 0)
												{
													wxString ocr_title = libelleTitle->value();
													elementTitle->rect = FindRect(ocr_title);
												}

												elementTitle->selected = true;
												wxString libelle;
												xml_node<> * spanword_node = spanligne_node->first_node("span");
												if (spanword_node != 0)
												{
													do
													{
														xml_attribute<char> * lblClass = spanword_node->first_attribute("class");
														string class_libelle = lblClass->value();
														wxString span_value = wxString::FromUTF8(spanword_node->value());
														if (spanword_node != 0)
															libelle.append(span_value);

														spanword_node = spanword_node->next_sibling();
														if (spanword_node != 0)
															libelle.append(" ");
													} while (spanword_node != 0);
												}

												elementTitle->numLigne = numLigne;
												elementTitle->label = libelle;

												if (libelle != "")
												{
													elementTitle->id = id++;
													listRect.push_back(elementTitle);
													numLigne++;
												}
												else
												{
													delete elementTitle;
												}
											}
										}
										spanligne_node = spanligne_node->next_sibling();
									} while (spanligne_node != 0);
								}
								p_node = p_node->next_sibling();
							} while (p_node != 0);
						}
						divchild_node = divchild_node->next_sibling();
					} while (divchild_node != 0);
				}
			}
		}
	}
	ocrLabelWnd->Update(listRect);
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
#ifdef WIN32
	resourcePath = resourcePath + "\\tessdata";
#else
	resourcePath = resourcePath + "/tessdata";
#endif    

			//ETEXT_DESC *monitor = new ETEXT_DESC();

			CRegardsBitmap * bitmapBackground = showBitmap->GetBitmap(true);

			// Initialize tesseract-ocr with English, without specifying tessdata path
			/*
			if (CExportOcr::api.Init(resourcePath, language)) {
				fprintf(stderr, "Could not initialize tesseract.\n");
				throw("Could not initialize tesseract.\n");
			}
			*/
			CLibPicture libPicture;
			wxString tempFile = CFileUtility::GetTempFile("temp.bmp");
			CImageLoadingFormat loadingformat(false);
			loadingformat.SetPicture(bitmapBackground);
			libPicture.SavePicture(tempFile, &loadingformat, 0, 0);

			

			wxString preprocess = CFileUtility::GetTempFile("preprocess.bmp");
			wxString outputFile = CFileUtility::GetTempFile("ocrfile.hocr");

			tesseract_preprocess(tempFile.ToStdString(), preprocess.ToStdString());

			OcrToPDF(preprocess, outputFile, language);

			//outputFile.append(".hocr");
			LoadOcrBoxFile(outputFile);

			/*
			Pix *image = pixRead(preprocess.ToStdString().c_str());

			wxTreeItemId rootId = treeCtrl->AddRoot("Text");

			//api->Init(CurDir, "eng");

			CExportOcr::api.SetPageSegMode(tesseract::PSM_AUTO_OSD); //PSM_SINGLE_BLOCK PSM_AUTO_OSD

			CExportOcr::api.SetImage(image);


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
			*/
			exportPdf->Enable(true);
			ocrPdf->Enable(false);

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
#ifdef WIN32
	resourcePath = resourcePath + "\\tessdata";
#else
	resourcePath = resourcePath + "/tessdata";
#endif    

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
	ocrPdf = new wxButton(panel, ID_BUT_OCR, _("&OCR"));
	//exportImage = new wxButton(panel, ID_BUT_EXPORT, _("&Export"));
	gsizer->Add(ocrPdf, wxGBPosition(row, 2), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	gsizer->Add(exportPdf, wxGBPosition(row, 3), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	//gsizer->Add(exportImage, wxGBPosition(row, 4), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxEXPAND);
	exportPdf->Enable(false);
	ocrPdf->Enable(true);
	//exportImage->Enable(true);
	panel->SetSizer(sizer);
	sizer->SetSizeHints(panel);
	return panel;
}


void COcrWnd::tesseract_preprocess(wxString source_file, wxString out_file) {

	/*
	char tempPath[128];
	GetTempPathA(128, tempPath);
	strcat_s(tempPath, "preprocess_ocr.bmp");

	char preprocessed_file[MAX_PATH];
	strcpy_s(preprocessed_file, tempPath);
	*/

	bool perform_negate = TRUE;
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
