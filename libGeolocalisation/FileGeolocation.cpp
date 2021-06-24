#include "header.h"
#include "FileGeolocation.h"
#include <libPicture.h>
#ifdef EXIV2
#include <MetadataExiv2.h>
#endif
#include "Gps.h"
#include <SqlPhotos.h>
#include <SqlCriteria.h>
#include <SqlCountry.h>
#include <SqlGps.h>
#include <window_id.h>
#include <SqlPhotoCriteria.h>
//#include <LibResource.h>
#include <MediaInfo.h>
using namespace Regards::Internet;
using namespace Regards::Sqlite;
using namespace Regards::Picture;
using namespace Regards::exiv2;
CountryVector CFileGeolocation::countryVector;

CFileGeolocation::CFileGeolocation(const wxString& urlServer)
{
	this->urlServer = urlServer;
	dateTimeInfos = "";
	hasGps = false;
	hasDataTime = false;
	latitudeGps = L"";
	longitudeGps = L"";
	flatitude = 0.0;
	flongitude = 0.0;
	infoGpsLocalisation = L"";
	filename = L"";
	isThumbnail = false;
};

CFileGeolocation::CFileGeolocation(const CFileGeolocation& filegeo)
{
	dateTimeInfos = filegeo.dateTimeInfos;
	hasGps = filegeo.hasGps;
	hasDataTime = filegeo.hasDataTime;
	latitudeGps = filegeo.latitudeGps;
	longitudeGps = filegeo.longitudeGps;
	flatitude = filegeo.flatitude;
	flongitude = filegeo.flongitude;
	infoGpsLocalisation = filegeo.infoGpsLocalisation;
	filename = filegeo.filename;
	isThumbnail = filegeo.isThumbnail;
	//gpsInfos = filegeo.gpsInfos;
	urlServer = filegeo.urlServer;
}

wxString CFileGeolocation::GetUrlServer()
{
	return urlServer;
}


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

void CFileGeolocation::Geolocalize()
{
	if (hasGps)
	{
		auto gps = new CGps(urlServer);
		infoGpsLocalisation = L"";
		//Execution de la requÍte de gÈolocalisation
		if (gps->GeolocalisationGPS(GetLatitude(), GetLongitude()))
		{
			GeoPluginVector* geoPluginVector = gps->GetGpsList();

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


bool CFileGeolocation::Geolocalisation(CListCriteriaPhoto* listCriteriaPhoto)
{
	CSqlCriteria sqlCriteria;
	CSqlPhotoCriteria sqlPhotoCriteria;

	printf("CFileGeolocation Geolocalisation \n");
	//Execution de la requête de géolocalisation
	if (hasGps)
	{
		auto gps = new CGps(urlServer);
		try
		{
			if (gps->GeolocalisationGPS(GetLatitude(), GetLongitude()))
			{
				printf("GeolocalisationGPS OK \n");
				ImportCountry();
				GeoPluginVector* geoPluginVector = gps->GetGpsList();
				for (auto it = geoPluginVector->begin(); it != geoPluginVector->end(); ++it)
				{
					CGeoPluginValue geoValue = *it;
					wxString value = GenerateGeolocalisationString(geoValue.GetCountryCode(), geoValue.GetRegion(),
					                                               geoValue.GetPlace());
					if (value != "")
					{
						auto insertCriteria = new CInsertCriteria();
						insertCriteria->type = CATEGORIE_GEO;
						insertCriteria->value = value;
						listCriteriaPhoto->listCriteria.push_back(insertCriteria);
					}
				}
			}
			else
				printf("GeolocalisationGPS FALSE \n");
		}
		catch (...)
		{
			printf("GeolocalisationGPS CGps * Error \n");
		}

		delete gps;
	}
	else
		return false;

	return true;
}

wxString CFileGeolocation::GenerateGeolocalisationString(const wxString& countryCode, const wxString& region,
                                                         const wxString& place)
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


void CFileGeolocation::SetFile(const wxString& picture, const wxString& libNotGeo)
{
	CLibPicture libPicture;
	filename = picture;
	dateTimeInfos = "";
	latitudeGps = "";
	longitudeGps = "";
	hasDataTime = false;
	hasGps = false;

	//exiv2::CMetadataExiv2 pictureMetadata(filename);
	if (libPicture.TestIsVideo(filename))
	{
		vector<CMetadata> listMeta = CMediaInfo::ReadMetadata(filename);
		for (CMetadata metadata : listMeta)
		{
			if (metadata.key == "General.com.apple.quicktime.creationdate")
			{
				//Create Date
				hasDataTime = true;
				dateTimeInfos = metadata.value;
			}
			if (metadata.key == "General.com.apple.quicktime.location.ISO6709")
			{
				wxString exifinfos = metadata.value;
				hasGps = true;
				wxString listValue[3];
				wchar_t listRef[3];
				int iStart = -1;

				for (auto i = 0; i < exifinfos.size(); i++)
				{
					char charValue = exifinfos.at(i);
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
			}
		}
	}
	else
	{
		CMetadataExiv2 pictureMetadata(filename);
		pictureMetadata.ReadPicture(hasGps, hasDataTime, dateTimeInfos, latitudeGps, longitudeGps);
	}

	if (hasGps)
	{
		PhotoGpsVector photogpsVector;
		CriteriaVector criteriaVector;
		//Recherche des données dans la base
		CSqlPhotos sqlPhotos;
		sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, filename, 1);
		if (criteriaVector.size() > 0)
		{
			//Gps Infos
			//wxString notGeo = CLibResource::LoadStringFromResource("LBLNOTGEO",1);
			wxString libelle = criteriaVector.at(0).GetLibelle();
			if (libelle != libNotGeo)
			{
				infoGpsLocalisation = libelle;
				//Recherche des infos GPS dans la base de données
				CSqlGps sqlGps;
				sqlGps.GetGps(&photogpsVector, filename);
				if (photogpsVector.size() > 0)
				{
					CPhotoGps photoGps = photogpsVector.at(0);
					latitudeGps = photoGps.GetLatitude();
					longitudeGps = photoGps.GetLongitude();
					if (latitudeGps != "" && longitudeGps != "")
						hasGps = true;
				}
			}
		}
	}

	if (!hasDataTime)
	{
		CriteriaVector criteriaVector;
		CSqlPhotos sqlPhotos;

		sqlPhotos.GetPhotoCriteriaByCategorie(&criteriaVector, filename, 3);
		if (criteriaVector.size() > 0)
		{
			dateTimeInfos = criteriaVector.at(0).GetLibelle();
			hasDataTime = true;
		}

		hasDataTime = true;
		wxStructStat strucStat;
		wxStat(picture, &strucStat);
		//wxFileOffset filelen=strucStat.st_size;
		wxDateTime last_modified_time(strucStat.st_mtime);
		dateTimeInfos = last_modified_time.FormatISOCombined(' ');
	}
}
