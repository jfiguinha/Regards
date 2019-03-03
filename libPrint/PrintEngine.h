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

namespace Regards
{
	namespace Print
	{
		class CPrintEngine
		{
		public:
			static void Initialize();
			static void Kill();
			static wxPrintData * GetPrintData();
			static wxPageSetupDialogData * GetPageSetupDialogData();

		private:

			static wxPrintData * g_printData;
			static wxPageSetupDialogData * g_pageSetupData;
		};
	}
}

