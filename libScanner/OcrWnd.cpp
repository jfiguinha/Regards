#include <header.h>
#include "ExportOcr.h"
#include "OcrWnd.h"
#include <wx/gbsizer.h>
#include <FileUtility.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <BitmapWndViewer.h>
#include <BitmapWnd3d.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <directoryctrl.h>
#include <FiltreEffet.h>
#include <FilterData.h>
#include <wx/progdlg.h>
#include <ShowElement.h>
#include <LibResource.h>
#include <Draw.h>
#include <wx/filefn.h>
#include <ConvertUtility.h>

extern "C" {
#include <leptonica\environ.h>
#include <leptonica\pix.h>
#include <leptonica\pix_internal.h>
}
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
using namespace Regards::Picture;

COcrWnd::COcrWnd(wxWindow* parent, wxWindowID id)
	: CWindowMain("OCR Window", parent, id)
{
	pageInfo = nullptr;
	//bitmapBackground = nullptr;
	CThemeTree themeTree;
	listOcr = CreateListTesseract(this);

	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	CThemeScrollBar themeScroll;
	viewerTheme->GetScrollTheme(&themeScroll);

	viewerTheme->GetTreeTheme(&themeTree);
	ocrLabelWnd = new COcrLabelWnd(this, wxID_ANY, themeScroll, themeTree, this->GetId());
	ocrLabelWnd->Show(true);

	auto hsizer = new wxBoxSizer(wxHORIZONTAL);
	hsizer->Add(listOcr, 1, wxEXPAND | wxALL, 5);
	hsizer->Add(ocrLabelWnd, 2, wxEXPAND | wxALL, 5);


	Connect(ID_BUT_OCR, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcr));
	Connect(ID_BUT_OCRPDF, wxEVT_BUTTON, wxCommandEventHandler(COcrWnd::OnOcrPDF));
	Connect(wxEVENT_CHECKTREE_CHOICE, wxCommandEventHandler(COcrWnd::OnSelChanged), nullptr, this);
	Connect(wxEVENT_CHECKTREE_READ, wxCommandEventHandler(COcrWnd::OnSelRead), nullptr, this);
}

void COcrWnd::OnSelRead(wxCommandEvent& aEvent)
{
	int id = aEvent.GetInt();

	for (ChOcrElement* text : listRect)
	{
		if (text->itemClass == "ocr_line")
		{
			auto bboxText = static_cast<ChOcrElementLine*>(text);
			if (id == bboxText->id)
			{
				if (bboxText != nullptr)
					wxString label = bboxText->label;
				break;
			}
		}
	}
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
	CBitmapWndViewer* wndViewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(BITMAPWINDOWVIEWERIDPDF));
	if (bitmapWindow != nullptr)
	{
		wndViewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	if (wndViewer != nullptr)
	{
		wndViewer->RemoveListener();
	}
}

CImageLoadingFormat* COcrWnd::ApplyMouseMoveEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                   CDraw* dessin)
{
	return nullptr;
}

float COcrWnd::XDrawingPosition(const float& m_lx, const long& m_lHScroll, const float& ratio)
{
	int x = (m_lx * ratio) - m_lHScroll;
	return x;
}

float COcrWnd::YDrawingPosition(const float& m_ly, const long& m_lVScroll, const float& ratio)
{
	int y = (m_ly * ratio) - m_lVScroll;
	return y;
}

void COcrWnd::Drawing(wxMemoryDC* dc, IBitmapDisplay* bitmapViewer, CDraw* m_cDessin)
{
	int hpos = bitmapViewer->GetHPos();
	int vpos = bitmapViewer->GetVPos();

	for (ChOcrElement* text : listRect)
	{
		if (text->itemClass == "ocr_line")
		{
			auto bbox = static_cast<ChOcrElementLine*>(text);
			if (bbox->selected)
			{
				wxRect rcTemp;
				rcTemp.x = XDrawingPosition(bbox->rect.x, hpos, bitmapViewer->GetRatio());
				rcTemp.width = bbox->rect.width * bitmapViewer->GetRatio();
				rcTemp.y = YDrawingPosition(bbox->rect.y, vpos, bitmapViewer->GetRatio());
				rcTemp.height = bbox->rect.height * bitmapViewer->GetRatio();

				CDraw::DessinerRectangleVide(dc, 2, rcTemp, wxColor(0, 0, 255, 255));
			}
		}
	}
}

void COcrWnd::UpdateScreenRatio()
{
	this->Resize();
}

void COcrWnd::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                 CFiltreEffet* filtreEffet, CDraw* dessin, int& widthOutput, int& heightOutput)
{
	auto imageLoad = new CImageLoadingFormat();
	cv::Mat mat = filtreEffet->GetBitmap(false);
	imageLoad->SetPicture(mat);
	wxImage image = imageLoad->GetwxImage();

	auto bitmap = wxBitmap(image);
	wxMemoryDC dc;
	dc.SelectObject(bitmap);

	//wxImage render = filtreEffet->GetwxImage();

	Drawing(&dc, bitmapViewer, dessin);

	dc.SelectObject(wxNullBitmap);

	wxImage local_image(bitmap.ConvertToImage());
	imageLoad->SetPicture(local_image);
	filtreEffet->SetBitmap(imageLoad);
}

void COcrWnd::OnSelChanged(wxCommandEvent& aEvent)
{
	int id = aEvent.GetInt();
	int checked = aEvent.GetExtraLong();

	for (ChOcrElement* text : listRect)
	{
		if (text->itemClass == "ocr_line")
		{
			auto bboxText = static_cast<ChOcrElementLine*>(text);
			if (id == bboxText->id)
			{
				if (bboxText != nullptr)
					bboxText->selected = checked;
				break;
			}
		}
	}
	//GenerateLayerBitmap();
	auto viewer = static_cast<CBitmapWnd3D*>(FindWindowById(BITMAPWINDOWVIEWERIDPDF));
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
		char* args[8];
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
		CExportOcr::ExportOcr(8, args, error);

		for (int i1 = 0; i1 < 8; i1++)
			delete[] args[i1];


		wxRename(outputFile + "." + extension, outputFile);
	}
}

void COcrWnd::OnOcrPDF(wxCommandEvent& event)
{
	//int timeout_ms = 5000;
	//const char* retry_config = nullptr;
	//bool textonly = false;
	//int jpg_quality = 92;

	//Get select 
	//int i = choice->GetSelection();
	wxString language = choice->GetStringSelection();

	wxString preprocess = CFileUtility::GetTempFile("preprocess.bmp", false);
	wxString filename = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);
	wxString exportOcr = CLibResource::LoadStringFromResource(L"LBLEXPORTOCR", 1);

	//wxFileDialog saveFileDialog(this, _("Export OCR to ... "), "", "", "PDF files (*.pdf)|*.pdf | TXT files (*.txt)|*.txt | boxfile files (*.boxfile) | *.boxfile | hOcr files (*.hocr) | *.hocr | Odt files (*.odt) | *.odt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	wxFileDialog saveFileDialog(nullptr, exportOcr, "", "",
	                            "PDF " + filename + " (*.pdf)|*.pdf | TXT " + filename + " (*.txt)|*.txt |boxfile " +
	                            filename + " (*.boxfile) | *.boxfile | hOcr " + filename + " (*.hocr) | *.hocr",
	                            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return; // the user changed idea...

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

	for (ChOcrElement* element : listRect)
	{
		if (element != nullptr)
			delete element;
	}

	listRect.clear();

	//wxTreeItemId rootId = treeCtrl->AddRoot("Text");
	xml_document<> doc;
	// Read the xml file into a vector
	//const char * fichier = CConvertUtility::ConvertFromwxString(filename);
	ifstream theFile(CConvertUtility::ConvertToStdString(boxfile));
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	if (buffer.size() > 0)
	{
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);

		xml_node<>* root_node;
		//long nodeSize = 0;
		root_node = doc.first_node("html");
		xml_node<>* body_node = root_node->first_node("body");
		if (body_node != nullptr)
		{
			int numLigne = 0;
			xml_node<>* div_node = body_node->first_node("div");
			if (div_node != nullptr)
			{
				xml_attribute<char>* libelle = div_node->first_attribute("class");
				if (libelle != nullptr)
				{
					string ocr_libelle = libelle->value();
					if (ocr_libelle == "ocr_page")
					{
						pageInfo = new CPage();
						xml_attribute<>* libelleTitle = div_node->first_attribute("title");
						if (libelleTitle != nullptr)
						{
							wxString ocr_title = libelleTitle->value();
							pageInfo->rect = FindRect(ocr_title);
							pageInfo->resolution = 300;
						}
					}
				}
				xml_node<>* divchild_node = div_node->first_node("div");

				if (divchild_node != nullptr)
				{
					do
					{
						xml_node<>* p_node = divchild_node->first_node("p");
						if (p_node != nullptr)
						{
							xml_attribute<char>* attribute = p_node->first_attribute("class");
							if (attribute != nullptr)
							{
								string ocr_libelle = attribute->value();
								if (ocr_libelle == "ocr_par")
								{
									auto elementPar = new ChOcrElementPar();
									xml_attribute<>* libelleTitle = p_node->first_attribute("title");
									if (libelleTitle != nullptr)
									{
										wxString ocr_title = libelleTitle->value();
										elementPar->rect = FindRect(ocr_title);
										listRect.push_back(elementPar);
									}
								}
							}

							do
							{
								xml_node<>* spanligne_node = p_node->first_node("span");
								if (spanligne_node != nullptr)
								{
									do
									{
										xml_attribute<char>* attribute = spanligne_node->first_attribute("class");
										if (attribute != nullptr)
										{
											string ocr_libelle = attribute->value();
											if (ocr_libelle == "ocr_line" || ocr_libelle == "ocr_header")
											{
												//BBoxText * bboxText = new BBoxText();

												auto elementTitle = new ChOcrElementLine();
												xml_attribute<>* libelleTitle = spanligne_node->
													first_attribute("title");
												if (libelleTitle != nullptr)
												{
													wxString ocr_title = libelleTitle->value();
													elementTitle->rect = FindRect(ocr_title);
												}

												elementTitle->selected = true;
												wxString wx_uni_char_refs;
												xml_node<>* spanword_node = spanligne_node->first_node("span");
												if (spanword_node != nullptr)
												{
													do
													{
														xml_attribute<char>* lblClass = spanword_node->first_attribute(
															"class");
														string class_libelle = lblClass->value();
														wxString span_value =
															wxString::FromUTF8(spanword_node->value());
														if (spanword_node != nullptr)
															wx_uni_char_refs.append(span_value);

														spanword_node = spanword_node->next_sibling();
														if (spanword_node != nullptr)
															wx_uni_char_refs.append(" ");
													}
													while (spanword_node != nullptr);
												}

												elementTitle->numLigne = numLigne;
												elementTitle->label = wx_uni_char_refs;

												if (wx_uni_char_refs != "")
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
									}
									while (spanligne_node != nullptr);
								}
								p_node = p_node->next_sibling();
							}
							while (p_node != nullptr);
						}
						divchild_node = divchild_node->next_sibling();
					}
					while (divchild_node != nullptr);
				}
			}
		}
	}
	ocrLabelWnd->Update(listRect);
}

void COcrWnd::OnOcr(wxCommandEvent& event)
{
	CBitmapWndViewer* viewer = nullptr;
	auto bitmapWindow = dynamic_cast<IBitmapWnd*>(FindWindowById(BITMAPWINDOWVIEWERIDPDF));
	if (bitmapWindow != nullptr)
	{
		viewer = static_cast<CBitmapWndViewer*>(bitmapWindow->GetWndPt());
	}

	if (viewer != nullptr)
	{
		viewer->SetListener(this);
		viewer->SetBitmapPreviewEffect(IDM_OCR);
	}

	//Get select 
	//int i = choice->GetSelection();
	wxString language = choice->GetStringSelection();

	auto showBitmap = static_cast<CShowElement*>(FindWindowById(SHOWBITMAPVIEWERIDPDF));
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

			CImageLoadingFormat* bitmapBackground = showBitmap->GetBitmap(true);

			CLibPicture libPicture;
			wxString tempFile = CFileUtility::GetTempFile("temp.bmp");
			libPicture.SavePicture(tempFile, bitmapBackground, 0, 0);

			wxString preprocess = CFileUtility::GetTempFile("preprocess.bmp");
			wxString outputFile = CFileUtility::GetTempFile("ocrfile.hocr");

			tesseract_preprocess(CConvertUtility::ConvertToStdString(tempFile),
			                     CConvertUtility::ConvertToStdString(preprocess));

			OcrToPDF(preprocess, outputFile, language);

			//outputFile.append(".hocr");
			LoadOcrBoxFile(outputFile);

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
	wxWindow* local = FindWindowById(BITMAPWINDOWVIEWERIDPDF);
	if (local != nullptr)
		local->Refresh();
}

wxPanel* COcrWnd::CreateListTesseract(wxWindow* parent)
{
	auto panel = new wxPanel(parent);
	auto sizer = new wxBoxSizer(wxVERTICAL);

	auto sbox = new wxStaticBox(panel, wxID_ANY, wxString("Language : "));
	auto sbsizer = new wxStaticBoxSizer(sbox, wxVERTICAL);
	auto gsizer = new wxGridBagSizer(5, 5);
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


void COcrWnd::tesseract_preprocess(wxString source_file, wxString out_file)
{
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


	l_int32 status;
	PIX* pixs = nullptr;
	//char *ext = NULL;


	// Read in source image 
	pixs = pixRead(source_file.c_str());


	// Convert to grayscale 
	pixs = pixConvertRGBToGray(pixs, 0.0f, 0.0f, 0.0f);


	if (perform_negate)
	{
		PIX* otsu_pixs = nullptr;

		status = pixOtsuAdaptiveThreshold(pixs, otsu_sx, otsu_sy, otsu_smoothx, otsu_smoothy, otsu_scorefract, nullptr,
		                                  &otsu_pixs);


		// Get the average intensity of the border pixels,
		//with average of 0.0 being completely white and 1.0 being completely black. 
		l_float32 border_avg = pixAverageOnLine(otsu_pixs, 0, 0, otsu_pixs->w - 1, 0, 1); // Top 
		border_avg += pixAverageOnLine(otsu_pixs, 0, otsu_pixs->h - 1, otsu_pixs->w - 1, otsu_pixs->h - 1, 1);
		// Bottom 
		border_avg += pixAverageOnLine(otsu_pixs, 0, 0, 0, otsu_pixs->h - 1, 1); // Left 
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
		status = pixOtsuAdaptiveThreshold(pixs, otsu_sx, otsu_sy, otsu_smoothx, otsu_smoothy, otsu_scorefract, nullptr,
		                                  &pixs);
	}


	// Write the image to file 
	status = pixWriteImpliedFormat(out_file.c_str(), pixs, 0, 0);

}
