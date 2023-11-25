#pragma once
#include <InfosSeparationBar.h>
#include "ThumbnailVertical.h"
using namespace Regards::Window;

namespace Regards::Control
{
	class CInfosSeparationBarEffect;

	class CThumbnailVerticalSeparator : public CThumbnailVertical
	{
	public:
		CThumbnailVerticalSeparator(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
		                   const bool& testValidity);
		~CThumbnailVerticalSeparator(void) override;


	protected:
		void UpdateScrollWithVScroll() override;
		void RenderIconeWithVScroll(wxDC* deviceContext) override;

		void ResizeThumbnailWithVScroll();
		void ResizeThumbnail() override;

		InfosSeparationBarVector listSeparator;
		int nbElement;
		int barseparationHeight;
		int widthThumbnail;
		int heightThumbnail;
	};
}
