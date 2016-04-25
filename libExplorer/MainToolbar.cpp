#include "MainToolbar.h"
#include "MainWindow.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <LibResource.h>
using namespace Regards::Explorer;

#define IDM_QUITTER 1
#define IDM_REFRESHFOLDER 2
#define IDM_INFOS 3
#define IDM_FOLDER 4
#define IDM_PICTURE 5
#define IDM_PICTURETOOLBAR 6
#define IDM_AFFICHAGE_IMAGE 7
#define IDM_DONATE 8
#define IDM_ADDFOLDER 9
#define IDM_REFRESHFILTER 10

CMainToolbar::CMainToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme)
	: CToolbarWindow(parent, id, theme)
{
	wxString libelleAddFolder = CLibResource::LoadStringFromResource(L"LBLADDFOLDER", 1);
	wxString libelleRefresh = CLibResource::LoadStringFromResource(L"LBLREFRESH",1);
    wxString libelleRefreshFilter = CLibResource::LoadStringFromResource(L"LBLREFRESHFILTER",1);
	wxString libellePhotoViewer = CLibResource::LoadStringFromResource(L"LBLPHOTOVIEWER", 1);
	wxString libelleDonate = CLibResource::LoadStringFromResource(L"LBLDONATE", 1);
	wxString libelleQuit = CLibResource::LoadStringFromResource(L"LBLQUIT", 1);
	wxString libelleInfos = CLibResource::LoadStringFromResource(L"LBLINFOS", 1);
	wxString libelleFolder = CLibResource::LoadStringFromResource(L"LBLFOLDER", 1);
	wxString libellePicture = CLibResource::LoadStringFromResource(L"LBLPICTUREDETAILS", 1);
	wxString libellePreviewPicture = CLibResource::LoadStringFromResource(L"LBLPREVIEWPICTURE", 1);

	CToolbarButton * addFolder = new CToolbarButton(themeToolbar.button);
	addFolder->SetButtonResourceId(L"IDB_NEWPNG");
	addFolder->SetLibelle(libelleAddFolder);
	addFolder->SetCommandId(IDM_ADDFOLDER);
	navElement.push_back(addFolder);

	CToolbarButton * refresh = new CToolbarButton(themeToolbar.button);
	refresh->SetButtonResourceId(L"IDB_FOLDER_REFRESH");
	refresh->SetLibelle(libelleRefresh);
	refresh->SetCommandId(IDM_REFRESHFOLDER);
	navElement.push_back(refresh);
    
    CToolbarButton * refreshFilter = new CToolbarButton(themeToolbar.button);
    refreshFilter->SetButtonResourceId(L"IDB_FOLDER_REFRESH");
    refreshFilter->SetLibelle(libelleRefreshFilter);
    refreshFilter->SetCommandId(IDM_REFRESHFILTER);
    navElement.push_back(refreshFilter);

	CToolbarButton * photoViewer = new CToolbarButton(themeToolbar.button);
	photoViewer->SetButtonResourceId(L"IDB_PHOTOVIEWERPNG");
	photoViewer->SetCommandId(IDM_AFFICHAGE_IMAGE);
	photoViewer->SetLibelle(libellePhotoViewer);
	navElement.push_back(photoViewer);
    
#ifdef WIN32
	//CToolbarButton * donate = new CToolbarButton(themeToolbar.button);
	//donate->SetButtonResourceId(L"IDB_DONATE");
	//donate->SetLibelle(libelleDonate);
	//donate->SetCommandId(IDM_DONATE);
	//navElement.push_back(donate);
#endif
    
	CToolbarButton * infos = new CToolbarButton(themeToolbar.button);
	infos->SetButtonResourceId(L"IDB_INFOS");
	infos->SetLibelle(libelleInfos);
	infos->SetCommandId(IDM_INFOS);
	navElement.push_back(infos);

	CToolbarButton * pictureToolbar = new CToolbarButton(themeToolbar.button);
	pictureToolbar->SetButtonResourceId(L"IDB_PICTURETOOLBAR");
	pictureToolbar->SetLibelle(libellePicture);
	pictureToolbar->SetCommandId(IDM_PICTURETOOLBAR);
	navElement.push_back(pictureToolbar);

	CToolbarButton * picture = new CToolbarButton(themeToolbar.button);
	picture->SetButtonResourceId(L"IDB_PICTURE");
	picture->SetLibelle(libellePreviewPicture);
	picture->SetCommandId(IDM_PICTURE);
	navElement.push_back(picture);

	CToolbarButton * imageFirst = new CToolbarButton(themeToolbar.button);
	imageFirst->SetButtonResourceId(L"IDB_EXIT");
	imageFirst->SetLibelle(libelleQuit);
	imageFirst->SetCommandId(IDM_QUITTER);
	navElement.push_back(imageFirst);
}


CMainToolbar::~CMainToolbar()
{
}

void CMainToolbar::EventManager(const int &id)
{
	CMainWindow * mainWnd = (CMainWindow *)this->FindWindowById(MAINEXPLORERWINDOWID);
	if (mainWnd != nullptr)
	{
		switch (id)
		{
#ifdef WIN32
		case IDM_DONATE:
		{
			wxString siteWeb = CLibResource::LoadStringFromResource(L"DONATE", 1);
			wxLaunchDefaultApplication(siteWeb);
		}
		break;
#endif

		case IDM_ADDFOLDER:
			if (mainWnd != nullptr)
				mainWnd->AddFolder();
			break;

		case IDM_PICTURETOOLBAR:
			if (mainWnd != nullptr)
				mainWnd->ShowToolbar();
			break;

		case IDM_INFOS:
			if (mainWnd != nullptr)
				mainWnd->ShowPreviewInfos();
			break;

		case IDM_PICTURE:
			if (mainWnd != nullptr)
				mainWnd->ShowPreview();
			break;

		case IDM_QUITTER:
			if (mainWnd != nullptr)
			{
				mainWnd->Exit();
			}
			break;

		case IDM_AFFICHAGE_IMAGE:
			if (mainWnd != nullptr)
				mainWnd->ShowRegardsViewer();
			break;

		case IDM_REFRESHFOLDER:
			if (mainWnd != nullptr)
				mainWnd->RefreshFolder();
			break;
                
        case IDM_REFRESHFILTER:
            if (mainWnd != nullptr)
                mainWnd->RefreshFilter();
            break;

		}
	}
}