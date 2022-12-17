#include <header.h>
#include "CentralWindow.h"
#include "ToolbarPDF.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include <window_id.h>
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
#include <RegardsPDF.h>
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
			wxString fileExtract = CRegardsPDF::ExtractPage(filename, listPage);

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
			CRegardsPDF::RemovePage(filename, listPage);
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
				CRegardsPDF::AddPage(file, filename, listPage, position);
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
							CRegardsPDF::AddPdfPage(&oPdfDocument, imageLoadingFormat, option, quality, numpage);
							nbPage++;
							delete imageLoadingFormat;
						}
					}
				}
				else
				{
					CImageLoadingFormat * imageFormat = libPicture.LoadPicture(filename);
					CRegardsPDF::AddPdfPage(&oPdfDocument, imageFormat, option, quality, i);
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
					CRegardsPDF::AddPage(listFile[i], temporyFile, listPage, oldAnimationPosition);
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
