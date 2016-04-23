#include "ThemeParam.h"
#include <fstream>
#ifndef __WXMSW__
#include <font_quality.h>
#endif
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

void CThemeParam::InitBitmapWindow()
{
	themeBitmapWindow.colorBack.Set(0, 0, 0);
	themeBitmapWindow.colorScreen.Set(128, 128, 128); 
	themeBitmapWindow.colorFullscreen.Set(0, 0, 0);
}

void CThemeParam::InitBitmapScrollbar()
{
	themeBitmapScrollbar.SetRectangleSize(10);
	themeBitmapScrollbar.colorBack.Set(29, 29, 29);
	themeBitmapScrollbar.colorTriangle.Set(74, 74, 74);
	themeBitmapScrollbar.colorTriangleActif.Set(128, 128, 128);
	themeBitmapScrollbar.colorBar.Set(74, 74, 74);
	themeBitmapScrollbar.colorBarActif.Set(128, 128, 128);
	themeBitmapScrollbar.SetMarge(2);
}

void CThemeParam::GetBitmapWindowTheme(CThemeBitmapWindow * theme)
{
	*theme = themeBitmapWindow;
}

void CThemeParam::GetBitmapScrollbarTheme(CThemeScrollBar * theme)
{
	*theme = themeBitmapScrollbar;
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
	themeVideoToolbar.colorTop.Set(74, 74, 74);
	themeVideoToolbar.colorBottom.Set(29, 29, 29);
	themeVideoToolbar.position = NAVIGATOR_CENTER;
	themeVideoToolbar.SetHeight(70);
	themeVideoToolbar.SetMargeX(5);
	themeVideoToolbar.SetMargeY(0);
	themeVideoToolbar.isVertical = false;

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
	themeBitmapToolbar.colorBack.Set(30, 30, 30);
	themeBitmapToolbar.colorTop.Set(74, 74, 74);
	themeBitmapToolbar.colorBottom.Set(29, 29, 29);
	themeBitmapToolbar.position = NAVIGATOR_CENTER;
	themeBitmapToolbar.SetHeight(40);
	themeBitmapToolbar.SetMargeX(5);
	themeBitmapToolbar.SetMargeY(0);
	themeBitmapToolbar.isVertical = false;

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

	themeBitmapToolbar.slider.SetWidth(200);
	themeBitmapToolbar.slider.SetHeight(30);
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
	ifstream theFile(filename.c_str());
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
