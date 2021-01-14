#pragma once
#include <BitmapInterface.h>
class CImageLoadingFormat;

namespace Regards
{
	namespace Control
	{
		class CShowBitmap;
	}
}
using namespace Regards::Control;

class CPreviewDlg : public wxDialog
{
public:

	CPreviewDlg(wxWindow* parent);
	~CPreviewDlg();
	bool SetBitmap(CImageLoadingFormat * bitmap);
	wxButton* btnCancel;
	wxButton* btnOK;
	wxStaticBitmap * bitmap;
	wxPanel * panel;
protected:
	
	void OnSize(wxSizeEvent& event);
	CShowBitmap * showBitmapWindow;

	void OnbtnOKClick(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);

};

