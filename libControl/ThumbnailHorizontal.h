#pragma once
#include "Thumbnail.h"
#include <Photos.h>
#include <wx/arrstr.h>
#ifdef __APPLE__
#include <AppleSelectFile.h>
#endif
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CThumbnailHorizontal : public CThumbnail
		{
		public:
			CThumbnailHorizontal(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail);
			virtual ~CThumbnailHorizontal(void);
            void SetListeFile(const vector<wxString> & files);
			void SetIconeSize(const int &width, const int &height);
            void InitPosition();
            
		protected:

			
			void RenderIcone(wxDC * deviceContext);
			void UpdateScroll();
			CIcone * FindElement(const int &xPos, const int &yPos);
			void SetActiveScroll(bool visible);
			void UpdateThumbnail();

			
		};
	}
}

