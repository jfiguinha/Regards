#include "header.h"
#include "ThumbnailBuffer.h"
#include <libPicture.h>
#include <ParamInit.h>
#include "RegardsConfigParam.h"
extern "C" {
#include <leptonica\environ.h>
#include <leptonica\pix.h>
#include <leptonica\pix_internal.h>
}




std::map<wxString, wxImage> CThumbnailBuffer::listPicture;
std::vector<wxString> CThumbnailBuffer::listFile;
PhotosVector *  CThumbnailBuffer::newPhotosVectorList;
std::mutex CThumbnailBuffer::muPictureBuffer;
std::mutex CThumbnailBuffer::muListFile;
std::mutex CThumbnailBuffer::muNewVector;
int  CThumbnailBuffer::vectorSize = 0;

void CThumbnailBuffer::RemovePicture(const wxString& filename)
{
	muPictureBuffer.lock();

	std::map<wxString, wxImage>::iterator it = listPicture.find(filename);

	if (it != listPicture.end())
	{
		listPicture.erase(it);
		muListFile.lock();
		std::vector<wxString>::iterator local = std::find(listFile.begin(), listFile.end(), filename);
		listFile.erase(local);
		muListFile.unlock();
	}
	muPictureBuffer.unlock();
	
}

int CThumbnailBuffer::GetVectorSize()
{
    return vectorSize;
}

wxString CThumbnailBuffer::FindPhotoById(int id)
{
    wxString file = "";
    bool isFound = false;
    muNewVector.lock();

    auto p = std::find_if(
        newPhotosVectorList->begin(), newPhotosVectorList->end(),
        [&](const auto& val)
    {
        auto photo = static_cast<CPhotos>(val);
		return photo.GetId() == id;
    }
    );

    if (p != newPhotosVectorList->end())
    {
        file = p->GetPath();
        isFound = true;
    }
    muNewVector.unlock();

    return file;
}

bool CThumbnailBuffer::FindValidFile(wxString localFilename)
{
    bool isFound = false;
    muNewVector.lock();

    auto p = std::find_if(
        newPhotosVectorList->begin(), newPhotosVectorList->end(),
        [&](const auto& val)
    {
        auto photo = static_cast<CPhotos>(val);
		return photo.GetPath() == localFilename;
    }
    );

    if (p != newPhotosVectorList->end())
        isFound = true;

    muNewVector.unlock();

    return isFound;
}

CPhotos CThumbnailBuffer::GetVectorValue(int i)
{
    bool find = false;
    CPhotos photo;
    muNewVector.lock();
    if (i < newPhotosVectorList->size())
    {
        find = true;
        photo = newPhotosVectorList->at(i);
    }
	muNewVector.unlock();

    if(!find)
        throw("not exist");
    return photo;
}

void CThumbnailBuffer::InitVectorList(PhotosVector * newVector)
{
    PhotosVector* old = newPhotosVectorList;
    muNewVector.lock();
    newPhotosVectorList = newVector;
    vectorSize = newPhotosVectorList->size();
    muNewVector.unlock();
    if (old != nullptr)
    {
        old->clear();
        delete old;
    }
}

wxImage CThumbnailBuffer::GetPicture(const wxString& filename)
{
	wxImage image;
	float diff_min = 0;
	wxString keyToDelete = "";
	int sizeBuffer = 100;
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
	{
		sizeBuffer = param->GetBufferSize();
	}
    
    if(sizeBuffer > 0)
    {

        muPictureBuffer.lock();

        std::map<wxString, wxImage>::iterator it = listPicture.find(filename);

        if (it == listPicture.end())
        {
            image = listPicture[filename] = Regards::Picture::CLibPicture::ReadThumbnail(filename);

            muListFile.lock();
            listFile.push_back(filename);
            muListFile.unlock();
        }
        else
        {

            image = it->second;
            muListFile.lock();
            std::vector<wxString>::iterator it = std::find(listFile.begin(), listFile.end(), filename);
            listFile.erase(it);
            listFile.push_back(filename);
            muListFile.unlock();
        }
        muPictureBuffer.unlock();

        if (listFile.size() > sizeBuffer)
        {
            muListFile.lock();
            wxString firstFile = listFile[0];
            listFile.erase(listFile.begin());
            muListFile.unlock();

            muPictureBuffer.lock();
            std::map<wxString, wxImage>::iterator it = listPicture.find(firstFile);
            listPicture.erase(it);
            muPictureBuffer.unlock();
        }

    }
    else
    {
         image = Regards::Picture::CLibPicture::ReadThumbnail(filename);
    }
	
	return image;
}