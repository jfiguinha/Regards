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
			GeoPluginVector geoPluginVector;
			//int error;
			wxString serverHttp;
		};
	}
}


