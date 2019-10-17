#pragma once
#include <ThemeParam.h>
using namespace rapidxml;
using namespace std;

namespace Regards
{
	namespace Viewer
	{
		class CMainTheme : public CThemeParam
		{
		public:
			CMainTheme();
			virtual ~CMainTheme();

			void GetInfosToolbarTheme(CThemeToolbar * theme);
			void GetFiltreToolbarTheme(CThemeToolbar * theme);
			void GetPreviewToolbarTheme(CThemeToolbar * theme);
			void GetThumbnailFaceToolbarTheme(CThemeToolbar& theme);
			void GetMainToolbarTheme(CThemeToolbar* theme);
			void GetThumbnailToolbarTheme(CThemeToolbar& theme);
			void GetBitmapInfosTheme(CThemeBitmapInfos * theme);
			void GetThumbnailToolbarZoomTheme(CThemeToolBarZoom &theme);
			



		private:

			void LoadTheme();
			void SaveTheme();
			void InitTheme();


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
