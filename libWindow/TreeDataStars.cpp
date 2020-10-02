#include <header.h>
#include "TreeDataStars.h"
using namespace Regards::Window;

CTreeDataStars::CTreeDataStars(void)
{
    type = 1;
	valeur = L"";
	exifKey = L"";
	key = L"";
	actif = false;
	type = 0;
}


CTreeDataStars::~CTreeDataStars(void)
{
}

int CTreeDataStars::GetNumPhotoId()
{
	return numPhotoId;
}

void CTreeDataStars::SetNumPhotoId(const int & numPhotoId)
{
	this->numPhotoId = numPhotoId;
}