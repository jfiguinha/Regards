#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
#include <Country.h>
using namespace std;

class CListCriteriaPhoto;

#define NUMCATALOGID 1
#define CATEGORIE_DATE 3
#define CATEGORIE_GEO 1

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
			void SetFile(const wxString & filename);
			wxString GetLatitude();
			wxString GetLongitude();
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

		private:
           
			void ImportCountry();
			wxString dateTimeInfos = "";
			bool hasGps = false;
			bool hasDataTime = false;
			wxString latitudeGps = L"";
			wxString longitudeGps = L"";
			float flatitude = 0.0;
			float flongitude = 0.0;
			wxString infoGpsLocalisation = L"";
			wxString filename = L"";
            bool isThumbnail = false;
            wxString gpsInfos;
            wxString urlServer;
			static CountryVector countryVector;
		};

	}
}