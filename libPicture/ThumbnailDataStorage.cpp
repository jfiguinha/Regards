#include <header.h>
#include "ThumbnailDataStorage.h"
#include "ImageLoadingFormat.h"


CThumbnailDataStorage::CThumbnailDataStorage(const wxString& filename)
	: CThumbnailData(filename)
{
	localbitmap = nullptr;
}


CThumbnailDataStorage::~CThumbnailDataStorage(void)
{
	if (localbitmap != nullptr)
		delete localbitmap;
}

bool CThumbnailDataStorage::TestBitmap()
{
	if (localbitmap != nullptr)
		return localbitmap->IsOk();

	return false;
}

void CThumbnailDataStorage::RemoveBitmap() {
	if (localbitmap != nullptr)
	{
		delete localbitmap;
		localbitmap = nullptr;
	}
}

wxImage CThumbnailDataStorage::GetwxImage()
{
	if (TestBitmap())
	{
		wxImage local(*localbitmap);
		return local;
	}
	return wxImage();
}

void CThumbnailDataStorage::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (localbitmap != nullptr)
		delete localbitmap;
	
	if (bitmap->IsOk())
	{
		localbitmap = bitmap->GetwxImage();
	}
}
