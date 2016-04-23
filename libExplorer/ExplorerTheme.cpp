#include "ExplorerTheme.h"
#if defined(__WXMSW__)
#include "../include/font_quality.h"
#else
#include <font_quality.h>
#endif
using namespace rapidxml;
using namespace Regards::Explorer;

CExplorerTheme::CExplorerTheme()
{

}

void CExplorerTheme::InitPanePreview()
{
	themePanePreview.SetHeight(20);
	themePanePreview.themeTitle.SetCroixWidth(15);
	themePanePreview.themeTitle.SetCroixHeight(15);
	themePanePreview.themeTitle.SetHeight(30);
	themePanePreview.themeTitle.SetMarge(5);
	themePanePreview.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePanePreview.themeTitle.font.SetFontSize(14);
}

void CExplorerTheme::InitPaneInfos()
{
	themePaneInfos.SetHeight(20);
	themePaneInfos.themeTitle.SetCroixWidth(15);
	themePaneInfos.themeTitle.SetCroixHeight(15);
	themePaneInfos.themeTitle.SetHeight(30);
	themePaneInfos.themeTitle.SetMarge(5);
	themePaneInfos.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePaneInfos.themeTitle.font.SetFontSize(14);
}

void CExplorerTheme::InitPanePreviewPicture()
{
	themePanePreviewPicture.SetHeight(20);
	themePanePreviewPicture.themeTitle.SetCroixWidth(15);
	themePanePreviewPicture.themeTitle.SetCroixHeight(15);
	themePanePreviewPicture.themeTitle.SetHeight(30);
	themePanePreviewPicture.themeTitle.SetMarge(5);
	themePanePreviewPicture.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePanePreviewPicture.themeTitle.font.SetFontSize(14);
}
void CExplorerTheme::InitPaneThumbnail()
{
	themePaneThumbnail.SetHeight(20);
	themePaneThumbnail.themeTitle.SetCroixWidth(15);
	themePaneThumbnail.themeTitle.SetCroixHeight(15);
	themePaneThumbnail.themeTitle.SetHeight(30);
	themePaneThumbnail.themeTitle.SetMarge(5);
	themePaneThumbnail.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePaneThumbnail.themeTitle.font.SetFontSize(14);
}

void CExplorerTheme::InitPaneFolder()
{
	themePaneFolder.SetHeight(20);
	themePaneFolder.themeTitle.SetCroixWidth(15);
	themePaneFolder.themeTitle.SetCroixHeight(15);
	themePaneFolder.themeTitle.SetHeight(30);
	themePaneFolder.themeTitle.SetMarge(5);
	themePaneFolder.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePaneFolder.themeTitle.font.SetFontSize(14);
}

void CExplorerTheme::InitPaneCategory()
{
	themePaneCategory.SetHeight(20);
	themePaneCategory.themeTitle.SetCroixWidth(15);
	themePaneCategory.themeTitle.SetCroixHeight(15);
	themePaneCategory.themeTitle.SetHeight(30);
	themePaneCategory.themeTitle.SetMarge(5);
	themePaneCategory.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePaneCategory.themeTitle.font.SetFontSize(14);
}

void CExplorerTheme::GetPaneInfosTheme(CThemePane &theme)
{
	theme = themePaneInfos;
}

void CExplorerTheme::GetPanePreviewTheme(CThemePane &theme)
{
	theme = themePanePreview;
}

void CExplorerTheme::GetPaneThumbnail(CThemePane &theme)
{
	theme = themePaneThumbnail;
}

void CExplorerTheme::GetPanePreviewPictureTheme(CThemePane &theme)
{
	theme = themePanePreviewPicture;
}

void CExplorerTheme::GetPaneFolder(CThemePane &theme)
{
	theme = themePaneFolder;
}

void CExplorerTheme::GetPaneCategory(CThemePane &theme)
{
	theme = themePaneCategory;
}

void CExplorerTheme::GetFolderTheme(CThemeFolder &theme)
{
	theme = themeFolder;
}

void CExplorerTheme::GetScrollInfosFileTheme(CThemeScrollBar &theme)
{
	theme = themeScrollInfosFile;
}

void CExplorerTheme::GetTreeInfosFileTheme(CThemeTree &theme)
{
	theme = themeInfosFileTree;
}

void CExplorerTheme::InitPreviewSplitter()
{
	themePreview.themeFast.firstColor.Set(29, 29, 29);
	themePreview.themeFast.secondColor.Set(128, 128, 128);
	themePreview.themeSeparation.firstColor.Set(29, 29, 29);
	themePreview.themeSeparation.secondColor.Set(128, 128, 128);
}

void CExplorerTheme::GetPreviewSplitterTheme(CThemeSplitter &theme)
{
	theme = themePreview;
}


void CExplorerTheme::InitCategoryFolderSplitter()
{
	themeCategoryFolderSplitter.themeFast.firstColor.Set(29, 29, 29);
	themeCategoryFolderSplitter.themeFast.secondColor.Set(128, 128, 128);
	themeCategoryFolderSplitter.themeSeparation.firstColor.Set(29, 29, 29);
	themeCategoryFolderSplitter.themeSeparation.secondColor.Set(128, 128, 128);
}

void CExplorerTheme::GetCategoryFolderSplitterTheme(CThemeSplitter &theme)
{
	theme = themeCategoryFolderSplitter;
}

void CExplorerTheme::InitInfosFileTree()
{
	themeInfosFileTree.bgColorOne.Set(29, 29, 29);
	themeInfosFileTree.bgColorActif.Set(255, 255, 255);
	themeInfosFileTree.bgColorTwo.Set(74, 74, 74);
	themeInfosFileTree.SetRowHeight(20);
	themeInfosFileTree.SetMargeX(5);
	themeInfosFileTree.themeCheckbox.SetWidth(15);
	themeInfosFileTree.themeCheckbox.SetHeight(15);
	themeInfosFileTree.themeSlide.SetHeight(20);
	themeInfosFileTree.themeSlide.SetWidth(250);
	themeInfosFileTree.themeSlide.font.SetColorFont(wxColour(255, 255, 255));
	themeInfosFileTree.themeSlide.font.SetFontName("Verdana");
	themeInfosFileTree.themeSlide.font.SetFontSize(14);
	//themeInfosFileTree.themeSlide.SetMarge(2);
	themeInfosFileTree.themeSlide.rectanglePast.Set(0, 0, 128);
	themeInfosFileTree.themeSlide.rectangleNext.Set(48, 128, 254);
	themeInfosFileTree.themeTexte.SetHeight(20);
	themeInfosFileTree.themeTexte.SetWidth(20);
	themeInfosFileTree.themeTexte.font.SetColorFont(wxColour(255, 255, 255));
	themeInfosFileTree.themeTexte.font.SetFontName("Verdana");
	themeInfosFileTree.themeTexte.font.SetFontSize(10);
	themeInfosFileTree.themeTriangle.SetMarge(2);
	themeInfosFileTree.themeTriangle.SetElementHeight(10);
	themeInfosFileTree.themeTriangle.SetHeight(10);
	themeInfosFileTree.themeTriangle.SetElementWidth(10);
	themeInfosFileTree.themeTriangle.SetWidth(10);
	themeInfosFileTree.themeTriangle.color.Set(0, 0, 0);
}

void CExplorerTheme::InitScrollInfosFile()
{
	themeScrollInfosFile.SetRectangleSize(10);
	themeScrollInfosFile.colorBack.Set(29, 29, 29);
	themeScrollInfosFile.colorTriangle.Set(74, 74, 74);
	themeScrollInfosFile.colorTriangleActif.Set(128, 128, 128);
	themeScrollInfosFile.colorBar.Set(74, 74, 74);
	themeScrollInfosFile.colorBarActif.Set(128, 128, 128);
	themeScrollInfosFile.SetMarge(2);
}

void CExplorerTheme::InitScrollCategory()
{
	themeScrollCategory.SetRectangleSize(10);
	themeScrollCategory.colorBack.Set(29, 29, 29);
	themeScrollCategory.colorTriangle.Set(74, 74, 74);
	themeScrollCategory.colorTriangleActif.Set(128, 128, 128);
	themeScrollCategory.colorBar.Set(74, 74, 74);
	themeScrollCategory.colorBarActif.Set(128, 128, 128);
	themeScrollCategory.SetMarge(2);
}

void CExplorerTheme::InitListImageView()
{
	themeImageListView.themeFast.firstColor.Set(29, 29, 29);
	themeImageListView.themeFast.secondColor.Set(128, 128, 128);
	themeImageListView.themeSeparation.firstColor.Set(29, 29, 29);
	themeImageListView.themeSeparation.secondColor.Set(128, 128, 128);
}

void CExplorerTheme::GetListImageViewTheme(CThemeSplitter &theme)
{
	theme = themeImageListView;
}

void CExplorerTheme::InitCentralWnd()
{
	themeCentralWnd.themeFast.firstColor.Set(29, 29, 29);
	themeCentralWnd.themeFast.secondColor.Set(128, 128, 128);
	themeCentralWnd.themeSeparation.firstColor.Set(29, 29, 29);
	themeCentralWnd.themeSeparation.secondColor.Set(128, 128, 128);
}

void CExplorerTheme::GetCentralWndTheme(CThemeSplitter &theme)
{
	theme = themeCentralWnd;
}

void CExplorerTheme::GetThumbnailTheme(CThemeThumbnail &theme)
{
	theme = themeThumbnail;
}

void CExplorerTheme::GetCategoryScrollTheme(CThemeScrollBar &theme)
{
	theme = themeScrollCategory;
}

void CExplorerTheme::GetThumbnailToolbarZoomTheme(CThemeToolBarZoom &theme)
{
	theme = themeThumbnailToolbarZoom;
}

void CExplorerTheme::GetThumbnailScrollTheme(CThemeScrollBar &theme)
{
	theme = themeThumbnailScroll;
}

void CExplorerTheme::InitThumbnailScrollTheme()
{
	themeThumbnailScroll.SetRectangleSize(10);
	themeThumbnailScroll.colorBack.Set(29, 29, 29);
	themeThumbnailScroll.colorTriangle.Set(74, 74, 74);
	themeThumbnailScroll.colorTriangleActif.Set(128, 128, 128);
	themeThumbnailScroll.colorBar.Set(74, 74, 74);
	themeThumbnailScroll.colorBarActif.Set(128, 128, 128);
	themeThumbnailScroll.SetMarge(2);
}

void CExplorerTheme::InitThumbnailTheme()
{
	/*
	themeThumbnail.themeIcone.width = 200;
	themeThumbnail.themeIcone.height = 200;
	themeThumbnail.themeIcone.showOnlyThumbnail = false;
	themeThumbnail.themeIcone.colorBack.Set(0, 0, 0);
	themeThumbnail.themeIcone.colorTop.Set(74, 74, 74);
	themeThumbnail.themeIcone.colorBottom.Set(74, 74, 74);
	themeThumbnail.themeIcone.colorSelectTop.Set(205, 205, 205);
	themeThumbnail.themeIcone.colorSelectBottom.Set(171, 171, 171);
	SetBackgroundColor(.Set(74, 74, 74));
	themeThumbnail.themeIcone.font.SetColorFont(wxColour(255, 255, 255));
	*/
	themeThumbnail.themeIcone.SetWidth(200);
	themeThumbnail.themeIcone.SetHeight(200);
	themeThumbnail.themeIcone.showOnlyThumbnail = false;
	themeThumbnail.themeIcone.colorBack.Set(0, 0, 0);
	themeThumbnail.themeIcone.colorTop.Set(74, 74, 74);
	themeThumbnail.themeIcone.colorBottom.Set(74, 74, 74);
	themeThumbnail.themeIcone.colorSelectTop.Set(205, 205, 205);
	themeThumbnail.themeIcone.colorSelectBottom.Set(171, 171, 171);
	themeThumbnail.colorBack.Set(74, 74, 74);
	themeThumbnail.themeIcone.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnail.themeSeparation.colorBack.Set(74, 74, 74);
	themeThumbnail.themeSeparation.colorBack.Set(45, 45, 48);
	themeThumbnail.themeSeparation.colorTop.Set(45, 45, 48);
	themeThumbnail.themeSeparation.colorBottom.Set(45, 45, 48);
	themeThumbnail.themeSeparation.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeThumbnail.themeSeparation.themeFont.SetFontName("Verdana");
	themeThumbnail.themeSeparation.themeFont.SetFontSize(14);
	themeThumbnail.themeSeparation.themeFont.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnail.themeSeparation.SetHeight(40);
}

void CExplorerTheme::InitThumbnailToolbarZoomTheme()
{
	themeThumbnailToolbarZoom.fontActif.SetFontName("Verdana");
	themeThumbnailToolbarZoom.fontActif.SetFontSize(16);
	themeThumbnailToolbarZoom.fontActif.SetFontQuality(DEFAULT_QUALITY);
	themeThumbnailToolbarZoom.font.SetFontName("Verdana");
	themeThumbnailToolbarZoom.font.SetFontSize(16);
	themeThumbnailToolbarZoom.font.SetFontQuality(DEFAULT_QUALITY);
	themeThumbnailToolbarZoom.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailToolbarZoom.colorBack.Set(29, 29, 29);
	themeThumbnailToolbarZoom.fontActif.SetColorFont(wxColour(48, 128, 254));
	themeThumbnailToolbarZoom.SetHeight(30);
    themeThumbnailToolbarZoom.SetCheckboxHeight(20);
    themeThumbnailToolbarZoom.SetCheckboxWidth(20);
}

CExplorerTheme::~CExplorerTheme()
{
	doc.clear();
}

void CExplorerTheme::GetCategoryTreeTheme(CThemeTree &theme)
{
	theme = themeCategoryTree;
}

void CExplorerTheme::InitCategoryTree()
{
	themeCategoryTree.SetMargeX(5);
	themeCategoryTree.bgColorOne.Set(29, 29, 29);
	themeCategoryTree.bgColorTwo.Set(128, 128, 128);
}

void CExplorerTheme::InitViewerToolbar()
{
	themeViewerToolbar.colorBack.Set(30, 30, 30);
	themeViewerToolbar.colorTop.Set(128, 128, 128);
	themeViewerToolbar.colorBottom.Set(29, 29, 29);
	themeViewerToolbar.position = NAVIGATOR_CENTER;
	themeViewerToolbar.SetWidth(60);
	themeViewerToolbar.SetHeight(60);
	themeViewerToolbar.SetMargeX(5);
	themeViewerToolbar.SetMargeY(0);
	themeViewerToolbar.isVertical = false;

	themeViewerToolbar.button.SetTailleX(60);
	themeViewerToolbar.button.SetTailleY(60);
	themeViewerToolbar.button.lineColorTop.Set(222, 222, 222);
	themeViewerToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeViewerToolbar.button.actifTop.Set(255, 255, 255);
	themeViewerToolbar.button.actifBottom.Set(223, 223, 223);
	themeViewerToolbar.button.font.SetFontName("Verdana");
	themeViewerToolbar.button.font.SetFontSize(8);
	themeViewerToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeViewerToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeViewerToolbar.button.SetRectangleSize(2);
	themeViewerToolbar.button.showButtonOnly = 0;
}

void CExplorerTheme::InitThumbnailToolbar()
{
	themeThumbnailToolbar.colorBack.Set(30, 30, 30);
	themeThumbnailToolbar.colorTop.Set(29, 29, 29);
	themeThumbnailToolbar.colorBottom.Set(29, 29, 29);
	themeThumbnailToolbar.position = NAVIGATOR_CENTER;
	themeThumbnailToolbar.SetWidth(30);
	themeThumbnailToolbar.SetHeight(30);
	themeThumbnailToolbar.SetMargeX(5);
	themeThumbnailToolbar.SetMargeY(0);
	themeThumbnailToolbar.isVertical = false;

	themeThumbnailToolbar.button.SetTailleX(30);
	themeThumbnailToolbar.button.SetTailleY(30);
	themeThumbnailToolbar.button.lineColorTop.Set(222, 222, 222);
	themeThumbnailToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeThumbnailToolbar.button.actifTop.Set(255, 255, 255);
	themeThumbnailToolbar.button.actifBottom.Set(223, 223, 223);
	themeThumbnailToolbar.button.font.SetFontName("Verdana");
	themeThumbnailToolbar.button.font.SetFontSize(10);
	themeThumbnailToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailToolbar.button.SetRectangleSize(4);
	themeThumbnailToolbar.button.showButtonOnly = 1;

	themeThumbnailToolbar.slider.isColorBackground = true;
	themeThumbnailToolbar.slider.SetWidth(100);
	themeThumbnailToolbar.slider.SetHeight(30);
}

void CExplorerTheme::InitFolderTheme()
{
	themeFolder.colorBack.Set(74, 74, 74);
	themeFolder.colorFont.Set(255, 255, 255);
}

void CExplorerTheme::InitExplorerToolbar()
{
	themeExplorerToolbar.colorBack.Set(30, 30, 30);
	themeExplorerToolbar.colorTop.Set(128, 128, 128);
	themeExplorerToolbar.colorBottom.Set(29, 29, 29);
	themeExplorerToolbar.position = NAVIGATOR_LEFT;
	themeExplorerToolbar.SetWidth(100);
	themeExplorerToolbar.SetHeight(70);
	themeExplorerToolbar.SetMargeX(5);
	themeExplorerToolbar.SetMargeY(0);
	themeExplorerToolbar.isVertical = false;

	themeExplorerToolbar.button.SetTailleX(80);
	themeExplorerToolbar.button.SetTailleY(60);
	themeExplorerToolbar.button.lineColorTop.Set(222, 222, 222);
	themeExplorerToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeExplorerToolbar.button.actifTop.Set(255, 255, 255);
	themeExplorerToolbar.button.actifBottom.Set(223, 223, 223);
	themeExplorerToolbar.button.font.SetFontName("Verdana");
	themeExplorerToolbar.button.font.SetFontSize(8);
	themeExplorerToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeExplorerToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeExplorerToolbar.button.SetRectangleSize(2);
	themeExplorerToolbar.button.showButtonOnly = 0;
}

void CExplorerTheme::InitInfosToolbar()
{
	themeInfosToolbar.colorBack.Set(30, 30, 30);
	themeInfosToolbar.colorTop.Set(74, 74, 74);
	themeInfosToolbar.colorBottom.Set(74, 74, 74);
	themeInfosToolbar.position = NAVIGATOR_CENTER;
	themeInfosToolbar.SetWidth(120);
	themeInfosToolbar.SetHeight(30);
	themeInfosToolbar.SetMargeX(0);
	themeInfosToolbar.SetMargeY(0);
	themeInfosToolbar.isVertical = false;

	themeInfosToolbar.button.SetTailleX(120);
	themeInfosToolbar.button.SetTailleY(30);
	themeInfosToolbar.button.actifTop.Set(48, 48, 48);
	themeInfosToolbar.button.actifBottom.Set(128, 128, 128);
	themeInfosToolbar.button.lineColorTop.Set(222, 222, 222);
	themeInfosToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeInfosToolbar.button.SetRectangleSize(2);

	themeInfosToolbar.texte.SetTailleX(120);
	themeInfosToolbar.texte.SetTailleY(30);
	themeInfosToolbar.texte.font.SetFontName("Verdana");
	themeInfosToolbar.texte.font.SetFontSize(20);
	themeInfosToolbar.texte.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeInfosToolbar.texte.font.SetColorFont(wxColour(255, 255, 255));
	themeInfosToolbar.texte.fontActif.SetFontName("Verdana");
	themeInfosToolbar.texte.fontActif.SetFontSize(20);
	themeInfosToolbar.texte.fontActif.SetFontQuality(ANTIALIASED_QUALITY);
	themeInfosToolbar.texte.fontActif.SetColorFont(wxColour(255, 255, 255));
	themeInfosToolbar.texte.SetRectangleSize(2);
}

void CExplorerTheme::InitClickFolderToolbar()
{
	themeClickFolderToolbar.colorBack.Set(30, 30, 30);
	themeClickFolderToolbar.colorTop.Set(74, 74, 74);
	themeClickFolderToolbar.colorBottom.Set(29, 29, 29);
	themeClickFolderToolbar.position = NAVIGATOR_CENTER;
	themeClickFolderToolbar.SetWidth(60);
	themeClickFolderToolbar.SetHeight(10);
	themeClickFolderToolbar.SetMargeX(0);
	themeClickFolderToolbar.SetMargeY(5);
	themeClickFolderToolbar.isVertical = 1;

	themeClickFolderToolbar.button.SetTailleX(60);
	themeClickFolderToolbar.button.SetTailleY(10);
	themeClickFolderToolbar.button.lineColorTop.Set(222, 222, 222);
	themeClickFolderToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeClickFolderToolbar.button.actifTop.Set(255, 255, 255);
	themeClickFolderToolbar.button.actifBottom.Set(223, 223, 223);
	themeClickFolderToolbar.button.font.SetFontName("Verdana");
	themeClickFolderToolbar.button.font.SetFontSize(10);
	themeClickFolderToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeClickFolderToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeClickFolderToolbar.button.SetRectangleSize(4);
	themeClickFolderToolbar.button.showButtonOnly = 0;
}
void CExplorerTheme::InitClickInfosToolbar()
{
	themeClickInfosToolbar.colorBack.Set(30, 30, 30);
	themeClickInfosToolbar.colorTop.Set(74, 74, 74);
	themeClickInfosToolbar.colorBottom.Set(29, 29, 29);
	themeClickInfosToolbar.position = NAVIGATOR_CENTER;
	themeClickInfosToolbar.SetWidth(60);
	themeClickInfosToolbar.SetHeight(10);
	themeClickInfosToolbar.SetMargeX(5);
	themeClickInfosToolbar.SetMargeY(0);
	themeClickInfosToolbar.isVertical = 0;

	themeClickInfosToolbar.button.SetTailleX(60);
	themeClickInfosToolbar.button.SetTailleY(10);
	themeClickInfosToolbar.button.lineColorTop.Set(222, 222, 222);
	themeClickInfosToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeClickInfosToolbar.button.actifTop.Set(255, 255, 255);
	themeClickInfosToolbar.button.actifBottom.Set(223, 223, 223);
	themeClickInfosToolbar.button.font.SetFontName("Verdana");
	themeClickInfosToolbar.button.font.SetFontSize(10);
	themeClickInfosToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeClickInfosToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeClickInfosToolbar.button.SetRectangleSize(4);
	themeClickInfosToolbar.button.showButtonOnly = 0;
}

void CExplorerTheme::InitClickPreviewToolbar()
{
	themeClickPreviewToolbar.colorBack.Set(30, 30, 30);
	themeClickPreviewToolbar.colorTop.Set(74, 74, 74);
	themeClickPreviewToolbar.colorBottom.Set(29, 29, 29);
	themeClickPreviewToolbar.position = NAVIGATOR_CENTER;
	themeClickPreviewToolbar.SetWidth(60);
	themeClickPreviewToolbar.SetHeight(10);
	themeClickPreviewToolbar.SetMargeX(0);
	themeClickPreviewToolbar.SetMargeY(5);
	themeClickPreviewToolbar.isVertical = 1;

	themeClickPreviewToolbar.button.SetTailleX(60);
	themeClickPreviewToolbar.button.SetTailleY(10);
	themeClickPreviewToolbar.button.lineColorTop.Set(222, 222, 222);
	themeClickPreviewToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeClickPreviewToolbar.button.actifTop.Set(255, 255, 255);
	themeClickPreviewToolbar.button.actifBottom.Set(223, 223, 223);
	themeClickPreviewToolbar.button.font.SetFontName("Verdana");
	themeClickPreviewToolbar.button.font.SetFontSize(10);
	themeClickPreviewToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeClickPreviewToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeClickPreviewToolbar.button.SetRectangleSize(4);
	themeClickPreviewToolbar.button.showButtonOnly = 0;
}

void CExplorerTheme::InitBitmapInfos()
{
	themeBitmapInfos.SetHeight(40);
	themeBitmapInfos.themeFont.SetFontName("Verdana");
	themeBitmapInfos.themeFont.SetFontSize(16);
	themeBitmapInfos.themeFont.SetFontQuality(DEFAULT_QUALITY);
	themeBitmapInfos.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeBitmapInfos.colorBack.Set(74, 74, 74);
}

void CExplorerTheme::GetClickFolderToolbarTheme(CThemeToolbar &theme)
{
	theme = themeClickFolderToolbar;
}

void CExplorerTheme::GetClickPreviewToolbarTheme(CThemeToolbar &theme)
{
	theme = themeClickPreviewToolbar;
}

void CExplorerTheme::GetClickPanelInfosToolbarTheme(CThemeToolbar &theme)
{
	theme = themeClickInfosToolbar;
}

void CExplorerTheme::GetThumbnailToolbarTheme(CThemeToolbar &theme)
{
	theme = themeThumbnailToolbar;
}

void CExplorerTheme::GetInfosToolbarTheme(CThemeToolbar &theme)
{
	theme = themeInfosToolbar;
}

void CExplorerTheme::GetExplorerToolbarTheme(CThemeToolbar &theme)
{
	theme = themeExplorerToolbar;
}

void CExplorerTheme::GetViewerToolbarTheme(CThemeToolbar &theme)
{
	theme = themeViewerToolbar;
}

void CExplorerTheme::GetBitmapInfosTheme(CThemeBitmapInfos &theme)
{
	theme = themeBitmapInfos;
}

//////////////////////////////////////////////////////////////////////////////////////////
//Saving Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CExplorerTheme::SaveTheme()
{
	doc.clear();
	// xml declaration
	xml_node<>* decl = doc.allocate_node(node_declaration);
	xml_attribute<>* ver = doc.allocate_attribute(stralloc("version"), stralloc("1.0"));
	xml_attribute<>* encoding = doc.allocate_attribute(stralloc("encoding"), stralloc("ISO-8859-1"));
	doc.append_attribute(ver);
	doc.append_attribute(encoding);
	doc.append_node(decl);

	xml_node<>* root = doc.allocate_node(node_element, stralloc("Theme"));
	doc.append_node(root);

	xml_node<> * section = node("Main");
	if (section != 0)
	{
		themeExplorerToolbar.SaveXML(doc, section);
		themeCentralWnd.SaveXML(doc, section);
		root->append_node(section);
	}


	section = node("InfosPreview");
	if (section != 0)
	{
		xml_node<> * sectionPreview = node("InfosPreviewSplitter");
		if (sectionPreview != 0)
		{
			themeImageListView.SaveXML(doc, sectionPreview);
			themePanePreviewPicture.SaveXML(doc, sectionPreview);
			section->append_node(sectionPreview);
		}

		sectionPreview = node("Preview");
		if (sectionPreview != 0)
		{
			xml_node<> * sectionChild = node("PanePreview");
			if (sectionChild != 0)
			{
				themePanePreview.SaveXML(doc, sectionChild);
				themeViewerToolbar.SaveXML(doc, sectionChild);
				themePreview.SaveXML(doc, sectionChild);
				themeBitmapInfos.SaveXML(doc, sectionChild);
				sectionPreview->append_node(sectionChild);
			}

			sectionChild = node("VideoPreview");
			if (sectionChild != 0)
			{
				themeVideoControl.SaveXML(doc, sectionChild);
				themeVideoToolbar.SaveXML(doc, sectionChild);
				themeSliderVideo.SaveXML(doc, sectionChild);
				sectionPreview->append_node(sectionChild);
			}

			sectionChild = node("BitmapPreview");
			if (sectionChild != 0)
			{
				themeBitmapWindow.SaveXML(doc, sectionChild);
				themeBitmapScrollbar.SaveXML(doc, sectionChild);
				themeBitmapToolbar.SaveXML(doc, sectionChild);
				sectionPreview->append_node(sectionChild);
			}

			section->append_node(sectionPreview);
		}

		sectionPreview = node("ClickInfos");
		if (sectionPreview != 0)
		{
			themeClickInfosToolbar.SaveXML(doc, sectionPreview);
			section->append_node(sectionPreview);
		}

		sectionPreview = node("Infos");
		if (sectionPreview != 0)
		{
			themePaneInfos.SaveXML(doc, sectionPreview);
			themeInfosToolbar.SaveXML(doc, sectionPreview);
			themeInfosFileTree.SaveXML(doc, sectionPreview);
			themeScrollInfosFile.SaveXML(doc, sectionPreview);
			section->append_node(sectionPreview);
		}

		sectionPreview = node("ClickToolbarPreview");
		if (sectionPreview != 0)
		{
			themeClickPreviewToolbar.SaveXML(doc, sectionPreview);
			section->append_node(sectionPreview);
		}


		root->append_node(section);
	}


	section = node("Thumbnail");
	if (section != 0)
	{
		themePaneThumbnail.SaveXML(doc, section);
		themeThumbnailToolbar.SaveXML(doc, section);
		themeThumbnail.SaveXML(doc, section);
		themeThumbnailScroll.SaveXML(doc, section);
		themeThumbnailToolbarZoom.SaveXML(doc, section);
		root->append_node(section);
	}

	section = node("CategoryFolder");
	if (section != 0)
	{
		xml_node<> * sectionCategory = node("CategoryFolderSplitter");
		if (sectionCategory != 0)
		{
			themeCategoryFolderSplitter.SaveXML(doc, sectionCategory);
			section->append_node(sectionCategory);
		}

		sectionCategory = node("Folder");
		if (sectionCategory != 0)
		{
			themePaneFolder.SaveXML(doc, sectionCategory);
			themeFolder.SaveXML(doc, sectionCategory);
			themeClickFolderToolbar.SaveXML(doc, sectionCategory);
			section->append_node(sectionCategory);
		}

		sectionCategory = node("Category");
		if (sectionCategory != 0)
		{
			themePaneCategory.SaveXML(doc, sectionCategory);
			themeCategoryTree.SaveXML(doc, sectionCategory);
			themeScrollCategory.SaveXML(doc, sectionCategory);
			section->append_node(sectionCategory);
		}
		root->append_node(section);
	}
	
	// save the xml data to a file (could equally well use any other ostream)
	std::ofstream file(filename.ToStdString());
	if (file.is_open())
	{
		file << doc;
		file.close();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//Loading Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CExplorerTheme::LoadTheme()
{
	xml_node<> * root_node;
	//long nodeSize = 0;
	root_node = doc.first_node("Theme");
	xml_node<> * child_node = root_node->first_node("Main");
	if (child_node != 0)
	{
		themeExplorerToolbar.LoadXML(child_node);
		themeCentralWnd.LoadXML(child_node);
	}
	else
	{
		InitExplorerToolbar();
		InitCentralWnd();
	}

	child_node = root_node->first_node("InfosPreview");
	if (child_node != 0)
	{
		xml_node<> * nodePreview = child_node->first_node("InfosPreviewSplitter");
		if (nodePreview != 0)
		{
			themeImageListView.LoadXML(nodePreview);
			themePanePreviewPicture.LoadXML(nodePreview);
		}
		else
		{
			InitPanePreviewPicture();
			InitListImageView();
		}

		nodePreview = child_node->first_node("Preview");
		if (nodePreview != 0)
		{
			xml_node<> * node = nodePreview->first_node("PanePreview");
			if (node != 0)
			{
				themePanePreview.LoadXML(node);
				themeViewerToolbar.LoadXML(node);
				themePreview.LoadXML(node);
				themeBitmapInfos.LoadXML(node);
			}
			else
			{
				InitBitmapInfos();
				InitViewerToolbar();
				InitPanePreview();
				InitPreviewSplitter();
			}

			node = nodePreview->first_node("VideoPreview");
			if (node != 0)
			{
				themeVideoControl.LoadXML(node);
				themeVideoToolbar.LoadXML(node);
				themeSliderVideo.LoadXML(node);
			}
			else
			{
				InitVideoControl();
				InitVideoToolbar();
				InitVideoSlider();
			}

			node = nodePreview->first_node("BitmapPreview");
			if (node != 0)
			{
				themeBitmapWindow.LoadXML(node);
				themeBitmapScrollbar.LoadXML(node);
				themeBitmapToolbar.LoadXML(node);
			}
			else
			{
				InitBitmapWindow();
				InitBitmapToolbar();
				InitBitmapScrollbar();
			}
		}
		else
		{
			InitViewerToolbar();
			InitPanePreview();
			InitPreviewSplitter();
			InitVideoControl();
			InitVideoToolbar();
			InitVideoSlider();
			InitBitmapWindow();
			InitBitmapToolbar();
			InitBitmapScrollbar();
		}

		nodePreview = child_node->first_node("ClickInfos");
		if (nodePreview != 0)
			themeClickInfosToolbar.LoadXML(nodePreview);
		else
			InitClickInfosToolbar();

		nodePreview = child_node->first_node("Infos");
		if (nodePreview != 0)
		{
			themePaneInfos.LoadXML(nodePreview);
			themeInfosToolbar.LoadXML(nodePreview);
			themeInfosFileTree.LoadXML(nodePreview);
			themeScrollInfosFile.LoadXML(nodePreview);
		}
		else
		{
			InitPaneInfos();
			InitInfosToolbar();
			InitInfosFileTree();
			InitScrollInfosFile();
		}

		nodePreview = child_node->first_node("ClickToolbarPreview");
		if (nodePreview != 0)
			themeClickPreviewToolbar.LoadXML(nodePreview);
		else
			InitClickPreviewToolbar();
	}
	else
	{
		InitPanePreviewPicture();
		InitListImageView();
		InitViewerToolbar();
		InitPanePreview();
		InitPreviewSplitter();
		InitVideoControl();
		InitVideoToolbar();
		InitVideoSlider();
		InitBitmapWindow();
		InitBitmapToolbar();
		InitBitmapScrollbar();
		InitClickInfosToolbar();
		InitPaneInfos();
		InitInfosToolbar();
		InitInfosFileTree();
		InitScrollInfosFile();
		InitClickPreviewToolbar();
	}

	child_node = root_node->first_node("Thumbnail");
	if (child_node != 0)
	{
		themePaneThumbnail.LoadXML(child_node);
		themeThumbnailToolbar.LoadXML(child_node);
		themeThumbnail.LoadXML(child_node);
		themeThumbnailScroll.LoadXML(child_node);
		themeThumbnailToolbarZoom.LoadXML(child_node);
	}
	else
	{
		InitPaneThumbnail();
		InitThumbnailToolbar();
		InitThumbnailTheme();
		InitThumbnailScrollTheme();
		InitThumbnailToolbarZoomTheme();
	}


	child_node = root_node->first_node("CategoryFolder");
	if (child_node != 0)
	{
		xml_node<> *nodePreview = child_node->first_node("CategoryFolderSplitter");
		if (nodePreview != 0)
			themeCategoryFolderSplitter.LoadXML(nodePreview);
		else
			InitCategoryFolderSplitter();
			
		nodePreview = child_node->first_node("Folder");
		if (nodePreview != 0)
		{
			themePaneFolder.LoadXML(nodePreview);
			themeFolder.LoadXML(nodePreview);
			themeClickFolderToolbar.LoadXML(nodePreview);
		}
		else
		{
			InitPaneFolder();
			InitFolderTheme();
			InitClickFolderToolbar();
		}

		nodePreview = child_node->first_node("Category");
		if (nodePreview != 0)
		{
			themePaneCategory.LoadXML(nodePreview);
			themeCategoryTree.LoadXML(nodePreview);
			themeScrollCategory.LoadXML(nodePreview);
		}
		else
		{
			InitCategoryTree();
			InitPaneCategory();
			InitScrollCategory();
		}
	}
	else
	{
		InitCategoryFolderSplitter();
		InitPaneFolder();
		InitFolderTheme();
		InitClickFolderToolbar();
		InitCategoryTree();
		InitPaneCategory();
		InitScrollCategory();
	}
}

void CExplorerTheme::InitTheme()
{
	InitExplorerToolbar();
	InitViewerToolbar();
	InitInfosToolbar();
	InitThumbnailToolbar();
	InitClickFolderToolbar();
	InitClickInfosToolbar();
	InitClickPreviewToolbar();

	InitCentralWnd();
	InitListImageView();
	InitPreviewSplitter();
	InitCategoryFolderSplitter();

	InitPanePreview();
	InitPaneInfos();
	InitPanePreviewPicture();
	InitPaneCategory();
	InitPaneFolder();
	InitPaneThumbnail();

	InitFolderTheme();
	InitThumbnailTheme();
	InitBitmapInfos();
	InitThumbnailToolbarZoomTheme();

	InitInfosFileTree();
	InitCategoryTree();

	InitScrollCategory();
	InitScrollInfosFile();
	InitThumbnailScrollTheme();

	InitVideoToolbar();
	InitVideoSlider();
	InitVideoControl();

	InitBitmapWindow();
	InitBitmapToolbar();
	InitBitmapScrollbar();
}