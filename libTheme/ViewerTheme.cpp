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

CMainTheme::CMainTheme()
{
}

void CMainTheme::InitThumbnailFaceThemeToolbar()
{
	themeThumbnailFaceToolbar.colorBack.Set(30, 30, 30);
	themeThumbnailFaceToolbar.colorTop.Set(29, 29, 29);
	themeThumbnailFaceToolbar.colorBottom.Set(29, 29, 29);
	themeThumbnailFaceToolbar.position = NAVIGATOR_CENTER;
	themeThumbnailFaceToolbar.SetWidth(30);
	themeThumbnailFaceToolbar.SetHeight(30);
	themeThumbnailFaceToolbar.SetMargeX(5);
	themeThumbnailFaceToolbar.SetMargeY(0);

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

void CMainTheme::InitThumbnailToolbar()
{
	themeThumbnailToolbar.colorBack.Set(30, 30, 30);
	themeThumbnailToolbar.colorTop.Set(50, 54, 63);
	themeThumbnailToolbar.colorBottom.Set(50, 54, 63);
	themeThumbnailToolbar.position = NAVIGATOR_CENTER;
	themeThumbnailToolbar.SetWidth(30);
	themeThumbnailToolbar.SetHeight(30);
	themeThumbnailToolbar.SetMargeX(5);
	themeThumbnailToolbar.SetMargeY(0);

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


void CMainTheme::GetThumbnailFaceToolbarTheme(CThemeToolbar& theme)
{
	theme = themeThumbnailFaceToolbar;
}

void CMainTheme::GetThumbnailToolbarTheme(CThemeToolbar& theme)
{
	theme = themeThumbnailToolbar;
}

void CMainTheme::GetThumbnailToolbarZoomTheme(CThemeToolBarZoom& theme)
{
	theme = themeThumbnailToolbarZoom;
}


void CMainTheme::InitThumbnailToolbarZoomTheme()
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


void CMainTheme::InitFolderTheme()
{
	themeFolder.colorBack.Set(74, 74, 74);
	themeFolder.colorFont.Set(255, 255, 255);
}


void CMainTheme::GetBitmapInfosTheme(CThemeBitmapInfos* theme)
{
	*theme = themeBitmapInfos;
}


void CMainTheme::InitBitmapInfos()
{
	themeBitmapInfos.themeFont.SetFontName("Verdana");
	themeBitmapInfos.themeFont.SetFontSize(16);
	themeBitmapInfos.themeFont.SetFontQuality(DEFAULT_QUALITY);
	themeBitmapInfos.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeBitmapInfos.colorBack.Set(61, 65, 73);
}


void CMainTheme::GetMainToolbarTheme(CThemeToolbar* theme)
{
	*theme = themeMainToolbar;
}

void CMainTheme::GetInfosToolbarTheme(CThemeToolbar* theme)
{
	*theme = themeInfosToolbar;
}

void CMainTheme::GetFiltreToolbarTheme(CThemeToolbar* theme)
{
	*theme = themeFiltreToolbar;
}

void CMainTheme::GetPreviewToolbarTheme(CThemeToolbar* theme)
{
	*theme = themePreviewToolbar;
}


void CMainTheme::InitFiltreToolbar()
{
	themeFiltreToolbar.colorBack.Set(30, 30, 30);
	themeFiltreToolbar.colorTop.Set(29, 29, 29);
	themeFiltreToolbar.colorBottom.Set(29, 29, 29);
	themeFiltreToolbar.position = NAVIGATOR_CENTER;
	themeFiltreToolbar.SetWidth(60);
	themeFiltreToolbar.SetHeight(40);
	themeFiltreToolbar.SetMargeX(5);
	themeFiltreToolbar.SetMargeY(0);

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


void CMainTheme::InitViewerToolbar()
{
	themeMainToolbar.colorBack.Set(30, 30, 30);
	themeMainToolbar.colorTop.Set(50, 54, 63);
	themeMainToolbar.colorBottom.Set(50, 54, 63);
	themeMainToolbar.position = NAVIGATOR_LEFT;
	themeMainToolbar.SetWidth(60);
	themeMainToolbar.SetHeight(60);
	themeMainToolbar.SetMargeX(5);
	themeMainToolbar.SetMargeY(0);

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

void CMainTheme::InitInfosToolbar()
{
	themeInfosToolbar.colorBack.Set(30, 30, 30);
	themeInfosToolbar.colorTop.Set(61, 65, 73);
	themeInfosToolbar.colorBottom.Set(61, 65, 73);
	themeInfosToolbar.position = NAVIGATOR_CENTER;
	themeInfosToolbar.SetWidth(120);
	themeInfosToolbar.SetHeight(30);
	themeInfosToolbar.SetMargeX(0);
	themeInfosToolbar.SetMargeY(0);

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


void CMainTheme::InitPreviewToolbar()
{
	themePreviewToolbar.colorBack.Set(30, 30, 30);
	themePreviewToolbar.colorTop.Set(50, 54, 63);
	themePreviewToolbar.colorBottom.Set(50, 54, 63);
	themePreviewToolbar.position = NAVIGATOR_CENTER;
	themePreviewToolbar.SetWidth(60);
	themePreviewToolbar.SetHeight(60);
	themePreviewToolbar.SetMargeX(5);
	themePreviewToolbar.SetMargeY(0);

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

CMainTheme::~CMainTheme()
{
	doc.clear();
}


//////////////////////////////////////////////////////////////////////////////////////////
//Saving Parameter
//////////////////////////////////////////////////////////////////////////////////////////
void CMainTheme::SaveTheme()
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
	if (section != nullptr)
	{
		xml_node<>* local_section = node("MainToolbar");
		if (local_section != nullptr)
		{
			themeMainToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("DefaultSplitterTheme");
		if (local_section != nullptr)
		{
			themeSplitter.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("DefaultScrollTheme");
		if (local_section != nullptr)
		{
			themeScroll.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("DefaultPaneTheme");
		if (local_section != nullptr)
		{
			themePane.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("DefaultTreeTheme");
		if (local_section != nullptr)
		{
			themeTree.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("DefaultClickToolbarTheme");
		if (local_section != nullptr)
		{
			themeClickToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		root->append_node(section);
	}


	section = node("VideoPreview");
	if (section != nullptr)
	{
		xml_node<>* local_section = node("VideoControl");
		if (local_section != nullptr)
		{
			themeVideoControl.SaveXML(doc, local_section);
			section->append_node(local_section);
		}


		local_section = node("VideoToolbar");
		if (local_section != nullptr)
		{
			themeVideoToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("SliderVideo");
		if (local_section != nullptr)
		{
			themeSliderVideo.SaveXML(doc, local_section);
			section->append_node(local_section);
		}
		root->append_node(section);
	}


	section = node("BitmapPreview");
	if (section != nullptr)
	{
		xml_node<>* local_section = node("BitmapWindow");
		if (local_section != nullptr)
		{
			themeBitmapWindow.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("BitmapToolbar");
		if (local_section != nullptr)
		{
			themeBitmapToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}
		root->append_node(section);
	}


	section = node("PreviewWindow");
	if (section != nullptr)
	{
		xml_node<>* local_section = node("PreviewToolbar");
		if (local_section != nullptr)
		{
			themePreviewToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("BitmapInfos");
		if (local_section != nullptr)
		{
			themeBitmapInfos.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		root->append_node(section);
	}


	section = node("Thumbnail");
	if (section != nullptr)
	{
		xml_node<>* local_section = node("Thumbnail");
		if (local_section != nullptr)
		{
			themeThumbnail.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("ThumbnailToolbarZoom");
		if (local_section != nullptr)
		{
			themeThumbnailToolbarZoom.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("ThumbnailToolbar");
		if (local_section != nullptr)
		{
			themeThumbnailToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}
		root->append_node(section);
	}

	section = node("InfoVideoPicture");
	if (section != nullptr)
	{
		xml_node<>* local_section = node("InfosToolbar");
		if (local_section != nullptr)
		{
			themeInfosToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		local_section = node("FiltreToolbar");
		if (local_section != nullptr)
		{
			themeFiltreToolbar.SaveXML(doc, local_section);
			section->append_node(local_section);
		}

		root->append_node(section);
	}

	section = node("CategoryFolder");
	if (section != nullptr)
	{
		xml_node<>* local_section = node("Folder");
		if (local_section != nullptr)
		{
			themeFolder.SaveXML(doc, local_section);
			section->append_node(local_section);
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
void CMainTheme::LoadTheme()
{
	xml_node<>* root_node;
	//long nodeSize = 0;
	root_node = doc.first_node("Theme");

	xml_node<>* child_node = root_node->first_node("Main");
	if (child_node != nullptr)
	{
		xml_node<>* nodePreview = child_node->first_node("MainToolbar");
		if (nodePreview != nullptr)
			themeMainToolbar.LoadXML(nodePreview);
		else
			InitViewerToolbar();

		nodePreview = child_node->first_node("DefaultScrollTheme");
		if (nodePreview != nullptr)
			themeScroll.LoadXML(nodePreview);
		else
			InitScrollTheme();

		nodePreview = child_node->first_node("DefaultPaneTheme");
		if (nodePreview != nullptr)
			themePane.LoadXML(nodePreview);
		else
			InitPaneTheme();

		nodePreview = child_node->first_node("DefaultTreeTheme");
		if (nodePreview != nullptr)
			themeTree.LoadXML(nodePreview);
		else
			InitTreeTheme();

		nodePreview = child_node->first_node("DefaultSplitterTheme");
		if (nodePreview != nullptr)
			themeSplitter.LoadXML(nodePreview);
		else
			InitSplitterTheme();

		nodePreview = child_node->first_node("DefaultClickToolbarTheme");
		if (nodePreview != nullptr)
			themeClickToolbar.LoadXML(nodePreview);
		else
			InitClickToolbarTheme();
	}
	else
	{
		InitViewerToolbar();
		InitSplitterTheme();
		InitScrollTheme();
		InitPaneTheme();
		InitTreeTheme();
		InitClickToolbarTheme();
	}

	child_node = root_node->first_node("VideoPreview");
	if (child_node != nullptr)
	{
		xml_node<>* nodePreview = child_node->first_node("VideoControl");
		if (nodePreview != nullptr)
			themeVideoControl.LoadXML(nodePreview);
		else
			InitVideoControl();

		nodePreview = child_node->first_node("VideoToolbar");
		if (nodePreview != nullptr)
			themeVideoToolbar.LoadXML(nodePreview);
		else
			InitVideoToolbar();

		nodePreview = child_node->first_node("SliderVideo");
		if (nodePreview != nullptr)
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
	if (child_node != nullptr)
	{
		xml_node<>* nodePreview = child_node->first_node("BitmapWindow");
		if (nodePreview != nullptr)
			themeBitmapWindow.LoadXML(nodePreview);
		else
			InitBitmapWindow();

		nodePreview = child_node->first_node("BitmapToolbar");
		if (nodePreview != nullptr)
			themeBitmapToolbar.LoadXML(nodePreview);
		else
			InitBitmapToolbar();
	}
	else
	{
		InitBitmapWindow();
		InitBitmapToolbar();
	}

	child_node = root_node->first_node("PreviewWindow");
	if (child_node != nullptr)
	{
		xml_node<>* nodePreview = child_node->first_node("PreviewToolbar");
		if (nodePreview != nullptr)
			themePreviewToolbar.LoadXML(nodePreview);
		else
			InitPreviewToolbar();

		nodePreview = child_node->first_node("BitmapInfos");
		if (nodePreview != nullptr)
			themeBitmapInfos.LoadXML(nodePreview);
		else
			InitBitmapInfos();
	}
	else
	{
		InitBitmapInfos();
		InitPreviewToolbar();
	}

	child_node = root_node->first_node("Thumbnail");
	if (child_node != nullptr)
	{
		xml_node<>* nodePreview = child_node->first_node("Thumbnail");
		if (nodePreview != nullptr)
			themeThumbnail.LoadXML(nodePreview);
		else
			InitThumbnail();

		nodePreview = child_node->first_node("ThumbnailToolbarZoom");
		if (nodePreview != nullptr)
			themeThumbnailToolbarZoom.LoadXML(nodePreview);
		else
			InitThumbnailToolbarZoomTheme();

		nodePreview = child_node->first_node("ThumbnailToolbar");
		if (nodePreview != nullptr)
			themeThumbnailToolbar.LoadXML(nodePreview);
		else
			InitThumbnailToolbar();
	}
	else
	{
		InitThumbnail();
		InitThumbnailToolbarZoomTheme();
		InitThumbnailToolbar();
	}

	child_node = root_node->first_node("InfoVideoPicture");
	if (child_node != nullptr)
	{
		xml_node<>* nodePreview = child_node->first_node("InfosToolbar");
		if (nodePreview != nullptr)
			themeInfosToolbar.LoadXML(nodePreview);
		else
			InitInfosToolbar();

		nodePreview = child_node->first_node("FiltreToolbar");
		if (nodePreview != nullptr)
			themeFiltreToolbar.LoadXML(nodePreview);
		else
			InitFiltreToolbar();
	}
	else
	{
		InitInfosToolbar();
		InitFiltreToolbar();
	}

	child_node = root_node->first_node("CategoryFolder");
	if (child_node != nullptr)
	{
		xml_node<>* node_local = child_node->first_node("Folder");
		if (node_local != nullptr)
			themeFolder.LoadXML(node_local);
		else
			InitFolderTheme();
	}
	else
	{
		InitFolderTheme();
	}
}

void CMainTheme::InitTheme()
{
	//Default Theme
	InitScrollTheme();
	InitPaneTheme();
	InitTreeTheme();
	InitSplitterTheme();
	InitThumbnail();
	InitClickToolbarTheme();

	//------------------------------------------
	InitFiltreToolbar();
	InitViewerToolbar();
	InitVideoControl();
	InitVideoToolbar();
	InitVideoSlider();
	InitBitmapWindow();
	InitBitmapToolbar();
	InitPreviewToolbar();
	InitInfosToolbar();

	InitThumbnailToolbarZoomTheme();
	InitThumbnailToolbar();
}
