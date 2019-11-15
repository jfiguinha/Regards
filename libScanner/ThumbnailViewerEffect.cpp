#include <header.h>
#include "ThumbnailViewerEffect.h"
#include "PanelInfosWnd.h"
#include <window_id.h>
using namespace Regards::Scanner;

CThumbnailViewerEffect::CThumbnailViewerEffect(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailEffect(parent, id, themeThumbnail, testValidity)
{

}


CThumbnailViewerEffect::~CThumbnailViewerEffect(void)
{

}


void CThumbnailViewerEffect::OnPictureClick(CThumbnailData * data)
{
	CPanelInfosWnd * panelInfos = (CPanelInfosWnd *)this->FindWindowById(PANELINFOSWNDSCANNERID);
	int numItem = data->GetNumPhotoId();
	if (panelInfos != nullptr)
	{
		panelInfos->ApplyEffect(numItem);
	}
}