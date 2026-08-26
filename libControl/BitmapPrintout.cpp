#include <header.h>
#include "BitmapPrintout.h"
#include <ImageLoadingFormat.h>
#include "PrintEngine.h"
#include <libPicture.h>
using namespace Regards::Control;
using namespace Regards::Print;

// ----------------------------------------------------------------------------
// MyPrintout
// ----------------------------------------------------------------------------
CBitmapPrintout::CBitmapPrintout(const wxString& filename, const wxString& title)
	: wxPrintout(title)
{
	//m_picture = image;
	Regards::Picture::CLibPicture libPicture;
	m_picture = std::unique_ptr<CImageLoadingFormat>(libPicture.LoadPicture(filename));
	typePicture = Type_Picture::picture;
}

CBitmapPrintout::CBitmapPrintout(cv::Mat& picture, const wxString& title)
	: wxPrintout(title)
{
	Regards::Picture::CLibPicture libPicture;
	m_picture = std::make_unique<CImageLoadingFormat>();
	m_picture->SetPicture(picture);
	typePicture = Type_Picture::matrix;
}

bool CBitmapPrintout::OnPrintPage(int page)
{
	wxDC* dc = GetDC();
	if (dc)
	{
		DrawPicture(page);

		// Draw page numbers at top left corner of printable area, sized so that
		// screen size of text matches paper size.
		MapScreenSizeToPage();

		//dc->DrawText(wxString::Format(wxT("PAGE %d"), page), 0, 0);

		return true;
	}
	return false;
}

bool CBitmapPrintout::OnBeginDocument(int startPage, int endPage)
{
	if (!wxPrintout::OnBeginDocument(startPage, endPage))
		return false;

	return true;
}

void CBitmapPrintout::GetPageInfo(int* minPage, int* maxPage, int* selPageFrom, int* selPageTo)
{
	switch(typePicture)
	{
	case  Type_Picture::picture:
		*minPage = 1;
		*maxPage = m_picture->GetNbPage();
		*selPageFrom = 1;
		*selPageTo = m_picture->GetNbPage();
		break;
	case Type_Picture::matrix:
		*minPage = 1;
		*maxPage = 1;
		*selPageFrom = 1;
		*selPageTo = 1;
		break;
	}
}

bool CBitmapPrintout::HasPage(int pageNum)
{
	switch (typePicture)
	{
	case  Type_Picture::picture:
		if (pageNum <= m_picture->GetNbPage())
			return true;
		break;
	case Type_Picture::matrix:
		if (pageNum <= 1)
			return true;
		break;
	}
	return false;
}
void CBitmapPrintout::DrawPicture(const int& pageNum)
{
	std::unique_ptr<CImageLoadingFormat> page;

	CImageLoadingFormat* image = nullptr;

	if (typePicture == Type_Picture::picture)
	{
		page.reset(m_picture->GetPage(pageNum - 1));
		image = page.get();
	}
	else
	{
		image = m_picture.get();
	}

	if (!image)
		return;

	wxDC* dc = GetDC();
	if (!dc)
		return;

	wxPageSetupDialogData* pageSetupData =
		CPrintEngine::GetPageSetupDialogData();

	wxRect printableRect =
		GetLogicalPageMarginsRect(*pageSetupData);

	wxImage img = image->GetwxImage();

	if (!img.IsOk())
		return;

	const double sx =
		static_cast<double>(printableRect.width) /
		img.GetWidth();

	const double sy =
		static_cast<double>(printableRect.height) /
		img.GetHeight();

	const double scale = std::min(sx, sy);

	const int w =
		static_cast<int>(img.GetWidth() * scale);

	const int h =
		static_cast<int>(img.GetHeight() * scale);

	img.Rescale(w, h, wxIMAGE_QUALITY_HIGH);

	const int x =
		printableRect.x +
		(printableRect.width - w) / 2;

	const int y =
		printableRect.y +
		(printableRect.height - h) / 2;

	dc->DrawBitmap(wxBitmap(img), x, y, false);
}

