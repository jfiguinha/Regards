#pragma once
#include <BitmapWnd3d.h>
#include <VideoControl_soft.h>
#include <ThumbnailViewerVideo.h>
#include <ScrollbarWnd.h>
#include <PanelInfosWnd.h>
#include <ThumbnailViewerPicture.h>
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Video;
using namespace Regards::Viewer;
class CVideoControlSoft;

#define SHOW_THUMBNAILVIDEO

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
    void OnOpenFile(wxCommandEvent& event);
private:
    
#ifdef SHOW_VIDEO
	CVideoControlSoft* videoWindow;
	CBitmapWnd3D* bitmapWindowRender;
#endif
#ifdef SHOW_THUMBNAILVIDEO
    CScrollbarWnd* scrollVideoWindow;
    Regards::Viewer::CThumbnailViewerVideo * thumbnailVideo;
#endif
#ifdef SHOW_THUMBNAIL
    CScrollbarWnd* scrollWindow;
    Regards::Viewer::CThumbnailViewerPicture * thumbnail;
#endif
#ifdef SHOW_INFOS
    Regards::Viewer::CPanelInfosWnd * panelInfosWindow;
#endif
	wxString filename = "";
    wxString lastFolder = "";
	wxTimer* stopMovie;
    bool firstTime = true;
    int i = 0;
};

