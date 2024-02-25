#pragma once
#include <ThumbnailVertical.h>
#include "TreatmentData.h"

#include "Photos.h"
using namespace Regards::Control;

namespace Regards::Viewer
{
	class CMainFrame;

	class CThumbnailFolder : public CThumbnailVertical, public ISeparatorClass
	{
	public:
		CThumbnailFolder(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
		                 const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnailFolder(void) override;
		
		void Init(const int& typeAffichage);
		CInfosSeparationBarExplorer * AddSeparatorBar(PhotosVector * _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement) override;
		CInfosSeparationBarExplorer * AddSeparatorBar(CIconeList* iconeListLocal, const wxString& libelle, int& nbElement);

	protected:
		void InitTypeAffichage(const int& typeAffichage);
		void OnPictureClick(CThumbnailData* data) override;
		void ResizeThumbnail() override;
		static bool compareInterval(int i1, int i2);
		static bool compareFilename(CPhotos i1, CPhotos i2);
		void SortVectorByFilename(PhotosVector* vector);
	private:


		static bool ItemCompFonctWithVScroll(int xPos, int yPos, CIcone *  icone, CWindowMain* parent);
		static bool ItemCompFonct(int xPos, int yPos, CIcone *  icone, CWindowMain* parent);
		void SetListeFile();

		CIcone *  FindElementWithVScroll(const int& xPos, const int& yPos) override;
		void FindOtherElement(wxDC* dc, const int& x, const int& y) override;
		CIcone *  FindElement(const int& xPos, const int& yPos) override;
		CInfosSeparationBar* FindSeparatorElement(const int& xPos, const int& yPos);
		void RenderIconeWithVScroll(wxDC* dc) override;

		void UpdateScrollWithVScroll() override;
		

		InfosSeparationBarVector * listSeparator = nullptr;
		//std::vector<int> listElementToShow;
		int barseparationHeight;
		int widthThumbnail;
		int heightThumbnail;
		bool updateThumbnail = false;
		//std::mutex muVector;
		//std::mutex muThumb;
		//std::thread* thread_thumbnail = nullptr;
	};
}
