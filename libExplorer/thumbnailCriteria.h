#pragma once
#include <Thumbnail.h>
#include <Icone.h>
#include "InfosSeparationBarExplorer.h"
using namespace Regards::Window;

#define SHOW_ALL 0
#define SHOW_BYYEAR 1
#define SHOW_BYMONTH 2
#define SHOW_BYDAY 3
#define SHOW_BYLOCALISATION 4

namespace Regards
{
	namespace Explorer
	{

		class CThumbnailCriteria : public CThumbnail
		{
		public:
			CThumbnailCriteria(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const CThemeThumbnail & themeThumbnail);
			~CThumbnailCriteria(void);

			//void SetIconeSize(const int &width, const int &height);
			void Init(const int &typeAffichage);
            //void UpdateAdd();
			int GetId(const int &numItem);
			int GetTypeAffichage();
			int GetNbElement();

		protected:

			//void CreateOrLoadStorageFile();
			void OnPictureClick(CThumbnailData * data);
			void SetNbFiles(const int &nbFiles);

		private:
			
            void ShowByMonth(int &nbElement);
            void ShowByYear(int &nbElement);
            void ShowByLocalisation(int &nbElement);
            void ShowByDay(int &nbElement);
            
            CIcone * FindIcone(const int &photoId);
            CIcone * FindIcone(const int &photoId, CInfosSeparationBarExplorer * infosSeparationBar);
            CInfosSeparationBar * FindSeparatorBar(const wxString &libelle);
			void AddSeparatorBar(const wxString &libelle, PhotosVector * photoVector, int &nbElement);
            void UpdateSeparatorBar(const wxString &libelle, PhotosVector * photoVector, int &nbElement);
			void GenerateIconeList();
			void SetActiveScroll(bool visible);
			void RenderIcone(wxDC * dc);
			void UpdateScroll();
			void FindOtherElement(wxDC * dc, const int &x, const int &y);
			CIcone * FindElement(const int &xPos, const int &yPos);
			CInfosSeparationBar * FindSeparatorElement(const int &xPos, const int &yPos);
			int Dayofweek(int d, int m, int y);

			int typeAffichage;
			int numCatalog;
			int numCategorie;
			int numParent;
			wxString key;
			int barseparationHeight = 40;
            int numPhotoId = 0;
			InfosSeparationBarVector listSeparator;
		};
	}
}
