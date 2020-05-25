#pragma once
#include <ThumbnailVertical.h>
#include "InfosSeparationBarExplorer.h"
using namespace Regards::Control;

#define SHOW_ALL 0

class CImageVideoThumbnail;

namespace Regards
{
	namespace Scanner
	{
		class CThumbnailFileSelection : public CThumbnailVertical
		{
		public:
			CThumbnailFileSelection(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailFileSelection(void);
			void Init(const wxString &filename, const int &typeAffichage = SHOW_ALL);

			vector<int> GetSelectItem();

		protected:

			void OnPictureClick(CThumbnailData * data);
			void ResizeThumbnail();

		private:
            
			void SetListeFile();
			void AddSeparatorBar(const wxString &libelle, int &nbElement);
			CIcone * FindElementWithVScroll(const int &xPos, const int &yPos);
			CIcone * FindIcone(const int &photoId);
			void FindOtherElement(wxDC * dc, const int &x, const int &y);
			CIcone * FindElement(const int &xPos, const int &yPos);
			CInfosSeparationBar * FindSeparatorElement(const int &xPos, const int &yPos);
			void RenderIconeWithVScroll(wxDC * dc);
			void UpdateScrollWithVScroll();
			void InitTypeAffichage(const int &typeAffichage);
			
			InfosSeparationBarVector listSeparator;
			vector<CImageVideoThumbnail *> photoVector;
			int barseparationHeight;
			int widthThumbnail;
			int heightThumbnail;
			wxString filename;
		};
	}
}


