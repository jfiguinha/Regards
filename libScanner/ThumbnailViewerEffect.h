#pragma once
#include <ThumbnailEffect.h>
using namespace Regards::Control;

namespace Regards
{
	namespace Scanner
	{
		class CPanelInfosWnd;

		class CThumbnailViewerEffect : public CThumbnailEffect
		{
		public:
			CThumbnailViewerEffect(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailViewerEffect(void);

		private:

			void OnPictureClick(CThumbnailData * data);
		};
	}
}


