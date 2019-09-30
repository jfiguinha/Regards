#pragma once
#include <ScrollbarWnd.h>
#include "ValidationToolbar.h"
#include "ThumbnailSelection.h"

using namespace Regards::Viewer;

class CSelectFileDlg : public wxDialog
{
public:
	CSelectFileDlg(wxWindow *parent, wxWindowID id,
		const wxString &filename,
		const wxString &title,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxSize(500, 400),
		const long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	~CSelectFileDlg();
	vector<int> GetSelectItem();

protected:
	//
	//  Event handlers
	//
	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);

	CThumbnailSelection * thumbnailFileSelection;

private:
};