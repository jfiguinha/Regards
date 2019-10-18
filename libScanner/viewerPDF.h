#pragma once
#include <ToolbarInterface.h>
#include <Photos.h>
#include <WindowMain.h>
#include <PanelWithClickToolbar.h>
#include "ToolbarPDF.h"
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

		class CViewerPDF : public CWindowMain, public CBitmapInterface
		{
		public:
			CViewerPDF(wxWindow* parent, CScannerFrame * frame, wxWindowID idCTreeWithScrollbarInterface);
			~CViewerPDF();

			void AnimationPictureNext();
			void AnimationPicturePrevious();

			void UpdateScreenRatio();
			void SetPosition(const long& timePosition);

			wxString SetImage(wxImage imageFile);
			void LoadFile(const wxString &filename);
			CImageLoadingFormat * GetImage();
			wxString GetFilename();
			int GetAnimationPosition();

		private:
			
			bool GetProcessEnd();
			void ClosePane(const int& id) {};
			void RefreshPane(const int& id) {};	
			void ShowPanelVideoThumbnail();
			void HidePanel();
			void Resize();
			void OnResize(wxCommandEvent& event);




			void LoadAnimationBitmap(const wxString &filename, const int &numFrame);
			void OnSize(wxSizeEvent& event);
			void AnimationSetPosition(wxCommandEvent& event);

			void ImageSuivante();
			void ImagePrecedente();
			void RedrawBarPos();
			wxRect GetWindowRect();
			int nbThumbnail;

			//Thumbnail Video
			CPanelWithClickToolbar * panelVideo;
			CScrollbarWnd * scrollVideoWindow;
			CThumbnailMultiPage * thumbnailVideo;



			//CPreviewThumbnailSplitter * previewThumbnailSplitter;
			CShowBitmap * showBitmapWindow;
			bool checkValidity;
			bool isFullscreen;
			wxString filename;
			int oldAnimationPosition = -1;
			wxString oldFilename = L"";
			int width;
			int height;

			bool showToolbar;
			int animationPosition;
			vector<CImageVideoThumbnail *> pageThumbnail;

			CScannerFrame * frame = nullptr;
		};


	}
}
