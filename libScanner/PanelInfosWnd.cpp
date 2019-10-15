#include <header.h>
#include "PanelInfosWnd.h"
#include "ThumbnailEffect.h"
#include "ToolbarInfos.h"
#include <BitmapWndViewer.h>
#include "ScannerTheme.h"
#include "ScannerThemeInit.h"
#include <window_id.h>
#include <config_id.h>
#include <FileUtility.h>
#include <LibResource.h>
#include <ImageLoadingFormat.h>
#include <FilterData.h>
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
	//bitmap = new CRegardsBitmap();
	width = 0;
	height = 0;
	windowVisible = WM_INFOS;
 
	//CRegardsConfigParam * config = CParamInit::getInstance();
	CScannerTheme * viewerTheme = new CScannerTheme();

    
    wxString folder = CFileUtility::GetDocumentFolderPath();
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

		infosFileWnd->Show(false);
        
        CTabWindowData * tabInfosFile = new CTabWindowData();
        tabInfosFile->window = infosFileWnd;
        tabInfosFile->windowMain = infosFileWnd;
        tabInfosFile->windowName = WM_INFOS;
        listWindow.push_back(tabInfosFile);
	}   
	
	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetInfosToolbarTheme(&theme);
		infosToolbar = new CToolbarInfos(this, wxID_ANY, theme, this, false);
	}
    
    toolbarWindow = infosToolbar;
    
    
}

wxString CPanelInfosWnd::GetFilename()
{
	return filename;
}

CPanelInfosWnd::~CPanelInfosWnd()
{
	delete(infosFileWnd);  
	delete(infosToolbar);
}

void CPanelInfosWnd::SetFile(const wxString &filename)
{
	if (this->filename != filename)
	{
		this->filename = filename;

		infosToolbar->SetInfosActif();

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
		}
	}
}

void CPanelInfosWnd::InfosUpdate()
{
    if(infosFileWnd != nullptr)
        infosFileWnd->InfosUpdate(filename);
}