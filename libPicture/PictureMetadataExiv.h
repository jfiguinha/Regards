#pragma once
#include <Metadata.h>
#include <exiv2/image.hpp>
#include <exiv2/iptc.hpp>

;

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
			std::vector<uint8_t> GetMetadataBuffer();
			wxString GetCreationDate();
			bool HasExif();
			bool HasThumbnail();
			int GetOrientation();
			std::vector<CMetadata> GetMetadata();
			bool CopyMetadata(const wxString& output);
			wxImage DecodeThumbnail(wxString& extension, int& orientation);
			void SetDateTime(const wxString& dateTime);
			void SetOrientation(const int& orientation);
			void SetGpsInfos(const wxString& latitudeRef, const wxString& longitudeRef, const wxString& latitude,
			                 const wxString& longitude);

			void ReadPicture(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
			                 wxString& longitude);

		private:



			bool ReadDateTime(const Exiv2::ExifData& exifData,
				wxString& dateTime);

			bool ReadGps(const Exiv2::ExifData& exifData,
				wxString& latitude,
				wxString& longitude);

			bool ReadGpsTag(const Exiv2::ExifData& exifData,
				const char* keyName,
				wxString& value);

			Exiv2::ExifData* GetExifData();
			Exiv2::URationalValue::UniquePtr GetGpsRationalValue(const wxString& gpsValue);
			wxString GetGpsfValue(const wxString& gpsValue);
			wxImage LoadThumbnailFromExif(Exiv2::ExifData* dataIn, wxString& extension, int& orientation);
			void AddAsciiValue(wxString key, wxString value, Exiv2::ExifData& exifData);
			void AddRationalValue(wxString keyName, wxString value, Exiv2::ExifData& exifData);
			wxString GetQuickTimeDate(int64_t dateQuicktime);
			void ReadExif(Exiv2::ExifData& exifData, std::vector<CMetadata>& metadataList);
			void ReadIpct(Exiv2::IptcData& ipctData, std::vector<CMetadata>& metadataList);
			void ReadXmp(Exiv2::XmpData& xmpData, std::vector<CMetadata>& metadataList);
			Exiv2::Image::UniquePtr exif;
			bool isExif;
			wxString filename;
		};
	}
}