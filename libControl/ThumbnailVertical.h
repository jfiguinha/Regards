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
			CThumbnailVertical(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailVertical(void);
			int GetTypeAffichage();
			int GetNumCatalog();
			int GetNumCategorie();
			wxString GetKey();
			void SetNoVScroll(const bool &noVscroll);
            wxString GetWaitingMessage();
            
		protected:

			void SetListeFile(const vector<wxString> & files);
			void SetListeFile(const wxArrayString & listFile, const bool &showSelectButton = false);
			void SetIconeSize(const int &width, const int &height);
			void SetListeFile(const PhotosVector & photoVector, const bool &erase = true);

			void SetActiveScroll(bool visible);
			virtual void RenderIcone(wxDC * dc);
			virtual void RenderIconeWithVScroll(wxDC * dc);
			virtual void RenderIconeWithoutVScroll(wxDC * dc);
			void UpdateScroll();
			virtual void UpdateScrollWithoutVScroll();
			virtual void UpdateScrollWithVScroll();

			CIcone * FindElement(const int &xPos, const int &yPos);
			virtual CIcone * FindElementWithVScroll(const int &xPos, const int &yPos);
			virtual CIcone * FindElementWithoutVScroll(const int &xPos, const int &yPos);

			int typeAffichage;
			int numCatalog;
			int numCategorie;
			int numParent;
			bool noVscroll;
			wxString key;
		};
	}
}

