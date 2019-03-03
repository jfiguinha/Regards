#include <header.h>
#include "ThumbnailDataStorage.h"
#include <FileUtility.h>
#include <LoadingResource.h>
#include "ImageLoadingFormat.h"
#include <wx/mstream.h>
#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif

CThumbnailDataStorage::CThumbnailDataStorage(const wxString & filename)
	: CThumbnailData(filename)
{
	data = nullptr;
	compressMethod = 0;
}


CThumbnailDataStorage::~CThumbnailDataStorage(void)
{
	if (data != nullptr)
	{
		if (compressMethod == 1)
		{
#ifdef TURBOJPEG
			tjFree(data);
#else
			delete[] data;
#endif
				
		}
		else if (compressMethod == 2)
		{
			free(data);
		}
		else
		{
			delete[] data;
		}
		data = nullptr;
	}
}

bool CThumbnailDataStorage::TestBitmap()
{
	if (data != nullptr)
		return true;

	return false;
}

wxImage CThumbnailDataStorage::GetwxImage()
{
	wxImage bitmap;
	if (data != nullptr)
	{
		wxMemoryInputStream jpegStream(data, size);
		bitmap.LoadFile(jpegStream, wxBITMAP_TYPE_JPEG);

	}
	return bitmap;
}

void CThumbnailDataStorage::SetBitmap(CImageLoadingFormat * bitmap)
{
	if(bitmap->IsOk())
	{
		if (data != nullptr)
		{
			if (compressMethod == 1)
			{
#ifdef TURBOJPEG
					tjFree(data);
#else
				delete[] data;
#endif
					
			}
			else if (compressMethod == 2)
			{
				free(data);
			}
			else
			{
				delete[] data;
			}
		}

		bitmap->ConvertToRGB24(true);
		data = bitmap->GetJpegData(size, compressMethod);
		format = bitmap->GetFormat();
	}
}

