#pragma once
#include "Thumbnail.h"
#include <Photos.h>
#include <Catalog.h>
#include <Criteria.h>
#include <PhotoCategorie.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CThumbnailVertical : public CThumbnail
		{
		public:
			CThumbnailVertical(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail);
			virtual ~CThumbnailVertical(void);

			void SetListeFile(const wxArrayString & listFile, const bool &showSelectButton = false);
			void SetIconeSize(const int &width, const int &height);
			void SetListeFile(const PhotosVector & photoVector, const bool &erase = true);

			int GetTypeAffichage();
			int GetNumCatalog();
			int GetNumCategorie();
			wxString GetKey();


		private:

			void CalculPosHorizontal(const int &numItem);
			void CalculPosVertical(const int &numItem);
			void SetActiveScroll(bool visible);
			void RenderIcone(wxDC * dc);
			void UpdateScroll();
			CIcone * FindElement(const int &xPos, const int &yPos);


			int typeAffichage;
			int numCatalog;
			int numCategorie;
			int numParent;
			wxString key;
		};
	}
}

