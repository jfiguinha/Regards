#include <header.h>
#include "BitmapPrintout.h"
#include <ImageLoadingFormat.h>
#include <PrintEngine.h>
using namespace Regards::Control;
using namespace Regards::Print;

// ----------------------------------------------------------------------------
// MyPrintout
// ----------------------------------------------------------------------------

CBitmapPrintout::~CBitmapPrintout()
{
	
	if(m_bitmap != nullptr)
		delete(m_bitmap);	
	if(m_picture != nullptr)
		delete(m_picture);

	m_bitmap = nullptr;
	m_picture = nullptr;
	
}

CBitmapPrintout::CBitmapPrintout()
{
	m_picture = nullptr;
}


bool CBitmapPrintout::OnPrintPage(int page)
{
	wxDC *dc = GetDC();
	if (dc)
	{
		DrawPicture(page);

		// Draw page numbers at top left corner of printable area, sized so that
		// screen size of text matches paper size.
		MapScreenSizeToPage();

		//dc->DrawText(wxString::Format(wxT("PAGE %d"), page), 0, 0);

		return true;
	}
	else
		return false;
}

bool CBitmapPrintout::OnBeginDocument(int startPage, int endPage)
{
	if (!wxPrintout::OnBeginDocument(startPage, endPage))
		return false;

	return true;
}

void CBitmapPrintout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
	if (typeImage == 1)
	{
		*minPage = 1;
		*maxPage = m_picture->GetNbPage();
		*selPageFrom = 1;
		*selPageTo = m_picture->GetNbPage();
	}
	else if(typeImage == 2)
	{
		*minPage = 1;
		*maxPage = 1;
		*selPageFrom = 1;
		*selPageTo = 1;
	}
}

bool CBitmapPrintout::HasPage(int pageNum)
{
	if (typeImage == 1)
	{
		if (pageNum <= m_picture->GetNbPage())
			return true;
	}
	else if (typeImage == 2)
	{
		if (pageNum <= 1)
			return true;
	}
	return false;
}

void CBitmapPrintout::DrawPicture(const int & pageNum)
{
	// You might use THIS code if you were scaling graphics of known size to fit
	// on the page. The commented-out code illustrates different ways of scaling
	// the graphics.

	// We know the graphic is 230x350. If we didn't know this, we'd need to
	// calculate it.
	CImageLoadingFormat * image = nullptr;

	if (typeImage == 1)
	{
		image = m_picture->GetPage(pageNum - 1);
	}
	else if(typeImage == 2)
	{
		image = new CImageLoadingFormat(false);
		image->SetPicture(m_bitmap);
	}

	wxCoord maxX = image->GetWidth();
	wxCoord maxY = image->GetHeight();
	wxPageSetupDialogData * g_pageSetupData = CPrintEngine::GetPageSetupDialogData();

	// This sets the user scale and origin of the DC so that the image fits
	// within the page margins as specified by g_PageSetupData, which you can
	// change (on some platforms, at least) in the Page Setup dialog. Note that
	// on Mac, the native Page Setup dialog doesn't let you change the margins
	// of a wxPageSetupDialogData object, so you'll have to write your own dialog or
	// use the Mac-only wxMacPageMarginsDialog, as we do in this program.
	FitThisSizeToPageMargins(wxSize(maxX, maxY), *g_pageSetupData);
	wxRect fitRect = GetLogicalPageMarginsRect(*g_pageSetupData);

	// This offsets the image so that it is centered within the reference
	// rectangle defined above.
	wxCoord xoff = (fitRect.width - maxX) / 2;
	wxCoord yoff = (fitRect.height - maxY) / 2;
	OffsetLogicalOrigin(xoff, yoff);

	wxDC * dc = GetDC();
	wxImage * _local = image->GetwxImage();
	dc->DrawBitmap(*_local, 0, 0);
	delete _local;
	delete image;
}


// Writes a header on a page. Margin units are in millimetres.
bool CBitmapPrintout::WritePageHeader(wxPrintout *printout, wxDC *dc, const wxString&text, float mmToLogical)
{
	/*
#if 0
	static wxFont *headerFont = (wxFont *)nullptr;
	if (!headerFont)
	{
		headerFont = wxTheFontList->FindOrCreateFont(16, wxSWISS, wxNORMAL, wxBOLD);
	}
	dc->SetFont(headerFont);
#endif

	int pageWidthMM, pageHeightMM;

	printout->GetPageSizeMM(&pageWidthMM, &pageHeightMM);
	wxUnusedVar(pageHeightMM);

	int leftMargin = 10;
	int topMargin = 10;
	int rightMargin = 10;

	float leftMarginLogical = (float)(mmToLogical*leftMargin);
	float topMarginLogical = (float)(mmToLogical*topMargin);
	float rightMarginLogical = (float)(mmToLogical*(pageWidthMM - rightMargin));

	wxCoord xExtent, yExtent;
	dc->GetTextExtent(text, &xExtent, &yExtent);

	float xPos = (float)(((((pageWidthMM - leftMargin - rightMargin) / 2.0) + leftMargin)*mmToLogical) - (xExtent / 2.0));
	dc->DrawText(text, (long)xPos, (long)topMarginLogical);

	dc->SetPen(*wxBLACK_PEN);
	dc->DrawLine((long)leftMarginLogical, (long)(topMarginLogical + yExtent),
		(long)rightMarginLogical, (long)topMarginLogical + yExtent);
		*/
	return true;
}
