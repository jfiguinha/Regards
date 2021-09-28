#include "header.h"
#include "MetadataExiv2.h"
#include "PictureMetadataExiv.h"
#include <ximage.h>
#include <libPicture.h>
#include <MediaInfo.h>
#include <picture_id.h>
#include <libexif/exif-data.h>

#include "Heic.h"
#include "ConvertUtility.h"


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
		CHeic::GetMetadata(CConvertUtility::ConvertToUTF8(filename), buffer, size);
		if (size > 0)
		{
			buffer = new uint8_t[size + 1];
			CHeic::GetMetadata(CConvertUtility::ConvertToUTF8(filename), buffer, size);
		}

		if (size > 0)
		{
			metaExiv = new CPictureMetadataExiv(buffer, size);
		}
	}
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
		if (size > 0)
		{
			CHeic::GetMetadata(CConvertUtility::ConvertToUTF8(filename), data, size);
		}
		else if (size == 0)
		{
			CHeic::GetMetadata(CConvertUtility::ConvertToUTF8(filename), data, size);
		}
	}
	else if (metaExiv != nullptr)
		metaExiv->GetMetadataBuffer(data, size);
}

bool CMetadataExiv2::CopyMetadata(const wxString& output)
{
	if (metaExiv != nullptr)
		return metaExiv->CopyMetadata(output);
	/*
	CLibPicture libPicture;
	int type = libPicture.TestImageFormat(filename);
	if (type == HEIC || type == AVIF)
	{
		buffer = nullptr;
		long size = 0;
		CHeic::GetMetadata(CConvertUtility::ConvertToUTF8(filename), buffer, size);
		if (size > 0)
		{
			buffer = new uint8_t[size + 1];
			CHeic::GetMetadata(CConvertUtility::ConvertToUTF8(filename), buffer, size);
		}

		Exiv2::Image::AutoPtr exif;
		//Read exif info from source file
		try
		{
			exif = Exiv2::ImageFactory::open(buffer, size);
			assert(exif.get() != 0);
			exif->readMetadata();

			Exiv2::Image::AutoPtr writeImg = Exiv2::ImageFactory::open(output.ToStdString());
			assert(writeImg.get() != 0);
			writeImg->setExifData(writeImg->exifData());
			writeImg->writeMetadata();

			return true;
		}
		catch (...)q
		{

		}
		
	}
	*/
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

vector<CMetadata> CMetadataExiv2::GetMetadata()
{
	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename))
	{
		return CMediaInfo::ReadMetadata(filename);
	}

	if (metaExiv != nullptr)
		return metaExiv->GetMetadata();

	vector<CMetadata> meta;
	return meta;
}


CxMemFile* CMetadataExiv2::DecodeThumbnail(wxString& extension, int& orientation)
{
	if (metaExiv != nullptr)
		return metaExiv->DecodeThumbnail(extension, orientation);
	return nullptr;
}
