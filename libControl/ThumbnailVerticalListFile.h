#pragma once
#include "ThumbnailVertical.h"
#include <Photos.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CThumbnailVerticalListFile : public CThumbnailVertical
		{
		public:
			CThumbnailVerticalListFile(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
			                           const bool& testValidity);
			~CThumbnailVerticalListFile(void) override;
			int GetTypeAffichage();
			int GetNumCatalog();
			int GetNumCategorie();
			wxString GetKey();
			wxString GetWaitingMessage() override;

		protected:
			void SetListeFile(const vector<wxString>& files);
			void SetListeFile(const wxArrayString& listFile, const bool& showSelectButton = false);
			void SetListeFile(const PhotosVector& photoVector);

			int typeAffichage;
			int numCatalog;
			int numCategorie;
			int numParent;
			bool noVscroll;
			wxString key;
		};
	}
}
