#pragma once
#include "Thumbnail.h"
#include <Photos.h>
#include <wx/arrstr.h>

using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CThumbnailHorizontal : public CThumbnail
		{
		public:
			CThumbnailHorizontal(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailHorizontal(void);
            void SetListeFile(const vector<wxString> & files);
			void SetIconeSize(const int &width, const int &height);
            void InitPosition();
            wxString GetWaitingMessage();
            
		protected:

			
			void RenderIcone(wxDC * deviceContext);
			void UpdateScroll();
			CIcone * FindElement(const int &xPos, const int &yPos);
			void SetActiveScroll(bool visible);
			//void UpdateThumbnail();

			
		};
	}
}

