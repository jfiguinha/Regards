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
;

namespace Regards
{
	namespace Control
	{
		// Defines a new printout class to print our document
		class CBitmapPrintout : public wxPrintout
		{
		public:
			CBitmapPrintout(const wxString& filename, const wxString& title = wxT("My picture"));
			CBitmapPrintout(cv::Mat& picture, const wxString& title = wxT("My picture"));
			bool OnPrintPage(int page) override;
			bool HasPage(int page) override;
			bool OnBeginDocument(int startPage, int endPage) override;
			void GetPageInfo(int* minPage, int* maxPage, int* selPageFrom, int* selPageTo) override;

			void DrawPicture(const int& pageNum);

		private:

			enum Type_Picture{picture,matrix};
			std::unique_ptr<CImageLoadingFormat> m_picture;
			Type_Picture typePicture = Type_Picture::picture;
		};
	}
}
