#pragma once
#include <ThumbnailVertical.h>
#include <InfosSeparationBarExplorer.h>
using namespace Regards::Control;

class CImageVideoThumbnail;

namespace Regards::Scanner
{
	class CThumbnailFileSelection : public CThumbnailVertical
	{
	public:
		CThumbnailFileSelection(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
		                        const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnailFileSelection(void) override;
		void Init(const wxString& filename, const int& typeAffichage = THUMB_SHOW_ALL);

		vector<int> GetSelectItem();

	protected:
		void OnPictureClick(CThumbnailData* data) override;
		void ResizeThumbnail() override;

	private:
		static bool ItemCompWithVScrollFonct(int x, int y, std::shared_ptr<CIcone> icone, CWindowMain* parent);
		static bool ItemCompFonct(int xPos, int yPos, std::shared_ptr<CIcone> icone, CWindowMain* parent);
		void SetListeFile();
		void AddSeparatorBar(CIconeList* iconeListLocal, const wxString& libelle, int& nbElement);
		std::shared_ptr<CIcone> FindElementWithVScroll(const int& xPos, const int& yPos) override;
		void FindOtherElement(wxDC* dc, const int& x, const int& y) override;
		std::shared_ptr<CIcone> FindElement(const int& xPos, const int& yPos) override;
		CInfosSeparationBar* FindSeparatorElement(const int& xPos, const int& yPos);
		void RenderIconeWithVScroll(wxDC* dc) override;
		void UpdateScrollWithVScroll() override;
		void InitTypeAffichage(const int& typeAffichage);

		InfosSeparationBarVector listSeparator;
		vector<CImageVideoThumbnail*> photoVector;
		int barseparationHeight;
		int widthThumbnail;
		int heightThumbnail;
		wxString filename;
	};
}
