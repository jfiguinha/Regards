#include "ThumbnailDataStorage.h"
#include <FileUtility.h>
#include <LoadingResource.h>

CThumbnailDataStorage::CThumbnailDataStorage(const wxString & filename)
	: CThumbnailData(filename)
{
}


CThumbnailDataStorage::~CThumbnailDataStorage(void)
{
	if (bitmap != nullptr)
		delete(bitmap);
}

bool CThumbnailDataStorage::TestBitmap()
{
	if (bitmap != nullptr)
		return true;

	return false;
}

wxImage CThumbnailDataStorage::GetwxImage()
{
	CLoadingResource loadingResource;
	return loadingResource.ConvertTowxImage(bitmap, false);
}

CRegardsBitmap * CThumbnailDataStorage::GetRegardsBitmap(const int &type)
{
	return bitmap;
}

void CThumbnailDataStorage::SetBitmap(CRegardsBitmap * bitmap)
{
	if (bitmap != nullptr)
	{
		if (this->bitmap != nullptr)
			delete(this->bitmap);

		this->bitmap = new CRegardsBitmap();
		*this->bitmap = *bitmap;
	}

}

