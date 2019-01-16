#include "IntroTheme.h"
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
using namespace rapidxml;
using namespace Regards::Introduction;

CIntroTheme::CIntroTheme()
{
	InitAboutScrollbar();
	InitAboutTree();
}

void CIntroTheme::GetAboutTreeControlTheme(CThemeTree * theme)
{
	*theme = themeAbout;
}

void CIntroTheme::GetAboutTexteTheme(CThemeIntroLogo * theme)
{
	*theme = themeIntroLogo;
}

void CIntroTheme::GetScrollAboutTheme(CThemeScrollBar * theme)
{
	*theme = themeAboutScroll;
}

void CIntroTheme::InitAboutScrollbar()
{
	themeAboutScroll.SetRectangleSize(10);
	themeAboutScroll.colorBack.Set(29, 29, 29);
	themeAboutScroll.colorTriangle.Set(74, 74, 74);
	themeAboutScroll.colorTriangleActif.Set(128, 128, 128);
	themeAboutScroll.colorBar.Set(74, 74, 74);
	themeAboutScroll.colorBarActif.Set(128, 128, 128);
	themeAboutScroll.SetMarge(2);
}

void CIntroTheme::InitAboutTree()
{
	themeAbout.bgColorOne.Set(29, 29, 29);
	themeAbout.bgColorActif.Set(255, 255, 255);
	themeAbout.bgColorTwo.Set(74, 74, 74);
	themeAbout.SetRowHeight(20);
	themeAbout.SetMargeX(5);
	themeAbout.themeCheckbox.SetWidth(15);
	themeAbout.themeCheckbox.SetHeight(15);
	themeAbout.themeSlide.SetHeight(20);
	themeAbout.themeSlide.SetWidth(250);
	themeAbout.themeSlide.font.SetColorFont(wxColour(255, 255, 255));
	themeAbout.themeSlide.font.SetFontName("Verdana");
	themeAbout.themeSlide.font.SetFontSize(10);
	//themeAbout.themeSlide.SetMarge(2);
	themeAbout.themeSlide.rectanglePast.Set(0, 0, 128);
	themeAbout.themeSlide.rectangleNext.Set(48, 128, 254);
	themeAbout.themeTexte.SetHeight(20);
	themeAbout.themeTexte.SetWidth(20);
	themeAbout.themeTexte.font.SetColorFont(wxColour(255, 255, 255));
	themeAbout.themeTexte.font.SetFontName("Verdana");
	themeAbout.themeTexte.font.SetFontSize(10);
	themeAbout.themeTriangle.SetMarge(2);
	themeAbout.themeTriangle.SetElementHeight(10);
	themeAbout.themeTriangle.SetHeight(10);
	themeAbout.themeTriangle.SetElementWidth(10);
	themeAbout.themeTriangle.SetWidth(10);
	themeAbout.themeTriangle.color.Set(0, 0, 0);
}
