#pragma once
#include "Thumbnail.h"
#include <wx/arrstr.h>

using namespace Regards::Window;

namespace Regards::Control
{
	class CThumbnailHorizontal : public CThumbnail
	{
	public:
		CThumbnailHorizontal(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
		                     const bool& testValidity);
		~CThumbnailHorizontal(void) override;
		void InitPosition();

	protected:
		void RenderIcone(wxDC* deviceContext) override;
		void UpdateScroll() override;
		CIcone * FindElement(const int& xPos, const int& yPos) override;
	};
}
