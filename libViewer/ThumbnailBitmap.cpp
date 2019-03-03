#include <header.h>
#include "ThumbnailBitmap.h"
#include "MainWindow.h"
#include <window_id.h>
using namespace Regards::Viewer;

CThumbnailBitmap::CThumbnailBitmap(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailHorizontal(parent, id, statusBarInterface, themeThumbnail, testValidity)
{
}


CThumbnailBitmap::~CThumbnailBitmap(void)
{

}


void CThumbnailBitmap::OnPictureClick(CThumbnailData * data)
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	int numItem = data->GetNumPhotoId();
	if (mainWindow != nullptr)
	{
		//mainWindow->PictureClick(numItem);
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_PICTURECLICK);
		evt.SetExtraLong(numItem);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

}