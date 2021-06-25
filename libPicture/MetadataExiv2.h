#pragma once
class CRegardsBitmap;
class CxMemFile;
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

			bool HasExif();
			bool HasThumbnail();
			int GetOrientation();
			vector<CMetadata> GetMetadata();
			bool CopyMetadata(const wxString& output);
			CxMemFile* DecodeThumbnail(wxString& extension, int& orientation);
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
