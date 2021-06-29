#include <header.h>
#include "PanelInfosWnd.h"
#include "ToolbarInfos.h"
#include <BitmapWndViewer.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <window_id.h>
#include <FileUtility.h>
#include <LibResource.h>
#include <FilterData.h>
#include "OcrWnd.h"
#include <wx/stdpaths.h>
#include "FiltreEffectWnd.h"
#include <InfoEffectWnd.h>
#include "ThumbnailViewerEffectWnd.h"
#include <ShowBitmap.h>
#include "ScannerParam.h"
#include "ScannerParamInit.h"
using namespace Regards::Internet;
using namespace Regards::Window;
using namespace Regards::Scanner;
using namespace Regards::Control;
#define WM_UPDATEINFOS 1

CPanelInfosWnd::CPanelInfosWnd(wxWindow* parent, wxWindowID id)
	: CTabWindow("CPanelInfosWnd",parent, id)
{
	infosFileWnd = nullptr;
	infosToolbar = nullptr;
	webBrowser = nullptr;
	//bitmap = new CRegardsBitmap();
	width = 0;
	height = 0;
	windowVisible = WM_INFOS;
	//Effect Parameter
	modificationManager = nullptr;
	url = "http://www.google.fr";
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	wxString folder = CFileUtility::GetDocumentFolderPath();

	modificationManager = new CModificationManager(folder);

#ifdef __APPLE__
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    folder = stdp.GetDocumentsDir();
#endif

	if (viewerTheme != nullptr)
	{
        CThemeScrollBar themeScroll;
        viewerTheme->GetScrollTheme(&themeScroll);
        
        CThemeTree theme;
        viewerTheme->GetTreeTheme(&theme);
        
        infosFileWnd = new CInfosFileWnd(this, wxID_ANY, themeScroll, theme);
		ocrWnd = new COcrWnd(this, SCANNER_OCRPAGE);

		infosFileWnd->Show(true);
        
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->SetWindow(infosFileWnd);
		tabInfosFile->SetId(WM_INFOS);
        listWindow.push_back(tabInfosFile);

		CTabWindowData * tabOcr = new CTabWindowData();
		tabOcr->SetWindow(ocrWnd);
		tabOcr->SetId(WM_OCR);
		listWindow.push_back(tabOcr);

	}   

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeTheme(&themeTree);

		filtreEffectWnd = new CFiltreEffectScrollWnd(this, wxID_ANY, themeScroll, themeTree, BITMAPWINDOWVIEWERIDPDF);
		filtreEffectWnd->Show(false);

		CTabWindowData * tabInfosFile = new CTabWindowData();
		tabInfosFile->SetWindow(filtreEffectWnd);
		tabInfosFile->SetId(WM_EFFECTPARAMETER);
		listWindow.push_back(tabInfosFile);
	}

	if (viewerTheme != nullptr)
	{
		CThemeScrollBar themeScroll;
		viewerTheme->GetScrollTheme(&themeScroll);

		CThemeTree themeTree;
		viewerTheme->GetTreeTheme(&themeTree);
		historyEffectWnd = new CInfoEffectWnd(this, wxID_ANY, themeScroll, themeTree, BITMAPWINDOWVIEWERIDPDF);
		historyEffectWnd->Show(false);

		CTabWindowData * tabInfosFile = new CTabWindowData();
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

		CMainParam * config = CMainParamInit::getInstance();
		if (config != nullptr)
			checkValidity = config->GetCheckThumbnailValidity();

		thumbnailEffectWnd = new CThumbnailViewerEffectWnd(this, wxID_ANY, themeScroll, themeThumbnail, PANELINFOSWNDSCANNERID, checkValidity);

		thumbnailEffectWnd->Show(false);

		CTabWindowData * tabInfosFileEffect = new CTabWindowData();
		tabInfosFileEffect->SetWindow(thumbnailEffectWnd);
		tabInfosFileEffect->SetId(WM_EFFECT);
		listWindow.push_back(tabInfosFileEffect);

	}

	if (webBrowser == nullptr)
	{
		webBrowser = wxWebView::New(this, wxID_ANY);
		webBrowser->Show(false);

		/*
		CTabWindowData * tabInfosFile = new CTabWindowData();
		tabInfosFile->window = webBrowser;
		tabInfosFile->windowName = WM_HTMLEDITOR;
		listWindow.push_back(tabInfosFile);
		*/
	}
	
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetInfosToolbarTheme(&theme);
		infosToolbar = new CToolbarInfos(this, wxID_ANY, theme, this, false);
	}
    
    toolbarWindow = infosToolbar;
	Connect(wxEVENT_APPLYEFFECT, wxCommandEventHandler(CPanelInfosWnd::ApplyEffect));
	Connect(wxEVENT_SHOWFILTRE, wxCommandEventHandler(CPanelInfosWnd::ShowFiltreEvent));
}

void CPanelInfosWnd::HistoryUpdate()
{
	wxString historyLibelle = CLibResource::LoadStringFromResource(L"LBLHISTORY", 1);
	CShowBitmap * bitmapViewer = (CShowBitmap *)this->FindWindowById(SHOWBITMAPVIEWERIDPDF);
	if (bitmapViewer != nullptr)
	{
		CRegardsBitmap * bitmap = bitmapViewer->GetBitmap(true);
		if(bitmap != nullptr)
		{
			wxString filename = bitmap->GetFilename();
			historyEffectWnd->HistoryUpdate(bitmap, filename, historyLibelle, modificationManager);
			delete bitmap;
		}
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
	filtreEffectWnd->ApplyEffect(numItem, historyEffectWnd, _filename, false, PANELINFOSWNDSCANNERID, PDFWINDOWID);
}

void CPanelInfosWnd::OnFiltreOk(const int &numFiltre)
{
	filtreEffectWnd->OnFiltreOk(numFiltre, historyEffectWnd);
	ClickShowButton(WM_EFFECT);
	infosToolbar->SetEffectParameterInactif();
}

CFiltreEffect * CPanelInfosWnd::GetFilterWindow(int &numFiltre)
{
	if (filtreEffectWnd != nullptr)
	{
		numFiltre = filtreEffectWnd->GetNumFiltre();
		return filtreEffectWnd->GetFiltreEffect();
	}
	return nullptr;
}


void CPanelInfosWnd::OnFiltreCancel()
{
	CBitmapWndViewer* bitmapViewer = (CBitmapWndViewer*)this->FindWindowById(BITMAPWINDOWVIEWERIDPDF);

	if (bitmapViewer != nullptr)
	{
		bitmapViewer->OnFiltreCancel();
		bitmapViewer->SetBitmapPreviewEffect(0);
		bitmapViewer->ReinitPicture();
	}

	ClickShowButton(WM_EFFECT);
	infosToolbar->SetEffectParameterInactif();
}

void CPanelInfosWnd::EffectUpdate()
{
	if (thumbnailEffectWnd->GetFilename() != _filename)
	{
		thumbnailEffectWnd->SetFile(_filename);
	}
}

void CPanelInfosWnd::ShowFiltre(const wxString &title)
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

wxString CPanelInfosWnd::GetFilename()
{
	return _filename;
}

CPanelInfosWnd::~CPanelInfosWnd()
{
	delete(infosFileWnd);  
	delete(ocrWnd);
	delete(infosToolbar);
	delete(historyEffectWnd);
	delete(filtreEffectWnd);
	delete(thumbnailEffectWnd);
	delete(modificationManager);
	delete(webBrowser);
}

void CPanelInfosWnd::SetFile(wxString filename)
{
	if (_filename != filename)
	{
		_filename = wxString(filename);

		infosToolbar->SetInfosActif();

		ocrWnd->Init();

		infosToolbar->SetEffectParameterInactif();

		LoadInfo();
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

		case WM_OCR:
			//InfosUpdate();
			infosToolbar->SetOcrPush();
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
		case WM_HTMLEDITOR:
		{
			wxString resourcePath = CFileUtility::GetResourcesFolderPath();
			wxString newUrl = "file:///" + resourcePath + "/ckeditor4.html";
			if (url != newUrl)
				DisplayURL(newUrl);
			url = newUrl;
			infosToolbar->SetEditorPush();
		}
		break;
		default: ;
		}
	}
}

void CPanelInfosWnd::DisplayURL(const wxString &url)
{
	webBrowser->LoadURL(url);
	Resize();
}

void CPanelInfosWnd::InfosUpdate()
{
    if(infosFileWnd != nullptr)
        infosFileWnd->InfosUpdate(_filename);
}