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

bool CThumbnailDataStorage::GetIsDefault()
{
    return isDefault;
}

void CThumbnailDataStorage::SetIsDefault(const bool &isDefault)
{
    this->isDefault = isDefault;
}

bool CThumbnailDataStorage::TestBitmap()
{
	return localbitmap.IsOk();
}

void CThumbnailDataStorage::RemoveBitmap()
{
}

wxImage CThumbnailDataStorage::GetwxImage()
{
	if (TestBitmap())
	{
		return localbitmap;
	}
	return wxImage();
}

void CThumbnailDataStorage::SetBitmap(wxImage bitmap)
{
	if (bitmap.IsOk())
	{
		localbitmap = bitmap;
	}
}
