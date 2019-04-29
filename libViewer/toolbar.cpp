#include <header.h>
#include "Toolbar.h"
#include <ToolbarButton.h>
#include <LibResource.h>
#include "MainWindow.h"
#include "ViewerFrame.h"
#include "FileUtility.h"
#include "CentralWnd.h"
#include <window_id.h>

using namespace Regards::Window;
using namespace Regards::Viewer;
#define ID_FILE_OPENNEWFOLDER 141
#define IDM_ASSOCIATE 142
#define IDM_DONATE 143
#define IDM_THUMBNAIL 144
#define IDM_PICTUREDETAILS 145
#define IDM_SHOWINFOS 146
#define IDM_QUITTER 147
#define IDM_REFRESHFOLDER 148
#define IDM_REFRESHTHUMBNAIL 149
#define IDM_REFRESHFILTER 150
#define IDM_CRITERIA 151
#define IDM_EXPORT 152
#define IDM_THUMBNAILFACE 153
#define IDM_VIEWERMODE 154
#define IDM_EXPLORERMODE 155

CToolbar::CToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme)
	: CToolbarWindow(parent,id,theme)
{
	wxString export_label = CLibResource::LoadStringFromResource(L"LBLEXPORT", 1);//L"Effect";
	wxString lblOpenFolder = CLibResource::LoadStringFromResource(L"LBLSELECTFILE",1);//L"Crop";
	wxString lblAssociate = CLibResource::LoadStringFromResource(L"LBLASSOCIATE",1);//L"Shrink Picture";
    wxString libelleRefreshFolder = CLibResource::LoadStringFromResource(L"LBLREFRESH",1);
    wxString libelleRefreshThumbnail = CLibResource::LoadStringFromResource(L"LBLREFRESHTHUMBNAIL",1);
    wxString libelleRefreshFilter = CLibResource::LoadStringFromResource(L"LBLREFRESHFILTER",1);
	wxString lblDonate = CLibResource::LoadStringFromResource(L"LBLDONATE", 1);// L"Zoom On";
	wxString lblQuit = CLibResource::LoadStringFromResource(L"LBLQUIT", 1);// L"Zoom Off
	wxString lblCriteria = CLibResource::LoadStringFromResource(L"LBLCRITERIA", 1);// L"Zoom Off";
	wxString lblThumbnail = CLibResource::LoadStringFromResource(L"LBLTHUMBNAIL", 1);// L"Zoom Off";
	wxString lblPictureDetails = CLibResource::LoadStringFromResource(L"LBLPICTUREDETAILS", 1);// L"Zoom Off";
	wxString lblInfos = CLibResource::LoadStringFromResource(L"LBLINFOS", 1);// L"Zoom Off";
	wxString lblListFace = "Face List";

    if(CViewerFrame::GetViewerMode())
    {
        CToolbarButton * screen = new CToolbarButton(themeToolbar.button);
        screen->SetButtonResourceId(L"IDB_FOLDER");
        screen->SetCommandId(ID_FILE_OPENNEWFOLDER);
        screen->SetLibelle(lblOpenFolder);
        navElement.push_back(screen);
        
        CToolbarButton * refreshFolder = new CToolbarButton(themeToolbar.button);
        refreshFolder->SetButtonResourceId(L"IDB_FOLDER_REFRESH");
        refreshFolder->SetLibelle(libelleRefreshFolder);
        refreshFolder->SetCommandId(IDM_REFRESHFOLDER);
        navElement.push_back(refreshFolder);
    }


    
    CToolbarButton * refresh = new CToolbarButton(themeToolbar.button);
    refresh->SetButtonResourceId(L"IDB_FOLDER_REFRESH");
    refresh->SetLibelle(libelleRefreshThumbnail);
    refresh->SetCommandId(IDM_REFRESHTHUMBNAIL);
    navElement.push_back(refresh);
    
    CToolbarButton * refreshFilter = new CToolbarButton(themeToolbar.button);
    refreshFilter->SetButtonResourceId(L"IDB_FOLDER_REFRESH");
    refreshFilter->SetLibelle(libelleRefreshFilter);
    refreshFilter->SetCommandId(IDM_REFRESHFILTER);
    navElement.push_back(refreshFilter);
    
    /*
	CToolbarButton * criteria = new CToolbarButton(themeToolbar.button);
	criteria->SetButtonResourceId(L"IDB_SEARCH");
	criteria->SetLibelle(lblCriteria);
	criteria->SetCommandId(IDM_CRITERIA);
	navElement.push_back(criteria);
    */
	CToolbarButton * thumbnail = new CToolbarButton(themeToolbar.button);
	thumbnail->SetButtonResourceId(L"IDB_THUMBNAILPNG");
	thumbnail->SetLibelle(lblThumbnail);
	thumbnail->SetCommandId(IDM_THUMBNAIL);
	navElement.push_back(thumbnail);

	CToolbarButton * pictureinfos = new CToolbarButton(themeToolbar.button);
	pictureinfos->SetButtonResourceId(L"IDB_PICTURETOOLBAR");
	pictureinfos->SetLibelle(lblPictureDetails);
	pictureinfos->SetCommandId(IDM_PICTUREDETAILS);
	navElement.push_back(pictureinfos);

	CToolbarButton * infos = new CToolbarButton(themeToolbar.button);
	infos->SetButtonResourceId(L"IDB_INFOS");
	infos->SetLibelle(lblInfos);
	infos->SetCommandId(IDM_SHOWINFOS);
	navElement.push_back(infos);

    /*
	CToolbarButton * thumbnailFace = new CToolbarButton(themeToolbar.button);
	thumbnailFace->SetButtonResourceId(L"IDB_PEOPLE_FACE");
	thumbnailFace->SetLibelle(lblListFace);
	thumbnailFace->SetCommandId(IDM_THUMBNAILFACE);
	navElement.push_back(thumbnailFace);
    */
    
	/*
	CToolbarButton * exportButton = new CToolbarButton(themeToolbar.button);
	exportButton->SetButtonResourceId(L"IDB_EXPORT");
	exportButton->SetCommandId(IDM_EXPORT);
	exportButton->SetLibelle(export_label);
	navElement.push_back(exportButton);
	*/
    
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
	CCentralWnd * centralWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);

	if (mainWindow != nullptr && nullptr != centralWnd)
	{
		switch (id)
		{
                
#ifdef WIN32
		case IDM_DONATE:
			{
				wxString siteWeb = CLibResource::LoadStringFromResource(L"DONATE",1);
				wxLaunchDefaultApplication(siteWeb);
			}
			break;


		case IDM_ASSOCIATE:
		{
			wxString programPath = CFileUtility::GetProgramFolderPath();
			programPath.append("\\RegardsAssociation.exe");
			ShellExecute(m_hWnd, L"runas", programPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        }
                break;
#endif
        case IDM_REFRESHFOLDER:
			if (mainWindow != nullptr)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHFOLDER);
				mainWindow->GetEventHandler()->AddPendingEvent(evt);
			}
            break;


        case IDM_REFRESHTHUMBNAIL:
			{
				wxWindow * window = this->FindWindowById(THUMBNAILFOLDER);
				if (window)
				{
					wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHTHUMBNAIL);
					evt.SetExtraLong(1);
					window->GetEventHandler()->AddPendingEvent(evt);
				}

				if (mainWindow != nullptr)
				{
					wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_CRITERIASHOWUPDATE);
					mainWindow->GetEventHandler()->AddPendingEvent(evt);
				}
			}
            break;
            
        case IDM_VIEWERMODE:
        {
            centralWnd->HidePanel();
        }
        break;
                               
		case IDM_REFRESHFILTER:
		{
			//centralWnd->UpdateCriteria(true);
			wxWindow * window = this->FindWindowById(CRITERIAFOLDERWINDOWID);
			if (window)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_UPDATECRITERIA);
				evt.SetExtraLong(3);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
            break;
                
		case IDM_CRITERIA:
		{
			wxWindow * window = this->FindWindowById(FILTERPREVIEWSPLITTER);
			if (window)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_PANELCRITERIA);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
			//centralWnd->ShowPanelCriteria();
			break;

		case ID_FILE_OPENNEWFOLDER:
		{
			wxWindow * window = this->FindWindowById(FILTERPREVIEWSPLITTER);
			if (window)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_PANELFOLDER);
				window->GetEventHandler()->AddPendingEvent(evt);
			}
		}
			//mainWindow->OpenFolder();
			//centralWnd->ShowPanelFolder();
			break;

		case IDM_THUMBNAIL:
		{
			//centralWnd->ShowPanelThumbnail();
			wxWindow * window = this->FindWindowById(FILTERPREVIEWSPLITTER);
			if (window)
			{
				wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWPANELTHUMBNAIL);
				window->GetEventHandler()->AddPendingEvent(evt);
			}

			centralWnd->ShowPanelVideoThumbnail();
		}
			break;

		case IDM_THUMBNAILFACE:
			{
				//centralWnd->ShowPanelThumbnail();
				wxWindow * window = this->FindWindowById(FILTERPREVIEWSPLITTER);
				if (window)
				{
					wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWPANELTHUMBNAIL);
					window->GetEventHandler()->AddPendingEvent(evt);
				}

				centralWnd->ShowListFace();
			}
			break;

		case IDM_SHOWINFOS:
			{
				wxWindow * window = this->FindWindowById(PREVIEWINFOWND);
				if (window)
				{
					wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_SHOWPANELINFO);
					window->GetEventHandler()->AddPendingEvent(evt);
				}
			}
			//centralWnd->ShowPanelInfos();
			break;

		case IDM_PICTUREDETAILS:
			mainWindow->ShowToolbar();
			break;


		case IDM_QUITTER:
			mainWindow->Exit();
			break;
		}
	}
}