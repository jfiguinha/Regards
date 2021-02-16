#include "header.h"
#include "theme.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <rapidxml.hpp>
#ifndef __WXMSW__
#include <font_quality.h>
#endif
#include <ConvertUtility.h>
using namespace rapidxml;



xml_node<>*  CThemeXml::node(xml_document<> & doc, const wxString & name, const wxString & content)
{
	return doc.allocate_node(node_element, stralloc(doc, name), stralloc(doc, content));
}

// allocate the given string from xml document memory
// to ensure that it says alive for the duration of xml
// operations
const char* CThemeXml::stralloc(xml_document<> & doc, const wxString & str)
{
	return doc.allocate_string(str.c_str());
}


CTheme::CTheme()
{
	colorBack.Set(128, 128, 128, 255);
};
CTheme::~CTheme()
{
};

int CThemeXml::CalculElementSizeForScreen(const int &elementSize, const bool &width)
{

    float ratio = 1.0;
    if(width)
        ratio = CalculRatioX();
    else
        ratio = CalculRatioY();
    
    
    float newElementSize = (float)elementSize * ratio;

    
    return newElementSize;
}

float CThemeXml::CalculRatioY()
{
    //int getScreenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
    //float ratio = 1.0;
    //ratio = (float)getScreenHeight / 1080.0;
    
    CRegardsConfigParam * config = CParamInit::getInstance();
    if(config != nullptr)
        return config->GetIconSizeRatio();
    return 1.0;//ratio;
}

float CThemeXml::CalculRatioX()
{
    //int getScreenWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
    //float ratio = 1.0;
    //ratio = (float)getScreenWidth / 1920.0;
    //return 1.0;//ratio;
    
    CRegardsConfigParam * config = CParamInit::getInstance();
    if(config != nullptr)
        return config->GetIconSizeRatio();
    return 1.0;//ratio;
}

void CTheme::LoadXML(xml_node<> * root_node)
{
	wxString value;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("Theme");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}
	}
}

void CTheme::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "Theme");
	section->append_node(node(doc, "colorBack", CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CTheme& CTheme::operator=(const CTheme& other)
{
	this->colorBack = other.colorBack;
	return *this;
}

void CThemeBitmapWindow::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("Theme");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("colorFullscreen");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorFullscreen.Set(value);
		}

		child_node = node->first_node("colorScreen");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorScreen.Set(value);
		}
	}
}

void CThemeBitmapWindow::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "Theme");
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorFullscreen", CConvertUtility::ConvertToStdString(colorFullscreen.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorScreen", CConvertUtility::ConvertToStdString(colorScreen.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeBitmapWindow::CThemeBitmapWindow()
{
	colorBack.Set(128,128,128);
	colorFullscreen.Set(0, 0, 0);
	colorScreen.Set(128, 128, 128);
}
CThemeBitmapWindow::~CThemeBitmapWindow()
{

}

CThemeBitmapWindow& CThemeBitmapWindow::operator=(const CThemeBitmapWindow& other)
{
	this->colorBack = other.colorBack;
	this->colorFullscreen = other.colorFullscreen;
	this->colorScreen = other.colorScreen;
	return *this;
}

CThemeFolder::CThemeFolder()
{
	colorBack.Set(74, 74, 74);
	colorFont.Set(255, 255, 255);
}

void CThemeFolder::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeFolder");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("colorFont");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorFont.Set(value);
		}
	}
}

void CThemeFolder::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeFolder");
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorFont",CConvertUtility::ConvertToStdString(colorFont.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeFolder& CThemeFolder::operator=(const CThemeFolder& other)
{
	this->colorFont = other.colorFont;
	this->colorBack = other.colorBack;
	return *this;
}

CThemeFont::CThemeFont()
{
	colorFont.Set(0, 0, 0);
	SetFontSize(10);
	fontQuality = 0;
	SetFontName("Verdana");
	bold = false;
	marge = 5;
	positionFont = RENDERFONT_CENTER;
};

CThemeFont& CThemeFont::operator=(const CThemeFont& other)
{
	this->colorFont = other.colorFont;
	this->SetFontName(other.fontName);
	this->fontSize = other.fontSize;
	this->fontQuality = other.fontQuality;
	this->bold = other.bold;
	this->marge = other.marge;
	this->positionFont = other.positionFont;
	return *this;
}

void CThemeFont::LoadXML(xml_node<> * root_node)
{
	wxString value;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeFont");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("bold");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			bold = atoi(value.c_str());
		}

		child_node = node->first_node("positionFont");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			positionFont = atoi(value.c_str());
		}

		child_node = node->first_node("colorFont");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorFont.Set(value);
		}

		child_node = node->first_node("fontName");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			fontName = value;
		}

		child_node = node->first_node("fontSize");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			fontSize = atoi(value.c_str());
		}

		child_node = node->first_node("marge");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			marge = atoi(value.c_str());
		}

		child_node = node->first_node("fontQuality");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			fontQuality = atoi(value.c_str());
		}
	}
}

void CThemeFont::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
    wxString fontname = fontName;
	xml_node<>* section = node(doc, "ThemeFont");
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "bold", to_string(bold)));
	section->append_node(node(doc, "positionFont", to_string(positionFont)));
	section->append_node(node(doc, "colorFont",CConvertUtility::ConvertToStdString(colorFont.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "fontName", fontname));
	section->append_node(node(doc, "fontSize", to_string(fontSize)));
	section->append_node(node(doc, "marge", to_string(marge)));
	section->append_node(node(doc, "fontQuality", to_string(fontQuality)));
	sectionPosition->append_node(section);
}

CThemeTreeElement::CThemeTreeElement()
{
    color.Set(128,128,128);
	height = 30;
	width = 100;
}

void CThemeTreeElement::LoadXML(xml_node<> * root_node)
{
	wxString value;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTreeElement");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("color");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			color.Set(value);
		}
	}
}

void CThemeTreeElement::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTreeElement");
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "color", CConvertUtility::ConvertToStdString(color.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeTreeElement& CThemeTreeElement::operator=(const CThemeTreeElement& other)
{
	this->width = other.width;
	this->height = other.height;
	this->color = other.color;
	return *this;
}

CThemeTreeTriangle::CThemeTreeTriangle()
{
	marge = 2;
    
	elementHeight = height = 10;
	elementWidth = width = 10;
	color.Set(0, 0, 0);
};

void CThemeTreeTriangle::LoadXML(xml_node<> * root_node)
{
	wxString value;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTreeTriangle");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("elementWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			elementWidth = atoi(value.c_str());
		}

		child_node = node->first_node("elementHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			elementHeight = atoi(value.c_str());
		}

		child_node = node->first_node("marge");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			marge = atoi(value.c_str());
		}

		child_node = node->first_node("color");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			color.Set(value);
		}
	}
}

void CThemeTreeTriangle::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTreeTriangle");
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "elementWidth", to_string(elementWidth)));
	section->append_node(node(doc, "elementHeight", to_string(elementHeight)));
	section->append_node(node(doc, "marge", to_string(marge)));
	section->append_node(node(doc, "color", CConvertUtility::ConvertToStdString(color.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeTreeTriangle& CThemeTreeTriangle::operator=(const CThemeTreeTriangle& other)
{
	this->width = other.width;
	this->height = other.height;
	this->marge = other.marge;
	this->elementWidth = other.elementWidth;
	this->elementHeight = other.elementHeight;
	this->color = other.color;
	return *this;
}

CThemeTreeDelete::CThemeTreeDelete()
{
	width = croixWidth = 15;
	height = croixHeight = 15;
	crossColor.Set(255, 0, 0);
	position = 0;
}

void CThemeTreeDelete::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTreeDelete");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("elementWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			croixWidth = atoi(value.c_str());
		}

		child_node = node->first_node("elementHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			croixHeight = atoi(value.c_str());
		}

		child_node = node->first_node("crossColor");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			crossColor.Set(value);
		}

		child_node = node->first_node("colorBackActif");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBackActif.Set(value);
		}

		child_node = node->first_node("position");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			position = atoi(value.c_str());
		}

		child_node = node->first_node("color");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			color.Set(value);
		}
	}
}

void CThemeTreeDelete::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTreeDelete");
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "elementWidth", to_string(croixWidth)));
	section->append_node(node(doc, "elementHeight", to_string(croixHeight)));
	section->append_node(node(doc, "crossColor", CConvertUtility::ConvertToStdString(crossColor.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBackActif", CConvertUtility::ConvertToStdString(colorBackActif.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "position", to_string(position)));
	section->append_node(node(doc, "color", CConvertUtility::ConvertToStdString(color.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeTreeDelete& CThemeTreeDelete::operator=(const CThemeTreeDelete& other)
{
	this->height = other.height;
	this->width = other.width;
	this->position = other.position;
	this->crossColor = other.crossColor;
	this->colorBackActif = other.colorBackActif;
	this->color = other.color;
	return *this;
}

CThemeTreeSlide::CThemeTreeSlide()
{
	height = 20;
	width = 250;
	elementWidth = 250;
	font.SetColorFont(wxColour(255, 255, 255));
	font.SetFontName("Verdana");
	font.SetFontSize(14);
	//marge = 10;
	rectanglePast.Set(0, 0, 128);
	rectangleNext.Set(48, 128, 254);
	buttonWidth = 17;
    buttonHeight = 17;
    marge = 4;
}

CThemeTreeSlide::~CThemeTreeSlide()
{
}

void CThemeTreeSlide::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTreeSlide");
	if (node != 0)
	{
		font.LoadXML(node);

		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("elementWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			elementWidth = atoi(value.c_str());
		}

		child_node = node->first_node("rectanglePast");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectanglePast.Set(value);
		}

		child_node = node->first_node("rectangleNext");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectangleNext.Set(value);
		}

		child_node = node->first_node("marge");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			marge = atoi(value.c_str());
		}

		child_node = node->first_node("buttonWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			buttonWidth = atoi(value.c_str());
		}

		child_node = node->first_node("buttonHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			buttonHeight = atoi(value.c_str());
		}

		child_node = node->first_node("color");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			color.Set(value);
		}
	}



}

void CThemeTreeSlide::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTreeSlide");
	font.SaveXML(doc, section);
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height))); 
	section->append_node(node(doc, "elementWidth", to_string(elementWidth)));
	section->append_node(node(doc, "rectanglePast", CConvertUtility::ConvertToStdString(rectanglePast.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "rectangleNext", CConvertUtility::ConvertToStdString(rectangleNext.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "marge", to_string(marge)));
	section->append_node(node(doc, "color", CConvertUtility::ConvertToStdString(color.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeTreeSlide& CThemeTreeSlide::operator=(const CThemeTreeSlide& other)
{
	this->font = other.font;
	this->width = other.width;
	this->height = other.height;
	this->elementWidth = other.elementWidth;
	this->rectanglePast = other.rectanglePast;
	this->rectangleNext = other.rectangleNext;
	this->marge = other.marge;
	this->color = other.color;
	return *this;
}

CThemeTreeTexte::CThemeTreeTexte()
{
	height = 20;
	width = 20;
	font.SetColorFont(wxColour(255, 255, 255));
	font.SetFontName("Verdana");
	font.SetFontSize(10);
};

CThemeTreeTexte& CThemeTreeTexte::operator=(const CThemeTreeTexte& other)
{
	this->font = other.font;
	this->width = other.width;
	this->height = other.height;
	this->color = other.color;
	return *this;
}

void CThemeTreeTexte::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTreeTexte");
	if (node != 0)
	{
		font.LoadXML(node);

		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("color");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			color.Set(value);
		}
	}
}

void CThemeTreeTexte::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTreeTexte");
	font.SaveXML(doc, section);
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "color", CConvertUtility::ConvertToStdString(color.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

void CThemeTreeCheckBox::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTreeCheckBox");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("elementWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			elementWidth = atoi(value.c_str());
		}

		child_node = node->first_node("elementHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			elementHeight = atoi(value.c_str());
		}

		child_node = node->first_node("color");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			color.Set(value);
		}
	}
}

void CThemeTreeCheckBox::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTreeCheckBox");
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "elementWidth", to_string(elementWidth)));
	section->append_node(node(doc, "elementHeight", to_string(elementHeight)));
	section->append_node(node(doc, "color", CConvertUtility::ConvertToStdString(color.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeTreeCheckBox& CThemeTreeCheckBox::operator=(const CThemeTreeCheckBox& other)
{
	this->height = other.height;
	this->width = other.width;
	this->elementHeight = other.elementHeight;
	this->elementWidth = other.elementWidth;
	this->color = other.color;
	return *this;
}


void CThemeTreeListBox::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTreeListBox");
	if (node != 0)
	{
		font.LoadXML(node);

		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("elementWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			elementWidth = atoi(value.c_str());
		}

		child_node = node->first_node("marge");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			marge = atoi(value.c_str());
		}

		child_node = node->first_node("buttonWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			buttonWidth = atoi(value.c_str());
		}

		child_node = node->first_node("buttonHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			buttonHeight = atoi(value.c_str());
		}

		child_node = node->first_node("color");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			color.Set(value);
		}


	}
}

void CThemeTreeListBox::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTreeListBox");
	font.SaveXML(doc, section);
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "elementWidth", to_string(elementWidth)));
	section->append_node(node(doc, "marge", to_string(marge)));
	section->append_node(node(doc, "buttonWidth", to_string(buttonWidth)));
	section->append_node(node(doc, "buttonHeight", to_string(buttonHeight)));
	section->append_node(node(doc, "color", CConvertUtility::ConvertToStdString(color.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeTreeListBox& CThemeTreeListBox::operator=(const CThemeTreeListBox& other)
{
	this->font = other.font;
	this->height = other.height;
	this->width = other.width;
	this->marge = other.marge;
	this->buttonWidth = other.buttonWidth;
	this->buttonHeight = other.buttonHeight;
	this->elementWidth = other.elementWidth;
	this->color = other.color;
	return *this;
}

void CThemeTree::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTree");
	if (node != 0)
	{
		themeTriangle.LoadXML(node);
		themeDelete.LoadXML(node);
		themeSlide.LoadXML(node);
		themeTexte.LoadXML(node);
		themeCheckbox.LoadXML(node);
		themeListbox.LoadXML(node);

		xml_node<> * child_node = node->first_node("rowHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rowHeight = atoi(value.c_str());
		}

		child_node = node->first_node("rowWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rowWidth = atoi(value.c_str());
		}


		child_node = node->first_node("margeX");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			margeX = atoi(value.c_str());
		}

		child_node = node->first_node("bgColorOne");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			bgColorOne.Set(value);
		}

		child_node = node->first_node("bgColorTwo");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			bgColorTwo.Set(value);
		}

		child_node = node->first_node("bgColorActif");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			bgColorActif.Set(value);
		}
        
		child_node = node->first_node("bgColorBackground");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			bgColorBackground.Set(value);
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}
	}
}

void CThemeTree::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTree");
	themeTriangle.SaveXML(doc, section);
	themeDelete.SaveXML(doc, section);
	themeSlide.SaveXML(doc, section);
	themeTexte.SaveXML(doc, section);
	themeCheckbox.SaveXML(doc, section);
	themeListbox.SaveXML(doc, section);
	section->append_node(node(doc, "rowWidth", to_string(rowWidth)));
	section->append_node(node(doc, "rowHeight", to_string(rowHeight)));
	section->append_node(node(doc, "margeX", to_string(margeX)));
	section->append_node(node(doc, "bgColorOne", CConvertUtility::ConvertToStdString(bgColorOne.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "bgColorTwo", CConvertUtility::ConvertToStdString(bgColorTwo.GetAsString(wxC2S_CSS_SYNTAX))));
    section->append_node(node(doc, "bgColorBackground", CConvertUtility::ConvertToStdString(bgColorBackground.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "bgColorActif", CConvertUtility::ConvertToStdString(bgColorActif.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));

	sectionPosition->append_node(section);
}

CThemeTree& CThemeTree::operator=(const CThemeTree& other)
{
	this->themeTriangle = other.themeTriangle;
	this->themeDelete = other.themeDelete;
	this->themeSlide = other.themeSlide;
	this->themeTexte = other.themeTexte;
	this->themeListbox = other.themeListbox;
	this->themeCheckbox = other.themeCheckbox;
	this->rowHeight = other.rowHeight;
	this->rowWidth = other.rowWidth;
	this->margeX = other.margeX;
	this->bgColorOne = other.bgColorOne;
	this->bgColorTwo = other.bgColorTwo;
	this->bgColorActif = other.bgColorActif;
	this->colorBack = other.colorBack;
	return *this;
}

void CThemeScrollBar::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeScrollBar");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("rectangleSize");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectangleSize = atoi(value.c_str());
		}

		child_node = node->first_node("marge");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			marge = atoi(value.c_str());
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("colorBar");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBar.Set(value);
		}

		child_node = node->first_node("colorTriangle");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorTriangle.Set(value);
		}

		child_node = node->first_node("colorBarActif");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBarActif.Set(value);
		}

		child_node = node->first_node("colorTriangleActif");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorTriangleActif.Set(value);
		}

	}
}

void CThemeScrollBar::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeScrollBar");
	section->append_node(node(doc, "rectangleSize", to_string(rectangleSize)));
	section->append_node(node(doc, "marge", to_string(marge)));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBar", CConvertUtility::ConvertToStdString(colorBar.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorTriangle", CConvertUtility::ConvertToStdString(colorTriangle.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBarActif", CConvertUtility::ConvertToStdString(colorBarActif.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorTriangleActif", CConvertUtility::ConvertToStdString(colorTriangleActif.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeScrollBar& CThemeScrollBar::operator=(const CThemeScrollBar& other)
{
	this->rectangleSize = other.rectangleSize;
	this->marge = other.marge;
	this->colorBack = other.colorBack;
	this->colorBar = other.colorBar;
	this->colorTriangle = other.colorTriangle;
	this->colorBarActif = other.colorBarActif;
	this->colorTriangleActif = other.colorTriangleActif;
	this->colorBack = other.colorBack;
	return *this;
}


CThemeFastDrawBar& CThemeFastDrawBar::operator=(const CThemeFastDrawBar& other)
{
	this->firstColor = other.firstColor;
	this->secondColor = other.secondColor;
	this->colorBack = other.colorBack;
	return *this;
}

void CThemeFastDrawBar::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeFastDrawBar");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("firstColor");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			firstColor.Set(value);
		}

		child_node = node->first_node("secondColor");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			secondColor.Set(value);
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("size");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			size = atoi(value.c_str());
		}
	}
}

void CThemeFastDrawBar::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeFastDrawBar");
	section->append_node(node(doc, "firstColor", CConvertUtility::ConvertToStdString(firstColor.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "secondColor", CConvertUtility::ConvertToStdString(secondColor.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "size", to_string(size)));
	sectionPosition->append_node(section);
}

void CThemeSeparationBar::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeSeparationBar");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("firstColor");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			firstColor.Set(value);
		}

		child_node = node->first_node("secondColor");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			secondColor.Set(value);
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("size");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			size = atoi(value.c_str());
		}

	}
}

void CThemeSeparationBar::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeSeparationBar");
	section->append_node(node(doc, "firstColor", CConvertUtility::ConvertToStdString(firstColor.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "secondColor", CConvertUtility::ConvertToStdString(secondColor.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "size", to_string(size)));
	sectionPosition->append_node(section);
}

CThemeSeparationBar& CThemeSeparationBar::operator=(const CThemeSeparationBar& other)
{
	this->firstColor = other.firstColor;
	this->secondColor = other.secondColor;
	this->colorBack = other.colorBack;
	this->colorBack = other.colorBack;
	return *this;
}

void CThemeSplitter::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeSplitter");
	if (node != 0)
	{
		themeSeparation.LoadXML(node);
		themeFast.LoadXML(node);

		xml_node<> * child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}


	}
}

void CThemeSplitter::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeSplitter");
	themeFast.SaveXML(doc, section);
	themeSeparation.SaveXML(doc, section);
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeSplitter& CThemeSplitter::operator=(const CThemeSplitter& other)
{
	this->themeFast = other.themeFast;
	this->themeSeparation = other.themeSeparation;
	this->colorBack = other.colorBack;
	return *this;
}

CThemeTitleBar::CThemeTitleBar()
{
	croixWidth = 15;
	croixHeight = 15;
	height = 30;
	marge = 5;
	colorBack.Set(45, 48, 56);
	colorCross.Set(0, 0, 0);
	font.SetColorFont(wxColour(255, 255, 255));
	font.SetFontSize(14);
}

void CThemeTitleBar::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeTitleBar");
	if (node != 0)
	{
		font.LoadXML(node);

		xml_node<> * child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("marge");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			marge = atoi(value.c_str());
		}

		child_node = node->first_node("croixWidth");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			croixWidth = atoi(value.c_str());
		}

		child_node = node->first_node("croixHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			croixHeight = atoi(value.c_str());
		}

		child_node = node->first_node("colorCross");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorCross.Set(value);
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}
	}
}

void CThemeTitleBar::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeTitleBar");
	font.SaveXML(doc, section);
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "marge", to_string(marge)));
	section->append_node(node(doc, "croixWidth", to_string(croixWidth)));
	section->append_node(node(doc, "croixHeight", to_string(croixHeight)));
	section->append_node(node(doc, "colorCross", CConvertUtility::ConvertToStdString(colorCross.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeTitleBar& CThemeTitleBar::operator=(const CThemeTitleBar& other)
{
	this->colorBack = other.colorBack;
	this->font = other.font;
	this->height = other.height;
	this->marge = other.marge;
	this->croixWidth = other.croixWidth;
	this->croixHeight = other.croixHeight;
	this->colorCross = other.colorCross;
	this->colorBack = other.colorBack;
	return *this;
}

void CThemePane::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemePane");
	if (node != 0)
	{
		themeTitle.LoadXML(node);

		xml_node<> * child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}
	}
}

void CThemePane::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemePane");
	themeTitle.SaveXML(doc, section);
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemePane& CThemePane::operator=(const CThemePane& other)
{
	this->themeTitle = other.themeTitle;
	this->height = other.height;
	this->colorBack = other.colorBack;
	return *this;
}

CThemeSlider::CThemeSlider()
{
	buttonWidth = 17;
    buttonHeight = 17;
	isColorBackground = true;
	rectangleHeight = 10;
	width = 200;
	height = 40;
	colorBack.Set(128, 128, 128);
	rectanglePast.Set(0, 0, 128);
	rectangleNext.Set(48, 128, 254);
    colorActifReplacement.Set(48, 128, 254);
    colorInactifReplacement.Set(255, 255, 255);
	font.SetColorFont(wxColour(255, 255, 255));
	font.SetFontName("Verdana");
	font.SetFontSize(8);
}

CThemeSlider::~CThemeSlider()
{

}

void CThemeSlider::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeSlider");
	if (node != 0)
	{
		font.LoadXML(node);

		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}
		
		child_node = node->first_node("rectangleHeight");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectangleHeight = atoi(value.c_str());
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("rectanglePast");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectanglePast.Set(value);
		}

		child_node = node->first_node("rectangleNext");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectangleNext.Set(value);
		}
        
		child_node = node->first_node("colorActifReplacement");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorActifReplacement.Set(value);
		}
        
		child_node = node->first_node("colorInactifReplacement");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorInactifReplacement.Set(value);
		}
	}
}

void CThemeSlider::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeSlider");
	font.SaveXML(doc, section);
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "rectangleHeight", to_string(rectangleHeight)));
	section->append_node(node(doc, "rectanglePast", CConvertUtility::ConvertToStdString(rectanglePast.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "rectangleNext", CConvertUtility::ConvertToStdString(rectangleNext.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
    section->append_node(node(doc, "colorActifReplacement",CConvertUtility::ConvertToStdString(colorActifReplacement.GetAsString(wxC2S_CSS_SYNTAX))));
    section->append_node(node(doc, "colorInactifReplacement",CConvertUtility::ConvertToStdString(colorInactifReplacement.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeSlider& CThemeSlider::operator=(const CThemeSlider& other)
{
	this->colorBack = other.colorBack;
	this->font = other.font;
	this->rectanglePast = other.rectanglePast;
	this->rectangleHeight = other.rectangleHeight;
	this->width = other.width;
	this->height = other.height;
	this->colorBack = other.colorBack;
    this->colorInactifReplacement = other.colorInactifReplacement;
    this->colorActifReplacement = other.colorActifReplacement;
	return *this;
}

CThemeSliderVideo::CThemeSliderVideo()
{
    buttonPlayWidth = 24;
    buttonPlayHeight = 24;
    buttonPauseWidth = 24;
    buttonPauseHeight = 24;
    buttonVolumeUpWidth = 24;
    buttonVolumeUpHeight = 24;
    buttonVolumeDownWidth = 24;
    buttonVolumeDownHeight = 24;
    buttonSpeakerWidth = 24;
    buttonSpeakerHeight = 24;
    marge = 10;
}

CThemeSliderVideo::~CThemeSliderVideo()
{
    
}

void CThemeSliderVideo::LoadXML(xml_node<> * root_node)
{
    
}

void CThemeSliderVideo::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
    
}

CThemeSliderVideo& CThemeSliderVideo::operator=(const CThemeSliderVideo& other)
{
    return *this;
}

CThemeBitmapInfos::CThemeBitmapInfos()
{
	themeFont.SetFontName("Verdana");
	themeFont.SetFontSize(16);
	themeFont.SetFontQuality(DEFAULT_QUALITY);
	themeFont.SetColorFont(wxColour(255, 255, 255));
	colorBack.Set(61, 65, 73);
	height = 40;
    checkboxHeight = 20;
    checkboxWidth = 20;
}

void CThemeBitmapInfos::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeBitmapInfos");
	if (node != 0)
	{
		themeFont.LoadXML(node);

		xml_node<> * child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}
        
        child_node = node->first_node("checkboxWidth");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxWidth = atoi(value.c_str());
        }
        
        child_node = node->first_node("checkboxHeight");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxHeight = atoi(value.c_str());
        }
	}
}

void CThemeBitmapInfos::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeBitmapInfos");
	themeFont.SaveXML(doc, section);
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "height", to_string(height)));
    section->append_node(node(doc, "checkboxHeight", to_string(checkboxHeight)));
    section->append_node(node(doc, "checkboxWidth", to_string(checkboxWidth)));
	sectionPosition->append_node(section);
}

CThemeBitmapInfos& CThemeBitmapInfos::operator=(const CThemeBitmapInfos& other)
{
	this->themeFont = other.themeFont;
	this->colorBack = other.colorBack;
    this->height = other.height;
    this->checkboxHeight = other.checkboxHeight;
    this->checkboxWidth = other.checkboxWidth;
	return *this;
}

CThemeIcone::CThemeIcone()
{
	colorBack.Set(0, 0, 0);
	colorTop.Set(255, 255, 255);
	colorBottom.Set(255, 255, 255);
	colorSelectTop.Set(128, 128, 128);
	colorSelectBottom.Set(128, 128, 128);
	width = 200;
	height = 200;
	marge = 5;
	font.SetColorFont(wxColour(0, 0, 0));
	font.SetFontName("Verdana");
	font.SetFontSize(10);
	font.SetFontQuality(ANTIALIASED_QUALITY);
    checkboxWidth = 20;
    checkboxHeight = 20;
};

void CThemeIcone::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeIcone");
	if (node != 0)
	{
		font.LoadXML(node);

		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}
        
        child_node = node->first_node("checkboxHeight");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxHeight = atoi(value.c_str());
        }
        
        child_node = node->first_node("checkboxWidth");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxWidth = atoi(value.c_str());
        }

		child_node = node->first_node("marge");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			marge = atoi(value.c_str());
		}

		child_node = node->first_node("showOnlyThumbnail");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			showOnlyThumbnail = atoi(value.c_str());
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("colorTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorTop.Set(value);
		}

		child_node = node->first_node("colorBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBottom.Set(value);
		}

		child_node = node->first_node("colorSelectTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorSelectTop.Set(value);
		}

		child_node = node->first_node("colorSelectBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorSelectBottom.Set(value);
		}
	}
}

void CThemeIcone::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeIcone");
	font.SaveXML(doc, section);
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "height", to_string(height)));
    section->append_node(node(doc, "checkboxWidth", to_string(checkboxWidth)));
    section->append_node(node(doc, "checkboxHeight", to_string(checkboxHeight)));
	section->append_node(node(doc, "showOnlyThumbnail", to_string(showOnlyThumbnail)));
	section->append_node(node(doc, "marge", to_string(marge)));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorTop", CConvertUtility::ConvertToStdString(colorTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBottom",CConvertUtility::ConvertToStdString(colorBottom.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorSelectTop", CConvertUtility::ConvertToStdString(colorSelectTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorSelectBottom", CConvertUtility::ConvertToStdString(colorSelectBottom.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeIcone& CThemeIcone::operator=(const CThemeIcone& other)
{
	this->font = other.font;
	this->showOnlyThumbnail = other.showOnlyThumbnail;
	this->width = other.width;
	this->height = other.height;
	this->marge = other.marge;
	this->colorBack = other.colorBack;
	this->colorTop = other.colorTop;
	this->colorBottom = other.colorBottom;
	this->colorSelectTop = other.colorSelectTop;
	this->colorSelectBottom = other.colorSelectBottom;
	return *this;
}

CThemeInfosSeparationBar::CThemeInfosSeparationBar()
{
    checkboxHeight = 20;
    checkboxWidth = 20;
	colorBack.Set(45, 45, 48);
	colorTop.Set(45, 45, 48);
	colorBottom.Set(45, 45, 48);
	themeFont.SetColorFont(wxColour(255, 255, 255));
	themeFont.SetFontName("Verdana");
	themeFont.SetFontSize(10);
	themeFont.SetFontQuality(ANTIALIASED_QUALITY);
	height = 40;
	calculHeight = 0;
}

void CThemeInfosSeparationBar::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeInfosSeparationBar");
	if (node != 0)
	{
		themeFont.LoadXML(node);

		xml_node<> * child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("colorTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorTop.Set(value);
		}

		child_node = node->first_node("colorBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBottom.Set(value);
		}

        child_node = node->first_node("checkboxHeight");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxHeight = atoi(value.c_str());
        }
        
        child_node = node->first_node("checkboxWidth");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxWidth = atoi(value.c_str());
        }
	}
}

void CThemeInfosSeparationBar::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeInfosSeparationBar");
	themeFont.SaveXML(doc, section);
	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorTop", CConvertUtility::ConvertToStdString(colorTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBottom", CConvertUtility::ConvertToStdString(colorBottom.GetAsString(wxC2S_CSS_SYNTAX))));
    section->append_node(node(doc, "checkboxHeight", to_string(checkboxHeight)));
    section->append_node(node(doc, "checkboxWidth", to_string(checkboxWidth)));
	sectionPosition->append_node(section);
}

CThemeInfosSeparationBar& CThemeInfosSeparationBar::operator=(const CThemeInfosSeparationBar& other)
{
	this->themeFont = other.themeFont;
	this->colorBack = other.colorBack;
	this->colorTop = other.colorTop;
	this->colorBottom = other.colorBottom;
	this->height = other.height;
	this->colorBack = other.colorBack;
	return *this;
}

CThemeThumbnail::CThemeThumbnail()
{
	themeIcone.SetWidth(200);
	themeIcone.SetHeight(200);
    themeIcone.SetCheckboxWidth(20);
    themeIcone.SetCheckboxHeight(20);
	themeIcone.showOnlyThumbnail = false;
	themeIcone.colorBack.Set(0, 0, 0);
	themeIcone.colorTop.Set(255, 255, 255);
	themeIcone.colorBottom.Set(255, 255, 255);
	themeIcone.colorSelectTop.Set(119, 181, 254);
	themeIcone.colorSelectBottom.Set(119, 181, 254);
	colorBack.Set(255, 255, 255);
	themeIcone.font.SetColorFont(wxColour(0, 0, 0));

	/*
	themeIcone.font.SetColorFont(wxColour(255, 255, 255));
	themeIcone.colorBack.Set(74, 74, 74);
	themeIcone.colorTop.Set(74, 74, 74);
	themeIcone.colorBottom.Set(74, 74, 74);
	themeIcone.colorSelectTop.Set(205, 205, 205);
	themeIcone.colorSelectBottom.Set(171, 171, 171);
	*/
	themeSeparation.colorBack.Set(74, 74, 74);
	themeSeparation.colorBack.Set(45, 45, 48);
	themeSeparation.colorTop.Set(45, 45, 48);
	themeSeparation.colorBottom.Set(45, 45, 48);
	themeSeparation.themeFont.SetColorFont(wxColour(255, 255, 255));
	themeSeparation.themeFont.SetFontName("Verdana");
	themeSeparation.themeFont.SetFontSize(10);
	themeSeparation.themeFont.SetFontQuality(ANTIALIASED_QUALITY);
	themeSeparation.SetHeight(40);
}

void CThemeThumbnail::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeThumbnail");
	if (node != 0)
	{
		themeIcone.LoadXML(node);
		themeSeparation.LoadXML(node);

		xml_node<> * child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}
	}
}

void CThemeThumbnail::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeThumbnail");
	themeIcone.SaveXML(doc, section);
	themeSeparation.SaveXML(doc, section);
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeThumbnail& CThemeThumbnail::operator=(const CThemeThumbnail& other)
{
	this->themeSeparation = other.themeSeparation;
	this->themeIcone = other.themeIcone;
	this->colorBack = other.colorBack;
	return *this;
}

CThemeToolBarZoom::CThemeToolBarZoom()
{
    checkboxHeight = 20;
    checkboxWidth = 20;
    font.SetFontName("Verdana");
    font.SetFontSize(16);
    font.SetFontQuality(DEFAULT_QUALITY);
    
	fontActif = font;
	font.SetColorFont(wxColour(255, 255, 255));
	this->colorBack.Set(40, 39, 116);
	fontActif.SetColorFont(wxColour(48, 128, 254));
	height = 30;
}

void CThemeToolBarZoom::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeToolBarZoom");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("Font");
		if (child_node != 0)
		{
			font.LoadXML(child_node);
		}

		child_node = node->first_node("FontActif");
		if (child_node != 0)
		{
			fontActif.LoadXML(child_node);
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}
        
        child_node = node->first_node("checkboxHeight");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxHeight = atoi(value.c_str());
        }
        
        child_node = node->first_node("checkboxWidth");
        if (child_node != 0)
        {
            value = child_node->value();
            nodeName = child_node->name();
            checkboxWidth = atoi(value.c_str());
        }
	}
}

void CThemeToolBarZoom::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeToolBarZoom");
	xml_node<>* sectionFont = node(doc, "Font");
	font.SaveXML(doc, sectionFont);
	section->append_node(sectionFont);
	xml_node<>* sectionFontActif = node(doc, "FontActif");
	fontActif.SaveXML(doc, sectionFontActif);
	section->append_node(sectionFontActif);
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "height", to_string(height)));
    section->append_node(node(doc, "checkboxHeight", to_string(checkboxHeight)));
    section->append_node(node(doc, "checkboxWidth", to_string(checkboxWidth)));
	sectionPosition->append_node(section);
}

CThemeToolBarZoom& CThemeToolBarZoom::operator=(const CThemeToolBarZoom& other)
{
	this->fontActif = other.fontActif;
	this->font = other.font;
	this->colorBack = other.colorBack;
	this->height = other.height;
	return *this;
}

void CThemeToolbarElement::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeToolbarElement");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("tailleX");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			tailleX = atoi(value.c_str());
		}

		child_node = node->first_node("tailleY");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			tailleY = atoi(value.c_str());
		}
	}
}

void CThemeToolbarElement::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeToolbarElement");
	section->append_node(node(doc, "tailleX", to_string(tailleX)));
	section->append_node(node(doc, "tailleY", to_string(tailleY)));
	sectionPosition->append_node(section);
}

CThemeToolbarElement& CThemeToolbarElement::operator=(const CThemeToolbarElement& other)
{
	this->tailleX = other.tailleX;
	this->tailleY = other.tailleY;
	return *this;
}

CThemeToolbarButton::CThemeToolbarButton()
{
	tailleX = 60;
	tailleY = 60;
    replaceColor.Set(255,255,255);
	lineColorTop.Set(222, 222, 222);
	lineColorBottom.Set(156, 156, 156);
	actifTop.Set(255, 255, 255);
	actifBottom.Set(223, 223, 223);
    colorActifReplacement.Set(48, 128, 254);
    replaceColor.Set(255,255,255);
	font.SetFontName("Verdana");
	font.SetFontSize(8);
	font.SetFontQuality(ANTIALIASED_QUALITY);
	font.SetColorFont(wxColour(255, 255, 255));
	rectangleSize =2;
	showButtonOnly = 0;
	marge = 5;
}

void CThemeToolbarButton::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeToolbarButton");
	if (node != 0)
	{
		font.LoadXML(node);

		xml_node<> * child_node = node->first_node("tailleX");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			tailleX = atoi(value.c_str());
		}

		child_node = node->first_node("tailleY");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			tailleY = atoi(value.c_str());
		}

		child_node = node->first_node("rectangleSize");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectangleSize = atoi(value.c_str());
		}

		child_node = node->first_node("showButtonOnly");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			showButtonOnly = atoi(value.c_str());
		}

		child_node = node->first_node("lineColorTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			lineColorTop.Set(value);
		}

		child_node = node->first_node("lineColorBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			lineColorBottom.Set(value);
		}

		child_node = node->first_node("actifTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			actifTop.Set(value);
		}

		child_node = node->first_node("actifBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			actifBottom.Set(value);
		}

		child_node = node->first_node("colorActifReplacement");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorActifReplacement.Set(value);
		}        

		child_node = node->first_node("replaceColor");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			replaceColor.Set(value);
		}
	}
}

void CThemeToolbarButton::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeToolbarButton");
	font.SaveXML(doc, section);
	section->append_node(node(doc, "tailleX", to_string(tailleX)));
	section->append_node(node(doc, "tailleY", to_string(tailleY)));
	section->append_node(node(doc, "rectangleSize", to_string(rectangleSize)));
	section->append_node(node(doc, "showButtonOnly", to_string(showButtonOnly)));
	section->append_node(node(doc, "lineColorTop", CConvertUtility::ConvertToStdString(lineColorTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "lineColorBottom", CConvertUtility::ConvertToStdString(lineColorBottom.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "actifTop", CConvertUtility::ConvertToStdString(actifTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "actifBottom", CConvertUtility::ConvertToStdString(actifBottom.GetAsString(wxC2S_CSS_SYNTAX))));
    section->append_node(node(doc, "colorActifReplacement", CConvertUtility::ConvertToStdString(colorActifReplacement.GetAsString(wxC2S_CSS_SYNTAX))));
    section->append_node(node(doc, "replaceColor", CConvertUtility::ConvertToStdString(replaceColor.GetAsString(wxC2S_CSS_SYNTAX))));
	sectionPosition->append_node(section);
}

CThemeToolbarButton& CThemeToolbarButton::operator=(const CThemeToolbarButton& other)
{
	this->font = other.font;
	this->tailleX = other.tailleX;
	this->tailleY = other.tailleY;
	this->rectangleSize = other.rectangleSize;
	this->showButtonOnly = other.showButtonOnly;
	this->lineColorTop = other.lineColorTop;
	this->lineColorBottom = other.lineColorBottom;
	this->actifTop = other.actifTop;
	this->actifBottom = other.actifBottom;
    this->replaceColor = other.replaceColor;
	return *this;
}

CThemeToolbarTexte::CThemeToolbarTexte()
{
	tailleX = 100;
	tailleY = 100;
	actifTop.Set(48, 48, 48);
	actifBottom.Set(128, 128, 128);
	rectTop.Set(222, 222, 222);
	rectBottom.Set(156, 156, 156);
	rectangleSize = 4;

	font.SetFontName("Verdana");
	font.SetFontSize(10);
	font.SetFontQuality(ANTIALIASED_QUALITY);
	font.SetColorFont(wxColour(255, 255, 255));

	fontActif.SetFontName("Verdana");
	fontActif.SetFontSize(10);
	fontActif.SetFontQuality(ANTIALIASED_QUALITY);
	fontActif.SetColorFont(wxColour(255, 255, 255));
}

void CThemeToolbarTexte::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeToolbarTexte");
	if (node != 0)
	{
		xml_node<> * child_node = node->first_node("Font");
		if (child_node != 0)
		{
			font.LoadXML(child_node);
		}

		child_node = node->first_node("FontActif");
		if (child_node != 0)
		{
			fontActif.LoadXML(child_node);
		}

		child_node = node->first_node("tailleX");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			tailleX = atoi(value.c_str());
		}

		child_node = node->first_node("tailleY");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			tailleY = atoi(value.c_str());
		}

		child_node = node->first_node("rectangleSize");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectangleSize = atoi(value.c_str());
		}

		child_node = node->first_node("rectTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectTop.Set(value);
		}

		child_node = node->first_node("rectBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			rectBottom.Set(value);
		}

		child_node = node->first_node("actifTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			actifTop.Set(value);
		}

		child_node = node->first_node("actifBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			actifBottom.Set(value);
		}

		child_node = node->first_node("colorBack");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBack.Set(value);
		}
	}
}

void CThemeToolbarTexte::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeToolbarTexte");

	xml_node<>* sectionFont = node(doc, "Font");
	font.SaveXML(doc, sectionFont);
	section->append_node(sectionFont);

	xml_node<>* sectionFontActif = node(doc, "FontActif");
	fontActif.SaveXML(doc, sectionFontActif);
	section->append_node(sectionFontActif);

	section->append_node(node(doc, "tailleX", to_string(tailleX)));
	section->append_node(node(doc, "tailleY", to_string(tailleY)));
	section->append_node(node(doc, "rectangleSize", to_string(rectangleSize)));
	section->append_node(node(doc, "colorBack",CConvertUtility::ConvertToStdString(colorBack.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "actifTop", CConvertUtility::ConvertToStdString(actifTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "actifBottom", CConvertUtility::ConvertToStdString(actifBottom.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "rectTop", CConvertUtility::ConvertToStdString(rectTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "rectBottom", CConvertUtility::ConvertToStdString(rectBottom.GetAsString(wxC2S_CSS_SYNTAX))));

	sectionPosition->append_node(section);

}

CThemeToolbarTexte& CThemeToolbarTexte::operator=(const CThemeToolbarTexte& other)
{
	this->font = other.font;
	this->fontActif = other.fontActif;
	this->colorBack = other.colorBack;
	this->actifTop = other.actifTop;
	this->actifBottom = other.actifBottom;
	this->rectTop = other.rectTop;
	this->rectBottom = other.rectBottom;
	this->rectangleSize = other.rectangleSize;
	this->tailleX = other.tailleX;
	this->tailleY = other.tailleY;
	return *this;
}

CThemeToolbar::CThemeToolbar()
{
	width = 60;
	height = 60;
	margeX = 5;
	margeY = 5;
	position = 0;
	colorBack.Set(30, 30, 30);
	//colorTop.Set(74, 74, 74);
    colorTop.Set(50, 54, 63);
	colorBottom.Set(50, 54, 63);
    replaceColor.Set(255,255,255);
};

void CThemeToolbar::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeToolbar");
	if (node != 0)
	{
		button.LoadXML(node);
		texte.LoadXML(node);
		slider.LoadXML(node);

		xml_node<> * child_node = node->first_node("width");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			width = atoi(value.c_str());
		}

		child_node = node->first_node("height");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			height = atoi(value.c_str());
		}

		child_node = node->first_node("colorTop");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorTop.Set(value);
		}

		child_node = node->first_node("colorBottom");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			colorBottom.Set(value);
		}
        
		child_node = node->first_node("replaceColor");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			replaceColor.Set(value);
		}

		child_node = node->first_node("margeX");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			margeX = atoi(value.c_str());
		}

		child_node = node->first_node("margeY");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			margeY = atoi(value.c_str());
		}

		child_node = node->first_node("position");
		if (child_node != 0)
		{
			value = child_node->value();
			nodeName = child_node->name();
			position = atoi(value.c_str());
		}
	}
}

void CThemeToolbar::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeToolbar");
	button.SaveXML(doc, section);
	texte.SaveXML(doc, section);
	slider.SaveXML(doc, section);

	section->append_node(node(doc, "height", to_string(height)));
	section->append_node(node(doc, "width", to_string(width)));
	section->append_node(node(doc, "colorTop", CConvertUtility::ConvertToStdString(colorTop.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "colorBottom", CConvertUtility::ConvertToStdString(colorBottom.GetAsString(wxC2S_CSS_SYNTAX))));
    section->append_node(node(doc, "replaceColor", CConvertUtility::ConvertToStdString(replaceColor.GetAsString(wxC2S_CSS_SYNTAX))));
	section->append_node(node(doc, "margeX", to_string(margeX)));
	section->append_node(node(doc, "margeY", to_string(margeY)));
	section->append_node(node(doc, "position", to_string(position)));

	sectionPosition->append_node(section);
}

CThemeToolbar& CThemeToolbar::operator=(const CThemeToolbar& other)
{
	this->colorTop = other.colorTop;
	this->colorBottom = other.colorBottom;
	this->width = other.width;
	this->height = other.height;
	this->margeX = other.margeX;
	this->margeY = other.margeY;
	this->position = other.position;
	this->button = other.button;
	this->texte = other.texte;
	this->slider = other.slider;
    this->replaceColor = other.replaceColor;
	return *this;
}

////////////////////////////////


CThemeIntroLogo::CThemeIntroLogo()
{
	colorBack.Set(30, 30, 30);
	font.SetFontName("Verdana");
	font.SetFontSize(30);
	font.SetFontQuality(ANTIALIASED_QUALITY);
	font.SetColorFont(wxColour(255, 255, 255));
};

void CThemeIntroLogo::LoadXML(xml_node<> * root_node)
{
	wxString value ;
	wxString nodeName;
	xml_node<> * node = root_node->first_node("ThemeIntroLogo");
	if (node != 0)
	{
		font.LoadXML(node);
	}
}

void CThemeIntroLogo::SaveXML(xml_document<> & doc, xml_node<>* sectionPosition)
{
	xml_node<>* section = node(doc, "ThemeToolbar");
	font.SaveXML(doc, section);
	sectionPosition->append_node(section);
}

CThemeIntroLogo& CThemeIntroLogo::operator=(const CThemeIntroLogo& other)
{
	this->font = other.font;
	return *this;
}

