#pragma once
#include "ThumbnailEffect.h"
using namespace Regards::Control;

namespace Regards
{
	namespace Control
	{
		class CThumbnailViewerEffect : public CThumbnailEffect
		{
		public:
			CThumbnailViewerEffect(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
			                       const CThemeThumbnail& themeThumbnail, const bool& testValidity, int panelInfosId);
			~CThumbnailViewerEffect(void) override;

		private:
			void OnPictureClick(CThumbnailData* data) override;
			int panelInfosId;
		};
	}
}
