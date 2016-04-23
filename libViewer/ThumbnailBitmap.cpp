#include "ThumbnailBitmap.h"
#include "MainWindow.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Viewer;

CThumbnailBitmap::CThumbnailBitmap(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail)
	: CThumbnailHorizontal(parent, id, statusBarInterface, themeThumbnail)
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
		mainWindow->PictureClick(numItem);

}