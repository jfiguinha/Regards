#include <header.h>
#include "ThumbnailData.h"
#include <ConvertUtility.h>


CThumbnailData::CThumbnailData(const wxString & filename)
{
	timePosition = 0;
	percent = 10;
	pictureLoad = false;
	pictureLoading = false;
	isProcess = false;
	typeElement = TYPEPHOTO;
	folder = "";
	key = "";
	numCategorie = 0;
	numParent = 0;
	numCatalog = 0;

	this->filename = filename;
	numElement = 0;
	libelleTimePosition = "";
	numPhotoId = -1;
}

void CThumbnailData::SetFilename(const wxString &filename)
{
	this->filename = filename;
}

void CThumbnailData::SetNumPhotoId(const int &id)
{
	numPhotoId = id;
}

int CThumbnailData::GetNumPhotoId()
{
	return numPhotoId;
}

CThumbnailData::~CThumbnailData(void)
{
}

int CThumbnailData::GetPercent()
{
	return percent;
}

void CThumbnailData::SetTimePosition(const int &timePosition)
{
	this->timePosition = timePosition;
	int hour = timePosition / 3600;
	int minute = (timePosition % 3600) / 60;
	int seconds = timePosition % 60;

	wxString shour;
	shour << hour;
	wxString sminute;
	sminute << minute;
	wxString sseconds;
	sseconds << seconds;

	libelleTimePosition = "";

	if (shour.size() < 2)
		libelleTimePosition.append("0");
	libelleTimePosition.append(shour);
	libelleTimePosition.append(":");
	if (sminute.size() < 2)
		libelleTimePosition.append("0");
	libelleTimePosition.append(sminute);
	libelleTimePosition.append(":");
	if (sseconds.size() < 2)
		libelleTimePosition.append("0");
	libelleTimePosition.append(sseconds);
}


int CThumbnailData::GetTimePosition()
{
	return timePosition;
}

wxString CThumbnailData::GetTimePositionLibelle()
{
	return libelleTimePosition;
}

void CThumbnailData::SetPercent(const int &percent)
{
	this->percent = percent;
}

void CThumbnailData::SetNumElement(const int &iNumElement)
{
	numElement = iNumElement;
}

int CThumbnailData::GetNumElement()
{
	return numElement;
}

wxString CThumbnailData::GetFilename()
{
	return filename;
}

bool CThumbnailData::IsLoad()
{
	if (!pictureLoad)
		pictureLoad = TestBitmap();
	return pictureLoad;
};

bool CThumbnailData::IsProcess()
{
	return isProcess;
}

void CThumbnailData::SetIsProcess(const bool &isProcess)
{
	this->isProcess = isProcess;
}

bool CThumbnailData::IsLoading()
{
	return pictureLoading;
};

void CThumbnailData::SetIsLoading(const bool &isLoading)
{
	pictureLoading = isLoading;
}

void CThumbnailData::SetTypeElement(const int &typeElement)
{
	this->typeElement = typeElement;
}

int CThumbnailData::GetTypeElement()
{
	return this->typeElement;
}

void CThumbnailData::SetNumCatalog(const int &numCatalog)
{
	this->numCatalog = numCatalog;
}

int CThumbnailData::GetNumCatalog()
{
	return this->numCatalog;
}

void CThumbnailData::SetNumCategorie(const int &numCategorie)
{
	this->numCategorie = numCategorie;
}

int CThumbnailData::GetNumCategorie()
{
	return this->numCategorie;
}

void CThumbnailData::SetNumParent(const int &numParent)
{
	this->numParent = numParent;
}

int CThumbnailData::GetNumParent()
{
	return this->numParent;
}

void CThumbnailData::SetKey(const wxString &key)
{
	this->key = key;
}

wxString CThumbnailData::GetKey()
{
	return this->key;
}

