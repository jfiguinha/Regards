#pragma once
#include <tesseract/baseapi.h>

namespace Regards::Scanner
{
	class CExportOcr
	{
	public:
		static int ExportOcr(
			int argc, char** argv,
			wxString& errorMessage);

#ifndef __WXGTK__
		static void monitorProgress(tesseract::ETEXT_DESC* monitor, int page);
		static void ocrProcess(tesseract::TessBaseAPI* api, tesseract::ETEXT_DESC* monitor);
#else
			static void monitorProgress(ETEXT_DESC* monitor, int page);
			static void ocrProcess(tesseract::TessBaseAPI* api, ETEXT_DESC* monitor);

#endif


		static tesseract::TessBaseAPI api;
	};
}
