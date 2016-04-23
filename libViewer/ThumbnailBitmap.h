#pragma once
#include <ThumbnailHorizontal.h>
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{
		class CMainFrame;

		class CThumbnailBitmap : public CThumbnailHorizontal
		{
		public:
			CThumbnailBitmap(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail);
			~CThumbnailBitmap(void);

		private:

			void OnPictureClick(CThumbnailData * data);
		};
	}
}


