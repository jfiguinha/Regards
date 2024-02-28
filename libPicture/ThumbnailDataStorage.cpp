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
	return !localbitmap.empty();
}

void CThumbnailDataStorage::RemoveBitmap()
{
}

cv::Mat CThumbnailDataStorage::GetImage(bool& isDefault)
{
    if(isDefault)
        return GetDefaultPicture();
    
	if (TestBitmap())
	{
		isDefault = false;
		return localbitmap;
	}
	else
		isDefault = true;
	return GetDefaultPicture();
}

void CThumbnailDataStorage::SetBitmap(cv::Mat bitmap)
{
	if (!bitmap.empty())
	{
		localbitmap = bitmap;
        isDefault = false;
	}
}
