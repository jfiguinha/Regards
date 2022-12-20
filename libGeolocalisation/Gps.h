#pragma once
#include "GeoPluginValue.h"

class CGpscurl;

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
            
			bool ImportationGeoPlugin(const wxString &xml);
			wxString FindElement(const wxString &xml, const wxString &baliseBegin, const wxString &baliseEnd);
			GeoPluginVector geoPluginVector;
			//int error;
			wxString serverHttp;
            wxString latitude;
            wxString longitude;
			CGpscurl * gpsUrl;
		};
	}
}


