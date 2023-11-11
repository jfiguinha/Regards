#pragma once
#include <Metadata.h>
#include <exiv2/image.hpp>
#include <exiv2/iptc.hpp>

class CRegardsBitmap;

namespace Regards
{
	namespace exiv2
	{
		class CPictureMetadataExiv
		{
		public:
			CPictureMetadataExiv(const wxString& filename);
			CPictureMetadataExiv(uint8_t* data, const long& size);
			~CPictureMetadataExiv();
			void GetMetadataBuffer(uint8_t*& data, unsigned int& size);
			wxString GetCreationDate();
			bool HasExif();
			bool HasThumbnail();
			int GetOrientation();
			vector<CMetadata> GetMetadata();
			bool CopyMetadata(const wxString& output);
			wxImage DecodeThumbnail(wxString& extension, int& orientation);
			void SetDateTime(const wxString& dateTime);
			void SetOrientation(const int& orientation);
			void SetGpsInfos(const wxString& latitudeRef, const wxString& longitudeRef, const wxString& latitude,
			                 const wxString& longitude);
			void ReadVideo(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
			               wxString& longitude);
			void ReadPicture(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
			                 wxString& longitude);

		private:
			Exiv2::URationalValue::UniquePtr GetGpsRationalValue(const wxString& gpsValue);
			wxString GetGpsfValue(const wxString& gpsValue);
			wxImage LoadThumbnailFromExif(Exiv2::ExifData* dataIn, wxString& extension, int& orientation);
			void AddAsciiValue(wxString key, wxString value, Exiv2::ExifData& exifData);
			void AddRationalValue(wxString keyName, wxString value, Exiv2::ExifData& exifData);
			wxString GetQuickTimeDate(int64_t dateQuicktime);
			vector<CMetadata> ReadExif(Exiv2::ExifData& exifData);
			vector<CMetadata> ReadIpct(Exiv2::IptcData& ipctData);
			vector<CMetadata> ReadXmp(Exiv2::XmpData& xmpData);
			Exiv2::Image::UniquePtr exif;
			bool isExif;
			wxString filename;
		};
	}
}
