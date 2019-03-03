#include "header.h"
#include "IconeList.h"
using namespace Regards::Window;

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