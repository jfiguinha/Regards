#include <header.h>
#include "SelectPage.h"
#include "ScannerParam.h"

CSelectFileDlg::CSelectFileDlg(wxWindow* parent, const wxWindowID id,
                               const wxString& filename,
                               const wxString& title, const wxPoint& pos,
                               const wxSize& size, const long style) :
	wxDialog(parent, id, title, pos, size, style), filename_(filename), title_(title), pos_(pos), size_(size), style_(style)
{
	auto sizer = new wxBoxSizer(wxVERTICAL);
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
