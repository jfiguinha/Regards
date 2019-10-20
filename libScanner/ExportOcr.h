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

			static tesseract::TessBaseAPI api;
		};
	}
}