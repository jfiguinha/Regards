#include "header.h"
#include "Gps.h"
#include <ConvertUtility.h>

#ifdef USECURL
#include <curl/curl.h>
#else
#include <wx/url.h>
#endif
#include <wx/sstream.h>
using namespace Regards::Internet;



//Test parameter /extras/location.gp?lat=48.896168&long=2.387500&format=xml wxString server = L"www.geoplugin.net";

CGps::CGps(const wxString &server)
{
	serverHttp = server;
	//OpenHttpRequest(server, error);
}


CGps::~CGps()
{
	//CloseHttpRequest();
}

float CGps::GetGpsfValue(const wxString& gpsValue)
{
	wxString returnValue = "";
	vector<wxString> latValue;
	int i = 0;

	//Conversion des valeurs des latitudes et des longitudes
	latValue = CConvertUtility::split(gpsValue, ' ');

	float outputValue = 0.0;

	for (vector<wxString>::iterator it = latValue.begin(); it != latValue.end(); it++)
	{

		vector<wxString> intValue = CConvertUtility::split(*it, '/');
		int valeur = atoi(intValue.at(0).c_str());
		int diviseur = atoi(intValue.at(1).c_str());

		float value = (float)valeur / (float)diviseur;
		if (i == 1)
		{
			value = value / 60;
		}
		else if (i == 2)
		{
			value = value / 3600;
		}

		outputValue += value;
		i++;
	}

	return outputValue;
}

wxString CGps::GetGpsValue(const float& gpsValue)
{
	return to_string(gpsValue);
}

#ifdef USECURL




size_t CGps::write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
	size_t index = data->size;
	size_t n = (size * nmemb);
	char* tmp;

	data->size += (size * nmemb);

#ifdef DEBUG
	fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
	tmp = (char *)realloc(data->data, data->size + 1); /* +1 for '\0' */

	if (tmp != NULL) {
		data->data = tmp;
	}
	else {
		if (data->data) {
			free(data->data);
		}
		fprintf(stderr, "Failed to allocate memory.\n");
		return 0;
	}

	memcpy((data->data + index), ptr, n);
	data->data[data->size] = '\0';

	return size * nmemb;
}

float CGps::GetFLatitude()
{
    double val;
    latitude.ToDouble(&val);
    return val;
}

float CGps::GetFLongitude()
{
    double val;
    longitude.ToDouble(&val);
    return val;
}

bool CGps::GeolocalisationGPS(const wxString &latitude, const wxString &longitude)
{

    
    printf("CGps GeolocalisationGPS \n");
	CURLcode res;
	CURL *curl;
    this->latitude = latitude;
    this->longitude = longitude;


	//int error = 0;
	bool returnValue = true;
	//wxString xml = L"";
	wxString httpAdress = serverHttp;
	httpAdress.append(L"/extras/location.gp?lat=");
	httpAdress.append(latitude);
	httpAdress.append(L"&long=");
	httpAdress.append(longitude);
	httpAdress.append(L"&format=xml");

	struct url_data data;
	data.size = 0;
	data.data = (char *)malloc(4096); /* reasonable size initial buffer */
	if (NULL == data.data) {
        printf("CGps GeolocalisationGPS Failed to allocate memory \n");
		fprintf(stderr, "Failed to allocate memory.\n");
		return false;
	}

	data.data[0] = '\0';

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, CConvertUtility::ConvertToUTF8(httpAdress));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	res = curl_easy_perform(curl); /* ignores error */
	if (res != CURLE_OK)
	{
		wxString error = curl_easy_strerror(res);
		wxMessageBox(error);
		printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);

    //wxString mystring2(chars, wxConvUTF8);
	wxString xml(data.data, wxConvUTF8);

    printf("URL  : %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
	printf("Data : %s \n", data.data);

	geoPluginVector.clear();
	ImportationGeoPlugin(xml);

	free(data.data);

	return returnValue;
}

#else

bool CGps::GeolocalisationGPS(const wxString &latitude, const wxString &longitude)
{
    printf("CGps::GeolocalisationGPS \n");
      
	//int error = 0;
    bool returnValue = true;
	wxString xml = L"";
	wxString httpAdress = serverHttp;
	httpAdress.append(L"/extras/location.gp?lat=");
	httpAdress.append(latitude);
	httpAdress.append(L"&long=");
	httpAdress.append(longitude);
	httpAdress.append(L"&format=xml");
   
    try
    {
        printf("http address : %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
        wxURL url(httpAdress);
        if (url.GetError() == wxURL_NOERR)
        {
            wxInputStream *in_stream = url.GetInputStream();
            if(in_stream != nullptr)
            {
                if (in_stream && in_stream->IsOk()) {
                    wxStringOutputStream html_stream(&xml);
                    in_stream->Read(html_stream);
                }
            }
            
            if(in_stream == nullptr)
            {
                geoPluginVector.clear();
                returnValue = false;
            }
            else
                delete in_stream;


            geoPluginVector.clear();
            ImportationGeoPlugin(xml);
        }
        else
        {
            printf("URL ERROR %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
            geoPluginVector.clear();
            returnValue = false;
        }
    }
    catch(...)
    {
        printf("wxURL ERROR CRASH %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
    }

	return returnValue;
}
#endif


GeoPluginVector * CGps::GetGpsList()
{
	return &geoPluginVector;
}

wxString CGps::FindElement(const wxString &xml, const wxString &baliseBegin, const wxString &baliseEnd)
{
	size_t i = xml.find(baliseBegin);
	if (i == -1)
		return L"";

	i += baliseBegin.size();
	size_t j = xml.find(baliseEnd, i);

	return xml.substr(i, j - i);
}


bool CGps::ImportationGeoPlugin(const wxString &xml)
{
	int i = 0;
	//int j = 0;
	wxString data = L"";
	wxString value = L"";
	wxString xmlData = xml;
	wxString baliseBegin = L"<geoPlugin>";
	wxString baliseEnd = L"</geoPlugin>";
	do
	{
		data = FindElement(xmlData, baliseBegin, baliseEnd);
		if (data != L"")
		{
			CGeoPluginValue geoValue;
			value = FindElement(data, L"<geoplugin_place>", L"</geoplugin_place>");
			geoValue.SetPlace(value);
			value = FindElement(data, L"<geoplugin_countryCode>", L"</geoplugin_countryCode>");
			geoValue.SetCountryCode(value);
			value = FindElement(data, L"<geoplugin_region>", L"</geoplugin_region>");
			geoValue.SetRegion(value);

			geoPluginVector.push_back(geoValue);

			i = (int)xmlData.find(baliseEnd);
			if (i != -1)
			{
				i += baliseEnd.length();
			}
			xmlData = xml.substr(i, xml.size() - i);
		}
	} while (data != L"");

	return true;

}

