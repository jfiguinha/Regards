#pragma once
#include <ThumbnailVertical.h>
#include "InfosSeparationBarExplorer.h"
#include "Photos.h"
using namespace Regards::Control;

namespace Regards::Viewer
{
	class CMainFrame;

	class CThumbnailFolder : public CThumbnailVertical
	{
	public:
		CThumbnailFolder(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
		                 const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnailFolder(void) override;
		vector<wxString> GetFileList();
		void Init(const int& typeAffichage);
		void AddSeparatorBar(CIconeList* iconeListLocal, const wxString& libelle, PhotosVector* photoVector,
		                     int& nbElement);

	protected:

		void VideoProcessThumbnail() override;
		void OnPictureClick(CThumbnailData* data) override;
		void ResizeThumbnail() override;
		static bool compareInterval(int i1, int i2);
	private:
		static bool ItemCompFonctWithVScroll(int xPos, int yPos, CIcone* icone, CWindowMain* parent);
		static bool ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent);
		void SetListeFile(PhotosVector* photoVector);


		CIcone* FindElementWithVScroll(const int& xPos, const int& yPos) override;
		void FindOtherElement(wxDC* dc, const int& x, const int& y) override;
		CIcone* FindElement(const int& xPos, const int& yPos) override;
		CInfosSeparationBar* FindSeparatorElement(const int& xPos, const int& yPos);
		void RenderIconeWithVScroll(wxDC* dc) override;

		void UpdateScrollWithVScroll() override;
		void InitTypeAffichage(PhotosVector* photoVector, const int& typeAffichage);

		InfosSeparationBarVector listSeparator;
		PhotosVector newPhotosVectorList;
		std::map<int, int> listElementToShow;
		int barseparationHeight;
		int widthThumbnail;
		int heightThumbnail;
	};
}
