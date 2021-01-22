#include <header.h>
#include "PreviewDlg.h"
#include <libPicture.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include "ShowPreview.h"
#include <theme.h>
#include <wx/xrc/xmlres.h>
using namespace Regards::Control;
using namespace Regards::Viewer;
using namespace Regards::Picture;
#define PANE_INFOS 2
#define PANE_PREVIEW 3

#ifdef NOTENCODE_FRAME
CPreviewDlg::CPreviewDlg(wxWindow* parent, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoEffectParameter * videoEffectParameter)
#else
CPreviewDlg::CPreviewDlg(wxWindow* parent, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoOptionCompress * videoOptionCompress)
#endif
{
	wxXmlResource::Get()->LoadObject(this, parent, _T("PreviewDlg"), _T("wxDialog"));
	panel = (wxPanel*)FindWindow(XRCID("IDPANEL"));
	

	CThemeBitmapWindow themeBitmap;
	showBitmapWindow = nullptr;
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
		viewerTheme->GetBitmapWindowTheme(&themeBitmap);

	showBitmapWindow = new CShowPreview(panel, SHOWBITMAPVIEWERDLGID, BITMAPWINDOWVIEWERIDDLG, MAINVIEWERWINDOWID, viewerTheme, videoFilename, openCLEngine, videoOptionCompress);
	showBitmapWindow->Show(true);

	//panel->AddChild(showBitmapWindow);

	panel->Bind(wxEVT_SIZE, &CPreviewDlg::OnSize, this);

	Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(CPreviewDlg::OnClose));


	panel->SetSize(640, 480);
	this->SetSize(640, 480);
	showBitmapWindow->SetFocus();
}

void CPreviewDlg::OnClose(wxCloseEvent& event)
{
	wxCommandEvent evt(wxEVENT_CLOSEPREVIEW);
	this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
}

void CPreviewDlg::OnSize(wxSizeEvent& event)
{
	showBitmapWindow->SetSize(panel->GetPosition().x, panel->GetPosition().y, panel->GetSize().x, panel->GetSize().y);
}
/*
void CPreviewDlg::OnbtnOKClick(wxCommandEvent& event)
{
	this->Close();
}

void CPreviewDlg::OnbtnCancelClick(wxCommandEvent& event)
{
	this->Close();
}
*/
CPreviewDlg::~CPreviewDlg()
{
	delete(showBitmapWindow);

}
#ifdef NOTENCODE_FRAME
void CPreviewDlg::UpdatePreview()
{
	showBitmapWindow->UpdateBitmap();
}
#else
void CPreviewDlg::UpdatePreview(CVideoOptionCompress * videoOptionCompress, const wxString &extension)
{
	showBitmapWindow->UpdateBitmap(videoOptionCompress, extension);
}
#endif