#include <header.h>
#include "ToolbarPDF.h"
#include <ToolbarButton.h>
#include <LibResource.h>
#include "FileUtility.h"
#include <window_id.h>
#include "ScannerFrame.h"

using namespace Regards::Window;
using namespace Regards::Viewer;


//
#define IDM_WINDOWSEARCH 152
#define IDM_THUMBNAILFACE 153
#define IDM_VIEWERMODE 154
#define IDM_EXPLORERMODE 155
#define IDM_SCANNER 156
#define IDM_PRINT 158
#define IDM_QUITTER 157
#define IDM_SAVE 159
#define IDM_ADDPAGE 160
#define IDM_DELETEPAGE 161
#define IDM_EXTRACT 162

CToolbarPDF::CToolbarPDF(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	wxString export_label = CLibResource::LoadStringFromResource(L"LBLEXPORT", 1);
	wxString lblOpenFolder = CLibResource::LoadStringFromResource(L"LBLSELECTFILE", 1);
	wxString lblScanner = CLibResource::LoadStringFromResource(L"LBLSCANNER", 1);
	wxString lblInfos = CLibResource::LoadStringFromResource(L"LBLINFOS", 1);
	wxString lblQuit = CLibResource::LoadStringFromResource(L"LBLQUIT", 1);
	wxString lblPrint = CLibResource::LoadStringFromResource(L"LBLPRINT", 1);
	wxString lblAddPages = CLibResource::LoadStringFromResource(L"LBLADDPAGE", 1);
	wxString lblDeletePages = CLibResource::LoadStringFromResource(L"LBLDELETEPAGES", 1);
	wxString lblSave = CLibResource::LoadStringFromResource(L"LBLSAVE", 1);
	wxString lblExtract = CLibResource::LoadStringFromResource(L"LBLEXTRACT", 1);

	CToolbarButton * screen = new CToolbarButton(themeToolbar.button);
	screen->SetButtonResourceId(L"IDB_FOLDER");
	screen->SetCommandId(IDM_WINDOWSEARCH);
	screen->SetLibelle(lblOpenFolder);
	navElement.push_back(screen);

	CToolbarButton * save = new CToolbarButton(themeToolbar.button);
	save->SetButtonResourceId(L"IDB_SAVE");
	save->SetLibelle(lblSave);
	save->SetCommandId(IDM_SAVE);
	navElement.push_back(save);

	CToolbarButton * addpage = new CToolbarButton(themeToolbar.button);
	addpage->SetButtonResourceId(L"IDB_PLUS");
	addpage->SetLibelle(lblAddPages);
	addpage->SetCommandId(IDM_ADDPAGE);
	navElement.push_back(addpage);

	CToolbarButton * deletepage = new CToolbarButton(themeToolbar.button);
	deletepage->SetButtonResourceId(L"IDB_DELETE");
	deletepage->SetLibelle(lblDeletePages);
	deletepage->SetCommandId(IDM_DELETEPAGE);
	navElement.push_back(deletepage);

	CToolbarButton* infos = new CToolbarButton(themeToolbar.button);
	infos->SetButtonResourceId(L"IDB_SCANNER");
	infos->SetLibelle(lblScanner);
	infos->SetCommandId(IDM_SCANNER);
	navElement.push_back(infos);

	CToolbarButton * print = new CToolbarButton(themeToolbar.button);
	print->SetButtonResourceId(L"IDB_PRINTERPNG");
	print->SetLibelle(lblPrint);
	print->SetCommandId(IDM_PRINT);
	navElement.push_back(print);

	CToolbarButton * extract = new CToolbarButton(themeToolbar.button);
	extract->SetButtonResourceId(L"IDB_EXTRACT");
	extract->SetLibelle(lblExtract);
	extract->SetCommandId(IDM_EXTRACT);
	navElement.push_back(extract);


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
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_OPENFILE);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_SAVE:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_SAVE);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_ADDPAGE:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_ADDPAGE);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_DELETEPAGE:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_DELETEPAGE);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_SCANNER:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_SCANNER);
		wxQueueEvent(central, event);
	}
	break;


	case IDM_PRINT:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_PRINT);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_EXTRACT:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_EXTRACT);
		wxQueueEvent(central, event);
	}
	break;

	case IDM_QUITTER:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVT_EXIT);
		wxQueueEvent(central, event);
	}
	break;
	}
}