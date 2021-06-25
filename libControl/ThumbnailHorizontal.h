#pragma once
#include "Thumbnail.h"
#include <wx/arrstr.h>

using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CThumbnailHorizontal : public CThumbnail
		{
		public:
			CThumbnailHorizontal(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
			                     const bool& testValidity);
			~CThumbnailHorizontal(void) override;
			void SetListeFile(const vector<wxString>& files);
			void InitPosition();
			wxString GetWaitingMessage() override;

		protected:
			void RenderIcone(wxDC* deviceContext) override;
			void UpdateScroll() override;
			CIcone* FindElement(const int& xPos, const int& yPos) override;
			//void SetActiveScroll(bool visible);
			//void UpdateThumbnail();
		};
	}
}
