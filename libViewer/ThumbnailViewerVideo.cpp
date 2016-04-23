#include "ThumbnailViewerVideo.h"
#include "MainWindow.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Viewer;

CThumbnailViewerVideo::CThumbnailViewerVideo(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail)
	: CThumbnailVideo(parent, id, statusBarInterface, themeThumbnail)
{

}


CThumbnailViewerVideo::~CThumbnailViewerVideo(void)
{

}


void CThumbnailViewerVideo::OnPictureClick(CThumbnailData * data)
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr && data != nullptr)
	{
		int timePosition = data->GetTimePosition() * 1000;
		mainWindow->PictureVideoClick(timePosition);
	}
}