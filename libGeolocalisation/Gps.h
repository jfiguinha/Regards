#pragma once
#include "GeoPluginValue.h"

class CGpscurl;

namespace Regards
{
	namespace Internet
	{
		struct url_data
		{
			size_t size;
			char* data;
		};

		class CGps
		{
		public:
			CGps(const wxString& server, const wxString& apiKey);
			~CGps();
			bool GeolocalisationGPS(const wxString& latitude, const wxString& longitude);
			GeoPluginVector* GetGpsList();
			wxString GetGpsValue(const float& gpsValue);
			float GetGpsfValue(const wxString& gpsValue);
			float GetFLatitude();
			float GetFLongitude();
			static bool IsLocalisationAvailable(const wxString& server, const wxString& apiKey);
		private:
			//bool ImportationGeoPlugin(const wxString& xml);
			bool ImportationGeocodePlugin(const wxString& xml);
			wxString FindElement(const wxString& xml, const wxString& baliseBegin, const wxString& baliseEnd);
			GeoPluginVector geoPluginVector;

			wxString serverHttp;
			wxString latitude;
			wxString apiKey;
			wxString longitude;
			CGpscurl* gpsUrl;
		};
	}
}
