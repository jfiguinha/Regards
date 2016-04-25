#pragma once
#include <EffectVideoParameter.h>
#include <Splitter.h>
#include <StatusBarInterface.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
		class CPreviewThumbnail;
		class CCategoryFolderWindow;
		class CExplorerParam;

		class CCentralWnd :public CSplitter
		{
		public:

			CCentralWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter);
			~CCentralWnd();

            void StopLoadingPicture();
			void ImageSuivante();
			void ImagePrecedente();
			void ImageDebut();
			void ImageFin();
			void TransitionEnd(){};

			void SetNumImage(const int &numElement, const wxString &filepath);

			void ShowToolbar();
			void HideToolbar();
            
            void UpdateScreenRatio();
            void UpdateCriteria();
            
		private:

			void OnHidePane(wxCommandEvent& event);
			//Variable
			CExplorerParam * explorerconfig = nullptr;
			CPreviewThumbnail * previewThumbnail = nullptr;
			CCategoryFolderWindow * categoryFolder = nullptr;
			int numCatalog;
			int numElement;

		};
	}
}

