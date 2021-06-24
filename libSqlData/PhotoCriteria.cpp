#include <header.h>
#include "PhotoCriteria.h"


CPhotoCriteria::CPhotoCriteria(): numPhotoId(0), numCriteriaId(0)
{
}


CPhotoCriteria::~CPhotoCriteria()
{
}

void CPhotoCriteria::SetPhotoId(const int& numId)
{
	this->numPhotoId = numId;
}

int CPhotoCriteria::GetPhotoId()
{
	return this->numPhotoId;
}

void CPhotoCriteria::SetCriteriaId(const int& numId)
{
	this->numCriteriaId = numId;
}

int CPhotoCriteria::GetCriteriaId()
{
	return numCriteriaId;
}
