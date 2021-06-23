#include <header.h>
#include "SelectPage.h"
#include "ScannerParam.h"
#include "ScannerParamInit.h"

CSelectFileDlg::CSelectFileDlg(wxWindow *parent, wxWindowID id,
	const wxString &filename,
	const wxString &title, const wxPoint &pos,
	const wxSize &size, const long style) :
	wxDialog(parent, id, title, pos, size, style)
{
	bool checkValidity = false;
	CMainParam * config = CMainParamInit::getInstance();
	if (config != nullptr)
		checkValidity = config->GetCheckThumbnailValidity();



	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(thumbnailFileSelection = new CThumbnailSelection(this, THUMBNAILSELECTION, filename), 1, wxEXPAND);
	SetSizer(sizer);

	Connect(wxID_OK, wxCommandEventHandler(CSelectFileDlg::OnOk));
	Connect(wxID_CANCEL, wxCommandEventHandler(CSelectFileDlg::OnCancel));
}

vector<int> CSelectFileDlg::GetSelectItem()
{
	return thumbnailFileSelection->GetSelectItem();
}


CSelectFileDlg::~CSelectFileDlg()
{
	delete(thumbnailFileSelection);
}

void CSelectFileDlg::OnOk(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void CSelectFileDlg::OnCancel(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}