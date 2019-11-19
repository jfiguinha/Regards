#include <header.h>
#include "ThumbnailViewerEffect.h"
#include <window_id.h>
using namespace Regards::Control;

CThumbnailViewerEffect::CThumbnailViewerEffect(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity, int panelInfosId)
	: CThumbnailEffect(parent, id, themeThumbnail, testValidity)
{
	this->panelInfosId = panelInfosId;
}


CThumbnailViewerEffect::~CThumbnailViewerEffect(void)
{

}


void CThumbnailViewerEffect::OnPictureClick(CThumbnailData * data)
{
	wxWindow * panelInfos = (wxWindow *)this->FindWindowById(panelInfosId);
	int numItem = data->GetNumPhotoId();
	if (panelInfos != nullptr)
	{
		wxCommandEvent evt(wxEVENT_APPLYEFFECT);
		evt.SetInt(numItem);
		panelInfos->GetEventHandler()->AddPendingEvent(evt);
	}
}