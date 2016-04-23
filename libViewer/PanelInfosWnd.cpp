#include "PanelInfosWnd.h"
#include "ThumbnailEffect.h"
#include "ToolbarInfos.h"
#include <BitmapWndViewer.h>
#include <FiltreEffet.h>
#include "CloudsEffectParameter.h"
#include "BrightAndContrastEffectParameter.h"
#include "FreeRotateEffectParameter.h"
#include "MotionBlurEffectParameter.h"
#include "LensFlareEffectParameter.h"
#include "SolarisationEffectParameter.h"
#include "PosterisationEffectParameter.h"
#include "PhotoFiltreEffectParameter.h"
#include "RgbEffectParameter.h"
#include "SwirlEffectParameter.h"
#include <libResource.h>
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
#include "wx/stdpaths.h"
using namespace Regards::Internet;
using namespace Regards::Window;
using namespace Regards::Viewer;

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
		InfosFileScroll = new CScrollbarWnd(this, wxID_ANY);

		CThemeTree themeTree;
		viewerTheme->GetTreeInfosFileTheme(&themeTree);
		treeInfos = new CTreeWindow(InfosFileScroll, wxID_ANY, themeTree);

		InfosFileScroll->SetCentralWindow(treeInfos, themeScroll);
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
		filtreEffectScroll = new CScrollbarWnd(this, wxID_ANY);

		CThemeTree themeTree;
		viewerTheme->GetTreeFiltreEffectTheme(&themeTree);
		treeFiltreEffect = new CTreeWindow(filtreEffectScroll, wxID_ANY, themeTree);

		filtreEffectScroll->SetCentralWindow(treeFiltreEffect, themeScroll);

		filtreEffectScroll->Show(false);
		//treeFiltreEffect->Show(false);
	}

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollHistoryEffectTheme(&themeScroll);
		historyEffectScroll = new CScrollbarWnd(this, wxID_ANY);

		CThemeTree themeTree;
		viewerTheme->GetTreeHistoryTheme(&themeTree);
		treeHistoryEffect = new CTreeWindow(historyEffectScroll, wxID_ANY, themeTree);

		historyEffectScroll->SetCentralWindow(treeHistoryEffect, themeScroll);

		historyEffectScroll->Show(false);
		//treeHistoryEffect->Show(false);
	}
	
    if (viewerTheme != nullptr)
        viewerTheme->GetBitmapWindowTheme(&themeBitmap);
    
	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollThumbnailEffectTheme(&themeScroll);
		thumbnailEffectScroll = new CScrollbarWnd(this, wxID_ANY);

		CThemeThumbnail themeThumbnail;
		viewerTheme->GetTreeThumbnailViewerEffectTheme(&themeThumbnail);
		thumbnailEffect = new CThumbnailViewerEffect(thumbnailEffectScroll, wxID_ANY, statusBarInterface, themeThumbnail);
		
		thumbnailEffectScroll->SetCentralWindow(thumbnailEffect, themeScroll);

		thumbnailEffectScroll->Show(false);
		//thumbnailEffect->Show(false);
	}

	webBrowser = wxWebView::New(this, wxID_ANY);
	webBrowser->Show(false);
	
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetInfosToolbarTheme(&theme);
		infosToolbar = new CToolbarInfos(this, wxID_ANY, theme, this);
	}

 
	effectParameter = nullptr;
	this->fileGeolocalisation = fileGeolocalisation;
	Connect(wxEVT_SIZE, wxSizeEventHandler(CPanelInfosWnd::OnSize));
    Connect(wxEVT_PAINT, wxPaintEventHandler(CPanelInfosWnd::OnPaint));
    
    //Connect(EVENT_UPDATETHUMBNAILTHREAD, wxCommandEventHandler(CPanelInfosWnd::UpdateThumbnailEffectEvent));
    Connect(EVENT_UPDATEINFOSTHREAD, wxCommandEventHandler(CPanelInfosWnd::UpdateTreeInfosEvent));
    
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

void CPanelInfosWnd::UpdateTreeInfosEvent(wxCommandEvent &event)
{
    CThreadLoadInfos * threadInfos = (CThreadLoadInfos *)event.GetClientData();
    if(threadInfos->filename == filename)
    {
        threadInfos->infosFileWnd->CreateElement();
        StopLoadingPicture(InfosFileScroll);
        treeInfos->SetTreeControl(threadInfos->infosFileWnd);
        delete(infosFileWndOld);
        infosFileWndOld = threadInfos->infosFileWnd;
    }
    else
        delete(threadInfos->infosFileWnd);
    
    threadInfos->threadLoadInfos->join();
    delete threadInfos->threadLoadInfos;
    delete threadInfos;

}

void CPanelInfosWnd::GenerateTreeInfos(CThreadLoadInfos * threadInfos)
{
    CInfosFile * infosFileWnd = threadInfos->infosFileWnd;
    infosFileWnd->SetFile(threadInfos->filename);
    
    wxCommandEvent event(EVENT_UPDATEINFOSTHREAD);
    event.SetClientData(threadInfos);
    wxPostEvent(threadInfos->panelInfos, event);
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
	delete(infosFileWndOld);
#ifdef EFFECT_VIDEO
	delete(effectVideoWndOld);
#endif
	delete(historyEffectOld);
	delete(filtreEffectOld);
	delete(treeFiltreEffect);
	delete(treeHistoryEffect);
	delete(treeInfos);
#ifdef EFFECT_VIDEO
	delete(treeEffectVideo);
#endif
	delete(modificationManager);
	delete(InfosFileScroll);
#ifdef EFFECT_VIDEO
	delete(effectVideoWndScroll);
#endif
	delete(historyEffectScroll);
	delete(thumbnailEffect);
	delete(thumbnailEffectScroll);
	delete(infosToolbar);
	delete(webBrowser);
	delete(filtreEffectScroll);
	delete(effectParameter);
}

void CPanelInfosWnd::UpdateScreenRatio()
{
    InfosFileScroll->UpdateScreenRatio();
    if(infosFileWndOld != nullptr)
    {
        infosFileWndOld->UpdateScreenRatio();
        treeInfos->UpdateScreenRatio();
    }
    
    
    historyEffectScroll->UpdateScreenRatio();
    if(historyEffectOld != nullptr)
    {
        historyEffectOld->UpdateScreenRatio();
        treeHistoryEffect->UpdateScreenRatio();
    }
    
    thumbnailEffectScroll->UpdateScreenRatio();
    thumbnailEffect->UpdateScreenRatio();
    
    filtreEffectScroll->UpdateScreenRatio();
    if(filtreEffectOld != nullptr)
    {
        filtreEffectOld->UpdateScreenRatio();
        treeFiltreEffect->UpdateScreenRatio();
    }

    
    
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

			if(webBrowser != nullptr)
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


	wxString solarisationLibelle = CLibResource::LoadStringFromResource(L"LBLfilterSolarize",1);
	wxString rotateFreeLibelle = CLibResource::LoadStringFromResource(L"LBLfilterRotate", 1);
	wxString lightContrastLibelle = CLibResource::LoadStringFromResource(L"LBLfilterLight", 1);
	wxString photoFiltreLibelle = CLibResource::LoadStringFromResource(L"LBLfilterPhoto", 1);
	wxString posterisationLibelle = CLibResource::LoadStringFromResource(L"LBLfilterPosterisation", 1);
	wxString colorBalanceLibelle = CLibResource::LoadStringFromResource(L"LBLfilterColor", 1);
	wxString swirlLibelle = CLibResource::LoadStringFromResource(L"LBLfilterSwirl", 1);
	wxString cloudsLibelle = CLibResource::LoadStringFromResource(L"LBLfilterClouds", 1);
	wxString redEyeLibelle = CLibResource::LoadStringFromResource(L"LBLfilterRedEye", 1);
    wxString cropLibelle = CLibResource::LoadStringFromResource("LBLCROP",1);
	wxString lensFlareLibelle = CLibResource::LoadStringFromResource(L"LBLfilterLensFlare", 1);
	wxString motionBlurLibelle = CLibResource::LoadStringFromResource(L"LBLfilterMotion", 1);

	CPreviewWnd * previewWindow = (CPreviewWnd *)this->FindWindowById(PREVIEWVIEWERID);
	CShowBitmap * showBitmap = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERID);

	if (showBitmap != nullptr)
	{
		CBitmapWndViewer * bitmapViewer = showBitmap->GetBitmapViewer();

		CFiltreEffect * filtreEffect = new CFiltreEffect(bitmapViewer, treeFiltreEffect->GetTheme(), treeFiltreEffect);

		if (bitmapViewer != nullptr)
		{
			switch (numItem)
			{
			case IDM_FILTRE_SOFTEN:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterSoften", 1));
				break;
			case IDM_FILTRE_FLOU:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterBlur", 1));
				break;

			case IDM_HISTOGRAMNORMALIZE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLHistogramNormalize", 1));
				break;

			case IDM_HISTOGRAMEQUALIZE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLHistogramEqualize", 1));
				break;

			case IDM_HISTOGRAMLOG:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLHistogramLog", 1));
				break;
                    
            case IDM_CROP:
                {
                    if (previewWindow != nullptr)
                        previewWindow->ShowValidationToolbar(true, IDM_CROP);
                    
                    bitmapViewer->SetTool(IDM_CROP);
                    bitmapViewer->SetBitmapPreviewEffect(IDM_CROP);
                }
                break;
                    
			case IDM_REDEYE:
			{
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_REDEYE);

				bitmapViewer->SetTool(IDM_REDEYE);
				bitmapViewer->SetBitmapPreviewEffect(IDM_REDEYE);

				//bitmapViewer->SetBitmapEffect(numItem);
				//historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(LBLfilterRedEye));
			}
			break;

			case IDM_AJUSTEMENT_SOLARISATION:
				delete(effectParameter);
				effectParameter = new CSolarisationEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(IDM_AJUSTEMENT_SOLARISATION, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_AJUSTEMENT_SOLARISATION);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_AJUSTEMENT_SOLARISATION);
				ShowFiltre(solarisationLibelle);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case IDM_FILTRE_FLOUGAUSSIEN:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterGaussian", 1));
				break;
			case IDM_FILTREANTIBRUIT:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterMedian", 1));
				break;
			case IDM_FILTRE_MOTIONBLUR:
				//bitmapViewer->SetBitmapEffect(numItem);
				//historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(LBLfilterMotion));
				delete(effectParameter);
				effectParameter = new CMotionBlurEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(IDM_FILTRE_MOTIONBLUR, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_FILTRE_MOTIONBLUR);
				ShowFiltre(motionBlurLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_FILTRE_MOTIONBLUR);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case IDM_ROTATE_FREE:
				delete(effectParameter);
				effectParameter = new CFreeRotateEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(IDM_ROTATE_FREE, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_ROTATE_FREE);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_ROTATE_FREE);
				ShowFiltre(rotateFreeLibelle);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case IDM_IMAGE_LIGHTCONTRAST:
				delete(effectParameter);
				effectParameter = new CBrightAndContrastEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(IDM_IMAGE_LIGHTCONTRAST, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_IMAGE_LIGHTCONTRAST);
				ShowFiltre(lightContrastLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_IMAGE_LIGHTCONTRAST);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case ID_AJUSTEMENT_PHOTOFILTRE:
				delete(effectParameter);
				effectParameter = new CPhotoFiltreEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(ID_AJUSTEMENT_PHOTOFILTRE, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), ID_AJUSTEMENT_PHOTOFILTRE);
				ShowFiltre(photoFiltreLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, ID_AJUSTEMENT_PHOTOFILTRE);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case ID_AJUSTEMENT_POSTERISATION:
				delete(effectParameter);
				effectParameter = new CPosterisationEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(ID_AJUSTEMENT_POSTERISATION, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), ID_AJUSTEMENT_POSTERISATION);
				ShowFiltre(posterisationLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, ID_AJUSTEMENT_POSTERISATION);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case IDM_FILTRELENSFLARE:
			{
				//bitmapViewer->SetBitmapEffect(numItem);
				//historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(LBLfilterLensFlare));
				delete(effectParameter);
				effectParameter = new CLensFlareEffectParameter();
				bitmapViewer->SetTool(IDM_FILTRELENSFLARE);
				bitmapViewer->SetBitmapPreviewEffect(IDM_FILTRELENSFLARE, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_FILTRELENSFLARE);
				ShowFiltre(lensFlareLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_FILTRELENSFLARE);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
			}
			break;
			case IDM_COLOR_BALANCE:
				delete(effectParameter);
				effectParameter = new CRgbEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(IDM_COLOR_BALANCE, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_COLOR_BALANCE);
				ShowFiltre(colorBalanceLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_COLOR_BALANCE);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case IDM_FILTRE_SWIRL:
				delete(effectParameter);
				effectParameter = new CSwirlEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(IDM_FILTRE_SWIRL, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_FILTRE_SWIRL);
				ShowFiltre(swirlLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_FILTRE_SWIRL);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case IDM_FILTRE_CLOUDS:
				delete(effectParameter);
				effectParameter = new CCloudsEffectParameter();
				bitmapViewer->SetBitmapPreviewEffect(IDM_FILTRE_CLOUDS, effectParameter);
				filtreEffect->Init(effectParameter, bitmapViewer->GetBitmap(), IDM_FILTRE_CLOUDS);
				ShowFiltre(cloudsLibelle);
				if (previewWindow != nullptr)
					previewWindow->ShowValidationToolbar(true, IDM_FILTRE_CLOUDS);
				treeFiltreEffect->SetTreeControl(filtreEffect);
				delete(filtreEffectOld);
				filtreEffectOld = filtreEffect;
				break;
			case IDM_FILTRE_ERODE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterErode", 1));
				break;
			case IDM_FILTRE_DILATE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterDilate", 1));
				break;
			case IDM_FILTRE_SHARPEN:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterSharpen", 1));
				break;
			case IDM_FILTRE_SHARPENSTRONG:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterSharpenStrong", 1));
				break;
			case IDM_FILTRENOISE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterNoise", 1));
				break;
			case IDM_FILTRE_MOSAIQUE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterMosaic", 1));
				break;
			case IDM_FILTRE_EMBOSS:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterEmboss", 1));
				break;
			case IDM_GREY_LEVEL:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterGrey", 1));
				break;
			case IDM_IMAGE_SEPIA:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterSepia", 1));
				break;
			case IDM_BLACKANDWHITE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterBlack", 1));
				break;
			case IDM_FILTRE_EDGE:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterEdge", 1));
				break;
			case IDM_NEGATIF:
				bitmapViewer->SetBitmapEffect(numItem, nullptr);
				historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterNegatif", 1));
				break;
			}
		}
	}
}

void CPanelInfosWnd::OnFiltreOk(const int &numFiltre)
{
	CBitmapWndViewer * bitmapViewer = (CBitmapWndViewer *)this->FindWindowById(BITMAPWINDOWVIEWERID);
	if (bitmapViewer != nullptr)
	{
		switch (numFiltre)
		{
		case IDM_AJUSTEMENT_SOLARISATION:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterSolarize",1));
			break;
		case IDM_ROTATE_FREE:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterRotate",1));
			break;
		case IDM_IMAGE_LIGHTCONTRAST:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterLight", 1));
			break;
		case ID_AJUSTEMENT_PHOTOFILTRE:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterPhoto", 1));
			break;
		case ID_AJUSTEMENT_POSTERISATION:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterPosterisation", 1));
			break;
		case IDM_COLOR_BALANCE:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterColor", 1));
			break;
		case IDM_FILTRE_SWIRL:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterSwirl", 1));
			break;
		case IDM_FILTRE_CLOUDS:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterClouds", 1));
			break;

		case IDM_REDEYE:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterRedEye",1));
			break;
                
        case IDM_CROP:
            bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
            historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLCROP",1));
            break;

		case IDM_FILTRE_MOTIONBLUR:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterMotion", 1));
			break;

		case IDM_FILTRELENSFLARE:
			bitmapViewer->SetBitmapEffect(numFiltre, effectParameter);
			historyEffectOld->AddModification(bitmapViewer->GetBitmap(), CLibResource::LoadStringFromResource(L"LBLfilterLensFlare", 1));
			break;
		}
	}
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
		if (thumbnailEffect->GetFilename() != filename)
			thumbnailEffect->SetFile(filename, bitmap);
	}
}

void CPanelInfosWnd::HistoryUpdate()
{
	if (historyEffectOld == nullptr || historyEffectOld->GetFilename() != filename)
	{
		wxString historyLibelle = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1);
		CShowBitmap * bitmapViewer = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERID);
		CInfoEffect * historyEffect = new CInfoEffect(bitmapViewer->GetBitmapViewer(), treeHistoryEffect->GetTheme(), treeHistoryEffect, modificationManager);
		historyEffect->Init(bitmap, filename, historyLibelle);
		treeHistoryEffect->SetTreeControl(historyEffect);
		delete(historyEffectOld);
		historyEffectOld = historyEffect;
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

void CPanelInfosWnd::ShowFiltre(const wxString &title)
{
	HideAllWindow();
    infosToolbar->SetEffectParameterPush();
	filtreEffectScroll->Show(true);
	treeFiltreEffect->Show(true);
	infosToolbar->SetEffectParameterActif(title);
	Resize();
}

void CPanelInfosWnd::ShowHistory()
{
	HideAllWindow();
	windowVisible = HISTORY_WINDOW;
	LoadInfo();
	historyEffectScroll->Show(true);
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
        thumbnailEffectScroll->Show(true);
        if (!thumbnailEffect->IsShown())
            thumbnailEffect->Show(true);
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
        thumbnailEffectScroll->Refresh();
    
#endif
}

void CPanelInfosWnd::ShowInfos()
{
	HideAllWindow();
	windowVisible = INFOS_WINDOW;
	LoadInfo();
	InfosFileScroll->Show(true);
	treeInfos->Show(true);
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
	CInfosFile * infosFileWnd = new CInfosFile(treeInfos->GetTheme(), treeInfos);
    StartLoadingPicture(InfosFileScroll);
    CThreadLoadInfos * threadInfos = new CThreadLoadInfos();
    threadInfos->infosFileWnd = infosFileWnd;
    threadInfos->panelInfos = this;
    threadInfos->filename = filename;
    threadInfos->threadLoadInfos = new thread(GenerateTreeInfos, threadInfos);
}

void CPanelInfosWnd::HideAllWindow()
{
	infosToolbar->SetEffectParameterInactif();
	InfosFileScroll->Show(false);
#ifdef EFFECT_VIDEO
	if (openGLVideoMode)
		effectVideoWndScroll->Show(false);
#endif
	historyEffectScroll->Show(false);
	thumbnailEffectScroll->Show(false);
	if(webBrowser != nullptr)
		webBrowser->Show(false);
	filtreEffectScroll->Show(false);
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
	else if (InfosFileScroll->IsShown())
		InfosFileScroll->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
#ifdef EFFECT_VIDEO
	else if (effectVideoWndScroll->IsShown() && openGLVideoMode)
		effectVideoWndScroll->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
#endif
	else if (thumbnailEffectScroll->IsShown())
		thumbnailEffectScroll->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (historyEffectScroll->IsShown())
		historyEffectScroll->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (webBrowser->IsShown())
		webBrowser->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
	else if (filtreEffectScroll->IsShown())
		filtreEffectScroll->SetSize(rcAffichageBitmap.x, infosToolbar->GetHeight(), rcAffichageBitmap.width, rcAffichageBitmap.height);
}
