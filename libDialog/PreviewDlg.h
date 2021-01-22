#pragma once
#include <BitmapInterface.h>
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

#ifdef NOTENCODE_FRAME
	CPreviewDlg(wxWindow* parent, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoEffectParameter * videoEffectParameter);
#else
	CPreviewDlg(wxWindow* parent, const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoOptionCompress * videoOptionCompress);
#endif
	~CPreviewDlg();

	void UpdatePreview(CVideoOptionCompress * videoOptionCompress, const wxString &extension);
	wxPanel * panel;
protected:
	void OnClose(wxCloseEvent& event);
	void OnSize(wxSizeEvent& event);
	CShowPreview * showBitmapWindow;
	
	//void OnbtnOKClick(wxCommandEvent& event);
	//void OnbtnCancelClick(wxCommandEvent& event);

};

