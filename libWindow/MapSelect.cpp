#include "header.h"
//
//  MapSelect.cpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 01/10/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#include "MapSelect.h"
#include "MapDialogBox.h"
#include <ConvertUtility.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <SqlCountry.h>
#include <Gps.h>
#include <FileGeolocation.h>
using namespace Regards::Window;
using namespace Regards::Internet;
using namespace Regards::Sqlite;


CMapSelect::CMapSelect()
{
	latitude = "";
	longitude = "";
}

CMapSelect::~CMapSelect()
{
}

double CMapSelect::GetLatitudeNumber()
{
	double dVal = 0.0;
	latitude.ToDouble(&dVal);
	return dVal;
}

double CMapSelect::GetLongitudeNumber()
{
	double dVal = 0.0;
	longitude.ToDouble(&dVal);
	return dVal;
}

wxString CMapSelect::GetLatitude()
{
	return latitude;
}

wxString CMapSelect::GetLongitude()
{
	return longitude;
}


wxString CMapSelect::SelectNewMapLocalisation(wxWindow* window, const wxString& url)
{
	wxString infoGpsLocalisation = "";
	if (mapDialog == nullptr)
		mapDialog = new CMapDialogBox(window);
	mapDialog->SetUrl(url);
	printf("CMapSelect::SelectNewMapLocalisation 1 \n");
	if (mapDialog->ShowModal() == wxID_OK)
	{
		wxString getMapUrl = mapDialog->GetUrl();
		vector<wxString> mapInfos = CConvertUtility::split(getMapUrl, '#');
		if (mapInfos.size() == 2)
		{
			vector<wxString> gpsInfos = CConvertUtility::split(mapInfos.at(1), '/');
			latitude = gpsInfos.at(1);
			longitude = gpsInfos.at(2);

			wxString urlServer = "";
			//Géolocalisation
			CRegardsConfigParam* param = CParamInit::getInstance();
			if (param != nullptr)
			{
				urlServer = param->GetUrlServer();
			}
			CFileGeolocation fileGeo(urlServer);
			auto gps = new CGps(urlServer);

			//Execution de la requÍte de gÈolocalisation
			if (gps->GeolocalisationGPS(latitude, longitude))
			{
				GeoPluginVector* geoPluginVector = gps->GetGpsList();

				for (CGeoPluginValue geoValue : *geoPluginVector)
				{
					infoGpsLocalisation = fileGeo.GenerateGeolocalisationString(
						geoValue.GetCountryCode(), geoValue.GetRegion(), geoValue.GetPlace());
					break;
				}
			}

			delete gps;
		}
	}
	return infoGpsLocalisation;
}
