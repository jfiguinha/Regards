#include <header.h>
#include "ImageList.h"
using namespace Regards::Viewer;

int CImageList::GetNbElement()
{
    muList.lock();    
    int nbElement = photolist.size();
    muList.unlock();
    return nbElement;
}

 CImageList::~CImageList()
 {
     photolist.clear();
 }

 PhotosVector CImageList::GetCopy()
 {
	 muList.lock();
	 PhotosVector listCopy;
	 if (photolist.size() > 0)
	 {
		 listCopy.reserve(photolist.size());
		 copy(photolist.begin(), photolist.end(), back_inserter(listCopy));
	 }
	 muList.unlock();
	 return listCopy;
 }
 
 void CImageList::SetImageList(const PhotosVector & pictures)
 {
    muList.lock();
 	if (pictures.size() > 0)
	{
        photolist.clear();
		photolist.reserve(pictures.size());
		copy(pictures.begin(), pictures.end(), back_inserter(photolist));
	}    
	else
	{
		photolist.clear();
	}
    muList.unlock();
 }
 
CPhotos CImageList::GetElement(const int &numElement, bool &isValid)
{
    CPhotos photo;
    isValid = false;
    muList.lock();
 	if (numElement < photolist.size())
	{
        photo = photolist.at(numElement);
        isValid = true;
	}    
    muList.unlock();
    return photo;
}

wxString CImageList::GetFilePath(const int &numElement, bool &isValid)
{
    CPhotos photo;
    wxString path = "";
    isValid = false;
    muList.lock();
 	if (numElement < photolist.size())
	{
        photo = photolist.at(numElement);
        path = photo.GetPath();
        if (wxFileExists(path))
            isValid = true;
	}    
    muList.unlock();
    return path;
}

int CImageList::FindFileIndex(const wxString & filename)
{
    int numElement = 0;
    muList.lock();

    for (CPhotos file : photolist)
    {
        if (file.GetPath() == filename)
        {
            break;
        }
        else
            numElement++;
    }
    muList.unlock();
    return numElement;    
}

int CImageList::FindFileIndex(const int & id)
{
    int numElement = 0;
    muList.lock();

    for (CPhotos file : photolist)
    {
        if (file.GetId() == id)
        {
            break;
        }
        else
            numElement++;
    }
    muList.unlock();
    return numElement;     
}


