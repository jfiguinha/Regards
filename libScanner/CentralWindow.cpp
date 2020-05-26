#include <header.h>
#include "CentralWindow.h"
#include "ToolbarPDF.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ConfigParam.h>
#include <ImageLoadingFormat.h>
#include <window_id.h>
#include <qpdf/QPDF.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>
#include <qpdf/QPDFWriter.hh>
#include <qpdf/QUtil.hh>
#include "SelectPage.h"
#include <FileUtility.h>
#include <ConvertUtility.h>
#include <libPicture.h>
#include <picture_id.h>
using namespace Regards::Window;

CCentralWindow::CCentralWindow(wxWindow* parent, wxWindowID id, CScannerFrame * frame)
	: CWindowMain("CentralWindow", parent, id)
{
	//CMainTheme * viewerTheme = new CMainTheme();

#ifdef __SCANNER_PROGRAM__
	Regards::Scanner::CMainTheme * viewerTheme = CMainThemeInit::getInstance();
#else
	Regards::Viewer::CMainTheme * viewerTheme = CMainThemeInit::getInstance();
#endif
	CThemeSplitter theme;
	viewerTheme->GetSplitterTheme(&theme);
	CThemeToolbar themeClickInfosToolbar;
	viewerTheme->GetClickToolbarTheme(&themeClickInfosToolbar);

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetMainToolbarTheme(&theme);
		toolbarPDF = new CToolbarPDF(this, wxID_ANY, theme, false);
		toolbarPDF->Show(true);
	}

	previewWindow = new CViewerPDF(this, frame, PDFWINDOWID);
	previewWindow->Show(true);

	this->frame = frame;

	Connect(wxEVT_SIZE, wxSizeEventHandler(CCentralWindow::OnSize));
	Connect(wxEVENT_OPENFILE, wxCommandEventHandler(CCentralWindow::OnOpenFile));
	Connect(wxEVENT_SCANNER, wxCommandEventHandler(CCentralWindow::OnScan));
	Connect(wxEVENT_PRINT, wxCommandEventHandler(CCentralWindow::OnPrint));
	Connect(wxEVT_EXIT, wxCommandEventHandler(CCentralWindow::OnExit));
	Connect(wxEVENT_SAVE, wxCommandEventHandler(CCentralWindow::OnSave));
	Connect(wxEVENT_ADDPAGE, wxCommandEventHandler(CCentralWindow::OnAddPage));
	Connect(wxEVENT_DELETEPAGE, wxCommandEventHandler(CCentralWindow::OnDeletePage));
	Connect(wxEVENT_EXTRACT, wxCommandEventHandler(CCentralWindow::OnExtractPage));
}

void CCentralWindow::OnExtractPage(wxCommandEvent& event)
{
	if (filename != "")
	{
		CSelectFileDlg selectFile(this, -1, filename, _("Select Page To Extract"));
		if (selectFile.ShowModal() == wxID_OK)
		{
			vector<int> listPage = selectFile.GetSelectItem();
			wxString fileExtract = ProcessExtractFile(listPage);

			wxFileDialog saveFileDialog(this, _("Save Extract PDF page"), "", "",
				"PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (saveFileDialog.ShowModal() == wxID_CANCEL)
				return;     // the user changed idea...

			wxString newfilename = saveFileDialog.GetPath();
			wxCopyFile(fileExtract, newfilename);
		}

	}
	else
	{
		wxMessageBox("Please Open a File !", "Error", wxICON_INFORMATION);
	}
}

void CCentralWindow::OnDeletePage(wxCommandEvent& event)
{
	if (filename != "")
	{
		CSelectFileDlg selectFile(this, -1, filename, _("Select Page To Delete"));
		if (selectFile.ShowModal() == wxID_OK)
		{
			vector<int> listPage = selectFile.GetSelectItem();
			ProcessFile(listPage);
			LoadFile(filename);
		}

	}
	else
	{
		wxMessageBox("Please Open a File !", "Error", wxICON_INFORMATION);
	}
}


void CCentralWindow::OnOpen()
{
	wxArrayString list;
	list.push_back("Scan");
	list.push_back("File");
	bool isOk = false;
	wxString file = "";
	int numSelect = wxGetSingleChoiceIndex("Select Source : ", "Source", list, 0, this);
	if (numSelect != -1)
	{
		if (numSelect == 0)
		{
			wxCommandEvent evt(wxEVENT_SCANNER);
			this->GetEventHandler()->AddPendingEvent(evt);
		}
		else
		{
			wxCommandEvent evt(wxEVENT_OPENFILE);
			this->GetEventHandler()->AddPendingEvent(evt);
		}

	}
}

void CCentralWindow::OnAddPage(wxCommandEvent& event)
{
	if (filename != "")
	{
		wxArrayString list;
		list.push_back("Scan");
		list.push_back("File");
		bool isOk = false;
		int numSelect = wxGetSingleChoiceIndex("Select Source : ", "Source", list, 0, this);
		int oldAnimationPosition = previewWindow->GetAnimationPosition();
		if (numSelect != -1)
		{
			if (numSelect == 0)
			{
				vector<int> listPage;
				listPage.push_back(0);
				wxString file = frame->ScanPage();
				if (file != "")
				{
					ProcessAddFile(file, filename, listPage, oldAnimationPosition);
					isOk = true;
				}
			}
			else
			{
				CLibPicture libPicture;
				wxString szFilter = "";
				std::vector<wxString> v = { ".pdf",".pnm",".bmp",".bpg",".pcx",".jpg",".tif",".gif",".png",".tga",".jp2",".jpc",".ppm",".mng",".webp",".iff",".xpm",".jxr",".exr",".j2k",".pfm" };
				szFilter = "Files PDF(*.PDF) | *.pdf|Files PNM (*.PNM)|*.pnm|Files BMP(*.BMP)|*.bmp|Files BPG(*.BPG)|*.bpg|Files PCX(*.PCX)|*.pcx|Files JPEG(*.JPG)|*.jpg|Files TIFF(*.TIF)|*.tif|Files GIF(*.GIF)|*.gif| Files PNG(*.PNG)|*.png|Files TGA(*.TGA)|*.tga|Files JPEG2000(*.JP2)|*.jp2|Files JPC(*.JPC)|*.jpc|Files PPM(*.PPM)|*.ppm|Files MNG(*.MNG)|*.mng|Files WEBP (*.WEBP)|*.webp|Files IFF (*.IFF)|*.iff|Files XPM (*.XPM)|*.xpm|Files JXR (*.JXR)|*.jxr|Files EXR (*.EXR)|*.exr|Files J2K (*.J2K)|*.j2k|Files PFM (*.PFM)|*.pfm";

				wxFileDialog openFileDialog(this, _("Open Picture file"), "", "",
					szFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

				//wxFileDialog openFileDialog(this, _("Open PDF file"), "", "",
				//	"PDF files (*.pdf)|*.pdf", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
				if (openFileDialog.ShowModal() == wxID_CANCEL)
					return;     // the user changed idea..

				

				wxString fileToadd = openFileDialog.GetPath();

				if (libPicture.TestIsPDF(fileToadd))
				{
					CSelectFileDlg selectFile(this, -1, fileToadd, _("Select Page To Add"));
					if (selectFile.ShowModal() == wxID_OK)
					{
						vector<int> listPage = selectFile.GetSelectItem();
						ProcessAddFile(fileToadd, filename, listPage, oldAnimationPosition);
						isOk = true;
					}
				}
				else
				{
					wxString documentPath = CFileUtility::GetDocumentFolderPath();
					wxString fileToAdd = "";

					int option = 0;
					int quality = 0;
					libPicture.SavePictureOption(PDF, option, quality);



#ifdef WIN32
					wxString tempFolder = documentPath + "\\temp";
					if (!wxMkDir(tempFolder)) {
#else
					wxString tempFolder = documentPath + "/temp";
					if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
#endif
						// handle the error here
					}
					else
					{
#ifdef WIN32
						fileToAdd = tempFolder + "\\fileToAdd.pdf";
#else
						fileToAdd = tempFolder + "/fileToAdd.pdf";
#endif

						if (wxFileExists(fileToAdd))
							wxRemoveFile(fileToAdd);

					}
					vector<int> listPageToAdd;
					listPageToAdd.push_back(0);
					if (libPicture.TestIsAnimation(fileToadd))
					{
						CSelectFileDlg selectFile(this, -1, fileToadd, _("Select Page To Add"));
						if (selectFile.ShowModal() == wxID_OK)
						{
							vector<int> listPage = selectFile.GetSelectItem();
							for (int i = 0; i < listPage.size(); i++)
							{

								CImageLoadingFormat * imageLoadingFormat = libPicture.LoadPicture(fileToadd, false, listPage[i]);
								libPicture.SavePicture(fileToAdd, imageLoadingFormat, option, quality);
								ProcessAddFile(fileToadd, filename, listPageToAdd, oldAnimationPosition);
								if (wxFileExists(fileToAdd))
									wxRemoveFile(fileToAdd);
								delete imageLoadingFormat;
								oldAnimationPosition++;
							}
							isOk = true;
						}
					}
					else
					{
						CImageLoadingFormat * imageLoadingFormat = libPicture.LoadPicture(fileToadd, false);
						libPicture.SavePicture(fileToAdd, imageLoadingFormat, option, quality);
						ProcessAddFile(fileToAdd, filename, listPageToAdd, oldAnimationPosition);
						if (wxFileExists(fileToAdd))
							wxRemoveFile(fileToAdd);
						delete imageLoadingFormat;
						isOk = true;
						oldAnimationPosition++;
					}
				}
			}
			if (isOk)
			{
				LoadFile(filename);
				previewWindow->SetPosition(oldAnimationPosition);
			}
		}
	}
	else
	{
		wxMessageBox("Please Open a File !", "Error", wxICON_INFORMATION);
	}

}

void CCentralWindow::OnSave(wxCommandEvent& event)
{
	if (filename != "")
	{
		wxFileDialog saveFileDialog(this, _("Save PDF file"), "", "",
			"PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return;     // the user changed idea...

		wxString newfilename = saveFileDialog.GetPath();
		wxCopyFile(filename, newfilename);

		LoadFile(newfilename);
	}
	else
	{
		wxMessageBox("Please Open a File !", "Error", wxICON_INFORMATION);
	}
}

void CCentralWindow::OnOpenFile(wxCommandEvent& event)
{
	filename = LoadFile();
}

void CCentralWindow::OnScan(wxCommandEvent& event)
{
	
	filename = frame->ScanPage();
	if (filename != "")
	{
		LoadFile(filename);
	}    
}

void CCentralWindow::OnPrint(wxCommandEvent& event)
{
	if (filename != "")
	{
		frame->PrintPreview(GetImage());
	}
	else
	{
		wxMessageBox("Please Open a File !", "Error", wxICON_INFORMATION);
	}
}

void CCentralWindow::OnExit(wxCommandEvent& event)
{
	frame->OnClose();
}


wxString CCentralWindow::LoadFile()
{
	wxString szFilter = "";
	std::vector<wxString> v = { ".pdf",".pnm",".bmp",".bpg",".pcx",".jpg",".tif",".gif",".png",".tga",".jp2",".jpc",".ppm",".mng",".webp",".iff",".xpm",".jxr",".exr",".j2k",".pfm" };
	szFilter = "Files PDF(*.PDF) | *.pdf|Files PNM (*.PNM)|*.pnm|Files BMP(*.BMP)|*.bmp|Files BPG(*.BPG)|*.bpg|Files PCX(*.PCX)|*.pcx|Files JPEG(*.JPG)|*.jpg|Files TIFF(*.TIF)|*.tif|Files GIF(*.GIF)|*.gif| Files PNG(*.PNG)|*.png|Files TGA(*.TGA)|*.tga|Files JPEG2000(*.JP2)|*.jp2|Files JPC(*.JPC)|*.jpc|Files PPM(*.PPM)|*.ppm|Files MNG(*.MNG)|*.mng|Files WEBP (*.WEBP)|*.webp|Files IFF (*.IFF)|*.iff|Files XPM (*.XPM)|*.xpm|Files JXR (*.JXR)|*.jxr|Files EXR (*.EXR)|*.exr|Files J2K (*.J2K)|*.j2k|Files PFM (*.PFM)|*.pfm";

	wxFileDialog openFileDialog(this, _("Open Picture file"), "", "",
		szFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return "";     // the user changed idea..


	//int filterIndex = openFileDialog.GetFilterIndex();
	wxString file = openFileDialog.GetPath();
	/*
	wxString ext = v.at(filterIndex);

	if (ext != ".pdf")
	{
		//Convert to pdf
	}
	*/
	return LoadFile(file);
	

}

wxString CCentralWindow::LoadFile(const wxString &filename)
{
    this->filename = filename;
	if (previewWindow != nullptr)
		previewWindow->LoadFile(filename);
	return previewWindow->GetFilename();
}

CImageLoadingFormat * CCentralWindow::GetImage()
{
	if (previewWindow != nullptr)
		return previewWindow->GetImage();
	return nullptr;
}

wxString CCentralWindow::GetFilename()
{
	if (previewWindow != nullptr)
		return previewWindow->GetFilename();
	return "";
}

void CCentralWindow::OnSize(wxSizeEvent& event)
{

	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	RedrawBarPos();
}

void CCentralWindow::RedrawBarPos()
{
	int topHeight = toolbarPDF->GetNavigatorHeight();
	toolbarPDF->SetSize(0, 0, width, topHeight);
	toolbarPDF->Refresh();

	if (previewWindow != nullptr)
	{
		previewWindow->SetSize(0, topHeight, width, height - topHeight);
	}
}


CCentralWindow::~CCentralWindow()
{
	if (previewWindow != nullptr)
		delete previewWindow;

	if(toolbarPDF != nullptr)
		delete(toolbarPDF);

}

void CCentralWindow::UpdateScreenRatio()
{
	this->Resize();
}

void CCentralWindow::ProcessAddFile(const wxString &fileToAdd, const wxString &filename, const vector<int> & listPage, int oldAnimationPosition)
{
	wxString file = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
    
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
#endif
		// handle the error here
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\add.pdf";
#else
		file = tempFolder + "/add.pdf";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

	}

	if (file != "")
	{
		QPDF oldpdf;
		oldpdf.processFile(filename.ToStdString().c_str());

		QPDF inpdf;
		inpdf.processFile(fileToAdd.ToStdString().c_str());

		std::string outfile = file.ToStdString();
		QPDF outpdf;
		outpdf.emptyPDF();

		std::vector<QPDFPageObjectHelper> oldpages = QPDFPageDocumentHelper(oldpdf).getAllPages();
		int oldpageno_len = oldpages.size();

		int i = 0;
		//std::vector<QPDFPageObjectHelper>::iterator iter = oldpages.begin();
		for (; i <= oldAnimationPosition; i++)
		{
			auto object = oldpages.at(i);
			QPDFPageObjectHelper& page(object);
			QPDFPageDocumentHelper(outpdf).addPage(page, false);
		}




		std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(inpdf).getAllPages();
		int pageno_len = pages.size();
		int pageno = 0;


		for (std::vector<QPDFPageObjectHelper>::iterator newiter = pages.begin(); newiter != pages.end(); ++newiter)
		{
			bool find = false;
			for (int i : listPage)
			{
				if (i == pageno)
				{
					find = true;
					break;
				}
			}

			if (find)
			{
				QPDFPageObjectHelper& page(*newiter);
				QPDFPageDocumentHelper(outpdf).addPage(page, false);
			}

			pageno++;
		}

		//for (int i = oldAnimationPosition; i < oldpageno_len; i++)
		for (; i < oldpageno_len; i++)
		{
			auto object = oldpages.at(i);
			QPDFPageObjectHelper& page(object);
			QPDFPageDocumentHelper(outpdf).addPage(page, false);
		}

		QPDFWriter outpdfw(outpdf, outfile.c_str());
		outpdfw.write();
	}
#ifndef DEMO
	if (wxFileExists(filename))
	{
		bool isRemove = wxRemoveFile(filename);
		if (!isRemove)
		{
			printf("not remove");
		}
	}

	wxCopyFile(file, filename);
#endif
}



void CCentralWindow::ProcessFile(const vector<int> & listPage)
{
	wxString file = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
#endif
		// handle the error here
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\delete.pdf";
#else
		file = tempFolder + "/delete.pdf";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

	}

	QPDF inpdf;
	inpdf.processFile(filename.ToStdString().c_str());
	std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(inpdf).getAllPages();
	//int pageno_len = QIntC::to_int(QUtil::uint_to_string(pages.size()).length());
	int pageno = 0;

	std::string outfile = file.ToStdString();
	QPDF outpdf;
	outpdf.emptyPDF();


	for (std::vector<QPDFPageObjectHelper>::iterator iter = pages.begin(); iter != pages.end(); ++iter)
	{
		bool find = false;
		for (int i : listPage)
		{
			if (i == pageno)
			{
				find = true;
				break;
			}
		}

		if (!find)
		{
			QPDFPageObjectHelper& page(*iter);
			QPDFPageDocumentHelper(outpdf).addPage(page, false);
		}

		pageno++;
	}

	QPDFWriter outpdfw(outpdf, outfile.c_str());
	outpdfw.write();

#ifndef DEMO
	if (wxFileExists(filename))
		wxRemoveFile(filename);

	wxCopyFile(file, filename);
#endif
}



wxString CCentralWindow::ProcessExtractFile(const vector<int> & listPage)
{
	wxString file = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
    if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
    if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
#endif
		// handle the error here
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\extract.pdf";
#else
		file = tempFolder + "/extract.pdf";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

	}

	QPDF inpdf;
	inpdf.processFile(filename.ToStdString().c_str());
	std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(inpdf).getAllPages();
	//int pageno_len = QIntC::to_int(QUtil::uint_to_string(pages.size()).length());
	int pageno = 0;

	std::string outfile = file.ToStdString();
	QPDF outpdf;
	outpdf.emptyPDF();


	for (std::vector<QPDFPageObjectHelper>::iterator iter = pages.begin(); iter != pages.end(); ++iter)
	{
		bool find = false;
		for (int i : listPage)
		{
			if (i == pageno)
			{
				find = true;
				break;
			}
		}

		if (find)
		{
			QPDFPageObjectHelper& page(*iter);
			QPDFPageDocumentHelper(outpdf).addPage(page, false);
		}

		pageno++;
	}

	QPDFWriter outpdfw(outpdf, outfile.c_str());
	outpdfw.write();

	return file;
}