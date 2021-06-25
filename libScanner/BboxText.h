#pragma once
class CImageLoadingFormat;

class ChOcrElement
{
public:
	wxString itemClass;
};

class ChOcrElementGraphic : public ChOcrElement
{
public:
	ChOcrElementGraphic(): numImage(0), picture(nullptr)
	{
		itemClass = "ocr_graphic";
	}

	int numImage;
	CImageLoadingFormat* picture;
	wxRect rect;
};

class ChOcrElementPar : public ChOcrElement
{
public:
	ChOcrElementPar()
	{
		itemClass = "ocr_par";
	}

	wxRect rect;
};

class ChOcrElementLine : public ChOcrElement
{
public:
	ChOcrElementLine(): id(0), confidence(0), numLigne(0), selected(false)
	{
		itemClass = "ocr_line";
		fontBold = false;
		fontItalic = false;
		fontSize = 10;
		fontFamily = "Times";
	}

	int id;
	wxRect rect;
	wxString label;
	wxString fontFamily;
	bool fontBold;
	bool fontItalic;
	double fontSize;
	int confidence;
	int numLigne;
	bool selected;
};

class ChOcrElementLineBreak : public ChOcrElement
{
public:
	ChOcrElementLineBreak()
	{
		itemClass = "ocr_line_break";
	}
};

/*
class BBoxText
{
public:
	int id;
	wxRect rect;
	wxString label;
	wxString fontFamily;
	bool fontBold;
	bool fontItalic;
	double fontSize;
	wxString itemClass;
	int confidence;
	int numLigne;
	bool selected;
};
*/
class CPage
{
public:
	wxRect rect;
	int resolution;
};
