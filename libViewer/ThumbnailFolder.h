#pragma once
#include <ThumbnailVertical.h>
#include "TreatmentData.h"

#include "Photos.h"
#include <memory>
using namespace Regards::Control;

namespace Regards::Viewer
{
	class CMainFrame;

	class CThumbnailFolder : public CThumbnailVertical, public ISeparatorClass
	{
	public:
		CThumbnailFolder(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
			const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnailFolder(void) = default;

		void Init(const int& typeAffichage, const bool& isDeleteFolder, const bool& isSqlUpdate);
		std::unique_ptr<CInfosSeparationBarExplorer> AddSeparatorBar(PhotosVector* _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement) override;
		void ChangeTypeAffichage(const int& typeAffichage, bool needFindNewItem = false);

	protected:

		void OnPictureClick(const int& numPhotoId) override;
		void ResizeThumbnail() override;
		static bool compareFilename(CPhotos i1, CPhotos i2);

	private:


		static bool ItemCompFonctWithVScroll(int xPos, int yPos, CIcone* icone, CWindowMain* parent);
		static bool ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent);

		CIcone* FindElementWithVScroll(const int& xPos, const int& yPos) override;
		void FindOtherElement(wxDC* dc, const int& x, const int& y) override;
		CIcone* FindElement(const int& xPos, const int& yPos) override;
		CInfosSeparationBar* FindSeparatorElement(const int& xPos, const int& yPos);
		void RenderIconeWithVScroll(wxDC* dc) override;

		void UpdateScrollWithVScroll() override;


		// Le vecteur ET ses éléments sont désormais possédés via unique_ptr
		// (InfosSeparationBarVector = std::vector<std::unique_ptr<CInfosSeparationBar>>).
		// Plus aucun new/delete manuel sur les CInfosSeparationBar* : la destruction
		// est entièrement automatique (destructeur, clear(), réaffectation...).
		std::unique_ptr<InfosSeparationBarVector> listSeparator;
		//std::vector<int> listElementToShow;
		int barseparationHeight;
		int widthThumbnail;
		int heightThumbnail;
		bool updateThumbnail = false;
		bool needFindNewItem = false;
		//std::mutex muVector;
		//std::mutex muThumb;
		//std::thread* thread_thumbnail = nullptr;
	};
}