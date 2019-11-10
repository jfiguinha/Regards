#include "header.h"
#include "ThemeParam.h"
#include <fstream>
#ifndef __WXMSW__
#include <font_quality.h>
#endif
#include <ConvertUtility.h>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
using namespace rapidxml;

CThemeParam::CThemeParam()
{
}


CThemeParam::~CThemeParam()
{
	doc.clear();
}


void CThemeParam::InitTreeTheme()
{
	themeTree.bgColorOne.Set(45, 48, 56);
	themeTree.bgColorActif.Set(255, 255, 255);
	themeTree.bgColorTwo.Set(80, 84, 91);
	themeTree.SetRowHeight(20);
	themeTree.SetMargeX(5);
	themeTree.themeCheckbox.SetWidth(15);
	themeTree.themeCheckbox.SetHeight(15);
	themeTree.themeSlide.SetHeight(20);
	themeTree.themeSlide.SetWidth(250);
	themeTree.themeSlide.font.SetColorFont(wxColour(255, 255, 255));
	themeTree.themeSlide.font.SetFontName("Verdana");
	themeTree.themeSlide.font.SetFontSize(10);
	themeTree.themeSlide.rectanglePast.Set(0, 0, 128);
	themeTree.themeSlide.rectangleNext.Set(48, 128, 254);
	themeTree.themeTexte.SetHeight(20);
	themeTree.themeTexte.SetWidth(20);
	themeTree.themeTexte.font.SetColorFont(wxColour(255, 255, 255));
	themeTree.themeTexte.font.SetFontName("Verdana");
	themeTree.themeTexte.font.SetFontSize(10);
	themeTree.themeTriangle.SetMarge(2);
	themeTree.themeTriangle.SetElementHeight(10);
	themeTree.themeTriangle.SetHeight(10);
	themeTree.themeTriangle.SetElementWidth(10);
	themeTree.themeTriangle.SetWidth(10);
	themeTree.themeTriangle.color.Set(0, 0, 0);
}

void CThemeParam::GetThumbnailTheme(CThemeThumbnail* theme)
{
	*theme = themeThumbnail;
}

void CThemeParam::GetScrollTheme(CThemeScrollBar* theme)
{
	*theme = themeScroll;
}

void CThemeParam::GetPaneTheme(CThemePane* theme)
{
	*theme = themePane;
}

void CThemeParam::InitScrollTheme()
{
	themeScroll.SetRectangleSize(10);
	themeScroll.colorBack.Set(29, 29, 29);
	themeScroll.colorTriangle.Set(74, 74, 74);
	themeScroll.colorTriangleActif.Set(128, 128, 128);
	themeScroll.colorBar.Set(50, 54, 63);
	themeScroll.colorBarActif.Set(128, 128, 128);
	themeScroll.SetMarge(2);
}

void CThemeParam::InitPaneTheme()
{
	themePane.SetHeight(20);
	themePane.themeTitle.SetCroixWidth(15);
	themePane.themeTitle.SetCroixHeight(15);
	themePane.themeTitle.SetHeight(30);
	themePane.themeTitle.SetMarge(5);
	themePane.themeTitle.font.SetColorFont(wxColour(255, 255, 255));
	themePane.themeTitle.font.SetFontSize(14);
}



void CThemeParam::GetTreeTheme(CThemeTree* theme)
{
	*theme = themeTree;
}

void CThemeParam::InitThumbnail()
{
	themeThumbnail.themeIcone.SetWidth(200);
	themeThumbnail.themeIcone.SetHeight(200);
	themeThumbnail.themeIcone.showOnlyThumbnail = false;
	themeThumbnail.themeIcone.colorBack.Set(40, 44, 53);
	themeThumbnail.themeIcone.colorTop.Set(80, 84, 91);
	themeThumbnail.themeIcone.colorBottom.Set(80, 84, 91);
	themeThumbnail.themeIcone.colorSelectTop.Set(205, 205, 205);
	themeThumbnail.themeIcone.colorSelectBottom.Set(171, 171, 171);
	themeThumbnail.colorBack.Set(50, 54, 63);
	themeThumbnail.themeIcone.font.SetColorFont(wxColour(255, 255, 255));
	themeThumbnail.themeSeparation.colorBack.Set(80, 84, 91);
	//wxColour(45, 48, 56);
	//themeThumbnail.themeSeparation.colorBack.Set(45, 48, 56);
	themeThumbnail.themeSeparation.colorTop.Set(45, 45, 48);
	themeThumbnail.themeSeparation.colorBottom.Set(45, 45, 48);
	themeThumbnail.themeSeparation.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeThumbnail.themeSeparation.themeFont.SetFontName("Verdana");
	themeThumbnail.themeSeparation.themeFont.SetFontSize(10);
	themeThumbnail.themeSeparation.themeFont.SetFontQuality(ANTIALIASED_QUALITY);
	themeThumbnail.themeSeparation.SetHeight(40);
}




void CThemeParam::GetClickToolbarTheme(CThemeToolbar* theme)
{
	*theme = themeClickToolbar;
}

void CThemeParam::GetSplitterTheme(CThemeSplitter* theme)
{
	*theme = themeSplitter;
}

void CThemeParam::InitSplitterTheme()
{
	themeSplitter.themeFast.firstColor.Set(29, 29, 29);
	themeSplitter.themeFast.secondColor.Set(128, 128, 128);
	themeSplitter.themeSeparation.firstColor.Set(29, 29, 29);
	themeSplitter.themeSeparation.secondColor.Set(128, 128, 128);
}

void CThemeParam::InitClickToolbarTheme()
{
	themeClickToolbar.colorBack.Set(30, 30, 30);
	themeClickToolbar.colorTop.Set(29, 29, 29);
	themeClickToolbar.colorBottom.Set(29, 29, 29);
	themeClickToolbar.position = NAVIGATOR_CENTER;
	themeClickToolbar.SetWidth(60);
	themeClickToolbar.SetHeight(10);
	themeClickToolbar.SetMargeX(5);
	themeClickToolbar.SetMargeY(0);

	themeClickToolbar.button.SetTailleX(60);
	themeClickToolbar.button.SetTailleY(10);
	themeClickToolbar.button.lineColorTop.Set(222, 222, 222);
	themeClickToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeClickToolbar.button.actifTop.Set(255, 255, 255);
	themeClickToolbar.button.actifBottom.Set(223, 223, 223);
	themeClickToolbar.button.font.SetFontName("Verdana");
	themeClickToolbar.button.font.SetFontSize(10);
	themeClickToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeClickToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeClickToolbar.button.SetRectangleSize(4);
	themeClickToolbar.button.showButtonOnly = 0;
}

void CThemeParam::InitBitmapWindow()
{
	themeBitmapWindow.colorBack.Set(0, 0, 0);
	themeBitmapWindow.colorScreen.Set(50, 54, 63); 
	themeBitmapWindow.colorFullscreen.Set(0, 0, 0);
}

void CThemeParam::GetBitmapWindowTheme(CThemeBitmapWindow * theme)
{
	*theme = themeBitmapWindow;
}



void CThemeParam::InitVideoSlider()
{
	themeSliderVideo.SetRectangleHeight(10);
	themeSliderVideo.rectanglePast.Set(0, 0, 128);
	themeSliderVideo.rectangleNext.Set(48, 128, 254);
	themeSliderVideo.font.SetColorFont(wxColour(255, 255, 255));
	themeSliderVideo.colorBack.Set(128, 128, 128);
}

void CThemeParam::InitVideoControl()
{
	themeVideoControl.colorBack.Set(0, 0, 0);
}

void  CThemeParam::GetVideoSliderTheme(CThemeSlider * theme)
{
	*theme = themeSliderVideo;
}

void CThemeParam::GetVideoControlTheme(CTheme * theme)
{
	*theme = themeVideoControl;
}

void CThemeParam::InitVideoToolbar()
{
	themeVideoToolbar.colorBack.Set(30, 30, 30);
	themeVideoToolbar.colorTop.Set(29, 29, 29);
	themeVideoToolbar.colorBottom.Set(29, 29, 29);
	themeVideoToolbar.position = NAVIGATOR_CENTER;
	themeVideoToolbar.SetHeight(70);
	themeVideoToolbar.SetMargeX(5);
	themeVideoToolbar.SetMargeY(0);

	themeVideoToolbar.button.SetTailleX(60);
	themeVideoToolbar.button.SetTailleY(60);
	themeVideoToolbar.button.lineColorTop.Set(222, 222, 222);
	themeVideoToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeVideoToolbar.button.actifTop.Set(255, 255, 255);
	themeVideoToolbar.button.actifBottom.Set(223, 223, 223);
	themeVideoToolbar.button.font.SetFontName("Verdana");
	themeVideoToolbar.button.font.SetFontSize(10);
	themeVideoToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeVideoToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeVideoToolbar.button.SetRectangleSize(4);
	themeVideoToolbar.button.showButtonOnly = 1;
}

void CThemeParam::InitBitmapToolbar()
{
	themeBitmapToolbar.colorBack.Set(45, 48, 56);
	themeBitmapToolbar.colorTop.Set(45, 48, 56);
	//themeBitmapToolbar.colorBottom.Set(74, 74, 74);
    themeBitmapToolbar.colorBottom.Set(45, 48, 56);
	themeBitmapToolbar.position = NAVIGATOR_CENTER;
	themeBitmapToolbar.SetHeight(40);
	themeBitmapToolbar.SetMargeX(5);
	themeBitmapToolbar.SetMargeY(0);

	themeBitmapToolbar.button.SetTailleX(30);
	themeBitmapToolbar.button.SetTailleY(30);
	themeBitmapToolbar.button.lineColorTop.Set(222, 222, 222);
	themeBitmapToolbar.button.lineColorBottom.Set(156, 156, 156);
	themeBitmapToolbar.button.actifTop.Set(255, 255, 255);
	themeBitmapToolbar.button.actifBottom.Set(223, 223, 223);
	themeBitmapToolbar.button.font.SetFontName("Verdana");
	themeBitmapToolbar.button.font.SetFontSize(10);
	themeBitmapToolbar.button.font.SetFontQuality(ANTIALIASED_QUALITY);
	themeBitmapToolbar.button.font.SetColorFont(wxColour(255, 255, 255));
	themeBitmapToolbar.button.SetRectangleSize(4);
	themeBitmapToolbar.button.showButtonOnly = 1;

	themeBitmapToolbar.slider.SetRectangleHeight(10);
	//themeBitmapToolbar.slider.rectanglePast.Set(45, 48, 56);
	//themeBitmapToolbar.slider.rectangleNext.Set(45, 48, 56);
	themeBitmapToolbar.slider.font.SetColorFont(wxColour(255, 255, 255));
	themeBitmapToolbar.slider.colorBack.Set(45, 48, 56);
	themeBitmapToolbar.slider.SetWidth(200);
	themeBitmapToolbar.slider.SetHeight(30);
    
	//themeBitmapToolbar.slider.rectangleNext.Set(45, 48, 56);
	//themeBitmapToolbar.slider.rectanglePast.Set(45, 48, 56);
	//themeBitmapToolbar.slider.SetWidth(200);
	//themeBitmapToolbar.slider.SetHeight(30);
}


void CThemeParam::GetVideoToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeVideoToolbar;
}

void CThemeParam::GetBitmapToolbarTheme(CThemeToolbar * theme)
{
	*theme = themeBitmapToolbar;
}

bool CThemeParam::OpenFile(const wxString &configFile)
{
	filename = configFile;
	doc.clear();
	// Read the xml file into a vector
    //const char * fichier = CConvertUtility::ConvertFromwxString(filename);
	ifstream theFile(CConvertUtility::ConvertToStdString(filename));
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	if (buffer.size() > 0)
	{
		buffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		LoadTheme();
	}
	else
		InitTheme();

	return true;

}

bool CThemeParam::SaveFile()
{
	SaveTheme();
	return true;
}

// allocate the given string from xml document memory
// to ensure that it says alive for the duration of xml
// operations
const char* CThemeParam::stralloc(const wxString& str)
{
	return doc.allocate_string(str.c_str());
}

// create a node
xml_node<>*  CThemeParam::node(const wxString& name, const wxString& content)
{
	return doc.allocate_node(node_element, stralloc(name), stralloc(content));
}
