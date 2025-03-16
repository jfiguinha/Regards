#pragma once

#include <Country.h>

using namespace std;

class CListCriteriaPhoto;

namespace Regards
{
	namespace Internet
	{
		class CFileGeolocation
		{
		public:
			CFileGeolocation(const wxString& urlServer);

			~CFileGeolocation(void)
			{
			};
			CFileGeolocation(const CFileGeolocation& filegeo);

			wxString GetUrlServer();

			void SetThumbnail(const bool& isThumbnail)
			{
				this->isThumbnail = isThumbnail;
			}

			void SetFile(const wxString& filename, const wxString& libNotGeo);
			wxString GetLatitude();
			wxString GetLongitude();
			float GetFLatitude();
			float GetFLongitude();
			bool HasGps();
			bool HasDateTime();
			wxString GetGpsInformation();
			wxString GetDateTimeInfos();
			wxString GetFilename();
			wxString Geolocalize();
			bool Geolocalisation(CListCriteriaPhoto* listCriteriaPhoto);
			// void RefreshData();
			wxString GenerateGeolocalisationString(const wxString& countryCode, const wxString& region,
			                                       const wxString& place);

		private:
			void ImportCountry();
			wxString dateTimeInfos;
			bool hasGps;
			bool hasDataTime;
			wxString latitudeGps;
			wxString longitudeGps;
			float flatitude;
			float flongitude;
			wxString infoGpsLocalisation;
			wxString filename;
			bool isThumbnail;

			//wxString gpsInfos;
			wxString urlServer;
			static CountryVector countryVector;
		};
	}
}
