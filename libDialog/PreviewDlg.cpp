#include <header.h>
#include "PreviewDlg.h"
#include <libPicture.h>
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ImageLoadingFormat.h>
#include <ShowBitmap.h>
#include <theme.h>
#include <wx/xrc/xmlres.h>
using namespace Regards::Control;
using namespace Regards::Viewer;
using namespace Regards::Picture;
#define PANE_INFOS 2
#define PANE_PREVIEW 3

CPreviewDlg::CPreviewDlg(wxWindow* parent)
{
	wxXmlResource::Get()->LoadObject(this, parent, _T("PreviewDlg"), _T("wxDialog"));
	panel = (wxPanel*)FindWindow(XRCID("IDPANEL"));
	btnOK = (wxButton*)FindWindow(XRCID("ID_OK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	bitmap = (wxStaticBitmap *)FindWindow(XRCID("ID_BITMAPVIDEO"));
	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CPreviewDlg::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CPreviewDlg::OnbtnCancelClick);

	CThemeBitmapWindow themeBitmap;
	showBitmapWindow = nullptr;
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
		viewerTheme->GetBitmapWindowTheme(&themeBitmap);
	
	showBitmapWindow = new CShowBitmap(bitmap->GetParent(), SHOWBITMAPVIEWERDLGID, BITMAPWINDOWVIEWERIDDLG, MAINVIEWERWINDOWID, nullptr, viewerTheme);
	showBitmapWindow->SetSize(bitmap->GetPosition().x, bitmap->GetPosition().y, bitmap->GetSize().x, bitmap->GetSize().y);
	showBitmapWindow->Show(true);
	bitmap->Show(false);
	//panel->AddChild(showBitmapWindow);
	//bitmap->Connect(wxEVT_SIZE, wxSizeEventHandler(CPreviewDlg::OnSize));

	panel->Bind(wxEVT_SIZE, &CPreviewDlg::OnSize, this);
}

void CPreviewDlg::OnSize(wxSizeEvent& event)
{
	showBitmapWindow->SetSize(panel->GetPosition().x, panel->GetPosition().y, panel->GetSize().x, panel->GetSize().y);
}

void CPreviewDlg::OnbtnOKClick(wxCommandEvent& event)
{
	this->Close();
}

void CPreviewDlg::OnbtnCancelClick(wxCommandEvent& event)
{
	this->Close();
}

CPreviewDlg::~CPreviewDlg()
{
	delete(showBitmapWindow);

}

bool CPreviewDlg::SetBitmap(CImageLoadingFormat * bitmap)
{
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CPreviewDlg::SetBitmap");
	OutputDebugString(L"\n");
	OutputDebugString(bitmap->GetFilename());
	OutputDebugString(L"\n");
#endif

	if (bitmap != nullptr && bitmap->IsOk())
	{
		showBitmapWindow->SetBitmap(bitmap, false);
	}
	return 1;
}

