#pragma once

class CGeoPluginValue
{
public:
	CGeoPluginValue();
	~CGeoPluginValue();

	void SetPlace(const wxString& place);
	wxString GetPlace();

	void SetCountryCode(const wxString& countryCode);
	wxString GetCountryCode();

	void SetRegion(const wxString& region);
	wxString GetRegion();

	void SetLatitude(const wxString& latitude);
	wxString GetLatitude();

	void SetLongitude(const wxString& longitude);
	wxString GetLongitude();

	void SetDistanceMiles(const wxString& distanceMiles);
	wxString GetDistanceMiles();

	void SetDistanceKm(const wxString& distanceKm);
	wxString GetDistanceKm();

	void SetDirectionAngle(const wxString& directionAngle);
	wxString GetDirectionAngle();

	void SetDirectionHeadling(const wxString& directionHeadling);
	wxString GetDirectionHeadling();

private:
	wxString place;
	wxString countryCode;
	wxString region;
	wxString latitude;
	wxString longitude;
	wxString distanceMiles;
	wxString distanceKm;
	wxString directionAngle;
	wxString directionHeadling;
};

using GeoPluginVector = std::vector<CGeoPluginValue>;
