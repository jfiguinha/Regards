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

void CThumbnailDataStorage::SetIsDefault(const bool & defaultValue)
{
    this->isDefault = defaultValue;
}

bool CThumbnailDataStorage::TestBitmap()
{
	return !localbitmap.empty();
}

void CThumbnailDataStorage::RemoveBitmap()
{
}

cv::Mat CThumbnailDataStorage::GetImage(bool& defaultValue)
{
    if(isDefault)
        return GetDefaultPicture();
    
	if (TestBitmap())
	{
		defaultValue = false;
		return localbitmap;
	}
	else
		defaultValue = true;
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
