#pragma once
#include <RegardsBitmap.h>

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

namespace Regards
{
	namespace Control
	{
		
		// Defines a new printout class to print our document
		class CBitmapPrintout: public wxPrintout
		{
		public:
			CBitmapPrintout(CRegardsBitmap * image, const wxString &title = wxT("My picture"))
				: wxPrintout(title) {
				m_picture = image;
			}

			CBitmapPrintout();
			~CBitmapPrintout();

			virtual bool OnPrintPage(int page);
			virtual bool HasPage(int page);
			virtual bool OnBeginDocument(int startPage, int endPage);
			virtual void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);

			void DrawPicture();

			// Writes a header on a page. Margin units are in millimetres.
			bool WritePageHeader(wxPrintout *printout, wxDC *dc, const wxString& text, float mmToLogical);

		private:
			CRegardsBitmap * m_picture;
		};
		
	}
}

