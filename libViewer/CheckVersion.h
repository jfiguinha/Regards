#pragma once
#include <GeoPluginValue.h>

namespace Regards
{
	namespace Internet
	{
        struct url_data {
            size_t size;
            char* data;
        };
        
		class CCheckVersion
		{
		public:
			CCheckVersion(const wxString &server);
			~CCheckVersion();
			wxString GetLastVersion();
           
		private:
            static size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data);
			GeoPluginVector geoPluginVector;
			//int error;
			wxString serverHttp;
		};
	}
}


