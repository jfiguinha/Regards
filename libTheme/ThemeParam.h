#pragma once
#include <iostream>
#include <fstream>
#include <rapidxml.hpp>
#include <theme.h>
using namespace rapidxml;

#define NAVIGATOR_LEFT 1
#define NAVIGATOR_CENTER 2
#define NAVIGATOR_RIGHT 3


class CThemeParam
{
public:
	CThemeParam();
	virtual ~CThemeParam();

	bool OpenFile(const wxString &themeFile);
	bool SaveFile();

	void GetVideoControlTheme(CTheme * theme);
	void GetVideoSliderTheme(CThemeSlider * theme);
	void GetVideoToolbarTheme(CThemeToolbar * theme);

	void GetBitmapWindowTheme(CThemeBitmapWindow * theme);
	void GetBitmapToolbarTheme(CThemeToolbar * theme);
	void GetBitmapScrollbarTheme(CThemeScrollBar * theme);

protected:

	void InitVideoToolbar();
	void InitVideoSlider();
	void InitVideoControl();

	void InitBitmapWindow();
	void InitBitmapToolbar();
	void InitBitmapScrollbar();



	virtual void LoadTheme(){};
	virtual void SaveTheme(){};
	virtual void InitTheme(){};

	const char* stralloc(const wxString & str);
	xml_node<>*  node(const wxString & name, const wxString & content = "");

	xml_document<> doc;
	wxString filename;

	CTheme themeVideoControl;
	CThemeToolbar themeVideoToolbar;
	CThemeSlider  themeSliderVideo;

	CThemeBitmapWindow themeBitmapWindow;
	CThemeToolbar themeBitmapToolbar;
	CThemeScrollBar themeBitmapScrollbar;

};

