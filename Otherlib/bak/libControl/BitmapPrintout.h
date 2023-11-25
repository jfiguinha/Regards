// ReSharper disable All
#pragma once


#include "wx/print.h"
#include "wx/printdlg.h"

#if wxUSE_POSTSCRIPT
#include "wx/generic/printps.h"
#include "wx/generic/prntdlgg.h"
#endif

#if wxUSE_GRAPHICS_CONTEXT
#include "wx/graphics.h"
#endif

#ifdef __WXMAC__
#include "wx/osx/printdlg.h"
#endif

using namespace std;

class CImageLoadingFormat;
class CRegardsBitmap;

namespace Regards
{
	namespace Control
	{
		// Defines a new printout class to print our document
		class CBitmapPrintout : public wxPrintout
		{
		public:
			CBitmapPrintout(CImageLoadingFormat* image, const wxString& title = wxT("My picture"))
				: wxPrintout(title)
			{
				m_picture = image;
				typeImage = 1;
			}

			CBitmapPrintout(cv::Mat image, const wxString& title = wxT("My picture"))
				: wxPrintout(title)
			{
				image.copyTo(m_bitmap);
				typeImage = 2;
				m_picture = nullptr;
			}

			CBitmapPrintout();
			~CBitmapPrintout() override;

			bool OnPrintPage(int page) override;
			bool HasPage(int page) override;
			bool OnBeginDocument(int startPage, int endPage) override;
			void GetPageInfo(int* minPage, int* maxPage, int* selPageFrom, int* selPageTo) override;

			void DrawPicture(const int& pageNum);

			// Writes a header on a page. Margin units are in millimetres.
			bool WritePageHeader(wxPrintout* printout, wxDC* dc, const wxString& text, float mmToLogical);

		private:
			int typeImage;
			cv::Mat m_bitmap;
			CImageLoadingFormat* m_picture;
		};
	}
}
