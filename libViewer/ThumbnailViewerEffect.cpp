#include <header.h>
#include "ThumbnailViewerEffect.h"
#include "PanelInfosWnd.h"
#include <window_id.h>
using namespace Regards::Viewer;

CThumbnailViewerEffect::CThumbnailViewerEffect(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailEffect(parent, id, themeThumbnail, testValidity)
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
		wxCommandEvent evt(wxEVENT_APPLYEFFECT);
		evt.SetInt(numItem);
		panelInfos->GetEventHandler()->AddPendingEvent(evt);
	}
}