#pragma once
#include <WindowMain.h>
#include <PanelWithClickToolbar.h>
#include "FiltreToolbar.h"
#include <BitmapInterface.h>
using namespace std;
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CShowBitmap;
	}
}

using namespace Regards::Control;
class CImageLoadingFormat;
class CScannerFrame;
class CImageVideoThumbnail;

namespace Regards
{
	namespace Scanner
	{
		class CThumbnailMultiPage;
		class CPanelInfosWnd;

		class CViewerPDF : public CWindowMain, public CBitmapInterface
		{
		public:
			CViewerPDF(wxWindow* parent, CScannerFrame* frame, wxWindowID idCTreeWithScrollbarInterface);
			~CViewerPDF() override;

			void AnimationPictureNext();
			void AnimationPicturePrevious();

			void UpdateScreenRatio() override;
			void SetPosition(const long& timePosition);

			wxString SetImage(wxImage imageFile);
			void LoadFile(const wxString& filename);
			CImageLoadingFormat* GetImage();
			wxString GetFilename();
			int GetAnimationPosition();

			void HideValidationToolbar();
			void ShowValidationToolbar(const bool& visible, const int& filtre);

		private:
			bool GetProcessEnd() override;
			void ShowPanelVideoThumbnail();
			void HidePanel();


			void LoadAnimationBitmap(const wxString& filename, const int& numFrame);

			void AnimationSetPosition(wxCommandEvent& event);
			void OnFiltreOK(wxCommandEvent& event);
			void OnFiltreCancel(wxCommandEvent& event);
			void OnShowToolbar(wxCommandEvent& event);
			void Resize() override;
			void ImageSuivante() override;
			void ImagePrecedente() override;
			int nbThumbnail;
			bool showValidationToolbar = false;
			//Thumbnail Video
			CScrollbarWnd* scrollVideoWindow;
			CThumbnailMultiPage* thumbnailVideo;
			CFiltreToolbar* filtreToolbar;
			CWindowManager* windowManager;
			CPanelInfosWnd* panelInfosWindow;

			//CPreviewThumbnailSplitter * previewThumbnailSplitter;
			CShowBitmap* showBitmapWindow;
			bool checkValidity;
			bool isFullscreen;
			wxString filename;
			int oldAnimationPosition = -1;
			wxString oldFilename = L"";

			bool showToolbar;
			int animationPosition;
			vector<CImageVideoThumbnail*> pageThumbnail;

			CScannerFrame* frame = nullptr;

			bool isEffect;
		};
	}
}
