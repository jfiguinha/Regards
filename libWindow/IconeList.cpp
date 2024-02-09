#include "header.h"
#include "IconeList.h"
#include <WindowMain.h>
#include <ThumbnailData.h>
#include <tbb/parallel_sort.h>
using namespace Regards::Window;

//Class use for finding element in IconeList
class CItemPos
{
public:
	CItemPos(int x, int y, pItemCompFonct* pf, CWindowMain* parent) : xPos(x), yPos(y), _parent(parent), _pf(pf)
	{
	}

	bool operator()(std::shared_ptr<CIcone> icone)
	{
		return (*_pf)(xPos, yPos, icone, _parent);
	}

	int xPos;
	int yPos;
	CWindowMain* _parent;
	pItemCompFonct* _pf;

};

//Class use for finding element in IconeList
class CItemPhotoId
{
public:
	CItemPhotoId(int photoId) : _photoId(photoId)
	{
	}

	bool operator()(std::shared_ptr<CIcone> icone)
	{
		int photoId = 0;
		if (icone != nullptr)
		{
			CThumbnailData* data = icone->GetData();
			if (data != nullptr)
				photoId = data->GetNumPhotoId();
		}
		return _photoId == photoId;
	}

	int _photoId;
	

};

class CItemString
{
public:
	CItemString(wxString filename, pItemStringCompFonct * pf) : _filename(filename), _pf(pf)
	{
	}

	bool operator()(std::shared_ptr<CIcone> icone)
	{
		return (*_pf)(_filename, icone);
	}

	wxString _filename;
	pItemStringCompFonct * _pf;

};


int CIconeList::GetNbElement()
{
	int nbElement = pIconeList.size();
	return nbElement;
}

CIconeList::~CIconeList()
{
	pIconeList.clear();
}

int CIconeList::GetPhotoId(const int& numElement)
{
	std::shared_ptr<CIcone> icone = nullptr;
	int photoId = -1;
	if (numElement < pIconeList.size())
		icone = pIconeList[numElement];

	if (icone != nullptr)
	{
		CThumbnailData* data = icone->GetData();
		if (data != nullptr)
			photoId = data->GetNumPhotoId();
	}

	return photoId;
}

std::shared_ptr<CIcone> CIconeList::GetElement(const int& numElement)
{
	std::shared_ptr<CIcone> icone = nullptr;
	if (numElement < pIconeList.size())
		icone = pIconeList[numElement];

	return icone;
}

void CIconeList::AddElement(std::shared_ptr<CIcone> icone)
{
	pIconeList.push_back(icone);
}

wxString CIconeList::GetFilename(const int& numElement)
{
	std::shared_ptr<CIcone> icone = nullptr;
	wxString filename = "";
	if (numElement < pIconeList.size())
		icone = pIconeList[numElement];

	if (icone != nullptr)
	{
		CThumbnailData* data = icone->GetData();
		if (data != nullptr)
			filename = data->GetFilename();
	}

	return filename;
}

std::shared_ptr<CIcone> CIconeList::FindElement(wxString filename, pItemStringCompFonct * _pf)
{
	IconeVector::iterator it;
	std::shared_ptr<CIcone> element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemString(filename, _pf));

	if (it != pIconeList.end())
		element = *it;
	return element;
}

std::shared_ptr<CIcone> CIconeList::FindElementPhotoId(const int& photoId)
{
	IconeVector::iterator it;
	std::shared_ptr<CIcone> element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemPhotoId(photoId));

	if (it != pIconeList.end())
		element = *it;


	return element;
}


std::shared_ptr<CIcone> CIconeList::FindElement(const int& xPos, const int& yPos, pItemCompFonct* _pf, CWindowMain* parent)
{
	IconeVector::iterator it;
	std::shared_ptr<CIcone> element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemPos(xPos, yPos, _pf, parent));

	if (it != pIconeList.end())
		element = *it;

	
	return element;
}

void CIconeList::EraseThumbnailList()
{
	pIconeList.clear();
}


// Compares two intervals
// according to starting times.
bool compareInterval(std::shared_ptr<CIcone> i1, std::shared_ptr<CIcone> i2)
{
	return (i1->GetNumElement() < i2->GetNumElement());
}

void CIconeList::SortById()
{
	tbb::parallel_sort(pIconeList.begin(), pIconeList.end(), compareInterval);
}


// Compares two intervals
// according to starting times.
bool compareFilename(std::shared_ptr<CIcone> i1, std::shared_ptr<CIcone> i2)
{
	return (i1->GetFilename() < i2->GetFilename());
}

void CIconeList::SortByFilename()
{
	tbb::parallel_sort(pIconeList.begin(), pIconeList.end(), compareFilename);
}