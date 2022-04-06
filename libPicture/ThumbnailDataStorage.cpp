#include <header.h>
#include "ThumbnailDataStorage.h"
#include "ImageLoadingFormat.h"


CThumbnailDataStorage::CThumbnailDataStorage(const wxString& filename)
	: CThumbnailData(filename)
{

}


CThumbnailDataStorage::~CThumbnailDataStorage(void)
{

}

bool CThumbnailDataStorage::TestBitmap()
{
	return localbitmap.IsOk();
}

void CThumbnailDataStorage::RemoveBitmap() {

}

wxImage CThumbnailDataStorage::GetwxImage()
{
	if (TestBitmap())
	{
		return localbitmap;
	}
	return wxImage();
}

void CThumbnailDataStorage::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (bitmap->IsOk())
	{
		localbitmap = bitmap->GetwxImage();
	}
}
