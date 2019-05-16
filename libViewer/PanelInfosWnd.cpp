#include <header.h>
#include "PanelInfosWnd.h"
#include "ThumbnailEffect.h"
#include "ToolbarInfos.h"
#include <BitmapWndViewer.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include "RedEyeParameter.h"
#include <Gps.h>
#include <window_id.h>
#include <config_id.h>
#include "MainWindow.h"
#include "PreviewWnd.h"
#include <FileUtility.h>
#include <LibResource.h>
#include "wx/stdpaths.h"
#include "FiltreEffectWnd.h"
#include "InfoEffectWnd.h"
#include "ThumbnailViewerEffectWnd.h"
#include <ImageLoadingFormat.h>
#include <FilterData.h>
using namespace Regards::Internet;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Control;
#define WM_UPDATEINFOS 1
wxDEFINE_EVENT(EVENT_ENDINFOSUPDATE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UPDATETHUMBNAILTHREAD, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CPanelInfosWnd::CPanelInfosWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CFileGeolocation * fileGeolocalisation)
	: CTabWindow("CPanelInfosWnd",parent, id)
{
	infosFileWnd = nullptr;
         
    historyEffectWnd = nullptr;
	thumbnailEffectWnd = nullptr;
	filtreEffectWnd = nullptr;
    criteriaTreeWnd = nullptr;

	webBrowser = nullptr;
	infosToolbar = nullptr;

	//Effect Parameter
	modificationManager = nullptr;

    isThumbnail = false;
	isVideo = false;
	filename = L"";
	//bitmap = new CRegardsBitmap();
	width = 0;
	height = 0;
    url = "http://www.google.fr";
	windowVisible = WM_INFOS;
 

	//CRegardsConfigParam * config = CParamInit::getInstance();
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

    
    wxString folder = CFileUtility::GetDocumentFolderPath();
#ifdef __APPLE__
    
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    folder = stdp.GetDocumentsDir();
#endif
    
	modificationManager = new CModificationManager(folder);

	if (viewerTheme != nullptr)
	{
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollTheme(&themeScroll);
        
        CThemeTree theme;
        viewerTheme->GetTreeTheme(&theme);
        
        infosFileWnd = new CInfosFileWnd(this, wxID_ANY, themeScroll, theme);

		infosFileWnd->Show(false);
        
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = infosFileWnd;
        tabInfosFile->windowMain = infosFileWnd;
        tabInfosFile->windowName = WM_INFOS;
        listWindow.push_back(tabInfosFile);
	}
	
 	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeTheme(&themeTree);

        filtreEffectWnd = new CFiltreEffectScrollWnd(this, wxID_ANY, themeScroll, themeTree);    
        filtreEffectWnd->Show(false);
        
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = filtreEffectWnd;
        tabInfosFile->windowMain = filtreEffectWnd;
        tabInfosFile->windowName = WM_EFFECTPARAMETER;
        listWindow.push_back(tabInfosFile);
	}

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeTheme(&themeTree);
		historyEffectWnd = new CInfoEffectWnd(this, wxID_ANY, themeScroll, themeTree);
		historyEffectWnd->Show(false);
        
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = historyEffectWnd;
        tabInfosFile->windowMain = historyEffectWnd;
        tabInfosFile->windowName = WM_HISTORY;
        listWindow.push_back(tabInfosFile);
	}
	
    if (viewerTheme != nullptr)
        viewerTheme->GetBitmapWindowTheme(&themeBitmap);
    
	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeThumbnail themeThumbnail;
		viewerTheme->GetThumbnailTheme(&themeThumbnail);
        
		thumbnailEffectWnd = new CThumbnailViewerEffectWnd(this, wxID_ANY, statusBarInterface, themeScroll, themeThumbnail);
		
		thumbnailEffectWnd->Show(false);
        
        CTabWindowData * tabInfosFileVideo = new CTabWindowData();
        tabInfosFileVideo->window = thumbnailEffectWnd;
        tabInfosFileVideo->windowMain = thumbnailEffectWnd;
        tabInfosFileVideo->windowName = WM_VIDEOEFFECT;
        listWindow.push_back(tabInfosFileVideo);
        
        CTabWindowData * tabInfosFileEffect = new CTabWindowData();
        tabInfosFileEffect->window = thumbnailEffectWnd;
        tabInfosFileEffect->windowMain = thumbnailEffectWnd;
        tabInfosFileEffect->windowName = WM_EFFECT;
        listWindow.push_back(tabInfosFileEffect);
        
        CTabWindowData * tabInfosFileAudioVideo = new CTabWindowData();
        tabInfosFileAudioVideo->window = thumbnailEffectWnd;
        tabInfosFileAudioVideo->windowMain = thumbnailEffectWnd;
        tabInfosFileAudioVideo->windowName = WM_AUDIOVIDEO;
        listWindow.push_back(tabInfosFileAudioVideo);
	}
    
    if(webBrowser == nullptr)
    {
        webBrowser = wxWebView::New(this, wxID_ANY);
        webBrowser->Show(false);   

        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = webBrowser;
        tabInfosFile->windowName = WM_MAPS;
        listWindow.push_back(tabInfosFile);     
    }

	
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetInfosToolbarTheme(&theme);
		infosToolbar = new CToolbarInfos(this, wxID_ANY, theme, this, false);
	}
    
    if (viewerTheme != nullptr)
    {
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollTheme(&themeScroll);
        
        CThemeTree themeTree;
        viewerTheme->GetTreeTheme(&themeTree);
        criteriaTreeWnd = new CCriteriaTreeWnd(this, wxID_ANY, MAINVIEWERWINDOWID, fileGeolocalisation, themeTree, themeScroll);
    
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = criteriaTreeWnd;
        tabInfosFile->windowMain = criteriaTreeWnd;
        tabInfosFile->windowName = WM_CRITERIA;
        listWindow.push_back(tabInfosFile);
    }


	this->fileGeolocalisation = fileGeolocalisation;

    toolbarWindow = infosToolbar;
    
    
}

CFiltreEffect * CPanelInfosWnd::GetFilterWindow(int &numFiltre)
{
	if(filtreEffectWnd != nullptr)
	{
		numFiltre = filtreEffectWnd->GetNumFiltre();
		return filtreEffectWnd->GetFiltreEffect();
	}
	return nullptr;
}

wxString CPanelInfosWnd::GetFilename()
{
	return filename;
}

CPanelInfosWnd::~CPanelInfosWnd()
{
	delete(infosFileWnd);  
	delete(historyEffectWnd);
	delete(filtreEffectWnd);
    delete(criteriaTreeWnd);
	delete(thumbnailEffectWnd);
	delete(infosToolbar);
	delete(webBrowser);
    delete(modificationManager);
}

void CPanelInfosWnd::SetAnimationFile(const wxString &filename)
{
	if (this->filename != filename)
	{
		infosToolbar->SetEffectParameterInactif();
		this->filename = filename;
		infosToolbar->SetPictureThumbnailToolbar();
		infosToolbar->SetInfosActif();
		infosToolbar->SetMapInactif();
		LoadInfo();
		this->isVideo = false;
	}
}

void CPanelInfosWnd::SetVideoFile(const wxString &filename)
{
	if (this->filename != filename)
	{
		infosToolbar->SetEffectParameterInactif();
		this->filename = filename;
        
		//fileGeolocalisation->SetFile(filename);
        
		if (!this->isVideo)
		{
			infosToolbar->SetVideoToolbar();
			infosToolbar->SetInfosActif();

			if (!fileGeolocalisation->HasGps())
				infosToolbar->SetMapInactif();
            else
                infosToolbar->SetMapActif();
		}
		else
		{
			if (!fileGeolocalisation->HasGps())
			{
				if (webBrowser->IsShown())
					if (!fileGeolocalisation->HasGps())
						infosToolbar->SetInfosActif();

				infosToolbar->SetMapInactif();
			}
            else
            {
                infosToolbar->SetMapActif();
            }
            
           
		}

		
		LoadInfo();
		this->isVideo = true;
	}
}

void CPanelInfosWnd::SetBitmapFile(const wxString &filename, const bool &isThumbnail)
{
	if(!isThumbnail)
		infosToolbar->SetPictureToolbar();
	else
		infosToolbar->SetPictureThumbnailToolbar();


	if (this->filename != filename)
	{


		infosToolbar->SetEffectParameterInactif();
		this->filename = filename;

		if (!fileGeolocalisation->HasGps())
			infosToolbar->SetMapInactif();
		else
			infosToolbar->SetMapActif();

		infosToolbar->SetInfosActif();

		if (!fileGeolocalisation->HasGps())
		{
			if (!fileGeolocalisation->HasGps())
				infosToolbar->SetMapInactif();

			if (webBrowser->IsShown())
				if (!fileGeolocalisation->HasGps())
					infosToolbar->SetInfosActif();
		}


		this->isVideo = false;

		LoadInfo();
	}
    
}

void CPanelInfosWnd::ApplyEffect(const int &numItem)
{
	//Test si l'history fonctionne ou pas 
	HistoryUpdate();
    filtreEffectWnd->ApplyEffect(numItem, historyEffectWnd, this, filename, isVideo);
}

void CPanelInfosWnd::OnFiltreOk(const int &numFiltre)
{
    filtreEffectWnd->OnFiltreOk(numFiltre, historyEffectWnd);
	ClickShowButton(WM_EFFECT);
}

void CPanelInfosWnd::OnFiltreCancel()
{
	CShowBitmap * showBitmap = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERID);
   
    if (showBitmap != nullptr)
	{
		CBitmapWndViewer * bitmapViewer = showBitmap->GetBitmapViewer();
		bitmapViewer->OnFiltreCancel();
        bitmapViewer->SetBitmapPreviewEffect(0);
	}
    
    
    CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
    if (mainWindow != nullptr)
    {
        wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_REFRESHPICTURE);
        mainWindow->GetEventHandler()->AddPendingEvent(evt);
    }
    ClickShowButton(WM_EFFECT);
}

void CPanelInfosWnd::EffectUpdate()
{
	if (thumbnailEffectWnd->GetFilename() != filename)
	{
		thumbnailEffectWnd->SetFile(filename);
	}
}

void CPanelInfosWnd::ShowFiltre(const wxString &title)
{
	HideAllWindow();
	infosToolbar->SetEffectParameterPush();
	filtreEffectWnd->Show(true);	
	if(windowVisible == WM_EFFECT)
	{
		infosToolbar->SetEffectParameterActif(title);	
	}
	Resize();
}

void CPanelInfosWnd::AudioVideoUpdate()
{
	if (thumbnailEffectWnd->GetFilename() != filename)
	{
		thumbnailEffectWnd->SetFile(filename);
	}
}

void CPanelInfosWnd::VideoEffectUpdate()
{
	if (thumbnailEffectWnd->GetFilename() != filename)
	{
		thumbnailEffectWnd->SetFile(filename);
	}
}

void CPanelInfosWnd::HistoryUpdate()
{
	if(!isVideo)
	{
		wxString historyLibelle = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1);
		CShowBitmap * bitmapViewer = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERID);
		if(bitmapViewer != nullptr)
		{
			CRegardsBitmap * bitmap = bitmapViewer->GetBitmap(true);
			historyEffectWnd->HistoryUpdate(bitmap, filename, historyLibelle, bitmapViewer->GetBitmapViewer(), modificationManager);
			delete bitmap;
		}
	}
}

void CPanelInfosWnd::LoadInfo()
{
	if (this->IsShown())
	{
		switch (windowVisible)
		{
		case WM_INFOS:
			InfosUpdate();
            infosToolbar->SetInfosPush();
			break;
        case WM_CRITERIA:
            criteriaTreeWnd->SetFile(filename);
            infosToolbar->SetCriteriaPush();
            break;
		case WM_HISTORY:
			HistoryUpdate();
            infosToolbar->SetHistoryPush();
			break;
		case WM_EFFECT:
            if (!thumbnailEffectWnd->IsShown())
                thumbnailEffectWnd->Show(true);
			EffectUpdate();
            infosToolbar->SetEffectPush();
            thumbnailEffectWnd->Refresh();
			break;
		case WM_AUDIOVIDEO:
            if (!thumbnailEffectWnd->IsShown())
                thumbnailEffectWnd->Show(true);
            filtreEffectWnd->ApplyEffect(IDM_FILTRE_AUDIOVIDEO, historyEffectWnd, this, filename, isVideo);
			AudioVideoUpdate();
            infosToolbar->SetAudioVideoPush();
            thumbnailEffectWnd->Refresh();
			break;
		case WM_VIDEOEFFECT:
            if (!thumbnailEffectWnd->IsShown())
                thumbnailEffectWnd->Show(true);
            filtreEffectWnd->ApplyEffect(IDM_FILTRE_VIDEO, historyEffectWnd, this, filename, isVideo);
			VideoEffectUpdate();
            infosToolbar->SetVideoEffectPush();
            thumbnailEffectWnd->Refresh();
			break;

		case WM_MAPS:
            {
                wxString newUrl = MapsUpdate();
                if(url != newUrl)
                    DisplayURL(newUrl);
                url = newUrl;
                infosToolbar->SetMapPush();
            }
			break;
		}
	}
}

wxString CPanelInfosWnd::MapsUpdate()
{
	wxString url = L"http://www.openstreetmap.org/?mlat=";
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"&mlon=");
	url.append(fileGeolocalisation->GetLongitude());
	url.append(L"#map=15/");
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"/");
	url.append(fileGeolocalisation->GetLongitude());
	//DisplayURL(url);
    return url;
}

void CPanelInfosWnd::UpdateData()
{
	fileGeolocalisation->RefreshData();

    if (!fileGeolocalisation->HasGps())
    {
        if (webBrowser->IsShown())
            if (!fileGeolocalisation->HasGps())
                infosToolbar->SetInfosActif();
        
        infosToolbar->SetMapInactif();
    }
    else
        infosToolbar->SetMapActif();
    
	
    LoadInfo();
}

void CPanelInfosWnd::DisplayURL(const wxString &url)
{
	webBrowser->LoadURL(url);
	infosToolbar->SetMapActif();
	Resize();
}

void CPanelInfosWnd::InfosUpdate()
{
    if(infosFileWnd != nullptr)
        infosFileWnd->InfosUpdate(filename);
}