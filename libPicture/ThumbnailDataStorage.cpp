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

void CThumbnailDataStorage::SetIsDefault(const bool & default)
{
    this->isDefault = default;
}

bool CThumbnailDataStorage::TestBitmap()
{
	return !localbitmap.empty();
}

void CThumbnailDataStorage::RemoveBitmap()
{
}

cv::Mat CThumbnailDataStorage::GetImage(bool& default)
{
    if(isDefault)
        return GetDefaultPicture();
    
	if (TestBitmap())
	{
		default = false;
		return localbitmap;
	}
	else
		default = true;
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
