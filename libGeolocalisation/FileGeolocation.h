#pragma once
#include <Country.h>
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
using namespace std;

class CListCriteriaPhoto;

namespace Regards
{
	namespace Internet
	{
		class CFileGeolocation
		{
		public:

			CFileGeolocation(const wxString &urlServer);
			~CFileGeolocation(void){};
            void SetThumbnail(const bool &isThumbnail)
            {
                this->isThumbnail = isThumbnail;
            }
			void SetFile(const wxString & filename, const bool &onlyFromFile = false);
			wxString GetLatitude();
			wxString GetLongitude();
            float GetFLatitude();
            float GetFLongitude();
			bool HasGps();
			bool HasDateTime();
			wxString GetGpsInformation();
			wxString GetDateTimeInfos();
			wxString GetFilename();

            bool IsThumbnail()
            {
                return isThumbnail;
            }
            
            void Geolocalize();
			bool Geolocalisation(CListCriteriaPhoto * listCriteriaPhoto);
            void RefreshData();
            wxString GenerateGeolocalisationString(const wxString &countryCode, const wxString & region, const wxString &place);
            
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

            wxString gpsInfos;
            wxString urlServer;
			CountryVector countryVector;
		};

	}
}
