#pragma once
#include <ThumbnailEffect.h>
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{
		class CPanelInfosWnd;

		class CThumbnailViewerEffect : public CThumbnailEffect
		{
		public:
			CThumbnailViewerEffect(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail);
			~CThumbnailViewerEffect(void);

		private:

			void OnPictureClick(CThumbnailData * data);
		};
	}
}


