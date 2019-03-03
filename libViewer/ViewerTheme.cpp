#include <header.h>
#include "ViewerTheme.h"
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#ifndef __WXMSW__
#include <font_quality.h>
#endif
#include <ConvertUtility.h>
using namespace rapidxml;
using namespace Regards::Viewer;

CViewerTheme::CViewerTheme()
{

}

void CViewerTheme::InitCategoryFolderSplitter()
{
	themeCategoryFolderSplitter.themeFast.firstColor.Set(29, 29, 29);
	themeCategoryFolderSplitter.themeFast.secondColor.Set(128, 128, 128);
	themeCategoryFolderSplitter.themeSeparation.firstColor.Set(29, 29, 29);
	themeCategoryFolderSplitter.themeSeparation.secondColor.Set(128, 128, 128);
}

void CViewerTheme::InitThumbnailFaceThemeToolbar()
{
	themeThumbnailFaceToolbar.colorBack.Set(30, 30, 30);
	themeThumbnailFaceToolbar.colorTop.Set(29, 29, 29);
	themeThumbnailFaceToolbar.colorBottom.Set(29, 29, 29);
	themeThumbnailFaceToolbar.position = NAVIGATOR_CENTER;
	themeThumbnailFaceToolbar.SetWidth(30);
	themeThumbnailFaceToolbar.SetHeight(30);
	themeThumbnailFaceToolbar.SetMargeX(5);
	themeThumbnailFaceToolbar.SetMargeY(0);
	themeThumbnailFaceToolbar.isVertical = false;

	themeThumbnailFaceToolbar.button.SetTailleX(30);
	themeThumbnailFaceToolbar.button.SetTailleY(30);
	themeThumbnailFaceToolbar.button.lineColorTop.Set(222, 222, 222);
	themeThumbnailFaceToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeThumbnailFaceToolbar.button.actifTop.Set(255, 255, 255);
	themeThumbnailFaceToolbar.button.actifBottom.Set(223, 223, 223);
	themeThumbnailFaceToolbar.button.font.SetFontName("Verdana");
	themeThumbnailFaceToolbar.button.font.SetFontSize(10);
	themeThumbnailFaceToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailFaceToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailFaceToolbar.button.SetRectangleSize(4);
	themeThumbnailFaceToolbar.button.showButtonOnly = 1;

	themeThumbnailFaceToolbar.slider.isColorBackground = true;
	themeThumbnailFaceToolbar.slider.SetWidth(200);
	themeThumbnailFaceToolbar.slider.SetHeight(30);
	themeThumbnailFaceToolbar.slider.SetWidth(200);
	themeThumbnailFaceToolbar.slider.SetHeight(30);

	themeThumbnailFaceToolbar.texte.SetTailleX(120);
	themeThumbnailFaceToolbar.texte.SetTailleY(30);
	themeThumbnailFaceToolbar.texte.actifTop.Set(48, 48, 48);
	themeThumbnailFaceToolbar.texte.actifBottom.Set(128, 128, 128);
	themeThumbnailFaceToolbar.texte.rectTop.Set(222, 222, 222);
	themeThumbnailFaceToolbar.texte.rectBottom.Set(156, 156, 156);
	themeThumbnailFaceToolbar.texte.SetRectangleSize(0);
	themeThumbnailFaceToolbar.texte.font.SetFontName("Verdana");
	themeThumbnailFaceToolbar.texte.font.SetFontSize(10);
	themeThumbnailFaceToolbar.texte.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailFaceToolbar.texte.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailFaceToolbar.texte.fontActif.SetFontName("Verdana");
	themeThumbnailFaceToolbar.texte.fontActif.SetFontSize(10);
	themeThumbnailFaceToolbar.texte.fontActif.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailFaceToolbar.texte.fontActif.SetColorFont(wxColour(255, 255, 255));
}

void CViewerTheme::InitThumbnailToolbar()
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
	themeThumbnailToolbar.slider.SetWidth(200);
	themeThumbnailToolbar.slider.SetHeight(30);
	themeThumbnailToolbar.slider.SetWidth(200);
	themeThumbnailToolbar.slider.SetHeight(30);

	themeThumbnailToolbar.texte.SetTailleX(120);
	themeThumbnailToolbar.texte.SetTailleY(30);
	themeThumbnailToolbar.texte.actifTop.Set(48, 48, 48);
	themeThumbnailToolbar.texte.actifBottom.Set(128, 128, 128);
	themeThumbnailToolbar.texte.rectTop.Set(222, 222, 222);
	themeThumbnailToolbar.texte.rectBottom.Set(156, 156, 156);
	themeThumbnailToolbar.texte.SetRectangleSize(0);
	themeThumbnailToolbar.texte.font.SetFontName("Verdana");
	themeThumbnailToolbar.texte.font.SetFontSize(12);
	themeThumbnailToolbar.texte.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailToolbar.texte.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailToolbar.texte.fontActif.SetFontName("Verdana");
	themeThumbnailToolbar.texte.fontActif.SetFontSize(12);
	themeThumbnailToolbar.texte.fontActif.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailToolbar.texte.fontActif.SetColorFont(wxColour(255, 255, 255));

}

void CViewerTheme::GetCategoryFolderSplitterTheme(CThemeSplitter &theme)
{
	theme = themeCategoryFolderSplitter;
}


void CViewerTheme::GetThumbnailTheme(CThemeThumbnail &theme)
{
	theme = themeThumbnail;
}

void CViewerTheme::GetThumbnailFaceToolbarTheme(CThemeToolbar &theme)
{
	theme = themeThumbnailFaceToolbar;
}

void CViewerTheme::GetThumbnailToolbarTheme(CThemeToolbar &theme)
{
	theme = themeThumbnailToolbar;
}

void CViewerTheme::GetThumbnailToolbarZoomTheme(CThemeToolBarZoom &theme)
{
	theme = themeThumbnailToolbarZoom;
}

void CViewerTheme::GetThumbnailScrollTheme(CThemeScrollBar &theme)
{
	theme = themeScrollThumbnail;
}

void CViewerTheme::InitScrollThumbnail()
{
	themeScrollThumbnail.SetRectangleSize(10);
	themeScrollThumbnail.colorBack.Set(29, 29, 29);
	themeScrollThumbnail.colorTriangle.Set(74, 74, 74);
	themeScrollThumbnail.colorTriangleActif.Set(128, 128, 128);
	themeScrollThumbnail.colorBar.Set(74, 74, 74);
	themeScrollThumbnail.colorBarActif.Set(128, 128, 128);
	themeScrollThumbnail.SetMarge(2);
}


/*
void CViewerTheme::InitThumbnailTheme()
{
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
*/
void CViewerTheme::InitThumbnailToolbarZoomTheme()
{
	themeThumbnailToolbarZoom.fontActif.SetFontName("Verdana");
	themeThumbnailToolbarZoom.fontActif.SetFontSize(12);
	themeThumbnailToolbarZoom.fontActif.SetFontQuality(DEFAULT_QUALITY);
	themeThumbnailToolbarZoom.font.SetFontName("Verdana");
	themeThumbnailToolbarZoom.font.SetFontSize(12);
	themeThumbnailToolbarZoom.font.SetFontQuality(DEFAULT_QUALITY);
	themeThumbnailToolbarZoom.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailToolbarZoom.colorBack.Set(29, 29, 29);
	themeThumbnailToolbarZoom.fontActif.SetColorFont(wxColour(48, 128, 254));
	themeThumbnailToolbarZoom.SetHeight(30);
	themeThumbnailToolbarZoom.SetCheckboxHeight(20);
	themeThumbnailToolbarZoom.SetCheckboxWidth(20);
}

void CViewerTheme::InitClickFolderToolbar()
{
	themeClickFolderToolbar.colorBack.Set(30, 30, 30);
	themeClickFolderToolbar.colorTop.Set(29, 29, 29);
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

void CViewerTheme::InitFolderTheme()
{
	themeFolder.colorBack.Set(74, 74, 74);
	themeFolder.colorFont.Set(255, 255, 255);
}

void CViewerTheme::InitPaneFolder()
{
	themePaneFolder.SetHeight(20);
	themePaneFolder.themeTitle.SetCroixWidth(15);
	themePaneFolder.themeTitle.SetCroixHeight(15);
	themePaneFolder.themeTitle.SetHeight(30);
	themePaneFolder.themeTitle.SetMarge(5);
	themePaneFolder.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePaneFolder.themeTitle.font.SetFontSize(14);
}

void CViewerTheme::InitScrollCategory()
{
	themeScrollCategory.SetRectangleSize(10);
	themeScrollCategory.colorBack.Set(29, 29, 29);
	themeScrollCategory.colorTriangle.Set(74, 74, 74);
	themeScrollCategory.colorTriangleActif.Set(128, 128, 128);
	themeScrollCategory.colorBar.Set(74, 74, 74);
	themeScrollCategory.colorBarActif.Set(128, 128, 128);
	themeScrollCategory.SetMarge(2);
}

void CViewerTheme::InitCategoryTree()
{
	themeCategoryTree.SetMargeX(5);
	themeCategoryTree.bgColorOne.Set(29, 29, 29);
	themeCategoryTree.bgColorTwo.Set(128, 128, 128);
}

void CViewerTheme::InitPaneCategory()
{
	themePaneCategory.SetHeight(20);
	themePaneCategory.themeTitle.SetCroixWidth(15);
	themePaneCategory.themeTitle.SetCroixHeight(15);
	themePaneCategory.themeTitle.SetHeight(30);
	themePaneCategory.themeTitle.SetMarge(5);
	themePaneCategory.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePaneCategory.themeTitle.font.SetFontSize(14);
}


void CViewerTheme::GetCategoryScrollTheme(CThemeScrollBar &theme)
{
	theme = themeScrollCategory;
}

void CViewerTheme::GetCategoryTreeTheme(CThemeTree &theme)
{
	theme = themeCategoryTree;
}

void CViewerTheme::GetPaneCategory(CThemePane &theme)
{
	theme = themePaneCategory;
}


void CViewerTheme::GetBitmapInfosTheme(CThemeBitmapInfos * theme)
{
	*theme = themeBitmapInfos;
}

void CViewerTheme::GetFiltreScrollTheme(CThemeScrollBar * theme)
{
	*theme = themeScrollFiltre;
}

void CViewerTheme::GetScrollThumbnailTheme(CThemeScrollBar * theme)
{
	*theme = themeScrollThumbnail;
}

void CViewerTheme::GetScrollThumbnailVideoTheme(CThemeScrollBar * theme)
{
	*theme = themeScrollThumbnailVideo;
}

void CViewerTheme::GetScrollThumbnailEffectTheme(CThemeScrollBar * theme)
{
	*theme = themeScrollThumbnailEffect;
}

void CViewerTheme::GetScrollInfosFileTheme(CThemeScrollBar * theme)
{
	*theme = themeScrollInfosFile;
}

void CViewerTheme::GetScrollHistoryEffectTheme(CThemeScrollBar * theme)
{
	*theme = themeScrollHistoryEffect;
}

void CViewerTheme::GetScrollEffectVideoTheme(CThemeScrollBar * theme)
{
	*theme = themeScrollEffectVideo;
}

void CViewerTheme::GetThumbnailTheme(CThemeThumbnail * theme)
{
	*theme = themeThumbnail;
}

void CViewerTheme::GetThumbnailVideoTheme(CThemeThumbnail * theme)
{
	*theme = themeThumbnailVideo;
}

void CViewerTheme::InitBitmapInfos()
{
	themeBitmapInfos.themeFont.SetFontName("Verdana");
	themeBitmapInfos.themeFont.SetFontSize(16);
	themeBitmapInfos.themeFont.SetFontQuality(DEFAULT_QUALITY);
	themeBitmapInfos.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeBitmapInfos.colorBack.Set(74, 74, 74);
}

void CViewerTheme::InitInfosFileTree()
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
	themeInfosFileTree.themeSlide.font.SetFontSize(10);
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

void CViewerTheme::InitEffectVideoWndTree()
{
	themeEffectVideoWndTree.bgColorOne.Set(29, 29, 29);
	themeEffectVideoWndTree.bgColorActif.Set(255, 255, 255);
	themeEffectVideoWndTree.bgColorTwo.Set(74, 74, 74);
	themeEffectVideoWndTree.SetRowHeight(20);
	themeEffectVideoWndTree.SetMargeX(5);
	themeEffectVideoWndTree.themeCheckbox.SetWidth(15);
	themeEffectVideoWndTree.themeCheckbox.SetHeight(15);
	themeEffectVideoWndTree.themeSlide.SetHeight(20);
	themeEffectVideoWndTree.themeSlide.SetWidth(250);
	themeEffectVideoWndTree.themeSlide.font.SetColorFont(wxColour(255, 255, 255));
	themeEffectVideoWndTree.themeSlide.font.SetFontName("Verdana");
	themeEffectVideoWndTree.themeSlide.font.SetFontSize(10);
	//themeEffectVideoWndTree.themeSlide.SetMarge(2);
	themeEffectVideoWndTree.themeSlide.rectanglePast.Set(0, 0, 128);
	themeEffectVideoWndTree.themeSlide.rectangleNext.Set(48, 128, 254);
	themeEffectVideoWndTree.themeTexte.SetHeight(20);
	themeEffectVideoWndTree.themeTexte.SetWidth(20);
	themeEffectVideoWndTree.themeTexte.font.SetColorFont(wxColour(255, 255, 255));
	themeEffectVideoWndTree.themeTexte.font.SetFontName("Verdana");
	themeEffectVideoWndTree.themeTexte.font.SetFontSize(10);
	themeEffectVideoWndTree.themeTriangle.SetMarge(2);
	themeEffectVideoWndTree.themeTriangle.SetElementHeight(10);
	themeEffectVideoWndTree.themeTriangle.SetHeight(10);
	themeEffectVideoWndTree.themeTriangle.SetElementWidth(10);
	themeEffectVideoWndTree.themeTriangle.SetWidth(10);
	themeEffectVideoWndTree.themeTriangle.color.Set(0, 0, 0);
}

void CViewerTheme::InitHistoryEffectTree()
{
	themeHistoryEffectTree.bgColorOne.Set(29, 29, 29);
	themeHistoryEffectTree.bgColorActif.Set(255, 255, 255);
	themeHistoryEffectTree.bgColorTwo.Set(74, 74, 74);
	themeHistoryEffectTree.SetRowHeight(20);
	themeHistoryEffectTree.SetMargeX(5);
	themeHistoryEffectTree.themeCheckbox.SetWidth(15);
	themeHistoryEffectTree.themeCheckbox.SetHeight(15);
	themeHistoryEffectTree.themeSlide.SetHeight(20);
	themeHistoryEffectTree.themeSlide.SetWidth(250);
	themeHistoryEffectTree.themeSlide.font.SetColorFont(wxColour(255, 255, 255));
	themeHistoryEffectTree.themeSlide.font.SetFontName("Verdana");
	themeHistoryEffectTree.themeSlide.font.SetFontSize(14);
	//themeHistoryEffectTree.themeSlide.SetMarge(2);
	themeHistoryEffectTree.themeSlide.rectanglePast.Set(0, 0, 128);
	themeHistoryEffectTree.themeSlide.rectangleNext.Set(48, 128, 254);
	themeHistoryEffectTree.themeTexte.SetHeight(20);
	themeHistoryEffectTree.themeTexte.SetWidth(20);
	themeHistoryEffectTree.themeTexte.font.SetColorFont(wxColour(255, 255, 255));
	themeHistoryEffectTree.themeTexte.font.SetFontName("Verdana");
	themeHistoryEffectTree.themeTexte.font.SetFontSize(10);
	themeHistoryEffectTree.themeTriangle.SetMarge(2);
	themeHistoryEffectTree.themeTriangle.SetElementHeight(10);
	themeHistoryEffectTree.themeTriangle.SetHeight(10);
	themeHistoryEffectTree.themeTriangle.SetElementWidth(10);
	themeHistoryEffectTree.themeTriangle.SetWidth(10);
	themeHistoryEffectTree.themeTriangle.color.Set(0, 0, 0);
}

void CViewerTheme::InitThumbnail()
{
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
	//themeThumbnail.themeSeparation.colorBack.Set(45, 45, 48);
	themeThumbnail.themeSeparation.colorTop.Set(45, 45, 48);
	themeThumbnail.themeSeparation.colorBottom.Set(45, 45, 48);
	themeThumbnail.themeSeparation.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeThumbnail.themeSeparation.themeFont.SetFontName("Verdana");
	themeThumbnail.themeSeparation.themeFont.SetFontSize(10);
	themeThumbnail.themeSeparation.themeFont.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnail.themeSeparation.SetHeight(40);
}

void CViewerTheme::InitThumbnailVideo()
{
	themeThumbnailVideo.themeIcone.SetWidth(200);
	themeThumbnailVideo.themeIcone.SetHeight(200);
	themeThumbnailVideo.themeIcone.showOnlyThumbnail = false;
	themeThumbnailVideo.themeIcone.colorBack.Set(0, 0, 0);
	themeThumbnailVideo.themeIcone.colorTop.Set(74, 74, 74);
	themeThumbnailVideo.themeIcone.colorBottom.Set(74, 74, 74);
	themeThumbnailVideo.themeIcone.colorSelectTop.Set(205, 205, 205);
	themeThumbnailVideo.themeIcone.colorSelectBottom.Set(171, 171, 171);
	themeThumbnailVideo.colorBack.Set(74, 74, 74);
	themeThumbnailVideo.themeIcone.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailVideo.themeSeparation.colorBack.Set(74, 74, 74);
	//themeThumbnailVideo.themeSeparation.colorBack.Set(45, 45, 48);
	themeThumbnailVideo.themeSeparation.colorTop.Set(45, 45, 48);
	themeThumbnailVideo.themeSeparation.colorBottom.Set(45, 45, 48);
	themeThumbnailVideo.themeSeparation.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailVideo.themeSeparation.themeFont.SetFontName("Verdana");
	themeThumbnailVideo.themeSeparation.themeFont.SetFontSize(10);
	themeThumbnailVideo.themeSeparation.themeFont.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailVideo.themeSeparation.SetHeight(40);
}

void CViewerTheme::InitThumbnailViewerEffectTree()
{
	themeThumbnailViewerEffectTree.themeIcone.SetWidth(200);
	themeThumbnailViewerEffectTree.themeIcone.SetHeight(200);
	themeThumbnailViewerEffectTree.themeIcone.showOnlyThumbnail = false;
	themeThumbnailViewerEffectTree.themeIcone.colorBack.Set(0, 0, 0);
	themeThumbnailViewerEffectTree.themeIcone.colorTop.Set(74, 74, 74);
	themeThumbnailViewerEffectTree.themeIcone.colorBottom.Set(74, 74, 74);
	themeThumbnailViewerEffectTree.themeIcone.colorSelectTop.Set(205, 205, 205);
	themeThumbnailViewerEffectTree.themeIcone.colorSelectBottom.Set(171, 171, 171);
	themeThumbnailViewerEffectTree.colorBack.Set(74, 74, 74);
	themeThumbnailViewerEffectTree.themeIcone.font.SetColorFont(wxColour(255, 255, 255));
	//themeThumbnailViewerEffectTree.themeSeparation.colorBack.Set(74, 74, 74);
	themeThumbnailViewerEffectTree.themeSeparation.colorBack.Set(45, 45, 48);
	themeThumbnailViewerEffectTree.themeSeparation.colorTop.Set(45, 45, 48);
	themeThumbnailViewerEffectTree.themeSeparation.colorBottom.Set(45, 45, 48);
	themeThumbnailViewerEffectTree.themeSeparation.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailViewerEffectTree.themeSeparation.themeFont.SetFontName("Verdana");
	themeThumbnailViewerEffectTree.themeSeparation.themeFont.SetFontSize(10);
	themeThumbnailViewerEffectTree.themeSeparation.themeFont.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnailViewerEffectTree.themeSeparation.SetHeight(40);
}

void CViewerTheme::InitFiltreEffectTree()
{
	themeFiltreEffectTree.bgColorOne.Set(29, 29, 29);
	themeFiltreEffectTree.bgColorActif.Set(255, 255, 255);
	themeFiltreEffectTree.bgColorTwo.Set(74, 74, 74);
	themeFiltreEffectTree.SetRowHeight(20);
    themeFiltreEffectTree.SetMargeX(5);
	themeFiltreEffectTree.themeCheckbox.SetWidth(15);
	themeFiltreEffectTree.themeCheckbox.SetHeight(15);
	themeFiltreEffectTree.themeSlide.SetHeight(20);
	themeFiltreEffectTree.themeSlide.SetWidth(250);
	themeFiltreEffectTree.themeSlide.font.SetColorFont(wxColour(255, 255, 255));
	themeFiltreEffectTree.themeSlide.font.SetFontName("Verdana");
	themeFiltreEffectTree.themeSlide.font.SetFontSize(10);
	//themeFiltreEffectTree.themeSlide.SetMarge(2);
	themeFiltreEffectTree.themeSlide.rectanglePast.Set(0, 0, 128);
	themeFiltreEffectTree.themeSlide.rectangleNext.Set(48, 128, 254);
	themeFiltreEffectTree.themeTexte.SetHeight(20);
	themeFiltreEffectTree.themeTexte.SetWidth(20);
	themeFiltreEffectTree.themeTexte.font.SetColorFont(wxColour(255, 255, 255));
	themeFiltreEffectTree.themeTexte.font.SetFontName("Verdana");
	themeFiltreEffectTree.themeTexte.font.SetFontSize(10);
	themeFiltreEffectTree.themeTriangle.SetMarge(2);
	themeFiltreEffectTree.themeTriangle.SetElementHeight(10);
	themeFiltreEffectTree.themeTriangle.SetHeight(10);
	themeFiltreEffectTree.themeTriangle.SetElementWidth(10);
	themeFiltreEffectTree.themeTriangle.SetWidth(10);
	themeFiltreEffectTree.themeTriangle.color.Set(0, 0, 0);
}

void CViewerTheme::InitScrollThumbnailEffect()
{
	themeScrollThumbnailEffect.SetRectangleSize(10);
	themeScrollThumbnailEffect.colorBack.Set(29, 29, 29);
	themeScrollThumbnailEffect.colorTriangle.Set(74, 74, 74);
	themeScrollThumbnailEffect.colorTriangleActif.Set(128, 128, 128);
	themeScrollThumbnailEffect.colorBar.Set(74, 74, 74);
	themeScrollThumbnailEffect.colorBarActif.Set(128, 128, 128);
	themeScrollThumbnailEffect.SetMarge(2);
}

void CViewerTheme::InitScrollInfosFile()
{
	themeScrollInfosFile.SetRectangleSize(10);
	themeScrollInfosFile.colorBack.Set(29, 29, 29);
	themeScrollInfosFile.colorTriangle.Set(74, 74, 74);
	themeScrollInfosFile.colorTriangleActif.Set(128, 128, 128);
	themeScrollInfosFile.colorBar.Set(74, 74, 74);
	themeScrollInfosFile.colorBarActif.Set(128, 128, 128);
	themeScrollInfosFile.SetMarge(2);
}

void CViewerTheme::InitScrollHistoryEffect()
{
	themeScrollHistoryEffect.SetRectangleSize(10);
	themeScrollHistoryEffect.colorBack.Set(29, 29, 29);
	themeScrollHistoryEffect.colorTriangle.Set(74, 74, 74);
	themeScrollHistoryEffect.colorTriangleActif.Set(128, 128, 128);
	themeScrollHistoryEffect.colorBar.Set(74, 74, 74);
	themeScrollHistoryEffect.colorBarActif.Set(128, 128, 128);
	themeScrollHistoryEffect.SetMarge(2);
}

void CViewerTheme::InitScrollEffectVideo()
{
	themeScrollEffectVideo.SetRectangleSize(10);
	themeScrollEffectVideo.colorBack.Set(29, 29, 29);
	themeScrollEffectVideo.colorTriangle.Set(74, 74, 74);
	themeScrollEffectVideo.colorTriangleActif.Set(128, 128, 128);
	themeScrollEffectVideo.colorBar.Set(74, 74, 74);
	themeScrollEffectVideo.colorBarActif.Set(128, 128, 128);
	themeScrollEffectVideo.SetMarge(2);
}



void CViewerTheme::InitScrollThumbnailVideo()
{
	themeScrollThumbnailVideo.SetRectangleSize(10);
	themeScrollThumbnailVideo.colorBack.Set(29, 29, 29);
	themeScrollThumbnailVideo.colorTriangle.Set(74, 74, 74);
	themeScrollThumbnailVideo.colorTriangleActif.Set(128, 128, 128);
	themeScrollThumbnailVideo.colorBar.Set(74, 74, 74);
	themeScrollThumbnailVideo.colorBarActif.Set(128, 128, 128);
	themeScrollThumbnailVideo.SetMarge(2);
}

void CViewerTheme::InitScrollFiltre()
{
	themeScrollFiltre.SetRectangleSize(10);
	themeScrollFiltre.colorBack.Set(29, 29, 29);
	themeScrollFiltre.colorTriangle.Set(74, 74, 74);
	themeScrollFiltre.colorTriangleActif.Set(128, 128, 128);
	themeScrollFiltre.colorBar.Set(74, 74, 74);
	themeScrollFiltre.colorBarActif.Set(128, 128, 128);
	themeScrollFiltre.SetMarge(2);
}

void CViewerTheme::GetWndCentralSplitterTheme(CThemeSplitter * theme)
{
	*theme = themeCentralWndSplitter;
}

void CViewerTheme::InitPreviewInfosSplitter()
{
	themePreviewInfos.themeFast.firstColor.Set(29, 29, 29);
	themePreviewInfos.themeFast.secondColor.Set(128, 128, 128);
	themePreviewInfos.themeSeparation.firstColor.Set(29, 29, 29);
	themePreviewInfos.themeSeparation.secondColor.Set(128, 128, 128);
}

void CViewerTheme::InitCentralWndSplitter()
{
	themeCentralWndSplitter.themeFast.firstColor.Set(29, 29, 29);
	themeCentralWndSplitter.themeFast.secondColor.Set(128, 128, 128);
	themeCentralWndSplitter.themeSeparation.firstColor.Set(29, 29, 29);
	themeCentralWndSplitter.themeSeparation.secondColor.Set(128, 128, 128);
}

void CViewerTheme::InitThumbnailPane()
{
	//themeThumbnailPane.themeTitle.colorTop.Set(128, 128, 128);
	//themeThumbnailPane.themeTitle.colorBottom.Set(74, 74, 74);
	themeThumbnailPane.themeTitle.colorBack.Set(128, 128, 128);
	themeThumbnailPane.themeTitle.colorCross.Set(0, 0, 0);
	themeThumbnailPane.SetHeight(20);
	themeThumbnailPane.themeTitle.SetCroixWidth(15);
	themeThumbnailPane.themeTitle.SetCroixHeight(15);
	themeThumbnailPane.themeTitle.SetHeight(30);
	themeThumbnailPane.themeTitle.SetMarge(5);
	themeThumbnailPane.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailPane.themeTitle.font.SetFontSize(14);
}

void CViewerTheme::InitThumbnailVideoPane()
{
	themeThumbnailVideoPane.themeTitle.colorBack.Set(128, 128, 128);
	themeThumbnailVideoPane.themeTitle.colorCross.Set(0, 0, 0);
	//themeThumbnailVideoPane.themeTitle.colorTop.Set(128, 128, 128);
	//themeThumbnailVideoPane.themeTitle.colorBottom.Set(74, 74, 74);
	themeThumbnailVideoPane.SetHeight(20);
	themeThumbnailVideoPane.themeTitle.SetCroixWidth(15);
	themeThumbnailVideoPane.themeTitle.SetCroixHeight(15);
	themeThumbnailVideoPane.themeTitle.SetHeight(30);
	themeThumbnailVideoPane.themeTitle.SetMarge(5);
	themeThumbnailVideoPane.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnailVideoPane.themeTitle.font.SetFontSize(14);
}

void CViewerTheme::InitInfosPane()
{
	themeInfosPane.themeTitle.colorBack.Set(128, 128, 128);
	themeInfosPane.themeTitle.colorCross.Set(0, 0, 0);
	//themeInfosPane.themeTitle.colorTop.Set(128, 128, 128);
	//themeInfosPane.themeTitle.colorBottom.Set(74, 74, 74);
	themeInfosPane.SetHeight(20);
	themeInfosPane.themeTitle.SetCroixWidth(15);
	themeInfosPane.themeTitle.SetCroixHeight(15);
	themeInfosPane.themeTitle.SetHeight(30);
	themeInfosPane.themeTitle.SetMarge(5);
	themeInfosPane.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themeInfosPane.themeTitle.font.SetFontSize(14);
}

void CViewerTheme::InitPreviewPane()
{
	themePreviewPane.themeTitle.colorBack.Set(128, 128, 128);
	themePreviewPane.themeTitle.colorCross.Set(0, 0, 0);
	//themePreviewPane.themeTitle.colorTop.Set(128, 128, 128);
	//themePreviewPane.themeTitle.colorBottom.Set(74, 74, 74);
	themePreviewPane.SetHeight(20);
	themePreviewPane.themeTitle.SetCroixWidth(15);
	themePreviewPane.themeTitle.SetCroixHeight(15);
	themePreviewPane.themeTitle.SetHeight(30);
	themePreviewPane.themeTitle.SetMarge(5);
	themePreviewPane.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePreviewPane.themeTitle.font.SetFontSize(14);
}

void CViewerTheme::GetTreeInfosFileTheme(CThemeTree * theme)
{
	*theme = themeInfosFileTree;
}

void CViewerTheme::GetTreeEffectVideoWndTheme(CThemeTree * theme)
{
	*theme = themeEffectVideoWndTree;
}

void CViewerTheme::GetTreeHistoryTheme(CThemeTree * theme)
{
	*theme = themeHistoryEffectTree;
}

void CViewerTheme::GetTreeThumbnailViewerEffectTheme(CThemeThumbnail * theme)
{
	*theme = themeThumbnailViewerEffectTree;
}

void CViewerTheme::GetTreeFiltreEffectTheme(CThemeTree * theme)
{
	*theme = themeFiltreEffectTree;
}

void CViewerTheme::GetPreviewInfosSplitterTheme(CThemeSplitter * theme)
{
	*theme = themePreviewInfos;
}

void CViewerTheme::GetPreviewPaneTheme(CThemePane * theme)
{
	*theme = themePreviewPane;
}

void CViewerTheme::GetInfosPaneTheme(CThemePane * theme)
{
	*theme = themeInfosPane;
}

void CViewerTheme::GetThumbnailPaneTheme(CThemePane * theme)
{
	*theme = themeThumbnailPane;
}

void CViewerTheme::GetThumbnailVideoPaneTheme(CThemePane * theme)
{
	*theme = themeThumbnailVideoPane;
}


void CViewerTheme::GetMainToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeMainToolbar;
}

void CViewerTheme::GetInfosToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeInfosToolbar;
}

void CViewerTheme::GetClickInfosToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeClickInfosToolbar;
}

void CViewerTheme::GetClickThumbnailToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeClickThumbnailToolbar;
}

void CViewerTheme::GetClickThumbnailVideoToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeClickThumbnailVideoToolbar;
}

void CViewerTheme::GetFiltreToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeFiltreToolbar;
}

void CViewerTheme::GetPreviewToolbarTheme(CThemeToolbar * theme)
{
	*theme = themePreviewToolbar;
}

void CViewerTheme::InitFiltreToolbar()
{
	themeFiltreToolbar.colorBack.Set(30, 30, 30);
	themeFiltreToolbar.colorTop.Set(29, 29, 29);
	themeFiltreToolbar.colorBottom.Set(29, 29, 29);
	themeFiltreToolbar.position = NAVIGATOR_CENTER;
	themeFiltreToolbar.SetWidth(60);
	themeFiltreToolbar.SetHeight(40);
	themeFiltreToolbar.SetMargeX(5);
	themeFiltreToolbar.SetMargeY(0);
	themeFiltreToolbar.isVertical = false;

	themeFiltreToolbar.button.SetTailleX(30);
	themeFiltreToolbar.button.SetTailleY(40);
	themeFiltreToolbar.button.lineColorTop.Set(222, 222, 222);
	themeFiltreToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeFiltreToolbar.button.actifTop.Set(255, 255, 255);
	themeFiltreToolbar.button.actifBottom.Set(223, 223, 223);
	themeFiltreToolbar.button.font.SetFontName("Verdana");
	themeFiltreToolbar.button.font.SetFontSize(10);
	themeFiltreToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeFiltreToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeFiltreToolbar.button.SetRectangleSize(4);
	themeFiltreToolbar.button.showButtonOnly = 1;
}

void CViewerTheme::InitClickThumbnailVideoToolbar()
{
	themeClickThumbnailVideoToolbar.colorBack.Set(30, 30, 30);
	themeClickThumbnailVideoToolbar.colorTop.Set(29, 29, 29);
	themeClickThumbnailVideoToolbar.colorBottom.Set(29, 29, 29);
	themeClickThumbnailVideoToolbar.position = NAVIGATOR_CENTER;
	themeClickThumbnailVideoToolbar.SetWidth(60);
	themeClickThumbnailVideoToolbar.SetHeight(10);
	themeClickThumbnailVideoToolbar.SetMargeX(5);
	themeClickThumbnailVideoToolbar.SetMargeY(0);
	themeClickThumbnailVideoToolbar.isVertical = 0;

	themeClickThumbnailVideoToolbar.button.SetTailleX(60);
	themeClickThumbnailVideoToolbar.button.SetTailleY(10);
	themeClickThumbnailVideoToolbar.button.lineColorTop.Set(222, 222, 222);
	themeClickThumbnailVideoToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeClickThumbnailVideoToolbar.button.actifTop.Set(255, 255, 255);
	themeClickThumbnailVideoToolbar.button.actifBottom.Set(223, 223, 223);
	themeClickThumbnailVideoToolbar.button.font.SetFontName("Verdana");
	themeClickThumbnailVideoToolbar.button.font.SetFontSize(10);
	themeClickThumbnailVideoToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeClickThumbnailVideoToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeClickThumbnailVideoToolbar.button.SetRectangleSize(4);
	themeClickThumbnailVideoToolbar.button.showButtonOnly = 0;
}

void CViewerTheme::InitClickThumbnailToolbar()
{
	themeClickThumbnailToolbar.colorBack.Set(30, 30, 30);
	themeClickThumbnailToolbar.colorTop.Set(29, 29, 29);
	themeClickThumbnailToolbar.colorBottom.Set(29, 29, 29);
	themeClickThumbnailToolbar.position = NAVIGATOR_CENTER;
	themeClickThumbnailToolbar.SetWidth(60);
	themeClickThumbnailToolbar.SetHeight(10);
	themeClickThumbnailToolbar.SetMargeX(5);
	themeClickThumbnailToolbar.SetMargeY(0);
	themeClickThumbnailToolbar.isVertical = 0;

	themeClickThumbnailToolbar.button.SetTailleX(60);
	themeClickThumbnailToolbar.button.SetTailleY(10);
	themeClickThumbnailToolbar.button.lineColorTop.Set(222, 222, 222);
	themeClickThumbnailToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeClickThumbnailToolbar.button.actifTop.Set(255, 255, 255);
	themeClickThumbnailToolbar.button.actifBottom.Set(223, 223, 223);
	themeClickThumbnailToolbar.button.font.SetFontName("Verdana");
	themeClickThumbnailToolbar.button.font.SetFontSize(10);
	themeClickThumbnailToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeClickThumbnailToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeClickThumbnailToolbar.button.SetRectangleSize(4);
	themeClickThumbnailToolbar.button.showButtonOnly = 0;
}

void CViewerTheme::InitViewerToolbar()
{
	themeMainToolbar.colorBack.Set(30, 30, 30);
	themeMainToolbar.colorTop.Set(29, 29, 29);
	themeMainToolbar.colorBottom.Set(29, 29, 29);
	themeMainToolbar.position = NAVIGATOR_LEFT;
	themeMainToolbar.SetWidth(60);
	themeMainToolbar.SetHeight(60);
	themeMainToolbar.SetMargeX(5);
	themeMainToolbar.SetMargeY(0);
	themeMainToolbar.isVertical = false;

	themeMainToolbar.button.SetTailleX(80);
	themeMainToolbar.button.SetTailleY(60);
	themeMainToolbar.button.lineColorTop.Set(222, 222, 222);
	themeMainToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeMainToolbar.button.actifTop.Set(255, 255, 255);
	themeMainToolbar.button.actifBottom.Set(223, 223, 223);
	themeMainToolbar.button.font.SetFontName("Verdana");
	themeMainToolbar.button.font.SetFontSize(8);
	themeMainToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeMainToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeMainToolbar.button.SetRectangleSize(2);
	themeMainToolbar.button.showButtonOnly = 0;
}

void CViewerTheme::InitInfosToolbar()
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

	themeInfosToolbar.texte.SetTailleX(120);
	themeInfosToolbar.texte.SetTailleY(30);
	themeInfosToolbar.texte.actifTop.Set(48, 48, 48);
	themeInfosToolbar.texte.actifBottom.Set(128, 128, 128);
	themeInfosToolbar.texte.rectTop.Set(222, 222, 222);
	themeInfosToolbar.texte.rectBottom.Set(156, 156, 156);
	themeInfosToolbar.texte.SetRectangleSize(2);
	themeInfosToolbar.texte.font.SetFontName("Verdana");
	themeInfosToolbar.texte.font.SetFontSize(12);
	themeInfosToolbar.texte.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeInfosToolbar.texte.font.SetColorFont(wxColour(255, 255, 255));
	themeInfosToolbar.texte.fontActif.SetFontName("Verdana");
	themeInfosToolbar.texte.fontActif.SetFontSize(12);
	themeInfosToolbar.texte.fontActif.SetFontQuality(ANTIALIASED_QUALITY);
	themeInfosToolbar.texte.fontActif.SetColorFont(wxColour(255, 255, 255));
}

void CViewerTheme::InitClickInfosToolbar()
{
	themeClickInfosToolbar.colorBack.Set(30, 30, 30);
	themeClickInfosToolbar.colorTop.Set(29, 29, 29);
	themeClickInfosToolbar.colorBottom.Set(29, 29, 29);
	themeClickInfosToolbar.position = NAVIGATOR_CENTER;
	themeClickInfosToolbar.SetWidth(10);
	themeClickInfosToolbar.SetHeight(60);
	themeClickInfosToolbar.SetMargeX(0);
	themeClickInfosToolbar.SetMargeY(5);
	themeClickInfosToolbar.isVertical = 1;

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

void CViewerTheme::InitPreviewToolbar()
{
	themePreviewToolbar.colorBack.Set(30, 30, 30);
	themePreviewToolbar.colorTop.Set(29, 29, 29);
	themePreviewToolbar.colorBottom.Set(29, 29, 29);
	themePreviewToolbar.position = NAVIGATOR_CENTER;
	themePreviewToolbar.SetWidth(60);
	themePreviewToolbar.SetHeight(60);
	themePreviewToolbar.SetMargeX(5);
	themePreviewToolbar.SetMargeY(0);
	themePreviewToolbar.isVertical = false;

	themePreviewToolbar.button.SetTailleX(60);
	themePreviewToolbar.button.SetTailleY(60);
	themePreviewToolbar.button.lineColorTop.Set(222, 222, 222);
	themePreviewToolbar.button.lineColorBottom.Set(156, 156, 156);
	themePreviewToolbar.button.actifTop.Set(255, 255, 255);
	themePreviewToolbar.button.actifBottom.Set(223, 223, 223);
	themePreviewToolbar.button.font.SetFontName("Verdana");
	themePreviewToolbar.button.font.SetFontSize(8);
	themePreviewToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themePreviewToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themePreviewToolbar.button.SetRectangleSize(2);
	themePreviewToolbar.button.showButtonOnly = 0;
}

CViewerTheme::~CViewerTheme()
{
	doc.clear();
}


//////////////////////////////////////////////////////////////////////////////////////////
//Saving Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CViewerTheme::SaveTheme()
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

	//xml_node<> * root_node;
	//long nodeSize = 0;
	//root_node = doc.first_node("Theme");

	xml_node<>* section = node("Main");
	if (section != 0)
	{
        xml_node<>* local_section = node("MainToolbar");
        if (local_section != 0)
        {
            themeMainToolbar.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("CentralWndSplitter");
        if (section != 0)
        {
            themeCentralWndSplitter.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
		
        root->append_node(section);
	}
	

	section = node("VideoPreview");
	if (section != 0)
	{
        xml_node<>* local_section = node("VideoControl");
        if (local_section != 0)
        {
            themeVideoControl.SaveXML(doc, local_section);
            section->append_node(local_section);
        }   
        
            
        local_section = node("VideoToolbar");
        if (section != 0)   
        {         
            themeVideoToolbar.SaveXML(doc, local_section);
            section->append_node(local_section);
        }   
        
        local_section = node("SliderVideo");
        if (section != 0)
        {               
            themeSliderVideo.SaveXML(doc, local_section);
            section->append_node(local_section);
        }  
		root->append_node(section);
	}
	

	section = node("BitmapPreview");
	if (section != 0)
	{
        xml_node<>* local_section = node("BitmapWindow");
        if (local_section != 0)
        {             
            themeBitmapWindow.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("BitmapScrollbar");
        if (local_section != 0)
        {             
            themeBitmapScrollbar.SaveXML(doc, local_section);
            section->append_node(local_section);
        }

        local_section = node("BitmapToolbar");
        if (local_section != 0)
        {             
            themeBitmapToolbar.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
		root->append_node(section);
	}
	

	section = node("PreviewWindow");
	if (section != 0)
	{
        xml_node<>* local_section = node("PreviewInfos");
        if (local_section != 0)
        {           
            themePreviewInfos.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("PreviewPane");
        if (local_section != 0)
        {               
            themePreviewPane.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("PreviewToolbar");
        if (local_section != 0)
        {               
            themePreviewToolbar.SaveXML(doc, local_section);
            section->append_node(local_section);
        }        
		
        local_section = node("BitmapInfos");
        if (local_section != 0)
        {               
            themeBitmapInfos.SaveXML(doc, local_section);
            section->append_node(local_section);
        }          
		
		root->append_node(section);
	}
	

	section = node("Thumbnail");
	if (section != 0)
	{
        xml_node<>* local_section = node("ThumbnailPane");
        if (local_section != 0)
        {
            themeThumbnailPane.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("Thumbnail");
        if (local_section != 0)
        {         
            themeThumbnail.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("ClickThumbnailToolbar");
        if (local_section != 0)
        {         
            themeClickThumbnailToolbar.SaveXML(doc, local_section);
            section->append_node(local_section);
        }        
        
        local_section = node("ScrollThumbnail");
        if (local_section != 0)
        {         
            themeScrollThumbnail.SaveXML(doc, local_section);
            section->append_node(local_section);
        } 

        local_section = node("ThumbnailToolbarZoom");
        if (local_section != 0)
        {  
            themeThumbnailToolbarZoom.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("ThumbnailToolbar");
        if (local_section != 0)
        {         
            themeThumbnailToolbar.SaveXML(doc, local_section);  
            section->append_node(local_section);   
        }
		root->append_node(section);
	}
	

	section = node("ThumbnailVideo");
	if (section != 0)
	{
        xml_node<>* local_section = node("ThumbnailVideoPane");
        if (local_section != 0)
        {
            themeThumbnailVideoPane.SaveXML(doc, local_section);
            section->append_node(local_section);   
        }
        
        local_section = node("ThumbnailVideo");
        if (local_section != 0)
        {        
            themeThumbnailVideo.SaveXML(doc, local_section);
            section->append_node(local_section);   
        }
        
        local_section = node("ClickThumbnailVideoToolbar");
        if (local_section != 0)
        {              
            themeClickThumbnailVideoToolbar.SaveXML(doc, local_section);
            section->append_node(local_section);   
        }
        
        local_section = node("ScrollThumbnailVideo");
        if (local_section != 0)
        {              
            themeScrollThumbnailVideo.SaveXML(doc, local_section);
            section->append_node(local_section);   
        }
		root->append_node(section);
	}
	

	section = node("InfoVideoPicture");
	if (section != 0)
	{
        xml_node<>* local_section = node("InfosToolbar");
        if (local_section != 0)
        {		
            themeInfosToolbar.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
        local_section = node("InfosPane");
        if (local_section != 0)
        {        
            themeInfosPane.SaveXML(doc, local_section);
            section->append_node(local_section);
        }
        
		xml_node<>* otherchild = node("Infos");
		if (otherchild != 0)
		{
            xml_node<>* local_section = node("ClickInfosToolbar");
            if (local_section != 0)
            {                   
                themeClickInfosToolbar.SaveXML(doc, local_section);
                otherchild->append_node(local_section);
            }
            
            local_section = node("InfosFileTree");
            if (local_section != 0)
            {                   
                themeInfosFileTree.SaveXML(doc, local_section);
                otherchild->append_node(local_section);
            }            
            
            local_section = node("ScrollInfosFile");
            if (local_section != 0)
            {  			
                themeScrollInfosFile.SaveXML(doc, local_section);
                otherchild->append_node(local_section);
            }
            
			section->append_node(otherchild);
		}
		
		otherchild = node("Filtre");
		if (otherchild != 0)
		{
            xml_node<>* local_section = node("FiltreToolbar");
            if (local_section != 0)
            {            
                themeFiltreToolbar.SaveXML(doc, local_section);
                otherchild->append_node(local_section);
            }
            
            local_section = node("FiltreEffectTree");
            if (local_section != 0)
            {            
                themeFiltreEffectTree.SaveXML(doc, local_section);
                otherchild->append_node(local_section);
            }

            local_section = node("ScrollFiltre");
            if (local_section != 0)
            {            
                themeScrollFiltre.SaveXML(doc, local_section);
                otherchild->append_node(local_section);
            }
            
			section->append_node(otherchild);
		}

		otherchild = node("History");
		if (otherchild != 0)
		{
            xml_node<>* local_section = node("ScrollHistoryEffect");
            if (local_section != 0)
            {              
                themeScrollHistoryEffect.SaveXML(doc, otherchild);
                otherchild->append_node(local_section);
            }
            
            local_section = node("HistoryEffectTree");
            if (local_section != 0)
            {               
                themeHistoryEffectTree.SaveXML(doc, otherchild);
                otherchild->append_node(local_section);
            }
            
			section->append_node(otherchild);
		}
		

		otherchild = node("ThumbnailFiltre");
		if (otherchild != 0)
		{
            xml_node<>* local_section = node("ScrollThumbnailEffect");
            if (local_section != 0)
            {              
                themeScrollThumbnailEffect.SaveXML(doc, otherchild);
                otherchild->append_node(local_section);
            }            

            local_section = node("ThumbnailViewerEffectTree");
            if (local_section != 0)
            {              
                themeThumbnailViewerEffectTree.SaveXML(doc, otherchild);
                otherchild->append_node(local_section);
            }      
			section->append_node(otherchild);
		}
		

		otherchild = node("EffectVideo");
		if (otherchild != 0)
		{
            xml_node<>* local_section = node("ScrollEffectVideo");
            if (local_section != 0)
            {              
                themeScrollEffectVideo.SaveXML(doc, otherchild);
                otherchild->append_node(local_section);
            }            

            local_section = node("EffectVideoWndTree");
            if (local_section != 0)
            {              
                themeEffectVideoWndTree.SaveXML(doc, otherchild);
                otherchild->append_node(local_section);
            } 

			section->append_node(otherchild);
		}
		
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
            xml_node<> * local_section = node("PaneFolder");
            if (local_section != 0)
            {              
                themePaneFolder.SaveXML(doc, local_section);
                sectionCategory->append_node(local_section);
            }   

            local_section = node("Folder");
            if (local_section != 0)
            {              
                themeFolder.SaveXML(doc, local_section);
                sectionCategory->append_node(local_section);
            }             

            local_section = node("ClickFolderToolbar");
            if (local_section != 0)
            {              
                themeClickFolderToolbar.SaveXML(doc, local_section);
                sectionCategory->append_node(local_section);
            } 

			section->append_node(sectionCategory);
		}

		sectionCategory = node("Category");
		if (sectionCategory != 0)
		{
            xml_node<> * local_section = node("PaneCategory");
            if (local_section != 0)
            {              
                themePaneCategory.SaveXML(doc, local_section);
                sectionCategory->append_node(local_section);
            }             
            
            local_section = node("CategoryTree");
            if (local_section != 0)
            {              
                themeCategoryTree.SaveXML(doc, local_section);
                sectionCategory->append_node(local_section);
            }             
            
            local_section = node("ScrollCategory");
            if (local_section != 0)
            {              
                themeScrollCategory.SaveXML(doc, local_section);
                sectionCategory->append_node(local_section);
            }              
			section->append_node(sectionCategory);
		}
		root->append_node(section);
	}

	

	// save the xml data to a file (could equally well use any other ostream)
	std::ofstream file(CConvertUtility::ConvertToStdString(filename));
	if (file.is_open())
	{
		file << doc;
		file.close();
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////
//Loading Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CViewerTheme::LoadTheme()
{
	xml_node<> * root_node;
	//long nodeSize = 0;
	root_node = doc.first_node("Theme");

	xml_node<> * child_node = root_node->first_node("Main");
	if (child_node != 0)
	{
        xml_node<> * nodePreview = child_node->first_node("MainToolbar");
        if (nodePreview != 0)        
            themeMainToolbar.LoadXML(nodePreview);
        else
            InitViewerToolbar();
            
        nodePreview = child_node->first_node("CentralWndSplitter");
        if (nodePreview != 0)            
            themeCentralWndSplitter.LoadXML(nodePreview);
        else
            InitCentralWndSplitter();
	}
	else
	{
		InitViewerToolbar();
		InitCentralWndSplitter();
	}

	child_node = root_node->first_node("VideoPreview");
	if (child_node != 0)
	{
        xml_node<> * nodePreview = child_node->first_node("VideoControl");
        if (nodePreview != 0)           
            themeVideoControl.LoadXML(nodePreview);
        else
            InitVideoControl();
        
        nodePreview = child_node->first_node("VideoToolbar");
        if (nodePreview != 0)      
            themeVideoToolbar.LoadXML(nodePreview);
        else
            InitVideoToolbar();
            
        nodePreview = child_node->first_node("SliderVideo");
        if (nodePreview != 0)             
            themeSliderVideo.LoadXML(nodePreview);
        else
            InitVideoSlider();
	}
	else
	{
		InitVideoControl();
		InitVideoToolbar();
		InitVideoSlider();
	}

	child_node = root_node->first_node("BitmapPreview");
	if (child_node != 0)
	{
        xml_node<> * nodePreview = child_node->first_node("BitmapWindow");
        if (nodePreview != 0)                  
            themeBitmapWindow.LoadXML(nodePreview);
        else
            InitBitmapWindow();
            
        nodePreview = child_node->first_node("BitmapScrollbar");
        if (nodePreview != 0)          
            themeBitmapScrollbar.LoadXML(nodePreview);
        else
            InitBitmapScrollbar();
            
        nodePreview = child_node->first_node("BitmapToolbar");
        if (nodePreview != 0)
            themeBitmapToolbar.LoadXML(nodePreview);
        else
            InitBitmapToolbar();
	}
	else
	{
		InitBitmapWindow();
		InitBitmapToolbar();
		InitBitmapScrollbar();
	}

	child_node = root_node->first_node("PreviewWindow");
	if (child_node != 0)
	{
        xml_node<> * nodePreview = child_node->first_node("PreviewInfos");
        if (nodePreview != 0)          
            themePreviewInfos.LoadXML(nodePreview);
        else
            InitPreviewInfosSplitter();
            
        nodePreview = child_node->first_node("PreviewPane");
        if (nodePreview != 0) 
            themePreviewPane.LoadXML(nodePreview);
        else
            InitPreviewPane();
           
        nodePreview = child_node->first_node("PreviewToolbar");
        if (nodePreview != 0)
            themePreviewToolbar.LoadXML(nodePreview);	
        else
            InitPreviewToolbar();
        
        nodePreview = child_node->first_node("BitmapInfos");
        if (nodePreview != 0)
            themeBitmapInfos.LoadXML(nodePreview);
        else
            InitBitmapInfos();
	}
	else
	{
		InitBitmapInfos();
		InitPreviewPane();
		InitPreviewToolbar();
		InitPreviewInfosSplitter();
	}

	child_node = root_node->first_node("Thumbnail");
	if (child_node != 0)
	{
        xml_node<> * nodePreview = child_node->first_node("ThumbnailPane");
        if (nodePreview != 0)        
            themeThumbnailPane.LoadXML(nodePreview);
        else
            InitThumbnailPane();
            
        nodePreview = child_node->first_node("Thumbnail");
        if (nodePreview != 0)
            themeThumbnail.LoadXML(nodePreview);
        else
            InitThumbnail();
            
        nodePreview = child_node->first_node("ClickThumbnailToolbar");
        if (nodePreview != 0)
            themeClickThumbnailToolbar.LoadXML(nodePreview);
        else
            InitClickThumbnailToolbar();
            
        nodePreview = child_node->first_node("ScrollThumbnail");
        if (nodePreview != 0)            
            themeScrollThumbnail.LoadXML(nodePreview);
        else
            InitScrollThumbnail();
                       
        nodePreview = child_node->first_node("ThumbnailToolbarZoom");
        if (nodePreview != 0)
            themeThumbnailToolbarZoom.LoadXML(nodePreview);
        else
            InitThumbnailToolbarZoomTheme();
            
        nodePreview = child_node->first_node("ThumbnailToolbar");
        if (nodePreview != 0)            
            themeThumbnailToolbar.LoadXML(nodePreview);
        else
            InitThumbnailToolbar();
	}
	else
	{
		InitThumbnailPane();
		InitThumbnail();
		InitClickThumbnailToolbar();
		InitScrollThumbnail();
		InitThumbnailToolbarZoomTheme();
		InitThumbnailToolbar();
	}

	child_node = root_node->first_node("ThumbnailVideo");
	if (child_node != 0)
	{
        xml_node<> * nodePreview = child_node->first_node("ThumbnailVideoPane");
        if (nodePreview != 0)         
            themeThumbnailVideoPane.LoadXML(nodePreview);
        else
            InitThumbnailVideoPane();
            
        nodePreview = child_node->first_node("ThumbnailVideo");
        if (nodePreview != 0)             
            themeThumbnailVideo.LoadXML(nodePreview);
        else
            InitThumbnailVideo();
            
        nodePreview = child_node->first_node("ClickThumbnailVideoToolbar");
        if (nodePreview != 0)             
            themeClickThumbnailVideoToolbar.LoadXML(nodePreview);
        else
            InitClickThumbnailVideoToolbar();
            
        nodePreview = child_node->first_node("ScrollThumbnailVideo");
        if (nodePreview != 0)             
            themeScrollThumbnailVideo.LoadXML(nodePreview);
        else
            InitScrollThumbnail();
	}
	else
	{
		InitThumbnailVideoPane();
		InitThumbnailVideo();
		InitClickThumbnailVideoToolbar();	
		InitScrollThumbnail();
	}

	child_node = root_node->first_node("InfoVideoPicture");
	if (child_node != 0)
	{
        xml_node<> * nodePreview = child_node->first_node("InfosToolbar");
        if (nodePreview != 0)       		
            themeInfosToolbar.LoadXML(nodePreview);           
            
        nodePreview = child_node->first_node("InfosPane");
        if (nodePreview != 0)       		
            themeInfosPane.LoadXML(nodePreview);
		
		xml_node<> * otherchild = child_node->first_node("Infos");
		if (otherchild != 0)
		{
            xml_node<> * nodePreview = child_node->first_node("ClickInfosToolbar");
            if (nodePreview != 0)                   
                themeClickInfosToolbar.LoadXML(nodePreview);
            else
                InitClickInfosToolbar();
            
            nodePreview = child_node->first_node("InfosFileTree");
            if (nodePreview != 0)                   
                themeInfosFileTree.LoadXML(nodePreview);
            else
                InitInfosFileTree(); 

            nodePreview = child_node->first_node("ScrollInfosFile");
            if (nodePreview != 0)                   
                themeScrollInfosFile.LoadXML(nodePreview);
            else
                InitScrollInfosFile();
		}
		else
		{
            InitClickInfosToolbar();
            InitInfosFileTree();
			InitScrollInfosFile();
			
		}

		otherchild = child_node->first_node("Filtre");
		if (otherchild != 0)
		{
            xml_node<> * nodePreview = child_node->first_node("FiltreToolbar");
            if (nodePreview != 0)             
                themeFiltreToolbar.LoadXML(nodePreview);
            else
                InitFiltreToolbar();
              
            nodePreview = child_node->first_node("InfosFileTree");
            if (nodePreview != 0)
                themeFiltreEffectTree.LoadXML(nodePreview);
            else
                InitFiltreEffectTree();
                
            nodePreview = child_node->first_node("ScrollInfosFile");
            if (nodePreview != 0)
                themeScrollFiltre.LoadXML(nodePreview);
            else
                InitScrollFiltre();
		}
		else
		{
			InitFiltreToolbar();
			InitScrollFiltre();
			InitFiltreEffectTree();
		}

		otherchild = child_node->first_node("History");
		if (otherchild != 0)
		{
            xml_node<> * nodePreview = child_node->first_node("ScrollHistoryEffect");
            if (nodePreview != 0)             
                themeScrollHistoryEffect.LoadXML(nodePreview);
            else
                InitScrollHistoryEffect();
               
            nodePreview = child_node->first_node("HistoryEffectTree");
            if (nodePreview != 0)
                themeHistoryEffectTree.LoadXML(nodePreview);
            else
                InitHistoryEffectTree();
		}
		else
		{
			InitScrollHistoryEffect();
			InitHistoryEffectTree();
		}


		otherchild = child_node->first_node("ThumbnailFiltre");
		if (otherchild != 0)
		{
            xml_node<> * nodePreview = child_node->first_node("ScrollThumbnailEffect");
            if (nodePreview != 0)               
                themeScrollThumbnailEffect.LoadXML(nodePreview);
            else
                InitScrollThumbnailEffect();
              
            nodePreview = child_node->first_node("ThumbnailViewerEffectTree");
            if (nodePreview != 0) 
                themeThumbnailViewerEffectTree.LoadXML(nodePreview);
            else
                InitThumbnailViewerEffectTree();
		}
		else
		{
			InitScrollThumbnailEffect();
			InitThumbnailViewerEffectTree();
		}

		otherchild = child_node->first_node("EffectVideo");
		if (otherchild != 0)
		{
            xml_node<> * nodePreview = child_node->first_node("ScrollEffectVideo");
            if (nodePreview != 0)            
                themeScrollEffectVideo.LoadXML(nodePreview);
            else
                InitScrollEffectVideo();
                
            nodePreview = child_node->first_node("EffectVideoWndTree");
            if (nodePreview != 0)                
                themeEffectVideoWndTree.LoadXML(nodePreview);
            else
                InitEffectVideoWndTree();
		}
		else
		{
			InitScrollEffectVideo();
			InitEffectVideoWndTree();
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
                xml_node<> * node_local = child_node->first_node("PaneFolder");
                if (node_local != 0)                 
                    themePaneFolder.LoadXML(node_local);
                else
                    InitPaneFolder();
                  
                node_local = child_node->first_node("Folder");
                if (node_local != 0)
                    themeFolder.LoadXML(node_local);
                else
                    InitFolderTheme();
                
                node_local = child_node->first_node("ClickFolderToolbar");
                if (node_local != 0)
                    themeClickFolderToolbar.LoadXML(node_local);
                else
                    InitClickFolderToolbar();
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
                xml_node<> * node_local = nodePreview->first_node("PaneCategory");
                if (node_local != 0)                 
                    themePaneCategory.LoadXML(node_local);
                else
                    InitPaneCategory();
                    
                node_local = nodePreview->first_node("CategoryTree");
                if (node_local != 0)                       
                    themeCategoryTree.LoadXML(node_local);
                else
                    InitCategoryTree();
                    
                node_local = nodePreview->first_node("ScrollCategory");
                if (node_local != 0)                    
                    themeScrollCategory.LoadXML(node_local);
                else
                    InitScrollCategory();
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
	else
	{
		InitInfosPane();
		InitInfosToolbar();
		InitScrollFiltre();
		InitFiltreToolbar();
		InitScrollThumbnailEffect();
		InitScrollInfosFile();
		InitScrollHistoryEffect();
		InitScrollEffectVideo();
		InitInfosFileTree();
		InitEffectVideoWndTree();
		InitHistoryEffectTree();
		InitThumbnailViewerEffectTree();
		InitFiltreEffectTree();
		InitThumbnail();
		InitThumbnailVideo();
		InitBitmapInfos();

		InitCategoryFolderSplitter();
		InitPaneFolder();
		InitFolderTheme();
		InitClickFolderToolbar();
		InitCategoryTree();
		InitPaneCategory();
		InitScrollCategory();
	}

}

void CViewerTheme::InitTheme()
{
	InitCentralWndSplitter();

	InitFiltreToolbar();

	InitViewerToolbar();
	
	InitVideoControl();
	InitVideoToolbar();
	InitVideoSlider();
	
	InitBitmapWindow();
	InitBitmapToolbar();
	InitBitmapScrollbar();
		
	InitClickThumbnailToolbar();
	InitThumbnailPane();
	InitScrollThumbnail();

	InitClickThumbnailVideoToolbar();
	InitThumbnailVideoPane();
	InitScrollThumbnailVideo();

	InitPreviewPane();
	InitPreviewToolbar();
	InitPreviewInfosSplitter();

	InitInfosPane();
	InitInfosToolbar();

	InitScrollFiltre();
	InitScrollThumbnailEffect();
	InitScrollInfosFile();
	InitScrollHistoryEffect();
	InitScrollEffectVideo();
	
	InitInfosFileTree();
	InitEffectVideoWndTree();
	InitHistoryEffectTree();
	InitThumbnailViewerEffectTree();
	InitFiltreEffectTree();

	InitThumbnail();
	InitThumbnailVideo();


	InitThumbnailToolbarZoomTheme();
	InitThumbnailToolbar();
}