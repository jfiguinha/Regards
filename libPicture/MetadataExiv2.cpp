#include "header.h"
#include "MetadataExiv2.h"

#include <libPicture.h>
#include <MediaInfo.h>
#include <picture_id.h>

using namespace Regards::Picture;
using namespace Regards::exiv2;


CMetadataExiv2::CMetadataExiv2(const wxString& filename)
{
	metaExiv = nullptr;
	CLibPicture libPicture;
	this->filename = filename;
	int type = libPicture.TestImageFormat(filename);
	metaExiv = std::unique_ptr<CPictureMetadataExiv>(new CPictureMetadataExiv(filename));
}


wxString CMetadataExiv2::GetCreationDate()
{
	if (metaExiv)
		return metaExiv->GetCreationDate();
	return "";
}

int CMetadataExiv2::GetOrientation()
{
	if (metaExiv)
		return metaExiv->GetOrientation();
	return 0;
}


bool CMetadataExiv2::HasExif()
{
	if (metaExiv)
		return metaExiv->HasExif();
	return false;
}

std::vector<uint8_t> CMetadataExiv2::GetMetadataBuffer()
{
	CLibPicture libPicture;
	int type = libPicture.TestImageFormat(filename);

	if (metaExiv)
		return metaExiv->GetMetadataBuffer();
	return std::vector<uint8_t>();
}

bool CMetadataExiv2::CopyMetadata(const wxString& output)
{
	if (metaExiv)
		return metaExiv->CopyMetadata(output);
	return false;
}

bool CMetadataExiv2::HasThumbnail()
{
	if (metaExiv)
		return metaExiv->HasThumbnail();
	return false;
}

void CMetadataExiv2::SetDateTime(const wxString& dateTime)
{
	if (metaExiv)
		metaExiv->SetDateTime(dateTime);
}

void CMetadataExiv2::SetOrientation(const int& orientation)
{
	if (metaExiv)
		metaExiv->SetOrientation(orientation);
}

void CMetadataExiv2::SetGpsInfos(const wxString& latitudeRef, const wxString& longitudeRef, const wxString& latitude,
                                 const wxString& longitude)
{
	if (metaExiv)
		metaExiv->SetGpsInfos(latitudeRef, longitudeRef, latitude, longitude);
}

void CMetadataExiv2::ReadPicture(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
                                 wxString& longitude)
{
	if (metaExiv)
		metaExiv->ReadPicture(hasGps, hasDataTime, dateTimeInfos, latitude, longitude);
}

std::vector<CMetadata> CMetadataExiv2::GetMetadata()
{
	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename))
	{
		return CMediaInfo::ReadMetadata(filename);
	}

	if (metaExiv)
		return metaExiv->GetMetadata();

	return std::vector<CMetadata>();
}


wxImage CMetadataExiv2::DecodeThumbnail(wxString& extension, int& orientation)
{
	if (metaExiv)
		return metaExiv->DecodeThumbnail(extension, orientation);
	return wxImage();
}