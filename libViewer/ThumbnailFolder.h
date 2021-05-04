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

		class CThumbnailFolder : public CThumbnailVertical
		{
		public:
			CThumbnailFolder(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailFolder(void);
			void Init(const int &typeAffichage = SHOW_ALL);
			void AddSeparatorBar(const wxString& libelle, PhotosVector* photoVector, int& nbElement);
		protected:

			void OnPictureClick(CThumbnailData * data);
			void ResizeThumbnail();
			
		private:
			static bool ItemCompFonctWithVScroll(int xPos, int yPos, CIcone * icone, CWindowMain * parent);
			static bool ItemCompFonct(int xPos, int yPos, CIcone * icone, CWindowMain * parent);
			void SetListeFile();
			
			CIcone * FindElementWithVScroll(const int &xPos, const int &yPos);
			CIcone * FindIcone(const int &photoId);
			void FindOtherElement(wxDC * dc, const int &x, const int &y);
			CIcone * FindElement(const int &xPos, const int &yPos);
			CInfosSeparationBar * FindSeparatorElement(const int &xPos, const int &yPos);
			void RenderIconeWithVScroll(wxDC * dc);

			void UpdateScrollWithVScroll();
			void InitTypeAffichage(const int &typeAffichage);
			
			InfosSeparationBarVector listSeparator;
			PhotosVector newPhotosVectorList;
			int barseparationHeight;
			int widthThumbnail;
			int heightThumbnail;
		};
	}
}


