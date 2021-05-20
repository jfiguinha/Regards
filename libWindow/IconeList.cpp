#include "header.h"
#include "IconeList.h"
#include <WindowMain.h>
#include <ThumbnailData.h>
using namespace Regards::Window;

//Class use for finding element in IconeList
class CItemPos
{
public:

	CItemPos(int x, int y, pItemCompFonct * pf, CWindowMain * parent) : xPos(x), yPos(y), _pf(pf), _parent(parent) {}

	bool operator()(CIcone * icone)
	{
		return (*_pf)(xPos, yPos, icone, _parent);
	}

	int xPos;
	int yPos;
	CWindowMain * _parent;
	pItemCompFonct * _pf;
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

CIcone * CIconeList::GetElement(const int &numElement)
{
    CIcone * icone = nullptr;
    if(numElement < pIconeList.size())
        icone = pIconeList[numElement];

    return icone;
}

void CIconeList::AddElement(CIcone * icone)
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
		CThumbnailData * data =  icone->GetData();
		if (data != nullptr)
			filename = data->GetFilename();
	}

	return filename;
}

CIcone *  CIconeList::FindElement(const int &xPos, const int &yPos, pItemCompFonct * _pf, CWindowMain * parent)
{
	IconeVector::iterator it;
	CIcone * element = nullptr;
	it =find_if(pIconeList.begin(), pIconeList.end(), CItemPos(xPos, yPos, _pf, parent));

	if (it != pIconeList.end())
		element = *it;

	return element;
}

 void CIconeList::EraseThumbnailList()
 {
	for (CIcone * pIcone : pIconeList)
	{
		if (pIcone != nullptr)
		{
			delete(pIcone);
			pIcone = nullptr;
		}
	}
	pIconeList.clear();  
 
 }
 
 void CIconeList::DestroyCacheThumbnailList()
 {
  	for (CIcone * icone : pIconeList)
    {
        icone->DestroyCache();
    }  
 }