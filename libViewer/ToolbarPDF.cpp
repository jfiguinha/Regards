#include <header.h>
#include "ToolbarPDF.h"
#include <ToolbarButton.h>
#include <LibResource.h>
#include "MainWindow.h"
#include "ViewerFrame.h"
#include "FileUtility.h"
#include "CentralWindow.h"
#include <window_id.h>
#include "ScannerFrame.h"

using namespace Regards::Window;
using namespace Regards::Viewer;


//
#define IDM_WINDOWSEARCH 152
#define IDM_THUMBNAILFACE 153
#define IDM_VIEWERMODE 154
#define IDM_EXPLORERMODE 155
#define IDM_SHOWINFOS 156
#define IDM_PRINT 158
#define IDM_QUITTER 157

CToolbarPDF::CToolbarPDF(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	wxString export_label = CLibResource::LoadStringFromResource(L"LBLEXPORT", 1);
	wxString lblOpenFolder = CLibResource::LoadStringFromResource(L"LBLSELECTFILE", 1);
	wxString lblInfos = CLibResource::LoadStringFromResource(L"LBLINFOS", 1);
	wxString lblQuit = CLibResource::LoadStringFromResource(L"LBLQUIT", 1);
	wxString lblPrint = CLibResource::LoadStringFromResource(L"LBLPRINT", 1);

	CToolbarButton * screen = new CToolbarButton(themeToolbar.button);
	screen->SetButtonResourceId(L"IDB_FOLDER");
	screen->SetCommandId(IDM_WINDOWSEARCH);
	screen->SetLibelle(lblOpenFolder);
	navElement.push_back(screen);

	CToolbarButton* infos = new CToolbarButton(themeToolbar.button);
	infos->SetButtonResourceId(L"IDB_INFOS");
	infos->SetLibelle(lblInfos);
	infos->SetCommandId(IDM_SHOWINFOS);
	navElement.push_back(infos);

	CToolbarButton * print = new CToolbarButton(themeToolbar.button);
	print->SetButtonResourceId(L"IDB_PRINTERPNG");
	print->SetLibelle(lblPrint);
	print->SetCommandId(IDM_PRINT);
	navElement.push_back(print);

	CToolbarButton * imageFirst = new CToolbarButton(themeToolbar.button);
	imageFirst->SetButtonResourceId(L"IDB_EXIT");
	imageFirst->SetLibelle(lblQuit);
	imageFirst->SetCommandId(IDM_QUITTER);
	navElement.push_back(imageFirst);
}

CToolbarPDF::~CToolbarPDF()
{
}


void CToolbarPDF::EventManager(const int &id)
{
	switch (id)
	{

	case IDM_WINDOWSEARCH:
	{
		wxWindow* central = this->FindWindowById(PDFWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_OPENFILE);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_SHOWINFOS:
	{
		wxWindow* central = this->FindWindowById(PDFWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_SHOWPANE);
		wxQueueEvent(central, event);
	}
	break;


	case IDM_PRINT:
	{
		wxWindow* central = this->FindWindowById(PDFWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_PRINT);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_QUITTER:
	{
		wxWindow* central = this->FindWindowById(PDFWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVT_EXIT);
		wxQueueEvent(central, event);
	}
	break;
	}
}