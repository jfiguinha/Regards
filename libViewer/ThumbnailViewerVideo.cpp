#include <header.h>
#include "ThumbnailViewerVideo.h"
#include "MainWindow.h"
#include <window_id.h>
using namespace Regards::Viewer;

CThumbnailViewerVideo::CThumbnailViewerVideo(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVideo(parent, id, statusBarInterface, themeThumbnail, testValidity)
{
	filename = L"";
}


CThumbnailViewerVideo::~CThumbnailViewerVideo(void)
{

}


void CThumbnailViewerVideo::OnPictureClick(CThumbnailData * data)
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr && data != nullptr)
	{
#ifdef FFMPEG
		int timePosition = data->GetTimePosition();
#else
		int timePosition = data->GetTimePosition() * 1000;
#endif
		//mainWindow->PictureVideoClick(timePosition);
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_PICTUREVIDEOCLICK);
		evt.SetExtraLong(timePosition);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}