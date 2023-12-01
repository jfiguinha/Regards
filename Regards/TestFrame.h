#pragma once
#include <BitmapWnd3d.h>
#include <VideoControl_soft.h>
#include <ThumbnailViewerVideo.h>
#include <ScrollbarWnd.h>
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::Viewer;
class CVideoControlSoft;

#define SHOW_VIDEO

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
    
#ifdef SHOW_VIDEO
	CVideoControlSoft* videoWindow;
	CBitmapWnd3D* bitmapWindowRender;
#endif
#ifdef SHOW_THUMBNAIL
    CScrollbarWnd* scrollVideoWindow;
    Regards::Viewer::CThumbnailViewerVideo * thumbnailVideo;
#endif
	wxString filename = "";
	wxTimer* stopMovie;
    int i = 0;
};

