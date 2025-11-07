#include "header.h"
#include "Gps.h"
#include <curl/curl.h>
#include <ConvertUtility.h>
#include <wx/sstream.h>
using namespace Regards::Internet;

class CGpscurl
{
public:
	CGpscurl()
	{
		curl = curl_easy_init();
	}

	~CGpscurl()
	{
		if (curl != nullptr)
		{
			curl_easy_cleanup(curl);
		}
	}


	static size_t write_data(void* ptr, size_t size, size_t nmemb, struct url_data* data)
	{
		size_t index = data->size;
		size_t n = (size * nmemb);
		char* tmp;

		data->size += (size * nmemb);

#ifdef DEBUG
		fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
		tmp = static_cast<char*>(realloc(data->data, data->size + 1)); /* +1 for '\0' */

		if (tmp != nullptr)
		{
			data->data = tmp;
		}
		else
		{
			if (data->data)
			{
				free(data->data);
			}
			fprintf(stderr, "Failed to allocate memory.\n");
			return 0;
		}

		memcpy((data->data + index), ptr, n);
		data->data[data->size] = '\0';

		return size * nmemb;
	}

	void PerformHttpGet(url_data* data, const char* httpAdress)
	{
		if (curl != nullptr)
		{
			CURLcode res;
			curl_easy_setopt(curl, CURLOPT_URL, httpAdress);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
			//curl_share_setopt(curl, CURLSHOPT_LOCKFUNC);
			res = curl_easy_perform(curl); /* ignores error */
			if (res != CURLE_OK)
			{
				wxString error = curl_easy_strerror(res);
				//wxMessageBox(error);
				printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			}
			//curl_share_setopt(curl, CURLSHOPT_UNLOCKFUNC);
		}
	}

	CURL* curl = nullptr;
};


//Test parameter /extras/location.gp?lat=48.896168&long=2.387500&format=xml wxString server = L"www.geoplugin.net";

CGps::CGps(const wxString& server, const wxString& apiKey)
{
	serverHttp = server;
	gpsUrl = new CGpscurl();
	this->apiKey = apiKey;
	if(apiKey != L"")
		isLocalisationAvailable = true;
	else
		isLocalisationAvailable = false;
	//OpenHttpRequest(server, error);
}


CGps::~CGps()
{
	if (gpsUrl)
		delete gpsUrl;
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

	for (auto it = latValue.begin(); it != latValue.end(); ++it)
	{
		vector<wxString> intValue = CConvertUtility::split(*it, '/');
		int valeur = atoi(intValue.at(0));
		int diviseur = atoi(intValue.at(1));

		float value = static_cast<float>(valeur) / static_cast<float>(diviseur);
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

bool CGps::GeolocalisationGPS(const wxString& latitude, const wxString& longitude)
{
	bool returnValue = true;

	if (!isLocalisationAvailable)
	{
		return false;
	}

	
	try
	{
		printf("CGps GeolocalisationGPS \n");


		this->latitude = latitude;
		this->longitude = longitude;

		//https://api.geoapify.com/v1/geocode/reverse?"; //lat=52.478117501285965&lon=13.47717282413089&type=postcode&apiKey=
		//wxString xml = L"";
		wxString httpAdress = serverHttp;
		httpAdress.append(L"/v1/geocode/reverse?lat=");
		httpAdress.append(latitude);
		httpAdress.append(L"&lon=");
		httpAdress.append(longitude);
		httpAdress.append("&format=xml");
		httpAdress.append(L"&type=postcode");
		httpAdress.append(L"&apiKey=");
		httpAdress.append(apiKey);
		struct url_data data;
		data.size = 0;
		data.data = static_cast<char*>(malloc(4096)); /* reasonable size initial buffer */
		if (nullptr == data.data)
		{
			printf("CGps GeolocalisationGPS Failed to allocate memory \n");
			fprintf(stderr, "Failed to allocate memory.\n");
			return false;
		}

		data.data[0] = '\0';
		gpsUrl->PerformHttpGet(&data, CConvertUtility::ConvertToUTF8(httpAdress));

		//wxString mystring2(chars, wxConvUTF8);
		wxString xml(data.data, wxConvUTF8);

		printf("URL  : %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
		printf("Data : %s \n", data.data);

		geoPluginVector.clear();
		ImportationGeocodePlugin(xml);

		free(data.data);

		if(xml.size() == 0)
			returnValue = false;
	}
	catch (...)
	{
		returnValue = false;
	}


	return returnValue;
}

GeoPluginVector* CGps::GetGpsList()
{
	return &geoPluginVector;
}

wxString CGps::FindElement(const wxString& xml, const wxString& baliseBegin, const wxString& baliseEnd)
{
	size_t i = xml.find(baliseBegin);
	if (i == -1)
		return L"";

	i += baliseBegin.size();
	size_t j = xml.find(baliseEnd, i);

	return xml.substr(i, j - i);
}

/*
bool CGps::ImportationGeoPlugin(const wxString& xml)
{
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

			int i = static_cast<int>(xmlData.find(baliseEnd));
			if (i != -1)
			{
				i += baliseEnd.length();
			}
			xmlData = xml.substr(i, xml.size() - i);
		}
	}
	while (data != L"");

	return true;
}
*/

bool CGps::ImportationGeocodePlugin(const wxString& xml)
{
	//int j = 0;
	wxString data = L"";
	wxString value = L"";
	wxString xmlData = xml;
	wxString baliseBegin = L"<results>";
	wxString baliseEnd = L"</results>";

	std::size_t found = xml.find("Bad Request");
	if (found != std::string::npos)
	{
		CGeoPluginValue geoValue;
		geoValue.SetAddress("not found");
		geoValue.SetPlace("not found");
		geoValue.SetCountryCode(value);
		geoValue.SetCity(value);
		geoValue.SetRegion(value);
		geoPluginVector.push_back(geoValue);
	}
	else
	{
		do
		{
			data = FindElement(xmlData, baliseBegin, baliseEnd);
			if (data != L"")
			{
				CGeoPluginValue geoValue;
				value = FindElement(data, L"<formatted>", L"</formatted>");
				geoValue.SetAddress(value);
				value = FindElement(data, L"<address_line1>", L"</address_line1>");
				geoValue.SetPlace(value);
				value = FindElement(data, L"<country_code>", L"</country_code>");
				geoValue.SetCountryCode(value);
				value = FindElement(data, L"<city>", L"</city>");
				geoValue.SetCity(value);
				value = FindElement(data, L"<state>", L"</state>");
				geoValue.SetRegion(value);
				geoPluginVector.push_back(geoValue);

				int i = static_cast<int>(xmlData.find(baliseEnd));
				if (i != -1)
				{
					i += baliseEnd.length();
				}
				xmlData = xml.substr(i, xml.size() - i);
			}
		} while (data != L"");
	}

	return true;
}
