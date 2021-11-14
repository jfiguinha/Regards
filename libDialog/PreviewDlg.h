#pragma once
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

class CImageLoadingFormat;
class CVideoEffectParameter;
class CVideoOptionCompress;

namespace Regards
{
	namespace Control
	{
		class CShowPreview;
	}
}

using namespace Regards::Control;

class CPreviewDlg : public wxDialog
{
public:
	CPreviewDlg(wxWindow* parent, const wxString& videoFilename,
	            CVideoOptionCompress* videoOptionCompress);
	~CPreviewDlg() override;

	void UpdatePreview(CVideoOptionCompress* videoOptionCompress, const wxString& extension);
	wxPanel* panel;
	wxStaticBitmap* bitmapPreview;
protected:
	void OnClose(wxCloseEvent& event);
	void on_size(wxSizeEvent& event);
	CShowPreview* showBitmapWindow;

	//void OnbtnOKClick(wxCommandEvent& event);
	//void OnbtnCancelClick(wxCommandEvent& event);
};
