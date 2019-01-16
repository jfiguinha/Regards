#include "ThumbnailViewerEffect.h"
#include "PanelInfosWnd.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace Regards::Viewer;

CThumbnailViewerEffect::CThumbnailViewerEffect(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailEffect(parent, id, statusBarInterface, themeThumbnail, testValidity)
{

}


CThumbnailViewerEffect::~CThumbnailViewerEffect(void)
{

}


void CThumbnailViewerEffect::OnPictureClick(CThumbnailData * data)
{
	CPanelInfosWnd * panelInfos = (CPanelInfosWnd *)this->FindWindowById(PANELINFOSWNDID);
	int numItem = data->GetNumPhotoId();
	if (panelInfos != nullptr)
	{
		panelInfos->ApplyEffect(numItem);
	}
}