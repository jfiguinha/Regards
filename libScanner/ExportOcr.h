#pragma once
#include <tesseract/baseapi.h>
namespace Regards
{
	namespace Scanner
	{
		class CExportOcr
		{
		public:
			static int ExportOcr(
				int argc, char** argv,
				wxString &errorMessage);


			static void monitorProgress(tesseract::ETEXT_DESC *monitor, int page);
			static void ocrProcess(tesseract::TessBaseAPI *api, tesseract::ETEXT_DESC *monitor);



			static tesseract::TessBaseAPI api;
		};
	}
}