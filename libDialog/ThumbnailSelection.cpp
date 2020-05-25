#include <header.h>
#include "ThumbnailSelection.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "ScannerParam.h"
#include "ScannerParamInit.h"
#include <ScrollbarWnd.h>
using namespace Regards::Window;
using namespace Regards::Scanner;

CThumbnailSelection::CThumbnailSelection(wxWindow* parent, wxWindowID id, wxString filename)
	: CWindowMain("ThumbnailSelection", parent, id)
{
	bool checkValidity = false;
	CMainParam * config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		

		//widthSize = theme.GetRectangleSize();

		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		thumbnailFileSelection = new CThumbnailFileSelection(this, THUMBNAILFILESELECTION, themeThumbnail, checkValidity);
		thumbscrollbar = new CScrollbarWnd(this, thumbnailFileSelection, wxID_ANY);
	}

	if (viewerTheme != nullptr)
	{
		CThemeToolbar theme;
		viewerTheme->GetFiltreToolbarTheme(&theme);
		validationToolbar = new CValidationToolbar(this, wxID_ANY, theme, false);
	}

	if (thumbscrollbar != nullptr)
		thumbscrollbar->Show(true);

	thumbscrollbar->ShowVerticalScroll();
	thumbnailFileSelection->SetNoVScroll(false);
	thumbnailFileSelection->SetCheck(true);

	thumbnailFileSelection->Init(filename);
	validationToolbar->Show(true);

	Connect(wxEVT_SIZE, wxSizeEventHandler(CThumbnailSelection::OnSize));
}


CThumbnailSelection::~CThumbnailSelection()
{
	delete(validationToolbar);
	delete(thumbnailFileSelection);
	delete(thumbscrollbar);
}

vector<int> CThumbnailSelection::GetSelectItem()
{
	return thumbnailFileSelection->GetSelectItem();
}

void CThumbnailSelection::UpdateScreenRatio()
{
	this->Resize();
}

void CThumbnailSelection::RedrawBarPos()
{
	int toolbarHeightSize = validationToolbar->GetHeight();
	wxRect rcAffichageBitmap;
	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = 0;
	rcAffichageBitmap.width = width;
	rcAffichageBitmap.height =height - toolbarHeightSize;

	thumbscrollbar->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, rcAffichageBitmap.height);

	if (validationToolbar->IsShown())
		validationToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarHeightSize);
}

void CThumbnailSelection::OnSize(wxSizeEvent& event)
{
	width = event.GetSize().GetWidth();
	height = event.GetSize().GetHeight();
	RedrawBarPos();
}