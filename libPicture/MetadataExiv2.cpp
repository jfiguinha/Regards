#include "header.h"
#include "MetadataExiv2.h"
#include <FreeImage.h>
#ifdef __NEW_EXIV2__
#include "PictureMetadataExiv_new.h"
#else
#include "PictureMetadataExiv.h"
#endif
#include <ximage.h>
#include <libPicture.h>
#include <MediaInfo.h>
#include <picture_id.h>
//#include <libexif/exif-data.h>
#include "avif.h"
#include "Heic.h"
#include "ConvertUtility.h"

#include <libexif/exif-data.h>

using namespace Regards::Picture;
using namespace Regards::exiv2;


CMetadataExiv2::CMetadataExiv2(const wxString& filename)
{
	metaExiv = nullptr;
	CLibPicture libPicture;
	this->filename = filename;
	int type = libPicture.TestImageFormat(filename);

	if (type == HEIC || type == AVIF)
	{
		buffer = nullptr;
		unsigned int size = 0;
		if (type == HEIC)
		{
			CHeic::GetMetadata(CConvertUtility::ConvertToStdString(filename), buffer, size);
			if (size > 0)
			{
				buffer = new uint8_t[size + 1];
				CHeic::GetMetadata(CConvertUtility::ConvertToStdString(filename), buffer, size);
			}
		}
		else if (type == AVIF)
		{
			CAvif::GetMetadata(CConvertUtility::ConvertToStdString(filename), buffer, size);
			if (size > 0)
			{
				buffer = new uint8_t[size + 1];
				CAvif::GetMetadata(CConvertUtility::ConvertToStdString(filename), buffer, size);
			}
		}

		if (size > 0)
		{
			metaExiv = new CPictureMetadataExiv(buffer, size);
		}
	}
	/*
	else if (type == PNG)
	{
		unsigned char* buf;
		unsigned int len;
		ExifData* d;
		d = exif_data_new_from_file(CConvertUtility::ConvertToUTF8(filename));
		if (!d) {
			fprintf(stderr, "Could not load data from '%s'!\n", CConvertUtility::ConvertToUTF8(filename));
			return;
		}
		exif_data_save_data(d, &buf, &len);
		exif_data_unref(d);

		metaExiv = new CPictureMetadataExiv(buf, len);


		free(buf);

	}*/
	else
		metaExiv = new CPictureMetadataExiv(filename);
}


wxString CMetadataExiv2::GetCreationDate()
{
	if (metaExiv != nullptr)
		return metaExiv->GetCreationDate();
	return "";
}

int CMetadataExiv2::GetOrientation()
{
	if (metaExiv != nullptr)
		return metaExiv->GetOrientation();
	return 0;
}

CMetadataExiv2::~CMetadataExiv2()
{
	if (buffer != nullptr)
		delete[] buffer;

	if (metaExiv != nullptr)
		delete metaExiv;
}

bool CMetadataExiv2::HasExif()
{
	if (metaExiv != nullptr)
		return metaExiv->HasExif();
	return false;
}

void CMetadataExiv2::GetMetadataBuffer(uint8_t*& data, unsigned int& size)
{
	CLibPicture libPicture;
	int type = libPicture.TestImageFormat(filename);
	if (type == HEIC || type == AVIF)
	{
		if (type == AVIF)
		{
			if (size > 0)
			{
				CAvif::GetMetadata(CConvertUtility::ConvertToStdString(filename), data, size);
			}
			else if (size == 0)
			{
				CAvif::GetMetadata(CConvertUtility::ConvertToStdString(filename), data, size);
			}
		}
		else if (type == HEIC)
		{
			if (size > 0)
			{
				CHeic::GetMetadata(CConvertUtility::ConvertToStdString(filename), data, size);
			}
			else if (size == 0)
			{
				CHeic::GetMetadata(CConvertUtility::ConvertToStdString(filename), data, size);
			}
		}
	}
	else if (metaExiv != nullptr)
		metaExiv->GetMetadataBuffer(data, size);
}

bool CMetadataExiv2::CopyMetadata(const wxString& output)
{
	if (metaExiv != nullptr)
		return metaExiv->CopyMetadata(output);
	return false;
}

bool CMetadataExiv2::HasThumbnail()
{
	if (metaExiv != nullptr)
		return metaExiv->HasThumbnail();
	return false;
}

void CMetadataExiv2::SetDateTime(const wxString& dateTime)
{
	if (metaExiv != nullptr)
		metaExiv->SetDateTime(dateTime);
}

void CMetadataExiv2::SetOrientation(const int& orientation)
{
	if (metaExiv != nullptr)
		metaExiv->SetOrientation(orientation);
}

void CMetadataExiv2::SetGpsInfos(const wxString& latitudeRef, const wxString& longitudeRef, const wxString& latitude,
                                 const wxString& longitude)
{
	if (metaExiv != nullptr)
		metaExiv->SetGpsInfos(latitudeRef, longitudeRef, latitude, longitude);
}

/*
void CMetadataExiv2::ReadVideo(bool & hasGps, bool & hasDataTime, wxString & dateTimeInfos, wxString & latitude, wxString & longitude)
{
	metaExiv->ReadVideo(hasGps, hasDataTime, dateTimeInfos, latitude, longitude);

}
*/

void CMetadataExiv2::ReadPicture(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
                                 wxString& longitude)
{
	if (metaExiv != nullptr)
		metaExiv->ReadPicture(hasGps, hasDataTime, dateTimeInfos, latitude, longitude);
}

tbb::concurrent_vector<CMetadata> CMetadataExiv2::GetMetadata()
{
	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename))
	{
		return CMediaInfo::ReadMetadata(filename);
	}

	if (metaExiv != nullptr)
		return metaExiv->GetMetadata();

	tbb::concurrent_vector<CMetadata> meta;
	return meta;
}


wxImage CMetadataExiv2::DecodeThumbnail(wxString& extension, int& orientation)
{
	wxImage image;
	if (metaExiv != nullptr)
		image = metaExiv->DecodeThumbnail(extension, orientation);
	return image;
}
