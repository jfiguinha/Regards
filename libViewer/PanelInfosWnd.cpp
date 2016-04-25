 #include "PanelInfosWnd.h"
#include "ThumbnailEffect.h"
#include "ToolbarInfos.h"
#include <BitmapWndViewer.h>
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include "RedEyeParameter.h"
#include <Gps.h>
#include <effect.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#include "../include/config_id.h"
#else
#include <window_id.h>
#include <config_id.h>
#endif
#include "MainWindow.h"
#include "PreviewWnd.h"
#include <FileUtility.h>
#include <LibResource.h>
#include "wx/stdpaths.h"
#include "FiltreEffectWnd.h"
#include "InfoEffectWnd.h"
#include "ThumbnailViewerEffectWnd.h"

using namespace Regards::Internet;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Control;
#define WM_UPDATEINFOS 1
wxDEFINE_EVENT(EVENT_ENDINFOSUPDATE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UPDATETHUMBNAILTHREAD, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CPanelInfosWnd::CPanelInfosWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CVideoEffectParameter * videoEffectParameter, CFileGeolocation * fileGeolocalisation)
	: CWindowMain(parent, id)
{
	//CRegardsConfigParam * config = CParamInit::getInstance();
	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

    
    wxString folder = CFileUtility::GetResourcesFolderPath();
#ifdef __APPLE__
    
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    folder = stdp.GetDocumentsDir();
#endif
    
	modificationManager = new CModificationManager(folder);
	this->videoEffectParameter = videoEffectParameter;

	if (viewerTheme != nullptr)
	{
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollInfosFileTheme(&themeScroll);
        
        CThemeTree theme;
        viewerTheme->GetTreeInfosFileTheme(&theme);
        
        infosFileWnd = new CInfosFileWnd(this, wxID_ANY, themeScroll, theme);
	}
	
    
#ifdef EFFECT_VIDEO
	if (config->GetVideoLibrary() == LIBOPENGL)
	{
		if (viewerTheme != nullptr)
		{
			openGLVideoMode = true;

			CThemeScrollBar themeScroll;
			viewerTheme->GetScrollEffectVideoTheme(&themeScroll);
			effectVideoWndScroll = new CScrollbarWnd(this, wxID_ANY);

			CThemeTree theme;
			viewerTheme->GetTreeEffectVideoWndTheme(&theme);
			treeEffectVideo = new CTreeWindow(effectVideoWndScroll, wxID_ANY, theme);

			effectVideoWndScroll->SetCentralWindow(treeEffectVideo, themeScroll);

			effectVideoWndScroll->Show(false);
			//treeEffectVideo->Show(false);
		}
	}
	else
		openGLVideoMode = false;
#endif

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
    
	webBrowser = wxWebView::New(this, wxID_ANY, url);
	webBrowser->Show(false);
	
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetInfosToolbarTheme(&theme);
		infosToolbar = new CToolbarInfos(this, wxID_ANY, theme, this);
	}

    
#ifdef VIEWER
    
    if (viewerTheme != nullptr)
    {
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollThumbnailEffectTheme(&themeScroll);
        
        CThemeTree themeTree;
        viewerTheme->GetTreeInfosFileTheme(&themeTree);
        criteriaTreeWnd = new CCriteriaTreeWnd(this, wxID_ANY, MAINVIEWERWINDOWID, fileGeolocalisation, themeTree, themeScroll);
    }
    
#endif

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

void CPanelInfosWnd::OnPaint(wxPaintEvent& event)
{
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
    delete bitmap;
	delete(infosFileWnd);
#ifdef EFFECT_VIDEO
	delete(effectVideoWndOld);
#endif
    
	delete(historyEffectWnd);
	delete(filtreEffectWnd);
    
#ifdef EFFECT_VIDEO
	delete(treeEffectVideo);
	delete(effectVideoWndScroll);
#endif
    
#ifdef VIEWER
    delete(criteriaTreeWnd);
#endif

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
    
#ifdef VIEWER
    if(criteriaTreeWnd != nullptr)
        criteriaTreeWnd->UpdateScreenRatio();
#endif

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

void CPanelInfosWnd::SetBitmapFile(const wxString &filename, CRegardsBitmap * bitmap, const bool &isThumbnail)
{
    if(!isThumbnail)
    {
        *this->bitmap = *bitmap;
        infosToolbar->SetPictureToolbar();
        
    }
    else
    {
        infosToolbar->SetPictureThumbnailToolbar();
        infosToolbar->SetInfosActif();
    }
    
	if (this->filename != filename)
	{
		infosToolbar->SetEffectParameterInactif();
		this->filename = filename;

		//fileGeolocalisation->SetFile(filename);

		if (!fileGeolocalisation->HasGps())
			infosToolbar->SetMapInactif();
		else
			infosToolbar->SetMapActif();

		if (isVideo)
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
    filtreEffectWnd->ApplyEffect(numItem, historyEffectWnd, this);
}

void CPanelInfosWnd::OnFiltreOk(const int &numFiltre)
{
    filtreEffectWnd->OnFiltreOk(numFiltre, historyEffectWnd);
	ShowEffect();
}

void CPanelInfosWnd::OnFiltreCancel()
{
    CBitmapWndViewer * bitmapViewer = (CBitmapWndViewer *)this->FindWindowById(BITMAPWINDOWVIEWERID);
    if (bitmapViewer != nullptr)
        bitmapViewer->SetBitmapPreviewEffect(0);
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
	if (isVideo)
	{
#ifdef EFFECT_VIDEO
		CEffectVideoWnd * effectVideo = new CEffectVideoWnd(videoEffectParameter, treeEffectVideo->GetTheme(), treeEffectVideo);
		effectVideo->Init();
		treeEffectVideo->SetTreeControl(effectVideo);
		delete(effectVideoWndOld);
		effectVideoWndOld = effectVideo;
#endif
	}
	else
	{
		if (thumbnailEffectWnd->GetFilename() != filename)
			thumbnailEffectWnd->SetFile(filename, bitmap);
	}
}

void CPanelInfosWnd::HistoryUpdate()
{
	wxString historyLibelle = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1);
	CShowBitmap * bitmapViewer = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERID);
    historyEffectWnd->HistoryUpdate(bitmap, filename, historyLibelle, bitmapViewer->GetBitmapViewer(), modificationManager);
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
#ifdef VIEWER
        case WM_CRITERIA:
            criteriaTreeWnd->SetFile(filename);
            infosToolbar->SetCriteriaPush();
            break;
#endif
		case HISTORY_WINDOW:
			HistoryUpdate();
            infosToolbar->SetHistoryPush();
			break;
		case EFFECT_WINDOW:
			EffectUpdate();
            infosToolbar->SetEffectPush();
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

#ifdef VIEWER
void CPanelInfosWnd::ShowCriteria()
{
    HideAllWindow();
    windowVisible = WM_CRITERIA;
    criteriaTreeWnd->SetFile(filename);
    criteriaTreeWnd->Show(true);
    Resize();
}

void CPanelInfosWnd::UpdateData()
{
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

#endif

void CPanelInfosWnd::ShowFiltre(const wxString &title)
{
	HideAllWindow();
    infosToolbar->SetEffectParameterPush();
	filtreEffectWnd->Show(true);
	infosToolbar->SetEffectParameterActif(title);
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

#ifdef EFFECT_VIDEO
	if (isVideo && openGLVideoMode)
	{
		effectVideoWndScroll->Show(true);
		if (!treeEffectVideo->IsShown())
			treeEffectVideo->Show(true);
	}
	else
	{
		thumbnailEffectScroll->Show(true);
		if (!thumbnailEffect->IsShown())
			thumbnailEffect->Show(true);
	}
#else
    
    if(!isVideo)
    {
        if (!thumbnailEffectWnd->IsShown())
            thumbnailEffectWnd->Show(true);
    }
    
#endif

	windowVisible = EFFECT_WINDOW;
	LoadInfo();

	Resize();
    
#ifdef EFFECT_VIDEO
	if (isVideo && openGLVideoMode)
		effectVideoWndScroll->Refresh();
	else
		thumbnailEffectScroll->Refresh();
#else
    
    if(!isVideo)
        thumbnailEffectWnd->Refresh();
    
#endif
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
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	Resize();
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
}

void CPanelInfosWnd::HideAllWindow()
{
	infosToolbar->SetEffectParameterInactif();
	infosFileWnd->Show(false);
#ifdef EFFECT_VIDEO
	if (openGLVideoMode)
		effectVideoWndScroll->Show(false);
#endif
	historyEffectWnd->Show(false);
	thumbnailEffectWnd->Show(false);
	webBrowser->Show(false);
	filtreEffectWnd->Show(false);
#ifdef VIEWER
    criteriaTreeWnd->Show(false);
#endif
}


void CPanelInfosWnd::Resize()
{
	wxRect rcAffichageBitmap;

	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = 0;
	rcAffichageBitmap.width = width;
	rcAffichageBitmap.height = height - infosToolbar->GetHeight();

	infosToolbar->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, infosToolbar->GetHeight());

    if(m_animationCtrl->IsShown())
    {
        wxAnimation animation = m_animationCtrl->GetAnimation();
        wxSize animationSize = animation.GetSize();
        
        int xPos = rcAffichageBitmap.x + (width - animationSize.GetWidth()) / 2;
        int yPos = infosToolbar->GetHeight() + ((height - infosToolbar->GetHeight()) - animationSize.GetHeight()) / 2;
        
        m_animationCtrl->SetSize(xPos, yPos, animationSize.GetWidth(), animationSize.GetHeight());
    }
	else if (infosFileWnd->IsShown())
		infosFileWnd->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
#ifdef EFFECT_VIDEO
	else if (effectVideoWndScroll->IsShown() && openGLVideoMode)
		effectVideoWndScroll->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
#endif
	else if (thumbnailEffectWnd->IsShown())
		thumbnailEffectWnd->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (historyEffectWnd->IsShown())
		historyEffectWnd->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (webBrowser->IsShown())
		webBrowser->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (filtreEffectWnd->IsShown())
		filtreEffectWnd->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
#ifdef VIEWER
    else if (criteriaTreeWnd->IsShown())
        criteriaTreeWnd->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
#endif
}
