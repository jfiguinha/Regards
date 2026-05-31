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


class CItemFaceString
{
public:
	CItemFaceString(wxString filename, int numFace, pItemCompFonctFace* pf) : _filename(filename), _numFace(numFace), _pf(pf)
	{
	}

	bool operator()(CIcone* icone)
	{
		return (*_pf)(_filename, _numFace, icone);
	}

	wxString _filename;
	int _numFace;
	pItemCompFonctFace* _pf;

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

void CIconeList::RemoveElement(int numElement)
{
    if (numElement >= pIconeList.size())
        return;

    CIcone* icone = pIconeList[numElement];

    if (icone != nullptr)
    {
		if (CThumbnailData* data = icone->GetData();
			data != nullptr)
		{
			pIconeByPhotoId[data->GetNumPhotoId()] = nullptr;
			pIconeByFilename[data->GetFilename()] = nullptr;
		}

        delete icone;
    }

    pIconeList[numElement] = nullptr;
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
    if (icone == nullptr)
        return;

    pIconeList.push_back(icone);

    if (CThumbnailData* data = icone->GetData();
        data != nullptr)
    {
        pIconeByPhotoId[data->GetNumPhotoId()] = icone;
        pIconeByFilename[data->GetFilename()] = icone;
    }
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

 bool CIconeList::IfElementExistByFilename(const wxString& filename)
 {
     auto it = pIconeByFilename.find(filename);

    if (it != pIconeByFilename.end())
        return true;

    return false;    
 }

CIcone* CIconeList::FindElementByFilename(const wxString& filename)
{
    auto it = pIconeByFilename.find(filename);

    if (it != pIconeByFilename.end() && it->second != nullptr)
        return it->second;

    return nullptr;
}


CIcone* CIconeList::FindElementByPhotoId(const int& photoId)
{
    auto it = pIconeByPhotoId.find(photoId);

    if (it != pIconeByPhotoId.end() && it->second != nullptr)
        return it->second;

    return nullptr;
}

CIcone* CIconeList::FindFaceElement(wxString filepath, int numFace, pItemCompFonctFace* _pf)
{
	IconeVector::iterator it;
	CIcone* element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemFaceString(filepath, numFace, _pf));

	if (it != pIconeList.end())
		element = *it;


	return element;
}

CIcone* CIconeList::FindElementByPosition(const int& xPos, const int& yPos, pItemCompFonct* _pf, CWindowMain* parent)
{
	IconeVector::iterator it;
	CIcone* element = nullptr;
	it = find_if(pIconeList.begin(), pIconeList.end(), CItemPos(xPos, yPos, _pf, parent));

	if (it != pIconeList.end())
		element = *it;

	
	return element;
}


CIcone* CIconeList::GetLastElement()
{
	CIcone* element = nullptr;
	int taille = pIconeList.size();
	if (taille > 0)
	{
		element = pIconeList[taille - 1];
	}
	return element;
}

void CIconeList::EraseThumbnailListWithIcon()
{
    printf("CIconeList::EraseThumbnailListWithIcon() \n");
 	for (CIcone* pIcone : pIconeList)
	{
		if (pIcone != nullptr)
		{
			delete(pIcone);
			pIcone = nullptr;
		}
	}
    pIconeList.clear();
    pIconeByPhotoId.clear();
    pIconeByFilename.clear();
}

void CIconeList::EraseThumbnailList()
{
    pIconeList.clear();
    pIconeByPhotoId.clear();
    pIconeByFilename.clear();
}

// Compares two intervals
// according to starting times.
bool compareInterval(CIcone* i1, CIcone* i2)
{
	if (i1 != nullptr && i2 != nullptr)
		return (i1->GetNumElement() < i2->GetNumElement());
	return false;
}

void CIconeList::SortById()
{
	tbb::parallel_sort(pIconeList.begin(), pIconeList.end(), compareInterval);
}


// Compares two intervals
// according to starting times.
bool compareFilename(CIcone* i1, CIcone* i2)
{
	if (i1 != nullptr && i2 != nullptr)
		return (i1->GetFilename() < i2->GetFilename());
	return false;
}

void CIconeList::SortByFilename()
{
	tbb::parallel_sort(pIconeList.begin(), pIconeList.end(), compareFilename);
}