#include <header.h>
#include "Painting.h"
#include <RegardsBitmap.h>
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>


CPainting::CPainting()
{
}


CPainting::~CPainting()
{
}

wxImage CPainting::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector)
{
	wxImage img;
	if (vector.size() > 0)
	{
		wxStringInputStream memBuffer(vector);
		wxSVGDocument svgDoc;
		svgDoc.Load(memBuffer);
		img = svgDoc.Render(buttonWidth, buttonHeight, NULL, true, true);
	}
	else
	{
		img.Create(buttonWidth, buttonHeight);
	}
	return img;
}

CRegardsBitmap * CPainting::ConvertWXImageToRegardsBitmap(wxImage * image)
{
	CRegardsBitmap * bitmap = new CRegardsBitmap(image->GetWidth(), image->GetHeight());
	uint8_t * dstData = bitmap->GetPtBitmap();
	//int widthSrcSize = image->GetWidth() << 2;
	uint8_t * data = image->GetData();
	uint8_t * alpha = nullptr;
	if (image->HasAlpha())
		alpha = image->GetAlpha();

	int size = image->GetHeight() * image->GetWidth();

	for (auto i = 0, j = 0, k = 0; i < size; i++)
	{
		dstData[j + 2] = data[k];
		dstData[j + 1] = data[k + 1];
		dstData[j + 0] = data[k + 2];
		if (alpha != nullptr)
			dstData[j + 3] = alpha[i];

		j = i * 4;
		k = i * 3;
	}

	return bitmap;
}