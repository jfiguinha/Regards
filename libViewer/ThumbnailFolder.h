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

		void Init(const int& typeAffichage);
		void ApplyTypeAffichage(CIconeList* iconeListLocal, InfosSeparationBarVector* _listSeparator, int typeAffichage);
		CInfosSeparationBarExplorer * AddSeparatorBar(PhotosVector * _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement);
		CIconeList* PrepareTypeAffichage(PhotosVector * _pictures, const int& typeAffichage, InfosSeparationBarVector* listSeparator);
		CInfosSeparationBarExplorer* AddSeparatorBar(CIconeList* iconeListLocal, const wxString& libelle, int& nbElement);
	protected:

		void VideoProcessThumbnail() override;
		void OnPictureClick(CThumbnailData* data) override;
		void ResizeThumbnail() override;
		static bool compareInterval(int i1, int i2);
	private:

		void EndGenThumbnail(wxCommandEvent& event);
		static bool ItemCompFonctWithVScroll(int xPos, int yPos, CIcone* icone, CWindowMain* parent);
		static bool ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent);
		void SetListeFile();
		void GenerateThumbnail();
		static void  ExecuteThumbnailGen(void* param);
		CIcone* FindElementWithVScroll(const int& xPos, const int& yPos) override;
		void FindOtherElement( const int& x, const int& y) override;
		CIcone* FindElement(const int& xPos, const int& yPos) override;
		CInfosSeparationBar* FindSeparatorElement(const int& xPos, const int& yPos);
		void RenderIconeWithVScroll(wxDC* dc) override;

		void UpdateScrollWithVScroll() override;
		void InitTypeAffichage(const int& typeAffichage);

		InfosSeparationBarVector * listSeparator = nullptr;
		std::vector<int> listElementToShow;
		int barseparationHeight;
		int widthThumbnail;
		int heightThumbnail;
		std::mutex muVector;
		std::mutex muThumb;
		std::thread* thread_thumbnail = nullptr;
	};
}
