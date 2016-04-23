#include "FileGeolocation.h"
#include <libPicture.h>
#ifdef EXIV2
#include <PictureMetadata.h>
#endif
#ifdef __APPLE__
#include "AppleReadExif.h"
#include <MediaInfo.h>
#endif
#ifdef WIN32
#include <PictureMetadata.h>
#include <MediaInfo.h>
#endif
#include "Gps.h"
#include <SqlPhotos.h>
#include <SqlCriteria.h>
#include <SqlCountry.h>
#include <SqlCatalog.h>
#include <SqlFolderCatalog.h>
#include <SqlPhotoCriteria.h>
#include <algorithm>
using namespace Regards::Internet;
using namespace Regards::Sqlite;

CountryVector CFileGeolocation::countryVector;

CFileGeolocation::CFileGeolocation(const wxString &urlServer)
{
    this->urlServer = urlServer;
};


wxString CFileGeolocation::GetLatitude()
{
	return latitudeGps;
}

wxString CFileGeolocation::GetLongitude()
{
	return longitudeGps;
}

bool CFileGeolocation::HasGps()
{
	return hasGps;
}

bool CFileGeolocation::HasDateTime()
{
	return hasDataTime;
}

wxString CFileGeolocation::GetFilename()
{
	return filename;
}

wxString CFileGeolocation::GetDateTimeInfos()
{
	return dateTimeInfos;
}

wxString CFileGeolocation::GetGpsInformation()
{
	return infoGpsLocalisation;
}

void CFileGeolocation::Geolocalize()
{
    if(hasGps)
    {
        /*
        CRegardsConfigParam * param = CParamInit::getInstance();
        if (param != nullptr)
        {
            urlServer = param->GetUrlServer();
        }
        */
        CGps * gps = new CGps(urlServer);
        infoGpsLocalisation = L"";
        //Execution de la requÍte de gÈolocalisation
        if (gps->GeolocalisationGPS(GetLatitude(), GetLongitude()))
        {
            GeoPluginVector * geoPluginVector = gps->GetGpsList();
            for (CGeoPluginValue geoValue : *geoPluginVector)
            {
                infoGpsLocalisation.append(geoValue.GetCountryCode());
                infoGpsLocalisation.append(L".");
                infoGpsLocalisation.append(geoValue.GetRegion());
                infoGpsLocalisation.append(L".");
                infoGpsLocalisation.append(geoValue.GetPlace());
                break;
            }
        }
        
        delete gps;
    }
}

void CFileGeolocation::ImportCountry()
{
	if (countryVector.size() == 0)
	{
		CSqlCountry sqlCountry;
		sqlCountry.GetCountry(&countryVector);
	}
}

bool CFileGeolocation::Geolocalisation(CListCriteriaPhoto * listCriteriaPhoto)
{
	CSqlCriteria sqlCriteria;
	CSqlPhotoCriteria sqlPhotoCriteria;
	
	//Execution de la requête de géolocalisation
	if (hasGps)
	{
		CGps * gps = new CGps(urlServer);
		if (gps->GeolocalisationGPS(GetLatitude(), GetLongitude()))
		{
			ImportCountry();
			GeoPluginVector * geoPluginVector = gps->GetGpsList();
			for (GeoPluginVector::iterator it = geoPluginVector->begin(); it != geoPluginVector->end(); it++)
			{

				CGeoPluginValue geoValue = *it;
				wxString value;
				CCountry local;
				CountryVector::iterator itCountry;
				local.SetCode(geoValue.GetCountryCode());

				//Recherche du nom du pays
				itCountry = std::find(countryVector.begin(), countryVector.end(), local);

				if (itCountry != countryVector.end())
				{
					//bool createCriteria = false;
					value.append(itCountry->GetContinent());
					value.append(L".");
					value.append(itCountry->GetLibelle());
					value.append(L".");
					value.append(geoValue.GetRegion());
					value.append(L".");
					value.append(geoValue.GetPlace());

					CInsertCriteria * insertCriteria = new CInsertCriteria();
					insertCriteria->type = CATEGORIE_GEO;
					insertCriteria->value = value;
					listCriteriaPhoto->listCriteria.push_back(insertCriteria);
				}
			}
		}
	}
	else
		return false;

	return true;
}

void CFileGeolocation::SetFile(const wxString & picture)
{
	filename = picture;
	dateTimeInfos = "";
    latitudeGps = "";
    longitudeGps = "";
	hasDataTime = false;
    hasGps = false;
    
    
#ifdef EXIV2
    CLibPicture libPicture;
	CPictureMetadata pictureMetadata(filename);
    wxString latitude;
    wxString longitude;
    
	if(libPicture.TestIsVideo(filename))
	{
		pictureMetadata.ReadVideo(hasGps, hasDataTime, dateTimeInfos, latitude, longitude);
	}
	else
	{
		pictureMetadata.ReadPicture(hasGps, hasDataTime, dateTimeInfos, latitude, longitude);

		printf("longitude %s \n",longitude.ToStdString().c_str());
		printf("latitude %s \n",latitude.ToStdString().c_str());

		if (!(hasGps && latitude.size() > 0 && longitude.size() > 0))
			hasGps = false;


	}
	
	if (hasGps)
	{
		latitudeGps = latitude;
		longitudeGps = longitude;
	}
	else
	{
		latitudeGps = "";
		longitudeGps = "";
	}

#endif
    
#if defined(__APPLE__) || defined(WIN32)
    
    CLibPicture libPicture;
    if(libPicture.TestIsVideo(filename))
    {
        //bool apple = false;
        //Extraction des données de mediainfolib et recherche de la date et des infos GPS
        CMediaInfo mediaInfo;
        vector<CMetadata> listItem = mediaInfo.ReadMetadata(picture);
        for(CMetadata meta : listItem)
        {
            
            wxString key = meta.key;
            wxString value = meta.value;

			if (key.Find("Recorded date") != std::string::npos)
			{
				dateTimeInfos = value; hasDataTime = true;
			}
			else if (key.Find("apple") != std::string::npos && key.Find("location") != std::string::npos)
            {
                hasGps = true;
                wxString listValue[3];
                wchar_t listRef[3];
                int iStart = -1;
                
                for (int i = 0; i < value.size(); i++)
                {
                    char charValue = value.at(i);
                    if (charValue == '+')
                    {
                        iStart++;
                        listRef[iStart] = '+';
                    }
                    else if (charValue == '-')
                    {
                        iStart++;
                        listRef[iStart] = '-';
                    }
                    else
                    {
                        listValue[iStart] += charValue;
                    }
                }
                
                if (listRef[0] == '-')
                    flatitude = -atof(listValue[0].c_str());
                else
                    flatitude = atof(listValue[0].c_str());
                
                if (listRef[1] == '-')
                    flongitude = -atof(listValue[1].c_str());
                else
                    flongitude = atof(listValue[1].c_str());
                
                latitudeGps = to_string(flatitude);
                longitudeGps = to_string(flongitude);
                
				hasGps = true;
            }
            else if (key.Find("apple") != std::string::npos && key.Find("creationdate") >= 0)
            {
				dateTimeInfos = value; hasDataTime = true;
            }
            
        }
        
    }
    else if(libPicture.TestImageFormat(filename))
    {
        flatitude = 0.0;
        flongitude = 0.0;
        
#ifdef __APPLE__
        CAppleReadExif appleExif;
        appleExif.ReadGps(filename, flatitude, flongitude);
        
        if (flatitude != 0.0 &&  flongitude != 0.0)
        {
            latitudeGps = to_wstring(flatitude);
            longitudeGps = to_wstring(flongitude);
            hasGps = true;
        }
        else
        {
            hasGps = false;
            latitudeGps = "";
            longitudeGps = "";
        }
        
        if(!appleExif.ReadDateTime(filename, dateTimeInfos))
            hasDataTime = false;
        else
            hasDataTime = true;

#else

		wxString latitudeRef;
		wxString longitudeRef;

		CPictureMetadata metadata(filename);
		if (metadata.GetGpsInformation(latitudeRef, longitudeRef, latitudeGps, longitudeGps))
			hasGps = true;
		else
			hasGps = false;

		dateTimeInfos = metadata.GetCreationDate();
		if (dateTimeInfos == "")
			hasDataTime = false;
		else
			hasDataTime = true;

#endif
    }
    


    
#endif
    
    //if(hasGps)
    //    Geolocalize();
    
    if(!hasDataTime)
    {
        hasDataTime = true;
        wxStructStat strucStat;
        wxStat(picture, &strucStat);
        //wxFileOffset filelen=strucStat.st_size;
        wxDateTime last_modified_time(strucStat.st_mtime);
        dateTimeInfos = last_modified_time.FormatISOCombined(' ');

    }

}

