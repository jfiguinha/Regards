#pragma once
#include "ThumbnailSelection.h"

using namespace Regards::Scanner;

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

	CThumbnailSelection * thumbnailFileSelection;


};