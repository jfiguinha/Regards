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

	bool operator()(CIcone* icone)
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

	bool operator()(CIcone* icone)
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

	bool operator()(CIcone* icone)
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
	EraseThumbnailList();
}

int CIconeList::GetPhotoId(const int& numElement)
{
	CIcone* icone = nullptr;
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

CIcone* CIconeList::GetElement(const int& numElement)
{
	CIcone* icone = nullptr;
	if (numElement < pIconeList.size())
		icone = pIconeList[numElement];

	return icone;
}

void CIconeList::AddElement(CIcone* icone)
{
	pIconeList.push_back(icone);
}

wxString CIconeList::GetFilename(const int& numElement)
{
	CIcone* icone = nullptr;
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

CIcone* CIconeList::FindElement(wxString filename, pItemStringCompFonct * _pf)
{
	IconeVector::iterator it;
	CIcone* element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemString(filename, _pf));

	if (it != pIconeList.end())
		element = *it;
	return element;
}

CIcone* CIconeList::FindElementPhotoId(const int& photoId)
{
	IconeVector::iterator it;
	CIcone* element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemPhotoId(photoId));

	if (it != pIconeList.end())
		element = *it;


	return element;
}


CIcone* CIconeList::FindElement(const int& xPos, const int& yPos, pItemCompFonct* _pf, CWindowMain* parent)
{
	IconeVector::iterator it;
	CIcone* element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemPos(xPos, yPos, _pf, parent));

	if (it != pIconeList.end())
		element = *it;

	
	return element;
}

void CIconeList::EraseThumbnailListWithIcon()
{
 	for (CIcone* pIcone : pIconeList)
	{
		if (pIcone != nullptr)
		{
			delete(pIcone);
			pIcone = nullptr;
		}
	}
	pIconeList.clear();   
}

void CIconeList::EraseThumbnailList()
{
	/*/
	for (CIcone* pIcone : pIconeList)
	{
		if (pIcone != nullptr)
		{
			delete(pIcone);
			pIcone = nullptr;
		}
	}*/
	pIconeList.clear();
}


// Compares two intervals
// according to starting times.
bool compareInterval(CIcone* i1, CIcone* i2)
{
	return (i1->GetNumElement() < i2->GetNumElement());
}

void CIconeList::SortById()
{
	tbb::parallel_sort(pIconeList.begin(), pIconeList.end(), compareInterval);
}


// Compares two intervals
// according to starting times.
bool compareFilename(CIcone* i1, CIcone* i2)
{
	return (i1->GetFilename() < i2->GetFilename());
}

void CIconeList::SortByFilename()
{
	tbb::parallel_sort(pIconeList.begin(), pIconeList.end(), compareFilename);
}