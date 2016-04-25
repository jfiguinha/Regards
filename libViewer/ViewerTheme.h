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
			~CViewerTheme();

			void GetInfosToolbarTheme(CThemeToolbar * theme);
			void GetFiltreToolbarTheme(CThemeToolbar * theme);
			void GetPreviewToolbarTheme(CThemeToolbar * theme);

			void GetClickInfosToolbarTheme(CThemeToolbar * theme);

			//Thumbnail
			void GetThumbnailPaneTheme(CThemePane * theme);
			void GetThumbnailTheme(CThemeThumbnail * theme);
			void GetClickThumbnailToolbarTheme(CThemeToolbar * theme);

			//Thumbnail Vidéo
			void GetThumbnailVideoPaneTheme(CThemePane * theme);
			void GetThumbnailVideoTheme(CThemeThumbnail * theme);
			void GetClickThumbnailVideoToolbarTheme(CThemeToolbar * theme);


			void GetInfosPaneTheme(CThemePane * theme);
			void GetPreviewPaneTheme(CThemePane * theme);

			void GetPreviewInfosSplitterTheme(CThemeSplitter * theme);

			void GetWndCentralSplitterTheme(CThemeSplitter * theme);
			void GetMainToolbarTheme(CThemeToolbar * theme);

			void GetScrollThumbnailTheme(CThemeScrollBar * theme);
			void GetScrollThumbnailVideoTheme(CThemeScrollBar * theme);

			//Infos
			void GetFiltreScrollTheme(CThemeScrollBar * theme);
			void GetScrollThumbnailEffectTheme(CThemeScrollBar * theme);
			void GetScrollInfosFileTheme(CThemeScrollBar * theme);
			void GetScrollHistoryEffectTheme(CThemeScrollBar * theme);
			void GetScrollEffectVideoTheme(CThemeScrollBar * theme);

			void GetTreeInfosFileTheme(CThemeTree * theme);
			void GetTreeEffectVideoWndTheme(CThemeTree * theme);
			void GetTreeHistoryTheme(CThemeTree * theme);
			void GetTreeThumbnailViewerEffectTheme(CThemeThumbnail * theme);
			void GetTreeFiltreEffectTheme(CThemeTree * theme);

			void GetBitmapInfosTheme(CThemeBitmapInfos * theme);

			void GetCategoryFolderSplitterTheme(CThemeSplitter &theme);
			void GetPaneCategory(CThemePane &theme);
			void GetCategoryTreeTheme(CThemeTree &theme);
			void GetCategoryScrollTheme(CThemeScrollBar &theme);

		private:

			void LoadTheme();
			void SaveTheme();
			void InitTheme();

			void InitInfosFileTree();
			void InitEffectVideoWndTree();
			void InitHistoryEffectTree();
			void InitThumbnailViewerEffectTree();
			void InitFiltreEffectTree();

			void InitInfosToolbar();
			void InitViewerToolbar();
			void InitPreviewToolbar();
			void InitFiltreToolbar();

			void InitClickThumbnailVideoToolbar();
			void InitClickThumbnailToolbar();

			void InitThumbnailPane();
			void InitThumbnailVideoPane();
			void InitInfosPane();
			void InitPreviewPane();

			void InitScrollFiltre();
			void InitScrollThumbnail();
			void InitScrollThumbnailVideo();
			void InitScrollThumbnailEffect();
			void InitScrollInfosFile();
			void InitScrollHistoryEffect();
			void InitScrollEffectVideo();

			void InitPreviewInfosSplitter();
			void InitCentralWndSplitter();

			void InitThumbnail();
			void InitThumbnailVideo();

			void InitBitmapInfos();

			//
			void InitScrollCategory();
			void InitCategoryTree();
			void InitPaneCategory();
			void InitCategoryFolderSplitter();
			void InitPaneFolder();
			void InitFolderTheme();
			void InitClickFolderToolbar();

			CThemeToolbar themeInfosToolbar;
			CThemeToolbar themeMainToolbar;
			CThemeToolbar themePreviewToolbar;
			CThemeToolbar themeFiltreToolbar;

			CThemeToolbar themeClickInfosToolbar;
			CThemeToolbar themeClickThumbnailToolbar;
			CThemeToolbar themeClickThumbnailVideoToolbar;

			CThemePane themeThumbnailPane;
			CThemePane themeThumbnailVideoPane;
			CThemePane themeInfosPane;
			CThemePane themePreviewPane;

			CThemeTree themeFiltreEffectTree;
			CThemeThumbnail themeThumbnailViewerEffectTree;
			CThemeTree themeInfosFileTree;
			CThemeTree themeHistoryEffectTree;
			CThemeTree themeEffectVideoWndTree;

			CThemeSplitter themePreviewInfos;
			CThemeSplitter themeCentralWndSplitter;


			CThemeScrollBar themeScrollThumbnailVideo;
			CThemeScrollBar themeScrollThumbnail;

			CThemeScrollBar themeScrollFiltre;
			CThemeScrollBar themeScrollThumbnailEffect;
			CThemeScrollBar themeScrollInfosFile;
			CThemeScrollBar themeScrollHistoryEffect;
			CThemeScrollBar themeScrollEffectVideo;

			CThemeThumbnail themeThumbnail;
			CThemeThumbnail themeThumbnailVideo;

			CThemeBitmapInfos themeBitmapInfos;


			CThemeTree themeCategoryTree;
			CThemeScrollBar themeScrollCategory;
			CThemePane themePaneCategory;
			CThemeSplitter themeCategoryFolderSplitter;
			CThemePane themePaneFolder;
			CThemeFolder themeFolder;
			CThemeToolbar themeClickFolderToolbar;

		};
	}
}
