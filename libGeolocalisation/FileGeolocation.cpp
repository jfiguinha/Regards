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
#include <SqlGps.h>
#include <SqlFolderCatalog.h>
#include <SqlPhotoCriteria.h>
#include <LibResource.h>
using namespace Regards::Internet;
using namespace Regards::Sqlite;

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

float CFileGeolocation::GetFLatitude()
{
    double val;
    latitudeGps.ToDouble(&val);
    return val;
}

float CFileGeolocation::GetFLongitude()
{
    double val;
    longitudeGps.ToDouble(&val);
    return val;
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

void CFileGeolocation::RefreshData()
{
    PhotoGpsVector photogpsVector;
    CriteriaVector criteriaVector;
    //Recherche des données dans la base
    CSqlPhotos sqlPhotos;
    sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, filename, 1);
    if(criteriaVector.size() > 0)
    {
        //Gps Infos
        wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO",1);
        wxString libelle = criteriaVector.at(0).GetLibelle();
        if(libelle != notGeo)
        {
            infoGpsLocalisation = libelle;
            //Recherche des infos GPS dans la base de données
            CSqlGps sqlGps;
            sqlGps.GetGps(&photogpsVector, filename);
            if(photogpsVector.size() > 0)
            {
                CPhotoGps photoGps = photogpsVector.at(0);
                latitudeGps = photoGps.GetLatitude();
                longitudeGps = photoGps.GetLongitude();
                hasGps = true;
            }
        }
        
    }
    //DateTime
    criteriaVector.clear();
    sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, filename, 3);
    if(criteriaVector.size() > 0)
    {
        dateTimeInfos = criteriaVector.at(0).GetLibelle();
        hasDataTime = true;
    }
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
                wxString value = GenerateGeolocalisationString(geoValue.GetCountryCode(),geoValue.GetRegion(),geoValue.GetPlace());
				if (value != "")
				{
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

wxString CFileGeolocation::GenerateGeolocalisationString(const wxString &countryCode, const wxString & region, const wxString &place)
{
    ImportCountry();
    wxString value;
    CCountry local;
    CountryVector::iterator itCountry;
    local.SetCode(countryCode);

    //Recherche du nom du pays
    itCountry = std::find(countryVector.begin(), countryVector.end(), local);

    if (itCountry != countryVector.end())
    {
        //bool createCriteria = false;
        value.append(itCountry->GetContinent());
        value.append(L".");
        value.append(itCountry->GetLibelle());
        value.append(L".");
        value.append(region);
        value.append(L".");
        value.append(place);
        
        return value;
    }
    
    return "";
}

void CFileGeolocation::SetFile(const wxString & picture, const bool &onlyFromFile)
{
    CLibPicture libPicture;
	filename = picture;
	dateTimeInfos = "";
    latitudeGps = "";
    longitudeGps = "";
	hasDataTime = false;
    hasGps = false;
    
    if(!onlyFromFile)
    {
        RefreshData();
    
        if(hasGps && hasDataTime)
            return;
    }
    
#ifdef EXIV2

	CPictureMetadata pictureMetadata(filename);
	if(libPicture.TestIsVideo(filename))
	{
		pictureMetadata.ReadVideo(hasGps, hasDataTime, dateTimeInfos, latitudeGps, longitudeGps);
	}
	else
	{
		pictureMetadata.ReadPicture(hasGps, hasDataTime, dateTimeInfos, latitudeGps, longitudeGps);
	}

#else
    
#if defined(__APPLE__) || defined(WIN32)
    
    
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
#endif
    }
    


    
#endif
    
    if(hasGps)
    {
        PhotoGpsVector photogpsVector;
        CSqlGps sqlGps;
        sqlGps.GetGps(&photogpsVector, filename);
        if(photogpsVector.size() == 0)
        {
            sqlGps.InsertGps(filename, latitudeGps, longitudeGps);
        }
    }
    else if(onlyFromFile)
    {
        PhotoGpsVector photogpsVector;
        CriteriaVector criteriaVector;
        //Recherche des données dans la base
        CSqlPhotos sqlPhotos;
        sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, filename, 1);
        if(criteriaVector.size() > 0)
        {
            //Gps Infos
            wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO",1);
            wxString libelle = criteriaVector.at(0).GetLibelle();
            if(libelle != notGeo)
            {
                infoGpsLocalisation = libelle;
                //Recherche des infos GPS dans la base de données
                CSqlGps sqlGps;
                sqlGps.GetGps(&photogpsVector, filename);
                if(photogpsVector.size() > 0)
                {
                    CPhotoGps photoGps = photogpsVector.at(0);
                    latitudeGps = photoGps.GetLatitude();
                    longitudeGps = photoGps.GetLongitude();
                    if(latitudeGps != "" && longitudeGps != "")
                        hasGps = true;
                }
            }
        }
    }
    
    if(!hasDataTime && onlyFromFile)
    {
        CriteriaVector criteriaVector;
        CSqlPhotos sqlPhotos;

        sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, filename, 3);
        if(criteriaVector.size() > 0)
        {
            dateTimeInfos = criteriaVector.at(0).GetLibelle();
            hasDataTime = true;
        }
    }
    
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

