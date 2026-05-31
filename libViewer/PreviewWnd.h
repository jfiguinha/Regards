#pragma once
#include <WindowMain.h>
#include <BitmapInterface.h>
#include <ToolbarInterface.h>
;

class CRegardsBitmap;
class CImageLoadingFormat;

namespace Regards
{
	namespace Window
	{
		class CBitmapInfos;
	}

	using namespace Window;


	namespace Control
	{
		class CShowElement;
		class CFiltreToolbar;
	}

	using namespace Control;


	namespace Viewer
	{
		class CPreviewToolbar;
		class CAnimationToolbar;

		class CPreviewWnd : public CWindowMain, public CBitmapInterface, public CToolbarInterface
		{
		public:
			CPreviewWnd(wxWindow* parent, wxWindowID id, const bool& horizontal = true);
			~CPreviewWnd() override;
			bool SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail, const bool& isAnimation = false);
			bool SetVideo(const wxString& filename, const bool& play = false);
			void SetEffect(const bool& effect);
			void StopVideo();
			void TransitionEnd() override;
			void ImageSuivante() override;
			void ImagePrecedente() override;
			void Resize() override;
			void IsNextPicture(const bool& value);
			//void SetVideoPosition(const int &videoTime);
			void UpdateScreenRatio() override;
			bool IsToolbarVisible();

			void ShowValidationToolbar(const bool& visible, const int& filtre);
			void HideValidationToolbar();

			void ClickShowButton(const int& id, const int& refresh = 1) override;
			void SetFullscreen(const bool& fullscreen);

			void SetDiaporamaMode();
			void SetNormalMode();
			void SetFaceMode();
			void StopDiaporamaMode();
			void UpdateInfos();
			bool isVideo()
			{
				return mediaMode_ == MediaMode::Video;
			}
			bool isBitmap()
			{
				return mediaMode_ == MediaMode::Bitmap;
			}	
			bool isAnimation()
			{
				return mediaMode_ == MediaMode::Animation;
			}

		protected:
			void ShowScreenButton(wxCommandEvent& event);
			void DisableScreenButton(wxCommandEvent& event);
			void HideSaveButton(wxCommandEvent& event);
			void ShowSaveButton(wxCommandEvent& event);
			void OnFiltreOK(wxCommandEvent& event);
			void OnFiltreCancel(wxCommandEvent& event);
			void ShowToolbar(wxCommandEvent& event);
			void HideToolbar(wxCommandEvent& event);
			void OnShowToolbar(wxCommandEvent& event);
			void StartAnimation(wxCommandEvent& event);
			void HideExportButton(wxCommandEvent& event);
			void ShowExportButton(wxCommandEvent& event);


			CShowElement* showElement = nullptr;

			CAnimationToolbar* animationToolbar = nullptr;
			CPreviewToolbar* previewToolbar = nullptr;
			std::unique_ptr<CBitmapInfos> bitmapInfos = nullptr;
			CFiltreToolbar* filtreToolbar = nullptr;

			enum class MediaMode
			{
				Aucun,
				Bitmap,
				Animation,
				Video
			};


			MediaMode mediaMode_ = MediaMode::Aucun;
			//PreviewMode previewMode_ = PreviewMode::Normal;


			//bool isAnimation;
			//bool isBitmap;
			//bool isVideo;
			bool isEffect;
			bool showToolbar;
			bool fullscreen;
			bool isDiaporama;

			wxString oldfilename;
			CThemeBitmapWindow themeBitmap;
			wxWindowID id_;
			const bool& horizontal_;
			wxWindow* parent_;
		};
	}
}
