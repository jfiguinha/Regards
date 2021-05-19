#pragma once
#include <ThumbnailVertical.h>
#include "InfosSeparationBarExplorer.h"
using namespace Regards::Control;

#define SHOW_ALL 0
#define SHOW_BYYEAR 1
#define SHOW_BYMONTH 2
#define SHOW_BYDAY 3
#define SHOW_BYLOCALISATION 4

namespace Regards
{
	namespace Viewer
	{
		class CMainFrame;

		class CThumbnailViewerPicture : public CThumbnailVertical
		{
		public:
			CThumbnailViewerPicture(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailViewerPicture(void);
			void Init(const int &typeAffichage = SHOW_ALL);
			void SetListeFile();

			int GetHeight()
			{
				return GetIconeHeight();
			}

		protected:
			void ResizeThumbnail();
			void ResizeThumbnailWithoutVScroll();
		private:

			

			void OnPictureClick(CThumbnailData * data);
			CIcone * FindIcone(const int &photoId);
			CIcone * FindElement(const int &xPos, const int &yPos);
			void RenderIconeWithoutVScroll(wxDC * deviceContext);
	

			int widthThumbnail;
			int heightThumbnail;
		};
	}
}


