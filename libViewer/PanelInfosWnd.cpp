#include <header.h>
#include "PanelInfosWnd.h"
#include "ToolbarInfos.h"
#include <BitmapWndViewer.h>
#include <BitmapWnd3d.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <window_id.h>
#include "MainWindow.h"
#include <FileUtility.h>
#include <LibResource.h>
#include "wx/stdpaths.h"
#include "FiltreEffectWnd.h"
#include "InfoEffectWnd.h"
#include "ThumbnailViewerEffectWnd.h"
#include <FilterData.h>
#include "ViewerParamInit.h"
#include <ShowElement.h>
#include <MainParamInit.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include "PicturePanel.h"
#include <effect_id.h>
#include "ParamInit.h"
using namespace Regards::Picture;
using namespace Regards::Internet;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Control;
/**
 * \brief 
 */
#define WM_UPDATEINFOS 1
wxDEFINE_EVENT(EVENT_ENDINFOSUPDATE, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UPDATETHUMBNAILTHREAD, wxCommandEvent);
wxDEFINE_EVENT(EVENT_UPDATEINFOSTHREAD, wxCommandEvent);

CPanelInfosWnd::CPanelInfosWnd(wxWindow* parent, wxWindowID id)
	: CTabWindow("CPanelInfosWnd", parent, id), id_(id)
{
	infosFileWnd = nullptr;
	firstTime = true;
	historyEffectWnd = nullptr;
	thumbnailEffectWnd = nullptr;
	filtreEffectWnd = nullptr;
	criteriaTreeWnd = nullptr;

	webBrowser = nullptr;
	infosToolbar = nullptr;
	picturePanel = nullptr;
	//Effect Parameter
	modificationManager = nullptr;

	isThumbnail = false;
	isVideo = false;
	filename = L"";
	//bitmap = new CRegardsBitmap();
	width = 0;
	height = 0;
	url = "http://www.google.fr";

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		windowVisible = config->GetVisibleWindowPanelInfos();
	else
		windowVisible = WM_INFOS;

	if (windowVisible == WM_HISTOGRAM)
		windowVisible = WM_INFOS;

	//CRegardsConfigParam * config = CParamInit::getInstance();
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();


	wxString folder = CFileUtility::GetDocumentFolderPath();

	modificationManager = new CModificationManager(folder);

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeTree theme;
		viewerTheme->GetTreeTheme(&theme);

		infosFileWnd = new CInfosFileWnd(this, wxID_ANY, themeScroll, theme);

		infosFileWnd->Show(false);

		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(infosFileWnd);
		tabInfosFile->SetId(WM_INFOS);
		listWindow.push_back(tabInfosFile);
	}

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeTheme(&themeTree);

		filtreEffectWnd = new CFiltreEffectScrollWnd(this, wxID_ANY, themeScroll, themeTree, BITMAPWINDOWVIEWERID);
		filtreEffectWnd->Show(false);

		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(filtreEffectWnd);
		tabInfosFile->SetId(WM_EFFECTPARAMETER);
		listWindow.push_back(tabInfosFile);

		auto tabInfosFileVideo = new CTabWindowData();
		tabInfosFileVideo->SetWindow(filtreEffectWnd);
		tabInfosFileVideo->SetId(WM_VIDEOEFFECT);
		listWindow.push_back(tabInfosFileVideo);

		auto tabInfosFileAudioVideo = new CTabWindowData();
		tabInfosFileAudioVideo->SetWindow(filtreEffectWnd);
		tabInfosFileAudioVideo->SetId(WM_AUDIOVIDEO);
		listWindow.push_back(tabInfosFileAudioVideo);
	}

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeTheme(&themeTree);
		historyEffectWnd = new CInfoEffectWnd(this, wxID_ANY, themeScroll, themeTree, BITMAPWINDOWVIEWERID);
		historyEffectWnd->Show(false);

		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(historyEffectWnd);
		tabInfosFile->SetId(WM_HISTORY);
		listWindow.push_back(tabInfosFile);
	}

	if (viewerTheme != nullptr)
		viewerTheme->GetBitmapWindowTheme(&themeBitmap);

	if (viewerTheme != nullptr)
	{
		bool checkValidity = false;

		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeThumbnail themeThumbnail;
		viewerTheme->GetThumbnailTheme(&themeThumbnail);

		CMainParam* main_param = CMainParamInit::getInstance();
		if (main_param != nullptr)
			checkValidity = main_param->GetCheckThumbnailValidity();

		thumbnailEffectWnd = new CThumbnailViewerEffectWnd(this, wxID_ANY, themeScroll, themeThumbnail, PANELINFOSWNDID,
		                                                   checkValidity);

		thumbnailEffectWnd->Show(false);

		auto tabInfosFileEffect = new CTabWindowData();
		tabInfosFileEffect->SetWindow(thumbnailEffectWnd);
		tabInfosFileEffect->SetId(WM_EFFECT);
		listWindow.push_back(tabInfosFileEffect);
	}

	if (webBrowser == nullptr)
	{
		webBrowser = wxWebView::New(this, wxID_ANY);
		webBrowser->Show(false);

		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(webBrowser);
		tabInfosFile->SetId(WM_MAPS);
		listWindow.push_back(tabInfosFile);
	}

	if (picturePanel == nullptr)
	{
		CThemeThumbnail themeThumbnail;
		viewerTheme->GetThumbnailTheme(&themeThumbnail);

		picturePanel = new CPicturePanel(this, wxID_ANY, themeThumbnail);
		picturePanel->Show(false);

		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(picturePanel);
		tabInfosFile->SetId(WM_HISTOGRAM);
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

		CThemeSplitter themeSplitter;
		viewerTheme->GetSplitterTheme(&themeSplitter);
		criteriaTreeWnd = new CCriteriaWindow(this, wxID_ANY, themeSplitter);

		auto tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(criteriaTreeWnd);
		tabInfosFile->SetId(WM_CRITERIA);
		listWindow.push_back(tabInfosFile);
	}

	toolbarWindow = infosToolbar;

	Connect(wxEVENT_APPLYEFFECT, wxCommandEventHandler(CPanelInfosWnd::ApplyEffect));
	Connect(wxEVENT_SHOWFILTRE, wxCommandEventHandler(CPanelInfosWnd::ShowFiltreEvent));
}

CFiltreEffect* CPanelInfosWnd::GetFilterWindow(int& numFiltre)
{
	if (filtreEffectWnd != nullptr)
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
	delete(picturePanel);
	delete(modificationManager);
}

void CPanelInfosWnd::SetAnimationFile(const wxString& filename)
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

void CPanelInfosWnd::SetVideoFile(const wxString& filename)
{
	if (this->filename != filename)
	{
		infosToolbar->SetEffectParameterInactif();
		this->filename = filename;

		wxString urlServer;
		CRegardsConfigParam* param = CParamInit::getInstance();
		if (param != nullptr)
		{
			urlServer = param->GetUrlServer();
		}

		wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
		auto fileGeolocalisation = new CFileGeolocation(urlServer);
		fileGeolocalisation->SetFile(filename, notGeo);

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
					infosToolbar->SetInfosActif();

				infosToolbar->SetMapInactif();
			}
			else
			{
				infosToolbar->SetMapActif();
			}
		}

		delete fileGeolocalisation;
		LoadInfo();
		this->isVideo = true;
	}
}

void CPanelInfosWnd::SetBitmapFile(const wxString& filename, const bool& isThumbnail)
{
	if (!isThumbnail)
		infosToolbar->SetPictureToolbar();
	else
		infosToolbar->SetPictureThumbnailToolbar();


	if (this->filename != filename)
	{
		wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
		wxString urlServer;
		CRegardsConfigParam* param = CParamInit::getInstance();
		if (param != nullptr)
		{
			urlServer = param->GetUrlServer();
		}
		auto fileGeolocalisation = new CFileGeolocation(urlServer);

		infosToolbar->SetEffectParameterInactif();
		this->filename = filename;
		fileGeolocalisation->SetFile(filename, notGeo);

		if (!fileGeolocalisation->HasGps())
			infosToolbar->SetMapInactif();
		else
			infosToolbar->SetMapActif();

		if (windowVisible == WM_INFOS)
			infosToolbar->SetInfosActif();

		if (!fileGeolocalisation->HasGps())
		{
			infosToolbar->SetMapInactif();

			if (webBrowser->IsShown())
				if (!fileGeolocalisation->HasGps())
					infosToolbar->SetInfosActif();
		}

		delete fileGeolocalisation;

		this->isVideo = false;


		LoadInfo();
	}
}

void CPanelInfosWnd::ShowFiltreEvent(wxCommandEvent& event)
{
	int numItem = event.GetInt();
	ShowFiltre(CFiltreData::GetFilterLabel(numItem));
}


void CPanelInfosWnd::ApplyEffect(wxCommandEvent& event)
{
	int numItem = event.GetInt();
	//Test si l'history fonctionne ou pas 
	HistoryUpdate();
	filtreEffectWnd->ApplyEffect(numItem, historyEffectWnd, filename, false, PANELINFOSWNDID, PREVIEWVIEWERID);
}

void CPanelInfosWnd::OnFiltreOk(const int& numFiltre)
{
	filtreEffectWnd->OnFiltreOk(numFiltre, historyEffectWnd);
	ClickShowButton(WM_EFFECT);
}

void CPanelInfosWnd::OnFiltreCancel()
{
	CBitmapWndViewer* bitmapViewer = nullptr;
	auto bitmapWindow = static_cast<CBitmapWnd3D*>(wxWindow::FindWindowById(BITMAPWINDOWVIEWERID));
	if (bitmapWindow != nullptr)
	{
		bitmapViewer = (CBitmapWndViewer*)bitmapWindow->GetWndPt();
	}

	if (bitmapViewer != nullptr)
	{
		bitmapViewer->OnFiltreCancel();
		bitmapViewer->SetBitmapPreviewEffect(0);
	}


	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_REFRESHPICTURE);
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

void CPanelInfosWnd::ShowFiltre(const wxString& title)
{
	HideAllWindow();
	infosToolbar->SetEffectParameterPush();
	filtreEffectWnd->Show(true);
	if (windowVisible == WM_EFFECT)
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
	if (!isVideo)
	{
		wxString historyLibelle = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1);
		auto bitmapViewer = static_cast<CShowElement*>(this->FindWindowById(SHOWBITMAPVIEWERID));
		if (bitmapViewer != nullptr)
		{
			CRegardsBitmap* bitmap = bitmapViewer->GetBitmap(true);
			historyEffectWnd->HistoryUpdate(bitmap, filename, historyLibelle, modificationManager);
			delete bitmap;
		}
	}
}

void CPanelInfosWnd::LoadInfo()
{
	//if (this->IsShown())
	//{
	switch (windowVisible)
	{
	case WM_INFOS:
		InfosUpdate();
		infosToolbar->SetInfosPush();
		windowVisible = WM_INFOS;
		break;
	case WM_CRITERIA:
		criteriaTreeWnd->SetFile(filename);
		infosToolbar->SetCriteriaPush();
		windowVisible = WM_CRITERIA;
		break;
	case WM_HISTORY:
		HistoryUpdate();
		infosToolbar->SetHistoryPush();
		windowVisible = WM_INFOS;
		break;
	case WM_EFFECT:
		if (!thumbnailEffectWnd->IsShown())
			thumbnailEffectWnd->Show(true);
		EffectUpdate();
		infosToolbar->SetEffectPush();
		thumbnailEffectWnd->Refresh();
		windowVisible = WM_INFOS;
		break;
	case WM_AUDIOVIDEO:
		if (!thumbnailEffectWnd->IsShown())
			thumbnailEffectWnd->Show(true);
		filtreEffectWnd->ApplyEffect(IDM_FILTRE_AUDIOVIDEO, historyEffectWnd, filename, isVideo, PANELINFOSWNDID,
		                             PREVIEWVIEWERID);
		AudioVideoUpdate();
		infosToolbar->SetAudioVideoPush();
		thumbnailEffectWnd->Refresh();
		windowVisible = WM_INFOS;
		break;
	case WM_VIDEOEFFECT:
		if (!thumbnailEffectWnd->IsShown())
			thumbnailEffectWnd->Show(true);
		filtreEffectWnd->ApplyEffect(IDM_FILTRE_VIDEO, historyEffectWnd, filename, isVideo, PANELINFOSWNDID,
		                             PREVIEWVIEWERID);
		VideoEffectUpdate();
		infosToolbar->SetVideoEffectPush();
		thumbnailEffectWnd->Refresh();
		windowVisible = WM_INFOS;
		break;

	case WM_MAPS:
		{
			wxString newUrl = MapsUpdate();
			if (url != newUrl)
				DisplayURL(newUrl);
			url = newUrl;
			infosToolbar->SetMapPush();
			windowVisible = WM_INFOS;
		}
		break;

	case WM_HISTOGRAM:
		HistogramUpdate();
		infosToolbar->SetHistogramPush();
		windowVisible = WM_HISTOGRAM;
		break;
	default: ;
	}
	//}
	this->ForceRefresh();
	//this->Refresh();
	this->Update();

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetVisibleWindowPanelInfos(windowVisible);
}

void CPanelInfosWnd::HistogramUpdate()
{
	CLibPicture picture;
	CImageLoadingFormat* pictureLoad = picture.LoadPicture(filename);
	CRegardsBitmap* bitmap = pictureLoad->GetRegardsBitmap();
	picturePanel->SetPictureToDisplay(bitmap);
	delete pictureLoad;

	picturePanel->Show(true);
	Resize();
}

wxString CPanelInfosWnd::MapsUpdate()
{
	wxString urlServer;
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		urlServer = param->GetUrlServer();
	}

	wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
	auto fileGeolocalisation = new CFileGeolocation(urlServer);
	fileGeolocalisation->SetFile(filename, notGeo);
	wxString url = L"http://www.openstreetmap.org/?mlat="; // NOLINT(clang-diagnostic-shadow)
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"&mlon=");
	url.append(fileGeolocalisation->GetLongitude());
	url.append(L"#map=15/");
	url.append(fileGeolocalisation->GetLatitude());
	url.append(L"/");
	url.append(fileGeolocalisation->GetLongitude());


	delete fileGeolocalisation;

	return url;
}

void CPanelInfosWnd::DisplayURL(const wxString& url)
{
	webBrowser->LoadURL(url);
	infosToolbar->SetMapActif();
	Resize();
}

void CPanelInfosWnd::InfosUpdate()
{
	if (infosFileWnd != nullptr)
		infosFileWnd->InfosUpdate(filename);
}
