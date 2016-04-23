#include "BitmapPrintout.h"
#include <LoadingResource.h>
#include <PrintEngine.h>
using namespace Regards::Control;
using namespace Regards::Print;

// ----------------------------------------------------------------------------
// MyPrintout
// ----------------------------------------------------------------------------

bool CBitmapPrintout::OnPrintPage(int page)
{
	wxDC *dc = GetDC();
	if (dc)
	{
		DrawPicture();

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
	*minPage = 1;
	*maxPage = 1;
	*selPageFrom = 1;
	*selPageTo = 1;
}

bool CBitmapPrintout::HasPage(int pageNum)
{
	return (pageNum == 1);
}

void CBitmapPrintout::DrawPicture()
{
	// You might use THIS code if you were scaling graphics of known size to fit
	// on the page. The commented-out code illustrates different ways of scaling
	// the graphics.

	// We know the graphic is 230x350. If we didn't know this, we'd need to
	// calculate it.
	wxCoord maxX = m_picture->GetBitmapWidth();
	wxCoord maxY = m_picture->GetBitmapHeight();
	wxPageSetupDialogData * g_pageSetupData = CPrintEngine::GetPageSetupDialogData();

	// This sets the user scale and origin of the DC so that the image fits
	// within the paper rectangle (but the edges could be cut off by printers
	// that can't print to the edges of the paper -- which is most of them. Use
	// this if your image already has its own margins.
	//    FitThisSizeToPaper(wxSize(maxX, maxY));
	//    wxRect fitRect = GetLogicalPaperRect();

	// This sets the user scale and origin of the DC so that the image fits
	// within the page rectangle, which is the printable area on Mac and MSW
	// and is the entire page on other platforms.
	//    FitThisSizeToPage(wxSize(maxX, maxY));
	//    wxRect fitRect = GetLogicalPageRect();

	// This sets the user scale and origin of the DC so that the image fits
	// within the page margins as specified by g_PageSetupData, which you can
	// change (on some platforms, at least) in the Page Setup dialog. Note that
	// on Mac, the native Page Setup dialog doesn't let you change the margins
	// of a wxPageSetupDialogData object, so you'll have to write your own dialog or
	// use the Mac-only wxMacPageMarginsDialog, as we do in this program.
	FitThisSizeToPageMargins(wxSize(maxX, maxY), *g_pageSetupData);
	wxRect fitRect = GetLogicalPageMarginsRect(*g_pageSetupData);

	// This sets the user scale and origin of the DC so that the image appears
	// on the paper at the same size that it appears on screen (i.e., 10-point
	// type on screen is 10-point on the printed page) and is positioned in the
	// top left corner of the page rectangle (just as the screen image appears
	// in the top left corner of the window).
	//    MapScreenSizeToPage();
	//    wxRect fitRect = GetLogicalPageRect();

	// You could also map the screen image to the entire paper at the same size
	// as it appears on screen.
	//    MapScreenSizeToPaper();
	//    wxRect fitRect = GetLogicalPaperRect();

	// You might also wish to do you own scaling in order to draw objects at
	// full native device resolution. In this case, you should do the following.
	// Note that you can use the GetLogicalXXXRect() commands to obtain the
	// appropriate rect to scale to.
	//    MapScreenSizeToDevice();
	//    wxRect fitRect = GetLogicalPageRect();

	// Each of the preceding Fit or Map routines positions the origin so that
	// the drawn image is positioned at the top left corner of the reference
	// rectangle. You can easily center or right- or bottom-justify the image as
	// follows.

	// This offsets the image so that it is centered within the reference
	// rectangle defined above.
	wxCoord xoff = (fitRect.width - maxX) / 2;
	wxCoord yoff = (fitRect.height - maxY) / 2;
	OffsetLogicalOrigin(xoff, yoff);

	// This offsets the image so that it is positioned at the bottom right of
	// the reference rectangle defined above.
	//    wxCoord xoff = (fitRect.width - maxX);
	//    wxCoord yoff = (fitRect.height - maxY);
	//    OffsetLogicalOrigin(xoff, yoff);

	//wxGetApp().Draw(*GetDC());

	CLoadingResource loadingResource;
	wxDC * dc = GetDC();
	dc->DrawBitmap(loadingResource.ConvertTowxImage(m_picture, false), 0, 0);
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
