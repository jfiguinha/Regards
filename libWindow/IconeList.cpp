#include "header.h"
#include "IconeList.h"
#include <WindowMain.h>
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
    muList.lock();    
    int nbElement = pIconeList.size();
    muList.unlock();
    return nbElement;
}

 CIconeList::~CIconeList()
 {
     EraseThumbnailList();
 }

CIcone * CIconeList::GetElement(const int &numElement)
{
     muList.lock();   
    CIcone * icone = nullptr;
    if(numElement < pIconeList.size())
    {
        icone = pIconeList[numElement];
    }
     muList.unlock();   
    return icone;
}

void CIconeList::AddElement(CIcone * icone)
{
     muList.lock();   
    pIconeList.push_back(icone);
     muList.unlock();   
}

void CIconeList::Lock()
{
	muList.lock();
}
void CIconeList::Unlock()
{
	muList.unlock();
}

CIcone *  CIconeList::FindElement(const int &xPos, const int &yPos, pItemCompFonct * _pf, CWindowMain * parent)
{
	IconeVector::iterator it;
	CIcone * element = nullptr;
	muList.lock();
	it =find_if(pIconeList.begin(), pIconeList.end(), CItemPos(xPos, yPos, _pf, parent));
	muList.unlock();

	if (it != pIconeList.end())
		element = *it;

	return element;
}

 void CIconeList::EraseThumbnailList()
 {
     muList.lock();
	for (CIcone * pIcone : pIconeList)
	{
		if (pIcone != nullptr)
		{
			delete(pIcone);
			pIcone = nullptr;
		}
	}
	pIconeList.clear();  
	muList.unlock();    
 }
 
 void CIconeList::DestroyCacheThumbnailList()
 {
     muList.lock();
  	for (CIcone * icone : pIconeList)
    {
        icone->DestroyCache();
    } 
	muList.unlock();    
 }