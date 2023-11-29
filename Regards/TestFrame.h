#pragma once
#include <BitmapWnd3d.h>
#include <VideoControl_soft.h>
#include "ScrollbarWnd.h"
using namespace Regards::Window;
using namespace Regards::Video;

class CVideoControlSoft;

namespace Regards
{
	namespace Window
	{
		class CBitmapWnd3D;
	}

}
class CTestFrame : public wxFrame
{
public:
	CTestFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~CTestFrame() override;
	void PlayMovie(const wxString& openfile);
	void StopMovie();
	void OnSize(wxSizeEvent& event);
	void OnStop(wxTimerEvent& event);
private:
	CVideoControlSoft* videoWindow;
	CBitmapWnd3D* bitmapWindowRender;
	wxString filename = "";
	wxTimer* stopMovie;
};

