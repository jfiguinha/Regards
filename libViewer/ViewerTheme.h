#pragma once
#include <ThemeParam.h>
using namespace rapidxml;
using namespace std;

namespace Regards
{
	namespace Viewer
	{
		class CViewerTheme : public CThemeParam
		{
		public:
			CViewerTheme();
			virtual ~CViewerTheme();

			void GetInfosToolbarTheme(CThemeToolbar * theme);
			void GetFiltreToolbarTheme(CThemeToolbar * theme);
			void GetPreviewToolbarTheme(CThemeToolbar * theme);
			void GetThumbnailFaceToolbarTheme(CThemeToolbar& theme);
			void GetMainToolbarTheme(CThemeToolbar* theme);
			void GetThumbnailToolbarTheme(CThemeToolbar& theme);
			void GetBitmapInfosTheme(CThemeBitmapInfos * theme);
			void GetThumbnailToolbarZoomTheme(CThemeToolBarZoom &theme);
			

			//Default Theme
			void GetScrollTheme(CThemeScrollBar* theme);
			void GetPaneTheme(CThemePane* theme);
			void GetTreeTheme(CThemeTree* theme);
			void GetSplitterTheme(CThemeSplitter* theme);
			void GetClickToolbarTheme(CThemeToolbar* theme);
			void GetThumbnailTheme(CThemeThumbnail* theme);

		private:

			void LoadTheme();
			void SaveTheme();
			void InitTheme();

			//Default Theme
			void InitScrollTheme();
			void InitPaneTheme();
			void InitTreeTheme();
			void InitSplitterTheme();
			void InitClickToolbarTheme();
			void InitThumbnail();

			//--------------------------------------------------------------
			void InitInfosToolbar();
			void InitViewerToolbar();
			void InitPreviewToolbar();
			void InitFiltreToolbar();
			
			void InitBitmapInfos();
			void InitFolderTheme();
			void InitThumbnailToolbarZoomTheme();
			void InitThumbnailToolbar();
			void InitThumbnailFaceThemeToolbar();


			//Default Theme
			CThemeScrollBar themeScroll;
			CThemePane themePane;
			CThemeTree themeTree;
			CThemeSplitter themeSplitter;
			CThemeToolbar themeClickToolbar;
			CThemeThumbnail themeThumbnail;
			//-------------------------------------------------------

			CThemeToolbar themeInfosToolbar;
			CThemeToolbar themeMainToolbar;
			CThemeToolbar themePreviewToolbar;
			CThemeToolbar themeFiltreToolbar;
			CThemeToolbar themeThumbnailFaceToolbar;
			CThemeToolbar themeThumbnailToolbar;

			CThemeBitmapInfos themeBitmapInfos;
			CThemeFolder themeFolder;
			
			CThemeToolBarZoom themeThumbnailToolbarZoom;


		};
	}
}
