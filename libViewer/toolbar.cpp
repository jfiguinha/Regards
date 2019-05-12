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
#define IDM_QUITTER 156

CToolbar::CToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme)
	: CToolbarWindow(parent,id,theme)
{
	wxString export_label = CLibResource::LoadStringFromResource(L"LBLEXPORT", 1);//L"Effect";
	wxString lblOpenFolder = CLibResource::LoadStringFromResource(L"LBLSELECTFILE",1);//L"Crop";
	wxString lblQuit = CLibResource::LoadStringFromResource(L"LBLQUIT", 1);// L"Zoom Off
	wxString lblListFace = "Face List";

	CToolbarButton * screen = new CToolbarButton(themeToolbar.button);
	screen->SetButtonResourceId(L"IDB_FOLDER");
	screen->SetCommandId(IDM_WINDOWSEARCH);
	screen->SetLibelle(lblOpenFolder);
	navElement.push_back(screen);

	CToolbarButton * thumbnailFace = new CToolbarButton(themeToolbar.button);
	thumbnailFace->SetButtonResourceId(L"IDB_PEOPLE_FACE");
	thumbnailFace->SetLibelle(lblListFace);
	thumbnailFace->SetCommandId(IDM_THUMBNAILFACE);
	navElement.push_back(thumbnailFace);

	CToolbarButton * explorer = new CToolbarButton(themeToolbar.button);
	explorer->SetButtonResourceId(L"IDB_SEARCH");
	explorer->SetLibelle("Explorer Mode");
	explorer->SetCommandId(IDM_EXPLORERMODE);
	navElement.push_back(explorer);
    
	CToolbarButton * viewer = new CToolbarButton(themeToolbar.button);
	viewer->SetButtonResourceId(L"IDB_THUMBNAILPNG");
	viewer->SetLibelle(L"Viewer Mode");
	viewer->SetCommandId(IDM_VIEWERMODE);
	navElement.push_back(viewer);
    
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
	CCentralWindow * centralWnd = (CCentralWindow *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);

	if (centralWnd != nullptr && mainWindow != nullptr)
	{
		switch (id)
		{
                
            
        case IDM_VIEWERMODE:
			centralWnd->SetMode(1);
	        break;
                               
		case IDM_THUMBNAILFACE:
			centralWnd->SetMode(2);
			break;

		case IDM_EXPLORERMODE:
			centralWnd->SetMode(3);
			break;


		case IDM_QUITTER:
			mainWindow->Exit();
			break;
		}
	}
}