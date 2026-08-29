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
#include <appcontext.h>
#include <ConvertUtility.h>
#include <MediaInfo.h>
#include <memory>
using namespace Regards::Internet;
using namespace Regards::Sqlite;
using namespace Regards::Picture;
using namespace Regards::exiv2;
extern AppContext application_context;
CountryVector CFileGeolocation::countryVector;

CFileGeolocation::CFileGeolocation(const wxString& urlServer, const wxString& apiKey)
{
	this->urlServer = urlServer;
	this->apiKey = apiKey;
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
	apiKey = filegeo.apiKey; // BUGFIX: apiKey n'était pas copié
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

wxString CFileGeolocation::Geolocalize()
{
	wxString geolocalisationString = "";
	if (hasGps)
	{
		// BUGFIX: unique_ptr pour être exception-safe (GeolocalisationGPS peut lever)
		auto gps = std::make_unique<CGps>(urlServer, apiKey);
		infoGpsLocalisation = L"";
		try
		{
			//Execution de la requête de géolocalisation
			if (gps->GeolocalisationGPS(GetLatitude(), GetLongitude()))
			{
				GeoPluginVector* geoPluginVector = gps->GetGpsList();

				for (CGeoPluginValue geoValue : *geoPluginVector)
				{
					geolocalisationString = GenerateGeolocalisationString(geoValue.GetCountryCode(), geoValue.GetRegion(), geoValue.GetCity());
					break;
				}
			}
		}
		catch (...)
		{
			//printf("Geolocalize CGps Error \n");
		}
	}
	return geolocalisationString;
}


void CFileGeolocation::ImportCountry()
{
	if (countryVector.size() == 0)
	{
		CSqlCountry sqlCountry;
		sqlCountry.GetCountry(&countryVector);
	}
}


wxString CFileGeolocation::Geolocalisation(CListCriteriaPhoto* listCriteriaPhoto)
{
	CSqlCriteria sqlCriteria;
	CSqlPhotoCriteria sqlPhotoCriteria;
	wxString value = "";
	//Execution de la requête de géolocalisation
	if (hasGps)
	{
		// BUGFIX: unique_ptr pour être exception-safe (idem)
		auto gps = std::make_unique<CGps>(urlServer, apiKey);
		try
		{
			if (gps->GeolocalisationGPS(GetLatitude(), GetLongitude()))
			{
				//printf("GeolocalisationGPS OK \n");
				ImportCountry();
				GeoPluginVector* geoPluginVector = gps->GetGpsList();
				for (auto it = geoPluginVector->begin(); it != geoPluginVector->end(); ++it)
				{
					CGeoPluginValue geoValue = *it;
					value = GenerateGeolocalisationString(geoValue.GetCountryCode(), geoValue.GetRegion(),
						geoValue.GetCity());
					break;
				}
			}
			//else
			//	printf("GeolocalisationGPS FALSE \n");
		}
		catch (...)
		{
			//printf("GeolocalisationGPS CGps * Error \n");
		}
	}

	return value;
}

wxString CFileGeolocation::GenerateGeolocalisationString(const wxString& countryCode, const wxString& region,
	const wxString& place)
{
	ImportCountry();
	wxString value;
	CCountry local;
	CountryVector::iterator itCountry;
	wxString str = countryCode;
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	local.SetCode(str);

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
	else if (place == "not found")
	{
		value.append(place);
		return value;
	}


	return "";
}

void CFileGeolocation::SetInfosGPS(const wxString& libelle)
{

	vector<wxString> gpsInfos = CConvertUtility::split(libelle, '/');
	if (gpsInfos.size() == 3)
	{
		if (gpsInfos[0] == application_context.special_key)
		{
			hasGps = true;
			latitudeGps = gpsInfos.at(1);
			longitudeGps = gpsInfos.at(2);
		}
	}




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
		std::vector<CMetadata> vectorMeta = CMediaInfo::ReadMetadata(filename);
		{
			std::vector<CMetadata>::iterator it = std::find_if(vectorMeta.begin(), vectorMeta.end(), [&](CMetadata val) -> bool {return val.key == "General.com.apple.quicktime.creationdate"; });
			if (it != vectorMeta.end())
			{
				//Create Date
				hasDataTime = true;
				dateTimeInfos = it->value;
			}
			if (!hasDataTime)
			{
				std::vector<CMetadata>::iterator it = std::find_if(vectorMeta.begin(), vectorMeta.end(), [&](CMetadata val) -> bool {return val.key == "General.Recorded date"; });
				if (it != vectorMeta.end())
				{
					//Create Date
					hasDataTime = true;
					dateTimeInfos = it->value;
				}
			}


		}
		{
			std::vector<CMetadata>::iterator it = std::find_if(vectorMeta.begin(), vectorMeta.end(), [&](CMetadata val) -> bool {return val.key == "General.com.apple.quicktime.location.ISO6709"; });
			if (it != vectorMeta.end())
			{
				wxString exifinfos = it->value;

				// BUGFIX: parsing sécurisé du format ISO6709.
				// - iStart démarre à -1 : on ignore tout caractère avant le premier '+'/'-'
				//   au lieu d'écrire en listValue[-1].
				// - bornes vérifiées (max 3 composantes : latitude / longitude / altitude).
				// - listRef initialisé à 0 pour éviter une lecture de mémoire non initialisée.
				// - conversion via wxString::ToDouble() plutôt que atof(wxString) (fragile en Unicode/UTF-8).
				// - hasGps n'est positionné à true qu'après un parsing valide (latitude ET longitude trouvées).
				wxString listValue[3];
				wchar_t listRef[3] = { 0, 0, 0 };
				int iStart = -1;
				bool parseError = false;

				for (size_t i = 0; i < exifinfos.size(); i++)
				{
					wxUniChar charValue = exifinfos.at(i);
					if (charValue == '+' || charValue == '-')
					{
						iStart++;
						if (iStart >= 3)
						{
							parseError = true;
							break;
						}
						listRef[iStart] = (charValue == '-') ? L'-' : L'+';
					}
					else if (iStart >= 0)
					{
						listValue[iStart] += charValue;
					}
					// caractères avant le premier signe : ignorés (ne devrait pas arriver
					// avec un ISO6709 valide, mais on évite ainsi l'accès hors bornes)
				}

				if (!parseError && iStart >= 1)
				{
					double latVal = 0.0;
					double lonVal = 0.0;
					listValue[0].ToDouble(&latVal);
					listValue[1].ToDouble(&lonVal);

					flatitude = (listRef[0] == L'-') ? -static_cast<float>(latVal) : static_cast<float>(latVal);
					flongitude = (listRef[1] == L'-') ? -static_cast<float>(lonVal) : static_cast<float>(lonVal);

					latitudeGps = to_string(flatitude);
					longitudeGps = to_string(flongitude);

					hasGps = true;
				}
			}
			else
			{
				it = std::find_if(vectorMeta.begin(), vectorMeta.end(), [&](CMetadata val) -> bool {return val.key == "General.Recorded location"; });
				if (it != vectorMeta.end())
				{
					wxString exifinfos = it->value;
					std::vector<wxString> gpsInfos = CConvertUtility::split(exifinfos, ' ');
					if (gpsInfos.size() >= 2)
					{
						const wxUniChar degreeSign(0x00B0); // '°' — portable Linux/Windows/macOS
						wxString latitude = gpsInfos[0];
						std::vector<wxString> gpsInfosLat = CConvertUtility::split(latitude, degreeSign);
						if (gpsInfosLat.size() > 1)
						{
							if (gpsInfosLat[1] == 'S')
								latitudeGps = "-" + gpsInfosLat[0];
							else
								latitudeGps = gpsInfosLat[0];
						}


						wxString longitude = gpsInfos[1];
						std::vector<wxString> gpsInfosLong = CConvertUtility::split(longitude, degreeSign);
						if (gpsInfosLong.size() > 1)
						{
							if (gpsInfosLong[1] == 'W')
								longitudeGps = "-" + gpsInfosLong[0];
							else
								longitudeGps = gpsInfosLong[0];
						}


						hasGps = true;
					}


				}
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