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
	: CWindowMain("CPanelInfosWnd",parent, id)
{
    m_animationCtrl = nullptr;
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
	windowVisible = INFOS_WINDOW;
 

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
        viewerTheme->GetScrollInfosFileTheme(&themeScroll);
        
        CThemeTree theme;
        viewerTheme->GetTreeInfosFileTheme(&theme);
        
        infosFileWnd = new CInfosFileWnd(this, wxID_ANY, themeScroll, theme);

		infosFileWnd->Show(false);
	}
	
 	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetFiltreScrollTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeFiltreEffectTheme(&themeTree);

        filtreEffectWnd = new CFiltreEffectScrollWnd(this, wxID_ANY, themeScroll, themeTree);    
        filtreEffectWnd->Show(false);
	}

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollHistoryEffectTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeHistoryTheme(&themeTree);
		historyEffectWnd = new CInfoEffectWnd(this, wxID_ANY, themeScroll, themeTree);

		historyEffectWnd->Show(false);
	}
	
    if (viewerTheme != nullptr)
        viewerTheme->GetBitmapWindowTheme(&themeBitmap);
    
	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollThumbnailEffectTheme(&themeScroll);

		CThemeThumbnail themeThumbnail;
		viewerTheme->GetTreeThumbnailViewerEffectTheme(&themeThumbnail);
        
		thumbnailEffectWnd = new CThumbnailViewerEffectWnd(this, wxID_ANY, statusBarInterface, themeScroll, themeThumbnail);
		
		thumbnailEffectWnd->Show(false);
	}
    
	webBrowser = wxWebView::New(this, wxID_ANY);
	webBrowser->Show(false);
	
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetInfosToolbarTheme(&theme);
		infosToolbar = new CToolbarInfos(this, wxID_ANY, theme, this);
	}
    
    if (viewerTheme != nullptr)
    {
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollThumbnailEffectTheme(&themeScroll);
        
        CThemeTree themeTree;
        viewerTheme->GetTreeInfosFileTheme(&themeTree);
        criteriaTreeWnd = new CCriteriaTreeWnd(this, wxID_ANY, MAINVIEWERWINDOWID, fileGeolocalisation, themeTree, themeScroll);
    }


	this->fileGeolocalisation = fileGeolocalisation;
	Connect(wxEVT_SIZE, wxSizeEventHandler(CPanelInfosWnd::OnSize));
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPanelInfosWnd::OnPaint));
    
    m_animationCtrl = new wxAnimationCtrl(this, wxID_ANY);
    m_animationCtrl->Show(false);

	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
#ifdef WIN32
	m_animationCtrl->LoadFile(resourcePath + "\\loading.gif");
#else
	m_animationCtrl->LoadFile(resourcePath + "//loading.gif");
#endif

	m_animationCtrl->SetBackgroundColour(themeBitmap.colorScreen);
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

void CPanelInfosWnd::OnPaint(wxPaintEvent& event)
{

    int width = GetWindowWidth();
    int height = GetWindowHeight();
    if(width == 0 || height == 0)
        return;
    
    wxPaintDC dc(this);
    wxRect rc = GetRect();
    this->FillRect(&dc, rc, themeBitmap.colorScreen);
       
}

wxString CPanelInfosWnd::GetFilename()
{
	return filename;
}

CPanelInfosWnd::~CPanelInfosWnd()
{
    delete(m_animationCtrl);
    //delete bitmap;
	delete(infosFileWnd);  
	delete(historyEffectWnd);
	delete(filtreEffectWnd);
    delete(criteriaTreeWnd);
	delete(thumbnailEffectWnd);
	delete(infosToolbar);
	delete(webBrowser);
    delete(modificationManager);
}

void CPanelInfosWnd::UpdateScreenRatio()
{
    if(infosFileWnd != nullptr)
        infosFileWnd->UpdateScreenRatio();
    
    if(historyEffectWnd != nullptr)
        historyEffectWnd->UpdateScreenRatio();
    
    if(thumbnailEffectWnd != nullptr)
        thumbnailEffectWnd->UpdateScreenRatio();
    
    if(filtreEffectWnd != nullptr)
        filtreEffectWnd->UpdateScreenRatio();

    if(criteriaTreeWnd != nullptr)
        criteriaTreeWnd->UpdateScreenRatio();

    infosToolbar->UpdateScreenRatio();

    this->Resize();
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
	ShowEffect();
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
    ShowEffect();
}

void CPanelInfosWnd::ClickShowButton(const int &id)
{
	switch (id)
	{
	case WM_INFOS:
		ShowInfos();
		break;
	case WM_HISTORY:
		ShowHistory();
		break;
	case WM_EFFECT:
		ShowEffect();
		break;
	case WM_MAPS:
		ShowMap();
		break;
	case WM_AUDIOVIDEO:
		ShowAudioVideo();
		break;
	case WM_VIDEOEFFECT:
		ShowVideoEffect();
		break;

#ifdef VIEWER
    case WM_CRITERIA:
        ShowCriteria();
        break;
#endif
	}
}



//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void CPanelInfosWnd::ShowSettings()
{
}

void CPanelInfosWnd::EffectUpdate()
{
	if (thumbnailEffectWnd->GetFilename() != filename)
	{
		thumbnailEffectWnd->SetFile(filename);
	}
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
		case INFOS_WINDOW:
			InfosUpdate();
            infosToolbar->SetInfosPush();
			break;
        case WM_CRITERIA:
            criteriaTreeWnd->SetFile(filename);
            infosToolbar->SetCriteriaPush();
            break;
		case HISTORY_WINDOW:
			HistoryUpdate();
            infosToolbar->SetHistoryPush();
			break;
		case EFFECT_WINDOW:
			EffectUpdate();
            infosToolbar->SetEffectPush();
			break;
		case WM_AUDIOVIDEO:
			AudioVideoUpdate();
            infosToolbar->SetAudioVideoPush();
			break;
		case WM_VIDEOEFFECT:
			VideoEffectUpdate();
            infosToolbar->SetVideoEffectPush();
			break;

		case WEB_WINDOW:
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

void CPanelInfosWnd::ShowCriteria()
{
    HideAllWindow();
    windowVisible = WM_CRITERIA;
    criteriaTreeWnd->SetFile(filename);
    criteriaTreeWnd->Show(true);
    Resize();
}

void CPanelInfosWnd::ShowVideoEffect()
{
	HideAllWindow();
    if (!thumbnailEffectWnd->IsShown())
		thumbnailEffectWnd->Show(true);
    
	filtreEffectWnd->ApplyEffect(IDM_FILTRE_VIDEO, historyEffectWnd, this, filename, isVideo);
	windowVisible = WM_VIDEOEFFECT;
	LoadInfo();

	Resize();
	thumbnailEffectWnd->Refresh();
}

void CPanelInfosWnd::ShowAudioVideo()
{
	HideAllWindow();
    if (!thumbnailEffectWnd->IsShown())
		thumbnailEffectWnd->Show(true);
    
	filtreEffectWnd->ApplyEffect(IDM_FILTRE_AUDIOVIDEO, historyEffectWnd, this, filename, isVideo);
	windowVisible = WM_AUDIOVIDEO;
	LoadInfo();

	Resize();
	thumbnailEffectWnd->Refresh();
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



void CPanelInfosWnd::ShowFiltre(const wxString &title)
{
	HideAllWindow();
	infosToolbar->SetEffectParameterPush();
	filtreEffectWnd->Show(true);	
	if(windowVisible == EFFECT_WINDOW)
	{
		infosToolbar->SetEffectParameterActif(title);	
	}
	Resize();
}

void CPanelInfosWnd::ShowHistory()
{
	HideAllWindow();
	windowVisible = HISTORY_WINDOW;
	LoadInfo();
	historyEffectWnd->Show(true);
	Resize();
}

void CPanelInfosWnd::ShowEffect()
{
	HideAllWindow();
    if (!thumbnailEffectWnd->IsShown())
		thumbnailEffectWnd->Show(true);
    

	windowVisible = EFFECT_WINDOW;
	LoadInfo();

	Resize();
	thumbnailEffectWnd->Refresh();
}

void CPanelInfosWnd::ShowInfos()
{
	HideAllWindow();
	windowVisible = INFOS_WINDOW;
	LoadInfo();
	infosFileWnd->Show(true);
	Resize();
}

void CPanelInfosWnd::ShowMap()
{
	HideAllWindow();
	windowVisible = WEB_WINDOW;
	LoadInfo();
	webBrowser->Show(true);
	Resize();
}

void CPanelInfosWnd::OnSize(wxSizeEvent& event)
{
    
	int pictureWidth = event.GetSize().GetWidth();
	int pictureHeight = event.GetSize().GetHeight();

	if(pictureWidth > 0 && pictureHeight > 0)
	{
		width = pictureWidth;
		height = pictureHeight;
#if defined(WIN32) && defined(_DEBUG)
		wxString toShow = "CPanelInfosWnd size x : " + to_string(width) + " y : " + to_string(height) + "\n";
		OutputDebugString(toShow.ToStdWstring().c_str());
		
#endif
		Resize();
	}
}


void CPanelInfosWnd::DisplayURL(const wxString &url)
{
	webBrowser->LoadURL(url);
	infosToolbar->SetMapActif();
	Resize();
}


void CPanelInfosWnd::StartLoadingPicture(wxWindow * window)
{
    window->Show(false);
    m_animationCtrl->Show(true);
    m_animationCtrl->Play();
    this->Resize();
    
}

void CPanelInfosWnd::StopLoadingPicture(wxWindow * window)
{
    m_animationCtrl->Stop();
    m_animationCtrl->Show(false);
    window->Show(true);
    this->Resize();

}

void CPanelInfosWnd::InfosUpdate()
{
    if(infosFileWnd != nullptr)
        infosFileWnd->InfosUpdate(filename);
	//infosFileWnd->Resize();

	//int x = infosFileWnd->GetSize().x;
	//int y = infosFileWnd->GetSize().y;
}

void CPanelInfosWnd::HideAllWindow()
{
	infosToolbar->SetEffectParameterInactif();
	infosFileWnd->Show(false);
	historyEffectWnd->Show(false);
	thumbnailEffectWnd->Show(false);
	webBrowser->Show(false);
	filtreEffectWnd->Show(false);
    criteriaTreeWnd->Show(false);

}


void CPanelInfosWnd::Resize()
{
	wxRect rcAffichageBitmap;

	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = 0;
	rcAffichageBitmap.width = width;
	rcAffichageBitmap.height = height - infosToolbar->GetHeight();

	if(infosToolbar != nullptr)
	{
		infosToolbar->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, infosToolbar->GetHeight());
		//infosToolbar->SendSizeEvent();
	}


    if(m_animationCtrl->IsShown())
    {
        wxAnimation animation = m_animationCtrl->GetAnimation();
        wxSize animationSize = animation.GetSize();
        
        int xPos = rcAffichageBitmap.x + (width - animationSize.GetWidth()) / 2;
        int yPos = infosToolbar->GetHeight() + ((height - infosToolbar->GetHeight()) - animationSize.GetHeight()) / 2;
        
        m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
		//m_animationCtrl->SendSizeEvent();
    }
	else
	{
		wxWindow * windowToShow = nullptr;
		if (infosFileWnd->IsShown())
			windowToShow = infosFileWnd;
		else if (thumbnailEffectWnd->IsShown())
			windowToShow = thumbnailEffectWnd;
		else if (historyEffectWnd->IsShown())
			windowToShow = historyEffectWnd;
		else if (webBrowser->IsShown())
			windowToShow = webBrowser;
		else if (filtreEffectWnd->IsShown())
			windowToShow = filtreEffectWnd;
		else if (criteriaTreeWnd->IsShown())
			windowToShow = criteriaTreeWnd;

		if(windowToShow != nullptr)
		{
			windowToShow->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
			//windowToShow->SendSizeEvent();
		}
	}



}
