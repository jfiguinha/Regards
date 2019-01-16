#include "RawMetadata.h"
#include "libraw\libraw.h"
#include <Metadata.h>

vector<CMetadata> CRawMetadata::GetMetadata(const string & fileName)
{
	vector<CMetadata> metadata;
	LibRaw RawProcessor;
	if (RawProcessor.open_file(fileName.c_str()) == LIBRAW_SUCCESS)
	{
		int width = RawProcessor.imgdata.sizes.iwidth;
		int height = RawProcessor.imgdata.sizes.iheight;

		RawProcessor.recycle();
	}
	return metadata;
}