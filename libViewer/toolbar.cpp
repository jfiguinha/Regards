#include <header.h>
#include "Toolbar.h"
#include <ToolbarButton.h>
#include <LibResource.h>
#include "MainWindow.h"
#include "ViewerFrame.h"
#include "FileUtility.h"
#include "CentralWindow.h"
#include <window_id.h>

using namespace Regards::Window;
using namespace Regards::Viewer;


//
#define IDM_WINDOWSEARCH 152
#define IDM_THUMBNAILFACE 153
#define IDM_VIEWERMODE 154
#define IDM_EXPLORERMODE 155
#define IDM_SHOWINFOS 156
#define IDM_QUITTER 157
#define IDM_PRINT 158
#define IDM_SCANNER 159
#define IDM_PICTUREMODE 160

CToolbar::CToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent,id,theme, vertical)
{
	wxString export_label = CLibResource::LoadStringFromResource(L"LBLEXPORT", 1);
	wxString lblOpenFolder = CLibResource::LoadStringFromResource(L"LBLSELECTFILE",1);
	wxString lblInfos = CLibResource::LoadStringFromResource(L"LBLINFOS", 1);
	wxString lblQuit = CLibResource::LoadStringFromResource(L"LBLQUIT", 1);
	wxString lblPrint = CLibResource::LoadStringFromResource(L"LBLPRINT", 1);
#ifndef __NOFACE_DETECTION__
	wxString lblListFace = CLibResource::LoadStringFromResource(L"LBLFACELIST", 1);
#endif
	wxString lblScanner = CLibResource::LoadStringFromResource(L"LBLSCANNER", 1);
	wxString lblExplorerMode = CLibResource::LoadStringFromResource(L"LBLEXPLORERMODE", 1);
	wxString lblViewerMode = CLibResource::LoadStringFromResource(L"LBLVIEWERMODE", 1);
	wxString lblPictureMode = CLibResource::LoadStringFromResource(L"LBLPICTUREMODE", 1);

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

#ifndef __NOFACE_DETECTION__
	CToolbarButton * thumbnailFace = new CToolbarButton(themeToolbar.button);
	thumbnailFace->SetButtonResourceId(L"IDB_PEOPLE_FACE");
	thumbnailFace->SetLibelle(lblListFace);
	thumbnailFace->SetCommandId(IDM_THUMBNAILFACE);
	navElement.push_back(thumbnailFace);
#endif
	CToolbarButton * explorer = new CToolbarButton(themeToolbar.button);
	explorer->SetButtonResourceId(L"IDB_SEARCH");
	explorer->SetLibelle(lblExplorerMode);
	explorer->SetCommandId(IDM_EXPLORERMODE);
	navElement.push_back(explorer);
    
	CToolbarButton * viewer = new CToolbarButton(themeToolbar.button);
	viewer->SetButtonResourceId(L"IDB_THUMBNAILPNG");
	viewer->SetLibelle(lblViewerMode);
	viewer->SetCommandId(IDM_VIEWERMODE);
	navElement.push_back(viewer);

	CToolbarButton * picture = new CToolbarButton(themeToolbar.button);
	picture->SetButtonResourceId(L"IDB_PHOTOVIEWERPNG");
	picture->SetLibelle(lblPictureMode);
	picture->SetCommandId(IDM_PICTUREMODE);
	navElement.push_back(picture);

	CToolbarButton* scanner = new CToolbarButton(themeToolbar.button);
	scanner->SetButtonResourceId(L"IDB_SCANNER");
	scanner->SetLibelle(lblScanner);
	scanner->SetCommandId(IDM_SCANNER);
	navElement.push_back(scanner);

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

CToolbar::~CToolbar()
{
}


void CToolbar::EventManager(const int &id)
{
	switch (id)
	{
                
	case IDM_WINDOWSEARCH:
		{
			wxWindow* central = this->FindWindowById(PHOTOSEEARCHPANEL);
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_SHOWPANE);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_SHOWINFOS:
		{
			wxWindow* central = this->FindWindowById(PANELCLICKINFOSWNDID);
			wxCommandEvent* event = new wxCommandEvent(wxEVENT_SHOWPANE);
			wxQueueEvent(central, event);
		}
		break;

	case IDM_PRINT:
	{
		wxWindow* central = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_PRINT);
		wxQueueEvent(central, event);
	}
	break;
            
    case IDM_VIEWERMODE:
		{
			wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(1);
			wxPostEvent(central, event);
		}
	    break;
                
#ifndef __NOFACE_DETECTION__
	case IDM_THUMBNAILFACE:
		{
			wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(2);
			wxPostEvent(central, event);
		}
		break;
#endif
	case IDM_EXPLORERMODE:
		{
			wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
			wxCommandEvent event(wxEVENT_SETMODEVIEWER);
			event.SetInt(3);
			wxPostEvent(central, event);
		}
		break;

	case IDM_PICTUREMODE:
	{
		wxWindow* central = this->FindWindowById(CENTRALVIEWERWINDOWID);
		wxCommandEvent event(wxEVENT_SETMODEVIEWER);
		event.SetInt(4);
		wxPostEvent(central, event);
	}
	break;

	case IDM_SCANNER:
	{
		wxWindow* central = this->FindWindowById(MAINVIEWERWINDOWID);
		wxCommandEvent* event = new wxCommandEvent(wxEVENT_SHOWSCANNER);
		wxQueueEvent(central, event);
		break;
	}

	case IDM_QUITTER:
		{
			wxWindow* central = this->FindWindowById(MAINVIEWERWINDOWID);
			wxCommandEvent* event = new wxCommandEvent(wxEVT_EXIT);
			wxQueueEvent(central, event);
		}
		break;
	}
}