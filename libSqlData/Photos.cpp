#include <header.h>
#include "Photos.h"

CPhotos::CPhotos()
{
	criteriaInsert = 0;
	numId = -1;
	numFolderId = -1;
	path = "";
}


CPhotos::~CPhotos()
{
}

void CPhotos::SetGpsInfos(const wxString &gpsInfos)
{
	this->gpsInfos = gpsInfos;
}

wxString CPhotos::GetGpsInfos()
{
	return gpsInfos;
}

void CPhotos::SetCreateDate(const wxString &createDate)
{
	this->createDate = createDate;
}

wxString CPhotos::GetCreateDate()
{
	return createDate;
}

void CPhotos::SetId(const int &numId)
{
	this->numId = numId;
}

int CPhotos::GetId()
{
	return numId;
}

void CPhotos::SetFolderId(const int &numId)
{
	this->numFolderId = numId;
}

int CPhotos::GetFolderId()
{
	return numFolderId;
}

void CPhotos::SetPath(const wxString &path)
{
	this->path = path;
}

wxString CPhotos::GetPath()
{
	return path;
}

void CPhotos::SetIsCriteriaInsert(const int &criteriaInsert)
{
	this->criteriaInsert = criteriaInsert;
}

int CPhotos::GetIsCriteriaInsert()
{
	return criteriaInsert;
}