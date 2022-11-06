#pragma once
#include <iostream>
#include <fstream>
#include <rapidxml/rapidxml.hpp>
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

	//Default Theme
	void GetScrollTheme(CThemeScrollBar* theme);
	void GetPaneTheme(CThemePane* theme);
	void GetTreeTheme(CThemeTree* theme);
	void GetSplitterTheme(CThemeSplitter* theme);
	void GetClickToolbarTheme(CThemeToolbar* theme);
	void GetThumbnailTheme(CThemeThumbnail* theme);

protected:

	void InitVideoToolbar();
	void InitVideoSlider();
	void InitVideoControl();

	void InitBitmapWindow();
	void InitBitmapToolbar();


	//Default Theme
	void InitScrollTheme();
	void InitPaneTheme();
	void InitTreeTheme();
	void InitSplitterTheme();
	void InitClickToolbarTheme();
	void InitThumbnail();

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


	//Default Theme
	CThemeScrollBar themeScroll;
	CThemePane themePane;
	CThemeTree themeTree;
	CThemeSplitter themeSplitter;
	CThemeToolbar themeClickToolbar;
	CThemeThumbnail themeThumbnail;

};

