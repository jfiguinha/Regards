#include <header.h>
#include "ToolbarPDF.h"
#include <ToolbarButton.h>
#include <LibResource.h>
#include "ScannerFrame.h"

using namespace Regards::Window;
using namespace Regards::Scanner;

#define IDM_OPENFILE 152
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

CToolbarPDF::CToolbarPDF(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	screen = CreateButton(L"IDB_FOLDER", L"LBLSELECTFILE", IDM_OPENFILE);
	save = CreateButton(L"IDB_SAVE", L"LBLSAVE", IDM_SAVE);
	addpage = CreateButton(L"IDB_PLUS", L"LBLADDPAGE", IDM_ADDPAGE);
	deletepage = CreateButton(L"IDB_DELETE", L"LBLDELETEPAGES", IDM_DELETEPAGE);
	print = CreateButton(L"IDB_PRINTERPNG", L"LBLPRINT", IDM_PRINT);
	extract = CreateButton(L"IDB_EXTRACT", L"LBLEXTRACT", IDM_EXTRACT);
	imageFirst = CreateButton(L"IDB_EXIT", L"LBLQUIT", IDM_QUITTER);
}

void CToolbarPDF::EventManager(const int& id)
{
	switch (id)
	{
	case IDM_OPENFILE:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_OPENFILE);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_SAVE:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_SAVE);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_ADDPAGE:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_ADDPAGE);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_DELETEPAGE:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_DELETEPAGE);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_SCANNER:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_SCANNER);
			wxQueueEvent(central, event);
		}
		break;


	case IDM_PRINT:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_PRINT);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_EXTRACT:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVENT_EXTRACT);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_QUITTER:
		{
			wxWindow* central = this->FindWindowById(SCANNER_CENTRALVIEWERWINDOWID);
			auto event = new wxCommandEvent(wxEVT_EXIT);
			wxQueueEvent(central, event);
		}
		break;
	default: ;
	}
}
