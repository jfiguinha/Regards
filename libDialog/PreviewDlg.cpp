#include <header.h>
#include "PreviewDlg.h"
#include <libPicture.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include "ShowPreview.h"
#include <theme.h>
#include <wx/xrc/xmlres.h>
using namespace Regards::Control;
using namespace Regards::Viewer;
using namespace Regards::Picture;
#define PANE_INFOS 2
#define PANE_PREVIEW 3


CPreviewDlg::CPreviewDlg(wxWindow* parent, const wxString& videoFilename,
                         CVideoOptionCompress* videoOptionCompress)
{
	wxXmlResource::Get()->LoadObject(this, parent, _T("PreviewDlg"), _T("wxDialog"));
	panel = static_cast<wxPanel*>(FindWindow(XRCID("IDPANEL")));
	bitmapPreview = static_cast<wxStaticBitmap*>(FindWindow(XRCID("ID_BITMAPVIDEO")));

	CThemeBitmapWindow themeBitmap;
	showBitmapWindow = nullptr;
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
		viewerTheme->GetBitmapWindowTheme(&themeBitmap);

	showBitmapWindow = new CShowPreview(panel, SHOWBITMAPVIEWERDLGID, BITMAPWINDOWVIEWERIDDLG, MAINVIEWERWINDOWID,
	                                    viewerTheme, videoFilename, videoOptionCompress);
	showBitmapWindow->Show(true);
	showBitmapWindow->Raise();
	showBitmapWindow->SetSize(bitmapPreview->GetPosition().x, bitmapPreview->GetPosition().y,
	                          bitmapPreview->GetSize().x, bitmapPreview->GetSize().y);
	bitmapPreview->Show(false);

	panel->Bind(wxEVT_SIZE, &CPreviewDlg::on_size, this);

	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(CPreviewDlg::OnClose));
}

void CPreviewDlg::OnClose(wxCloseEvent& event)
{
	wxCommandEvent evt(wxEVENT_CLOSEPREVIEW);
	this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
}

void CPreviewDlg::on_size(wxSizeEvent& event)
{
	showBitmapWindow->SetSize(panel->GetPosition().x, panel->GetPosition().y, panel->GetSize().x, panel->GetSize().y);
}

CPreviewDlg::~CPreviewDlg()
{
	delete(showBitmapWindow);
}

void CPreviewDlg::UpdatePreview(CVideoOptionCompress* videoOptionCompress, const wxString& extension)
{
	showBitmapWindow->UpdateBitmap(videoOptionCompress, extension);
}
