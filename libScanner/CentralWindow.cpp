#include <header.h>
#include "CentralWindow.h"
#include "ToolbarPDF.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include <window_id.h>
#include <qpdf/QPDF.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>
#include <qpdf/QPDFWriter.hh>
#include "SelectPage.h"
#include <FileUtility.h>
#include <libPicture.h>
#include <picture_id.h>
#include <wx/progdlg.h>
#include <wx/pdfdocument.h>
#include <LibResource.h>
#include <SavePicture.h>
#include <ConvertUtility.h>
#include <wx/busyinfo.h>
#include <ConvertUtility.h>
using namespace Regards::Window;
using namespace Regards::Picture;
#define OPENFILE 0
#define ADDFILE 1

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
		CThemeToolbar theme_toolbar;
		viewerTheme->GetMainToolbarTheme(&theme_toolbar);
		toolbarPDF = new CToolbarPDF(this, wxID_ANY, theme_toolbar, false);
		toolbarPDF->Show(true);
	}

	previewWindow = new CViewerPDF(this, frame, PDFWINDOWID);
	previewWindow->Show(true);

	this->frame = frame;

	Connect(wxEVT_SIZE, wxSizeEventHandler(CCentralWindow::OnSize));
	Connect(wxEVENT_PRINT, wxCommandEventHandler(CCentralWindow::OnPrint));
	Connect(wxEVT_EXIT, wxCommandEventHandler(CCentralWindow::OnExit));
	Connect(wxEVENT_SAVE, wxCommandEventHandler(CCentralWindow::OnSave));
	Connect(wxEVENT_ADDPAGE, wxCommandEventHandler(CCentralWindow::OnAddPage));
	Connect(wxEVENT_DELETEPAGE, wxCommandEventHandler(CCentralWindow::OnDeletePage));
    Connect(wxEVENT_SCANNER, wxCommandEventHandler(CCentralWindow::OnScan));
	Connect(wxEVENT_EXTRACT, wxCommandEventHandler(CCentralWindow::OnExtractPage));
	Connect(wxEVENT_OPENFILE, wxCommandEventHandler(CCentralWindow::OnOpenFile));
}

void CCentralWindow::OnScan(wxCommandEvent& event)
{
    if(frame != nullptr)
    {
    	wxString pdfFile = frame->ScanPage();
        if (pdfFile != "")
        {
            LoadFile(pdfFile);
        }
    }


}

void CCentralWindow::OnExtractPage(wxCommandEvent& event)
{
	if (filename != "")
	{
		CSelectFileDlg selectFile(nullptr, -1, filename, _("Select Page To Extract"));
		if (selectFile.ShowModal() == wxID_OK)
		{
			vector<int> listPage = selectFile.GetSelectItem();
			wxString fileExtract = ProcessExtractFile(listPage);

			wxFileDialog saveFileDialog(nullptr, _("Save Extract PDF page"), "", "",
				"PDF files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (saveFileDialog.ShowModal() == wxID_CANCEL)
				return;     // the user changed idea...

			wxString newfilename = saveFileDialog.GetPath();
			wxCopyFile(fileExtract, newfilename);
		}

	}
	else
	{
		wxString openfile = CLibResource::LoadStringFromResource("LBLOPENAFILE", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(openfile, infos, wxICON_INFORMATION);
	}
	//CSavePicture::ExportPicture(this, filename);
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
		wxString openfile = CLibResource::LoadStringFromResource("LBLOPENAFILE", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(openfile, infos, wxICON_INFORMATION);
	}
}


int CCentralWindow::OnOpen(const int &type)
{
	wxArrayString list;
	list.push_back("Scan");
	list.push_back("File");
	list.push_back("Multifile");
	//bool isOk = false;

	//Check Temp Folder
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
	if (!wxMkDir(tempFolder)) {
#else
	wxString tempFolder = documentPath + "/temp";
	if (!wxMkDir(tempFolder, wxS_DIR_DEFAULT)) {
#endif
	}

	if (type != ADDFILE)
	{
#ifdef WIN32
		filename = tempFolder + "\\local_pdf_file.pdf";
#else
		filename = tempFolder + "/local_pdf_file.pdf";
#endif

		if (wxFileExists(filename))
			wxRemoveFile(filename);

	}

	wxString file = "";
	int numSelect = wxGetSingleChoiceIndex("Select Source : ", "Source", list, 0, this);
	if (numSelect != -1)
	{
		switch (numSelect)
		{
			case 0:
			{
				file = frame->ScanPage();
				break;
			}
			case 1:
			{
				file = LoadFile();
				break;
			}
			case 2:
			{
				wxArrayString listFile = CSavePicture::LoadMultiplePicture();
				file = ProcessLoadFiles(listFile);
				break;
			}
		default: ;
		}

		int position = 0;
		if (type == ADDFILE)
		{
			CLibPicture libPicture;
			vector<int> listPage;

			if (file != "")
			{
				position = previewWindow->GetAnimationPosition();
				int nbPage = libPicture.GetNbImage(file);
				for (int i = 0; i < nbPage; i++)
					listPage.push_back(i);
				ProcessAddFile(file, filename, listPage, position);
				position += nbPage;

			}
		}
		else
		{
			CLibPicture libPicture;

			if (wxFileExists(file))
				wxCopyFile(file, filename);

		}

		if (wxFileExists(filename))
		{
			if (previewWindow != nullptr)
				previewWindow->LoadFile(filename);

			previewWindow->SetPosition(0);
		}
	}

	return numSelect;
}

void CCentralWindow::OnAddPage(wxCommandEvent& event)
{
	if (filename != "")
	{
		OnOpen(ADDFILE);
	}
	else
	{
		wxString openfile = CLibResource::LoadStringFromResource("LBLOPENAFILE", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(openfile, infos, wxICON_INFORMATION);
	}
}

void CCentralWindow::OnSave(wxCommandEvent& event)
{
	/*
	if (filename != "")
	{
		//CSavePicture::ExportPicture(this, filename);

		//CBitmapWndViewer* bitmapWindow = (CBitmapWndViewer*)this->FindWindowById(parentId);

		wxString filename = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);
		wxString savePdfFile = CLibResource::LoadStringFromResource(L"LBLSAVEPDFFILE", 1);

		wxFileDialog saveFileDialog(nullptr, savePdfFile, "", "",
			"PDF " + filename + " (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return;     // the user changed idea...

		wxString newfilename = saveFileDialog.GetPath();
		wxCopyFile(filename, newfilename);

		LoadFile(newfilename);
	}
	else
	{
		wxString openfile = CLibResource::LoadStringFromResource("LBLOPENAFILE", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(openfile, infos, wxICON_INFORMATION);
	}
	*/
	if (filename != "")
	{
		wxString tempFolder = CFileUtility::GetDocumentFolderPath();

#ifdef WIN32
		tempFolder.append("\\temp");
#else
		tempFolder.append("/temp");
#endif


#ifdef WIN32
		filename = tempFolder + "\\local_pdf_file.pdf";
#else
		filename = tempFolder + "/local_pdf_file.pdf";
#endif

		//wxString filenameTitle = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);
		wxString savePdfFile = CLibResource::LoadStringFromResource(L"LBLSAVEPDFFILE", 1);

		wxFileDialog saveFileDialog(nullptr, savePdfFile, "", "",
			"PDF Files (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return;     // the user changed idea...

		wxString newfilename = saveFileDialog.GetPath();
		wxCopyFile(filename, newfilename);

		wxString savecompleted = CLibResource::LoadStringFromResource("LBLSAVEFILECOMPLETED", 1);
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox(savecompleted, infos);

	}
	else
	{
		wxString openfile = CLibResource::LoadStringFromResource("LBLOPENAFILE", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(openfile, infos, wxICON_INFORMATION);
	}
}

void CCentralWindow::AddPdfPage(wxPdfDocument & oPdfDocument, CImageLoadingFormat * imageFormat, int option, int quality, int numpage)
{
	CLibPicture libPicture;
	wxString file;
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

	//Save
	if (option == 0)
	{

#ifdef WIN32
		file = tempFolder + "\\temporary" + to_string(numpage) + ".jpg";
#else
		file = tempFolder + "/temporary" + to_string(numpage) + ".jpg";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

		libPicture.SavePicture(file, imageFormat, 0, quality);
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\temporary" + to_string(numpage) + ".tiff";
#else
		file = tempFolder + "/temporary" + to_string(numpage) + ".tiff";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

		libPicture.SavePicture(file, imageFormat, 0, quality);
	}

	wxImage image;
	image.LoadFile(file);
	//int nResolutionX = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
	//int nResolutionY = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);
	//	double dblDpiBy72 = (double)m_nDpi / 72.0;

	image.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES);


	if (image.HasOption(wxIMAGE_OPTION_RESOLUTIONUNIT))
	{
		//int nResolutionUnit = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONUNIT);
		int nResolution = 0;

		// Get image resolution-
		if (image.HasOption(wxIMAGE_OPTION_RESOLUTION))
		{
			nResolution = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTION);
		}
		else if (image.HasOption(wxIMAGE_OPTION_RESOLUTIONX) && (image.HasOption(wxIMAGE_OPTION_RESOLUTIONY)))
		{
			int nResolutionX = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
			int nResolutionY = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);

			if (nResolutionX == nResolutionY)
			{
				nResolution = nResolutionX;
			}
		}
		if (nResolution)
		{


			//int tpl = oPdfDocument.BeginTemplate(0, 0, image.GetWidth(), image.GetHeight());
			wxPrintOrientation orientation = (image.GetHeight() > image.GetWidth()) ? wxPORTRAIT : wxLANDSCAPE;
			oPdfDocument.AddPage(orientation);

			if (option == 0)
				oPdfDocument.Image(file, 0, 0, oPdfDocument.GetPageWidth(), oPdfDocument.GetPageHeight(), wxT("image/jpeg"));
			else
				oPdfDocument.Image(file, 0, 0, oPdfDocument.GetPageWidth(), oPdfDocument.GetPageHeight(), wxT("image/tiff"));


		}
	}
}

int CCentralWindow::LoadPictureFile(wxArrayString & listFile, wxString filenameOutput)
{
	int nbPage = 0;
	CLibPicture libPicture;
	//wxString _filename = "";


	if (listFile.size() > 0)
	{
			int i = 0;
			int option = 0;
			int quality = 0;
			//int iformat = libPicture.TestImageFormat(listFile[0]);

			wxString documentPath = CFileUtility::GetDocumentFolderPath();

			libPicture.SavePictureOption(PDF, option, quality);

			wxProgressDialog dialog("Add File to PDF", "Filename", listFile.size(), this, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);

			// Create a PDF document, add a page, and put the image on it.
			wxPdfDocument oPdfDocument;

			for (wxString filename : listFile)
			{
				wxString message = "Add File : " + filename;

				if (false == dialog.Update(i++, message))
					break;

				if (libPicture.GetNbImage(filename) > 1)
				{
					CSelectFileDlg selectFile(this, -1, filename, _("Select Page To Add"));
					if (selectFile.ShowModal() == wxID_OK)
					{
						vector<int> listPage = selectFile.GetSelectItem();
						for (int numpage = 0; numpage < listPage.size(); numpage++)
						{

							CImageLoadingFormat * imageLoadingFormat = libPicture.LoadPicture(filename, false, listPage[numpage]);
							AddPdfPage(oPdfDocument, imageLoadingFormat, option, quality, numpage);
							nbPage++;
							delete imageLoadingFormat;
						}
					}
				}
				else
				{
					CImageLoadingFormat * imageFormat = libPicture.LoadPicture(filename);
					AddPdfPage(oPdfDocument, imageFormat, option, quality, i);
					nbPage++;
					delete imageFormat;
				}
			}

			oPdfDocument.Close();
			oPdfDocument.SaveAsFile(filenameOutput);

	}
	return nbPage;
}

wxString CCentralWindow::ProcessLoadFiles(wxArrayString & listFile)
{
	wxString temporyFile = "";
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
		temporyFile = tempFolder + "\\temporary_file.pdf";
#else
		temporyFile = tempFolder + "/temporary_file.pdf";
#endif

		if (wxFileExists(temporyFile))
			wxRemoveFile(temporyFile);

	}

	CLibPicture libPicture;

	if (listFile.size() > 0)
	{
		if (libPicture.TestIsPDF(listFile[0]))
		{
			int oldAnimationPosition = -1;
			for (int i = 0; i < listFile.size(); i++)
			{
				CSelectFileDlg selectFile(this, -1, listFile[i], _("Select Page To Add"));
				if (selectFile.ShowModal() == wxID_OK)
				{
					vector<int> listPage = selectFile.GetSelectItem();
					ProcessAddFile(listFile[i], temporyFile, listPage, oldAnimationPosition);
				}
			}
		}
		else
		{
			LoadPictureFile(listFile, temporyFile);
		}
	}

	return temporyFile;


}

void CCentralWindow::OnOpenFile(wxCommandEvent& event)
{
	//LoadFile();
	OnOpen(0);
}

void CCentralWindow::OnPrint(wxCommandEvent& event)
{
	if (filename != "")
	{
		frame->PrintPreview(GetImage());
	}
	else
	{
		wxString openfile = CLibResource::LoadStringFromResource("LBLOPENAFILE", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(openfile, infos, wxICON_INFORMATION);
	}
}

void CCentralWindow::OnExit(wxCommandEvent& event)
{
	frame->OnClose();
}




wxString CCentralWindow::LoadFile()
{
	wxString file = CSavePicture::LoadPicture();
	wxArrayString listFile;
	listFile.push_back(file);

	return ProcessLoadFiles(listFile);

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
		wxBusyInfo wait("Please wait, working...");
		std::vector<QPDFPageObjectHelper> oldpages;
		int oldpageno_len = 0;
		int i = 0;
		bool fileIn = false;
		if (filename != "" && wxFileExists(filename))
			fileIn = true;

		QPDF oldpdf;
		if(fileIn)
			oldpdf.processFile(CConvertUtility::ConvertToUTF8(filename));

		QPDF inpdf;
		inpdf.processFile(CConvertUtility::ConvertToUTF8(fileToAdd));


		std::string outfile = CConvertUtility::ConvertToStdString(file);
		QPDF outpdf;
		outpdf.emptyPDF();


		if (fileIn)
		{
			oldpages = QPDFPageDocumentHelper(oldpdf).getAllPages();
			oldpageno_len = oldpages.size();

			for (; i <= oldAnimationPosition; i++)
			{
				auto object = oldpages.at(i);
				QPDFPageObjectHelper& page(object);
				QPDFPageDocumentHelper(outpdf).addPage(page, false);
			}
		}




		std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(inpdf).getAllPages();
		//int pageno_len = pages.size();
		int pageno = 0;


		for (std::vector<QPDFPageObjectHelper>::iterator newiter = pages.begin(); newiter != pages.end(); ++newiter)
		{
			bool find = false;
			for (int i1 : listPage)
			{
				if (i1 == pageno)
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

		if (fileIn)
		{
			//for (int i = oldAnimationPosition; i < oldpageno_len; i++)
			for (; i < oldpageno_len; i++)
			{
				auto object = oldpages.at(i);
				QPDFPageObjectHelper& page(object);
				QPDFPageDocumentHelper(outpdf).addPage(page, false);
			}
		}

		QPDFWriter outpdfw(outpdf, outfile.c_str());
		outpdfw.setCompressStreams(true);
		outpdfw.setRecompressFlate(true);
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

	{
		wxBusyInfo wait("Please wait, working...");
		QPDF inpdf;
		inpdf.processFile(CConvertUtility::ConvertToUTF8(filename));
		std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(inpdf).getAllPages();
		//int pageno_len = QIntC::to_int(QUtil::uint_to_string(pages.size()).length());
		int pageno = 0;

		std::string outfile = CConvertUtility::ConvertToStdString(file);
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

		QPDFWriter outpdfw(outpdf, CConvertUtility::ConvertToUTF8(outfile));
		outpdfw.write();
	}




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
	inpdf.processFile(CConvertUtility::ConvertToStdString(filename).c_str());
	std::vector<QPDFPageObjectHelper> pages = QPDFPageDocumentHelper(inpdf).getAllPages();
	//int pageno_len = QIntC::to_int(QUtil::uint_to_string(pages.size()).length());
	int pageno = 0;

	std::string outfile = CConvertUtility::ConvertToStdString(file);
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
