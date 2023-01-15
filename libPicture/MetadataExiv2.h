#pragma once
class CRegardsBitmap;
class CMetadata;

namespace Regards
{
	namespace exiv2
	{
		class CPictureMetadataExiv;

		class CMetadataExiv2
		{
		public:
			CMetadataExiv2(const wxString& filename);
			~CMetadataExiv2();
			void GetMetadataBuffer(uint8_t* & data, unsigned int& size);
			bool HasExif();
			bool HasThumbnail();
			int GetOrientation();
			void SetOrientation(const int& orientation);
			vector<CMetadata> GetMetadata();
			bool CopyMetadata(const wxString& output);
			wxMemoryInputStream* DecodeThumbnail(wxString& extension, int& orientation);
			void SetDateTime(const wxString& dateTime);
			wxString GetCreationDate();
			void SetGpsInfos(const wxString& latitudeRef, const wxString& longitudeRef, const wxString& latitude,
			                 const wxString& longitude);
			//void ReadVideo(bool & hasGps, bool & hasDataTime, wxString & dateTimeInfos, wxString & latitude, wxString & longitude);
			void ReadPicture(bool& hasGps, bool& hasDataTime, wxString& dateTimeInfos, wxString& latitude,
			                 wxString& longitude);

		private:
			CPictureMetadataExiv* metaExiv;
			wxString filename;
			uint8_t* buffer = nullptr;
		};
	}
}
