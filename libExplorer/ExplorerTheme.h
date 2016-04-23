#pragma once
#include <ThemeParam.h>

using namespace rapidxml;
using namespace std;

namespace Regards
{
	namespace Explorer
	{
		class CExplorerTheme : public CThemeParam
		{
		public:
			CExplorerTheme();
			~CExplorerTheme();

			void GetCentralWndTheme(CThemeSplitter &theme);
			void GetListImageViewTheme(CThemeSplitter &theme);
			void GetCategoryFolderSplitterTheme(CThemeSplitter &theme);
			void GetPreviewSplitterTheme(CThemeSplitter &theme);

			void GetExplorerToolbarTheme(CThemeToolbar &theme);
			void GetViewerToolbarTheme(CThemeToolbar &theme);
			void GetThumbnailToolbarTheme(CThemeToolbar &theme);
			void GetInfosToolbarTheme(CThemeToolbar &theme);
			void GetClickFolderToolbarTheme(CThemeToolbar &theme);
			void GetClickPreviewToolbarTheme(CThemeToolbar &theme);
			void GetClickPanelInfosToolbarTheme(CThemeToolbar &theme);

			void GetPaneInfosTheme(CThemePane &theme);
			void GetPanePreviewTheme(CThemePane &theme);
			void GetPaneThumbnail(CThemePane &theme);
			void GetPanePreviewPictureTheme(CThemePane &theme);
			void GetPaneFolder(CThemePane &theme);

			void GetPaneCategory(CThemePane &theme);
			void GetCategoryTreeTheme(CThemeTree &theme);
			void GetCategoryScrollTheme(CThemeScrollBar &theme);

			void GetFolderTheme(CThemeFolder &theme);

			void GetScrollInfosFileTheme(CThemeScrollBar &theme);
			void GetTreeInfosFileTheme(CThemeTree &theme);
			void GetBitmapInfosTheme(CThemeBitmapInfos &theme);

			void GetThumbnailTheme(CThemeThumbnail &theme);
			void GetThumbnailToolbarZoomTheme(CThemeToolBarZoom &theme);
			void GetThumbnailScrollTheme(CThemeScrollBar &theme);
		private:

			void InitExplorerToolbar();
			void InitViewerToolbar();
			void InitInfosToolbar();
			void InitThumbnailToolbar();
			void InitClickFolderToolbar();
			void InitClickInfosToolbar();
			void InitClickPreviewToolbar();

			void InitCentralWnd();
			void InitListImageView();
			void InitPreviewSplitter();
			void InitCategoryFolderSplitter();

			void InitPanePreview();
			void InitPaneInfos();
			void InitPanePreviewPicture();
			void InitPaneCategory();
			void InitPaneFolder();
			void InitPaneThumbnail();

			void InitFolderTheme();
			void InitThumbnailTheme();
			void InitBitmapInfos();
			void InitThumbnailToolbarZoomTheme();

			void InitInfosFileTree();
			void InitCategoryTree();

			void InitScrollCategory();
			void InitScrollInfosFile();
			void InitThumbnailScrollTheme();


			void LoadTheme();
			void SaveTheme();
			void InitTheme();


			CThemeToolbar themeInfosToolbar;
			CThemeToolbar themeExplorerToolbar;
			CThemeToolbar themeViewerToolbar;
			CThemeToolbar themeThumbnailToolbar;
			CThemeToolbar themeClickInfosToolbar;
			CThemeToolbar themeClickFolderToolbar;
			CThemeToolbar themeClickPreviewToolbar;

			CThemeTree themeCategoryTree;
			CThemeScrollBar themeScrollCategory;

			CThemeSplitter themeCentralWnd;
			CThemeSplitter themeImageListView;
			CThemeSplitter themePreview;
			CThemeSplitter themeCategoryFolderSplitter;

			CThemePane themePanePreview;
			CThemePane themePaneInfos;
			CThemePane themePanePreviewPicture;
			CThemePane themePaneThumbnail;
			CThemePane themePaneFolder;
			CThemePane themePaneCategory;

			CThemeFolder themeFolder;


			CThemeScrollBar themeScrollInfosFile;
			CThemeTree themeInfosFileTree;


			CThemeThumbnail themeThumbnail;
			CThemeToolBarZoom themeThumbnailToolbarZoom;
			CThemeScrollBar themeThumbnailScroll;
			CThemeBitmapInfos themeBitmapInfos;
		};
	}
}
