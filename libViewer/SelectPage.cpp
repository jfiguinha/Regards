#include <header.h>
#include "SelectPage.h"
#include "ViewerTheme.h"
#include "ViewerThemeInit.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"

CSelectFileDlg::CSelectFileDlg(wxWindow *parent, wxWindowID id,
	const wxString &filename,
	const wxString &title, const wxPoint &pos,
	const wxSize &size, const long style) :
	wxDialog(parent, id, title, pos, size, style)
{
	bool checkValidity = false;
	CViewerParam * config = CViewerParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();

	CViewerTheme * viewerTheme = CViewerThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		CThemeThumbnail themeThumbnail;
		CThemeScrollBar theme;
		viewerTheme->GetScrollTheme(&theme);
		thumbscrollbar = new CScrollbarWnd(this, wxID_ANY);

		widthSize = theme.GetRectangleSize();

		viewerTheme->GetThumbnailTheme(&themeThumbnail);
		thumbnailFileSelection = new CThumbnailFileSelection(thumbscrollbar, THUMBNAILFILESELECTION, themeThumbnail, checkValidity);
		
		thumbscrollbar->SetCentralWindow(thumbnailFileSelection, theme);
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


	Connect(wxID_OK, wxCommandEventHandler(CSelectFileDlg::OnOk));
	Connect(wxID_CANCEL, wxCommandEventHandler(CSelectFileDlg::OnCancel));
	Connect(wxEVT_SIZE, wxSizeEventHandler(CSelectFileDlg::OnSize));
}

vector<int> CSelectFileDlg::GetSelectItem()
{
	return thumbnailFileSelection->GetSelectItem();
}

void CSelectFileDlg::OnSize(wxSizeEvent& event)
{
	int _width = event.GetSize().GetX();
	int _height = event.GetSize().GetY() - 60;
	int toolbarHeightSize = validationToolbar->GetHeight();
	wxRect rcAffichageBitmap;
	rcAffichageBitmap.x = 0;
	rcAffichageBitmap.y = 0;
	rcAffichageBitmap.width = _width - widthSize;
	rcAffichageBitmap.height = _height - toolbarHeightSize;

	thumbscrollbar->SetSize(rcAffichageBitmap.x, 0, rcAffichageBitmap.width, rcAffichageBitmap.height);

	if (validationToolbar->IsShown())
		validationToolbar->SetSize(rcAffichageBitmap.x, rcAffichageBitmap.height, rcAffichageBitmap.width, toolbarHeightSize);
}

CSelectFileDlg::~CSelectFileDlg()
{
	delete(validationToolbar);
	delete(thumbnailFileSelection);
	delete(thumbscrollbar);
}

void CSelectFileDlg::OnOk(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void CSelectFileDlg::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}