#pragma once
#include <BitmapInterface.h>
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

class CImageLoadingFormat;
class CVideoEffectParameter;

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

	CPreviewDlg(wxWindow* parent, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoEffectParameter * videoEffectParameter);
	~CPreviewDlg();
	void UpdatePreview();
	//wxButton* btnCancel;
	//wxButton* btnOK;
	wxStaticBitmap * bitmap;
	wxPanel * panel;
protected:
	
	void OnSize(wxSizeEvent& event);
	CShowPreview * showBitmapWindow;
	
	//void OnbtnOKClick(wxCommandEvent& event);
	//void OnbtnCancelClick(wxCommandEvent& event);

};

