#pragma once
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
using namespace rapidxml;

class CThemeXml
{
public:

    CThemeXml(){
        
    }; 
    virtual ~CThemeXml(){
        
    };
	virtual void LoadXML(xml_node<> * root_node) = 0;
	virtual void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition) = 0;

    float CalculRatioX();
    float CalculRatioY();
    int CalculElementSizeForScreen(const int &elementSize, const bool &width = true);
    
protected:
	// create a node
	xml_node<>*  node(xml_document<> & doc, const wxString & name, const wxString & content = "");

	// allocate the given string from xml document memory
	// to ensure that it says alive for the duration of xml
	// operations
	const char* stralloc(xml_document<> & doc, const wxString & str);
};

class CTheme : public CThemeXml
{
public:

	CTheme();
	virtual ~CTheme();

	virtual void LoadXML(xml_node<> * root_node);

	virtual void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);
    
    

	CTheme& operator=(const CTheme& other);

	wxColour colorBack;

};

class CThemeBitmapWindow : public CTheme
{
public:

	CThemeBitmapWindow();
	~CThemeBitmapWindow();

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeBitmapWindow& operator=(const CThemeBitmapWindow& other);

	wxColour colorFullscreen;
	wxColour colorScreen;

};

class CThemeFolder : public CTheme
{
public:

	CThemeFolder();
	~CThemeFolder(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeFolder& operator=(const CThemeFolder& other);

	wxColour colorFont;
};


#define RENDERFONT_LEFT 1
#define RENDERFONT_CENTER 2
#define RENDERFONT_RIGHT 3

class CThemeFont : public CTheme
{
public:
	CThemeFont();
	~CThemeFont(){};

	CThemeFont& operator=(const CThemeFont& other);

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

    int GetBold()
    {
        return bold;
    }
    
    void SetBold(const int & bold)
    {
        this->bold = bold;
    }
    
    int GetPositionFont()
    {
        return positionFont;
    }
    
    void SetPositionFont(const int & positionFont)
    {
        this->positionFont = positionFont;
    }
    
    wxColour GetColorFont()
    {
        return colorFont;
    }
    
    void SetColorFont(const wxColour & colorFont)
    {
        this->colorFont = colorFont;
    }
    
    wxString GetFontName()
    {
        return fontName;
    }
    
    void SetFontName(const wxString &fontName)
    {
        this->fontName = fontName;
    }
    
    int GetFontRealSize()
    {
        return fontSize;
    }    
    
    int GetFontSize()
    {
        return CalculElementSizeForScreen(fontSize);
    }
    
    void SetFontSize(const int & fontSize)
    {
        this->fontSize = fontSize;
    }
    
    int GetFontMarge()
    {
        return CalculElementSizeForScreen(marge);
    }
    
    void SetFontMarge(const int & marge)
    {
        this->marge = marge;
    }
    
    int GetFontQuality()
    {
        return fontQuality;
    }
    
    void SetFontQuality(const int & fontQuality)
    {
        this->fontQuality = fontQuality;
    }
    
private:
    
	int bold;
	int positionFont;
	wxColour colorFont;
	wxString fontName;
	int fontSize;
	int marge;
	int fontQuality;
};

class CThemeTreeElement : public CThemeXml
{
public:
	CThemeTreeElement();

	virtual ~CThemeTreeElement()
	{
	};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeTreeElement& operator=(const CThemeTreeElement& other);
	
    int GetWidth()
    {
        return CalculElementSizeForScreen(width);
        //return width;
    }
    
    int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
        //return height;
    }
    
    void SetWidth(const int &width)
    {
        this->width = width;
    }
    
    void SetHeight(const int &height)
    {
        this->height = height;
    }
	
    wxColour color;

protected:
    int width;
    int height;
};

class CThemeTreeTriangle : public CThemeTreeElement
{
public:
	CThemeTreeTriangle();

	~CThemeTreeTriangle()
	{
	};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeTreeTriangle& operator=(const CThemeTreeTriangle& other);

    int GetElementWidth()
    {
        return CalculElementSizeForScreen(elementWidth);
    }
    
    int GetElementHeight()
    {
        return CalculElementSizeForScreen(elementHeight, false);
    }
    
    int GetMarge()
    {
        return CalculElementSizeForScreen(marge);
    }
    
    void SetMarge(const int &marge)
    {
        this->marge = marge;
    }
    
    void SetElementWidth(const int &width)
    {
        this->elementWidth = width;
    }
    
    void SetElementHeight(const int &elementHeight)
    {
        this->elementHeight = height;
    }
    
    
    
private:
    
	int elementHeight;
	int elementWidth;
	int marge;
};

class CThemeTreeDelete : public CThemeTreeElement
{
public:
	CThemeTreeDelete();

	~CThemeTreeDelete(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeTreeDelete& operator=(const CThemeTreeDelete& other);

    int GetCroixWidth()
    {
        return CalculElementSizeForScreen(croixWidth);
    }
    
    int GetCroixHeight()
    {
        return CalculElementSizeForScreen(croixHeight, false);
    }
    
   
    void SetCroixWidth(const int &width)
    {
        this->croixWidth = width;
    }
    
    void SetCroixHeight(const int &elementHeight)
    {
        this->croixHeight = height;
    }
    
    wxColour crossColor;
	wxColour colorBackActif;
    int position;
    
private:
    
	int croixWidth;
	int croixHeight;
	
};

class CThemeTreeSlide : public CThemeTreeElement
{
public:
	CThemeTreeSlide();
	~CThemeTreeSlide();

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeTreeSlide& operator=(const CThemeTreeSlide& other);

	CThemeFont font;

	wxColour rectanglePast;
	wxColour rectangleNext;
    
    float GetRatio()
    {
        return CalculRatioX();
    }
    
    int GetElementWidth()
    {
        return CalculElementSizeForScreen(elementWidth);
    }
    
    int GetMarge()
    {
        return CalculElementSizeForScreen(marge, false);
    }
    
    void SetMarge(const int &marge)
    {
        this->marge = marge;
    }
   
    void SetElementWidth(const int &width)
    {
        this->elementWidth = width;
    }
    
    int GetButtonWidth()
    {
        return CalculElementSizeForScreen(buttonWidth);
    }
    
    int GetButtonHeight()
    {
        return CalculElementSizeForScreen(buttonHeight);
    }
    
private:
    
    int buttonWidth;
    int buttonHeight;
    int marge;
    int elementWidth;
};

class CThemeTreeTexte : public CThemeTreeElement
{
public:
	CThemeTreeTexte();

	~CThemeTreeTexte()
	{
	};

	CThemeTreeTexte& operator=(const CThemeTreeTexte& other);

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeFont font;
};

class CThemeTreeCheckBox : public CThemeTreeElement
{
public:
	CThemeTreeCheckBox()
	{
        margeX = 3;
        margeY = 3;
		elementWidth = width = 15;
		elementHeight = height = 15;
	    checkBoxWidth = 14;
		checkBoxHeight = 14;
	};
	~CThemeTreeCheckBox(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeTreeCheckBox& operator=(const CThemeTreeCheckBox& other);
    
    int GetCheckBoxWidth()
    {
        return CalculElementSizeForScreen(checkBoxWidth);
    }
    
    int GetCheckBoxHeight()
    {
        return CalculElementSizeForScreen(checkBoxHeight, false);
    }

    int GetMargeX()
    {
        return CalculElementSizeForScreen(margeX);
    }
    
    int GetMargeY()
    {
        return CalculElementSizeForScreen(margeY);
    }
    
    void SetMargeX(int marge)
    {
        margeX = marge;
    }
    
    void SetMargeY(int marge)
    {
        margeY = marge;
    }
    
    int GetElementWidth()
    {
        return CalculElementSizeForScreen(elementWidth);
    }
    
    int GetElementHeight()
    {
        return CalculElementSizeForScreen(elementHeight, false);
    }
    
    void SetElementWidth(const int &width)
    {
        this->elementWidth = width;
    }
    
    void SetElementHeight(const int &elementHeight)
    {
        this->elementHeight = height;
    }
    
private:
	int elementHeight;
	int elementWidth;
    int margeX;
    int margeY;
    int checkBoxWidth;
    int checkBoxHeight;
};

class CThemeTreeListBox : public CThemeTreeElement
{
public:
	CThemeTreeListBox()
	{
		height = 20;
		width = 250;
		elementWidth = 250;
		font.SetColorFont(wxColour(255, 255, 255));
		font.SetFontName("Verdana");
		font.SetFontSize(12);
		buttonWidth = 17;
		buttonHeight = 17;
		marge = 4;
	};
	~CThemeTreeListBox(){};

	CThemeFont font;

    void SetMarge(const int &marge)
    {
        this->marge = marge;
    }

	int GetMarge()
    {
        return CalculElementSizeForScreen(marge, false);
    }

    int GetButtonWidth()
    {
        return CalculElementSizeForScreen(buttonWidth);
    }
    
    int GetButtonHeight()
    {
        return CalculElementSizeForScreen(buttonHeight);
    }

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeTreeListBox& operator=(const CThemeTreeListBox& other);
    
   
    int GetElementWidth()
    {
        return CalculElementSizeForScreen(elementWidth);
    }
    
    void SetElementWidth(const int &width)
    {
        this->elementWidth = width;
    }

    
private:

    int buttonWidth;
    int buttonHeight;
    int marge;
    int elementWidth;
};

class CThemeTree : public CTheme
{
public:
	CThemeTree()
	{
		bgColorOne = wxColour(29, 29, 29);
		bgColorActif = wxColour(255, 255, 255);
		bgColorTwo = wxColour(74, 74, 74);
#ifndef WIN32
		bgColorBackground = wxColour(255, 255, 255, 255);
#else
		bgColorBackground = wxColour(255, 255, 255, 0);
#endif
		margeX = 17;
		rowHeight = 20;
		rowWidth = 20;
	}

	~CThemeTree()
	{
	}

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeTree& operator=(const CThemeTree& other);
    
    int GetRowWidth()
    {
        return CalculElementSizeForScreen(rowWidth);
    }
    
    int GetRowHeight()
    {
        return CalculElementSizeForScreen(rowHeight, false);
    }
    
    int GetMargeX()
    {
        return CalculElementSizeForScreen(margeX);
    }
    
    void SetMargeX(const int &marge)
    {
        this->margeX = marge;
    }
    
    void SetRowWidth(const int &width)
    {
        this->rowWidth = width;
    }
    
    void SetRowHeight(const int &elementHeight)
    {
        this->rowHeight = elementHeight;
    }

	CThemeTreeTriangle themeTriangle;
	CThemeTreeDelete themeDelete;
	CThemeTreeSlide themeSlide;
	CThemeTreeTexte themeTexte;
	CThemeTreeListBox themeListbox;
	CThemeTreeCheckBox themeCheckbox;
	wxColour bgColorOne;
	wxColour bgColorTwo;
	wxColour bgColorActif;
    wxColour bgColorBackground;
    
private:
	int rowHeight;
	int rowWidth;
	int margeX;
};

class CThemeScrollBar : public CTheme
{
public:
	CThemeScrollBar()
	{
		rectangleSize = 10;
		colorBack = wxColour(29, 29, 29);
		colorTriangle = wxColour(74, 74, 74);
		colorTriangleActif = wxColour(128, 128, 128);
		colorBar = wxColour(74, 74, 74);
		colorBarActif = wxColour(128, 128, 128);
		marge = 2;
	};
	~CThemeScrollBar()
	{
	};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeScrollBar& operator=(const CThemeScrollBar& other);

    int GetMarge()
    {
        return CalculElementSizeForScreen(marge);
    }
    
    void SetMarge(const int &marge)
    {
        this->marge = marge;
    }
    
    int GetRectangleSize()
    {
        return CalculElementSizeForScreen(rectangleSize);
    }
    
    void SetRectangleSize(const int &rectangleSize)
    {
        this->rectangleSize = rectangleSize;
    }

	wxColour colorBar;
	wxColour colorTriangle;
	wxColour colorBarActif;
	wxColour colorTriangleActif;
    
private:
    int rectangleSize;
    int marge;
};

class CThemeFastDrawBar : public CTheme
{
public:
	CThemeFastDrawBar()
	{
		firstColor = wxColour(200, 200, 200);
		secondColor = wxColour(255, 255, 255);
		size = 4;
	};
	~CThemeFastDrawBar(){};

	CThemeFastDrawBar& operator=(const CThemeFastDrawBar& other);

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	wxColour firstColor;
	wxColour secondColor;
	int size;
};

class CThemeSeparationBar : public CTheme
{
public:
	CThemeSeparationBar()
	{
		firstColor = wxColour(200, 200, 200);
		secondColor = wxColour(255, 255, 255);
		size = 10;
	};
	~CThemeSeparationBar(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeSeparationBar& operator=(const CThemeSeparationBar& other);

	wxColour firstColor;
	wxColour secondColor;
	int size;
};

class CThemeSplitter : public CTheme
{
public:
	CThemeSplitter()
	{
	};
	~CThemeSplitter(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeSplitter& operator=(const CThemeSplitter& other);

	CThemeFastDrawBar themeFast;
	CThemeSeparationBar themeSeparation;
};

class CThemeTitleBar : public CTheme
{
public:
	CThemeTitleBar();

	~CThemeTitleBar(){}

	void LoadXML(xml_node<> * root_node);
	
	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);
	
	CThemeTitleBar& operator=(const CThemeTitleBar& other);

	CThemeFont font;

	wxColour colorCross;
    
    int GetCroixWidth()
    {
        return CalculElementSizeForScreen(croixWidth);
    }
    
    int GetCroixHeight()
    {
        return CalculElementSizeForScreen(croixHeight, false);
    }
    
    int GetMarge()
    {
        return CalculElementSizeForScreen(marge);
    }
    
    void SetMarge(const int &marge)
    {
        this->marge = marge;
    }
    
    void SetCroixWidth(const int &width)
    {
        this->croixWidth = width;
    }
    
    void SetCroixHeight(const int &elementHeight)
    {
        this->croixHeight = height;
    }
    
    int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
    }
       
    void SetHeight(const int &height)
    {
        this->height = height;
    }
    
private:
    int height;
    int marge;
    int croixWidth;
    int croixHeight;
};

class CThemePane : public CTheme
{
public:
	CThemePane()
	{
		height = 20;
	}
	~CThemePane(){}
    
    int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
    }

    void SetHeight(const int &height)
    {
        this->height = height;
    }

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);
	
	CThemePane& operator=(const CThemePane& other);

	CThemeTitleBar themeTitle;
    
private:
	int height;
};

class CThemeSlider : public CTheme
{
public:

	CThemeSlider();

	virtual ~CThemeSlider();

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeSlider& operator=(const CThemeSlider& other);

	CThemeFont font;
	wxColour rectanglePast;
	wxColour rectangleNext;
    wxColor colorActifReplacement;
    wxColor colorInactifReplacement;
	wxBitmap bitmapBackground;
	bool isColorBackground;
    

    
    void SetHeight(const int &height)
    {
        this->height = height;
    }
    
    int GetWidth()
    {
        return CalculElementSizeForScreen(width);
    }
    
    int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
    }
    
    void SetWidth(const int &width)
    {
        this->width = width;
    }
    
    int GetRectangleHeight()
    {
        return CalculElementSizeForScreen(rectangleHeight, false);
    }
    
    void SetRectangleHeight(const int &rectangleHeight)
    {
        this->rectangleHeight = rectangleHeight;
    }
    
    float GetRatio()
    {
        return CalculRatioX();
    }
    
    int GetButtonWidth()
    {
        return CalculElementSizeForScreen(buttonWidth);
    }
    
    int GetButtonHeight()
    {
        return CalculElementSizeForScreen(buttonHeight, false);
    }
    
private:
    int buttonWidth;
    int buttonHeight;
    int rectangleHeight;
    int width;
    int height;
};

class CThemeSliderVideo : public CThemeSlider
{
public:
    
    CThemeSliderVideo();
    
    ~CThemeSliderVideo();
    
    void LoadXML(xml_node<> * root_node);
    
    void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);
    
    CThemeSliderVideo& operator=(const CThemeSliderVideo& other);
    
    int GetButtonPlayWidth()
    {
        return CalculElementSizeForScreen(buttonPlayWidth);
    }
    
    int GetButtonPlayHeight()
    {
        return CalculElementSizeForScreen(buttonPlayHeight, false);
    }
    
    int GetButtonPauseWidth()
    {
        return CalculElementSizeForScreen(buttonPauseWidth);
    }
    
    int GetButtonPauseHeight()
    {
        return CalculElementSizeForScreen(buttonPauseHeight, false);
    }
    
    int GetButtonVolumeUpWidth()
    {
        return CalculElementSizeForScreen(buttonVolumeUpWidth);
    }
    
    int GetButtonVolumeUpHeight()
    {
        return CalculElementSizeForScreen(buttonVolumeUpHeight, false);
    }
    
    int GetButtonVolumeDownWidth()
    {
        return CalculElementSizeForScreen(buttonVolumeDownWidth);
    }
    
    int GetButtonVolumeDownHeight()
    {
        return CalculElementSizeForScreen(buttonVolumeDownHeight, false);
    }

    int GetButtonSpeakerWidth()
    {
        return CalculElementSizeForScreen(buttonSpeakerWidth);
    }
    
    int GetButtonSpeakerHeight()
    {
        return CalculElementSizeForScreen(buttonSpeakerHeight, false);
    }
    
    int GetMarge()
    {
        return CalculElementSizeForScreen(marge);
    }
    
private:
    
    int buttonPlayWidth;
    int buttonPlayHeight;
    int buttonPauseWidth;
    int buttonPauseHeight;
    int buttonVolumeUpWidth;
    int buttonVolumeUpHeight;
    int buttonVolumeDownWidth;
    int buttonVolumeDownHeight;
    int buttonSpeakerWidth;
    int buttonSpeakerHeight;
    int marge;
};

class CThemeBitmapInfos : public CTheme
{
public:
	CThemeBitmapInfos();

	~CThemeBitmapInfos(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);
	
	CThemeBitmapInfos& operator=(const CThemeBitmapInfos& other);

    int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
    }
    
    void SetHeight(const int &height)
    {
        this->height = height;
    }
    
	CThemeFont themeFont;
    
    
    void SetCheckboxWidth(const int &width)
    {
        this->checkboxWidth = width;
    }
    
    void SetCheckboxHeight(const int &height)
    {
        this->checkboxHeight = height;
    }
    
    int GetCheckboxWidth()
    {
        return CalculElementSizeForScreen(checkboxWidth);
    }
    
    int GetCheckboxHeight()
    {
        return CalculElementSizeForScreen(checkboxHeight,false);
    }

    
private:
    int checkboxWidth;
    int checkboxHeight;
	int height;
};

class CThemeIcone : public CTheme
{
public:
	CThemeIcone();

	~CThemeIcone(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeIcone& operator=(const CThemeIcone& other);
    
    int GetMarge()
    {
        return CalculElementSizeForScreen(marge);
    }
    
    void SetMarge(const int & marge)
    {
        this->marge = marge;
    }

    const int GetWidth()
    {
        return CalculElementSizeForScreen(width);
    }
    
    const int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
    }
    
    const int GetRealWidth()
    {
        return width;
    }
    
    const int GetRealHeight()
    {
        return height;
    }
    
    void SetWidth(const int &width)
    {
        this->width = width;
    }
    
    void SetHeight(const int &height)
    {
        this->height = height;
    }
    
    void SetCheckboxWidth(const int &width)
    {
        this->checkboxWidth = width;
    }
    
    void SetCheckboxHeight(const int &height)
    {
        this->checkboxHeight = height;
    }
    
    int GetCheckboxWidth()
    {
        return CalculElementSizeForScreen(checkboxWidth);
    }
    
    int GetCheckboxHeight()
    {
        return CalculElementSizeForScreen(checkboxHeight,false);
    }
    
	CThemeFont font;
	int showOnlyThumbnail;

	wxColour colorBack;
	wxColour colorTop;
	wxColour colorBottom;
	wxColour colorSelectTop;
	wxColour colorSelectBottom;
    
private:
    int width;
    int height;
    int marge;
    int checkboxWidth;
    int checkboxHeight;
};

class CThemeInfosSeparationBar : public CTheme
{
public:

	CThemeInfosSeparationBar();

	~CThemeInfosSeparationBar()
	{

	}
    
    const int & GetHeight()
    {
        calculHeight = CalculElementSizeForScreen(height, false);
        return calculHeight;
    }
    
    void SetHeight(const int &height)
    {
        this->height = height;
    }
    
    int GetCheckboxHeight()
    {
        return CalculElementSizeForScreen(checkboxHeight, false);
    }
    
    int GetCheckboxWidth()
    {
        return CalculElementSizeForScreen(checkboxWidth);
    }
    
    void SetCheckboxHeight(const int &checkboxHeight)
    {
        this->checkboxHeight = checkboxHeight;
    }
    
    void SetCheckboxWidth(const int &checkboxWidth)
    {
        this->checkboxWidth = checkboxWidth;
    }

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeInfosSeparationBar& operator=(const CThemeInfosSeparationBar& other);

	CThemeFont themeFont;

	wxColour colorTop;
	wxColour colorBottom;
    
private:
    
    int checkboxHeight;
    int checkboxWidth;
    int height;
    int calculHeight;
};

class CThemeThumbnail : public CTheme
{
public:
	CThemeThumbnail();
	~CThemeThumbnail(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeThumbnail& operator=(const CThemeThumbnail& other);

	CThemeInfosSeparationBar themeSeparation;
	CThemeIcone themeIcone;
};

class CThemeToolBarZoom : public CTheme
{
public:

	CThemeToolBarZoom();

	~CThemeToolBarZoom()
	{

	}

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeToolBarZoom& operator=(const CThemeToolBarZoom& other);

    int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
    }
    
    void SetHeight(const int &height)
    {
        this->height = height;
    }
    
    int GetCheckboxHeight()
    {
        return CalculElementSizeForScreen(checkboxHeight, false);
    }
    
    int GetCheckboxWidth()
    {
        return CalculElementSizeForScreen(checkboxWidth);
    }
    
    void SetCheckboxHeight(const int &checkboxHeight)
    {
        this->checkboxHeight = checkboxHeight;
    }
    
    void SetCheckboxWidth(const int &checkboxWidth)
    {
        this->checkboxWidth = checkboxWidth;
    }
    
	CThemeFont font;
	CThemeFont fontActif;
    
private:
	int height;
    int checkboxHeight;
    int checkboxWidth;
};


class CThemeToolbarElement : public CThemeXml
{
public:
	CThemeToolbarElement(){};
	virtual ~CThemeToolbarElement(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeToolbarElement& operator=(const CThemeToolbarElement& other);

    int GetTailleX()
    {
        return CalculElementSizeForScreen(tailleX);
    }
    
    int GetRealTailleX()
    {
        return tailleX;
    }
    
    void SetTailleX(const int &tailleX)
    {
        this->tailleX = tailleX;
    }
    
    int GetTailleY()
    {
        return CalculElementSizeForScreen(tailleY, false);
    }
    
    int GetRealTailleY()
    {
        return tailleY;
    }
    
    void SetTailleY(const int &tailleY)
    {
        this->tailleY = tailleY;
    }
    
protected:
    
	int tailleX;
	int tailleY;
};

class CThemeToolbarButton : public CThemeToolbarElement
{

public:
	CThemeToolbarButton();

	~CThemeToolbarButton(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeToolbarButton& operator=(const CThemeToolbarButton& other);

    
    int GetRectangleSize()
    {
        return CalculElementSizeForScreen(rectangleSize);
    }
    
    void SetRectangleSize(const int &rectangleSize)
    {
        this->rectangleSize = rectangleSize;
    }
    
    int GetMarge()
    {
        return CalculElementSizeForScreen(marge);
    }
    
    void SetMarge(const int & marge)
    {
        this->marge = marge;
    }
    
	CThemeFont font;
	wxColour lineColorTop;
	wxColour lineColorBottom;
	wxColour actifTop;
	wxColour actifBottom;
    wxColour colorActifReplacement;
    wxColour replaceColor;
    int showButtonOnly;
    
private:
	int rectangleSize;
	
	int marge;
};

class CThemeToolbarTexte : public CThemeToolbarElement
{
public:
	CThemeToolbarTexte();

	~CThemeToolbarTexte(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeToolbarTexte& operator=(const CThemeToolbarTexte& other);

    int GetRectangleSize()
    {
        return CalculElementSizeForScreen(rectangleSize);
    }
    
    void SetRectangleSize(const int &rectangleSize)
    {
        this->rectangleSize = rectangleSize;
    }
    
	CThemeFont font;
	CThemeFont fontActif;
	wxColour colorBack;
	wxColour actifTop;
	wxColour actifBottom;
	wxColour rectTop;
	wxColour rectBottom;
    
private:
	int rectangleSize;
};

class CThemeToolbar : public CTheme
{
public:
	CThemeToolbar();

	~CThemeToolbar(){};

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeToolbar& operator=(const CThemeToolbar& other);

	wxColour colorTop;
	wxColour colorBottom;
    wxColour replaceColor;

	int position;
	int isVertical;

	CThemeToolbarButton button;
	CThemeToolbarTexte texte;
	CThemeSlider slider;
    
    int GetMargeX()
    {
        return CalculElementSizeForScreen(margeX);
    }
    
    void SetMargeX(const int & margeX)
    {
        this->margeX = margeX;
    }
    
    int GetMargeY()
    {
        return CalculElementSizeForScreen(margeY, false);
    }
    
    void SetMargeY(const int & margeY)
    {
        this->margeY = margeY;
    }
    
    int GetWidth()
    {
        return CalculElementSizeForScreen(width);
    }
    
    int GetHeight()
    {
        return CalculElementSizeForScreen(height, false);
    }
    
    void SetWidth(const int &width)
    {
        this->width = width;
    }
    
    void SetHeight(const int &height)
    {
        this->height = height;
    }
    
private:
    int height;
    int width;
    int margeX;
    int margeY;
};


class CThemeIntroLogo : public CTheme
{
public:

	CThemeIntroLogo();

	~CThemeIntroLogo()
	{

	}

	void LoadXML(xml_node<> * root_node);

	void SaveXML(xml_document<> & doc, xml_node<>* sectionPosition);

	CThemeIntroLogo& operator=(const CThemeIntroLogo& other);

	CThemeFont font;
};