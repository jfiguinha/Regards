#pragma once
#include "Thumbnail.h"
using namespace Regards::Window;

namespace Regards::Control
{
	class CThumbnailVertical : public CThumbnail
	{
	public:
		CThumbnailVertical(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
		                   const bool& testValidity);
		~CThumbnailVertical(void) override;
		void SetNoVScroll(const bool& noVscroll);
		wxString GetWaitingMessage() override;

	protected:
		void RenderIcone(wxDC* dc) override;
		virtual void RenderIconeWithVScroll(wxDC* dc);
		virtual void RenderIconeWithoutVScroll(wxDC* dc);
		void UpdateScroll() override;
		virtual void UpdateScrollWithoutVScroll();
		virtual void UpdateScrollWithVScroll();

		CIcone* FindElement(const int& xPos, const int& yPos) override;
		virtual CIcone* FindElementWithVScroll(const int& xPos, const int& yPos);
		virtual CIcone* FindElementWithoutVScroll(const int& xPos, const int& yPos);

		bool noVscroll;
		const bool& test_validity_;
		const CThemeThumbnail& theme_thumbnail_;
		const wxWindowID id_;
	};
}
