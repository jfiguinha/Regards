#include <header.h>
#include "GeoPluginValue.h"


CGeoPluginValue::CGeoPluginValue()
{
}


CGeoPluginValue::~CGeoPluginValue()
{
}

void CGeoPluginValue::SetPlace(const wxString &place)
{
	this->place = place;
}

wxString CGeoPluginValue::GetPlace()
{
	return place;
}

void CGeoPluginValue::SetCountryCode(const wxString &countryCode)
{
	this->countryCode = countryCode;
}

wxString CGeoPluginValue::GetCountryCode()
{
	return countryCode;
}

void CGeoPluginValue::SetRegion(const wxString &region)
{
	this->region = region;
}

wxString CGeoPluginValue::GetRegion()
{
	return region;
}

void CGeoPluginValue::SetLatitude(const wxString &latitude)
{
	this->latitude = latitude;
}

wxString CGeoPluginValue::GetLatitude()
{
	return latitude;
}

void CGeoPluginValue::SetLongitude(const wxString &longitude)
{
	this->longitude = longitude;
}

wxString CGeoPluginValue::GetLongitude()
{
	return longitude;
}

void CGeoPluginValue::SetDistanceMiles(const wxString &distanceMiles)
{
	this->distanceMiles = distanceMiles;
}

wxString CGeoPluginValue::GetDistanceMiles()
{
	return distanceMiles;
}

void CGeoPluginValue::SetDistanceKm(const wxString &distanceKm)
{
	this->distanceKm = distanceKm;
}

wxString CGeoPluginValue::GetDistanceKm()
{
	return distanceKm;
}

void CGeoPluginValue::SetDirectionAngle(const wxString &directionAngle)
{
	this->directionAngle = directionAngle;
}

wxString CGeoPluginValue::GetDirectionAngle()
{
	return directionAngle;
}

void CGeoPluginValue::SetDirectionHeadling(const wxString &directionHeadling)
{
	this->directionHeadling = directionHeadling;
}

wxString CGeoPluginValue::GetDirectionHeadling()
{
	return directionHeadling;
}