#pragma once
#include <string>
#include <GeoPluginValue.h>
using namespace std;

namespace Regards
{
	namespace Internet
	{
        struct url_data {
            size_t size;
            char* data;
        };
        
		class CGps
		{
		public:
			CGps(const wxString &server);
			~CGps();
			bool GeolocalisationGPS(const wxString &latitude, const wxString &longitude);
			GeoPluginVector * GetGpsList();
			wxString GetGpsValue(const float& gpsValue);
			float GetGpsfValue(const wxString & gpsValue);
            float GetFLatitude();
            float GetFLongitude();
            
		private:
            static size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data);
			bool ImportationGeoPlugin(const wxString &xml);
			wxString FindElement(const wxString &xml, const wxString &baliseBegin, const wxString &baliseEnd);
			GeoPluginVector geoPluginVector;
			int error;
			wxString serverHttp;
            wxString latitude;
            wxString longitude;

		};
	}
}


