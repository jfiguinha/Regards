#pragma once
#include <ThumbnailVertical.h>
#include "InfosSeparationBarExplorer.h"
#include "Photos.h"
#include "MainWindow.h"
using namespace Regards::Control;

#define SHOW_ALL 0
#define SHOW_BYYEAR 1
#define SHOW_BYMONTH 2
#define SHOW_BYDAY 3
#define SHOW_BYLOCALISATION 4

namespace Regards::Viewer
{
	class CMainFrame;

	class CThumbnailViewerPicture : public CThumbnailVertical
	{
	public:
		CThumbnailViewerPicture(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
		                        const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnailViewerPicture(void) override;
		void Init(const int& typeAffichage = SHOW_ALL);
		void SetListeFile();
		vector<wxString> GetFileList();
		void ApplyListeFile();

		CIconeList* PregenerateList(PhotosVector * _pictures);

		int GetHeight() override
		{
			return GetIconeHeight();
		}

		static bool ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent);

	protected:
		void ResizeThumbnail() override;
		void ResizeThumbnailWithoutVScroll();

	private:
		void OnPictureClick(CThumbnailData* data) override;
		CIcone* FindElement(const int& xPos, const int& yPos) override;
		void RenderIconeWithoutVScroll(wxDC* deviceContext) override;

		static std::mutex localmu;
		int widthThumbnail;
		int heightThumbnail;
	};
}
