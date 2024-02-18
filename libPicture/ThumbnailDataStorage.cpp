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

wxImage CThumbnailDataStorage::GetwxImage(bool& isDefault)
{
	if (TestBitmap())
	{
		isDefault = false;
		return localbitmap;
	}
	else
		isDefault = true;
	return GetDefaultPicture();
}

void CThumbnailDataStorage::SetBitmap(wxImage bitmap)
{
	if (bitmap.IsOk())
	{
		localbitmap = bitmap;
	}
}
