#include "header.h"
#include "ThumbnailBuffer.h"
#include <ParamInit.h>
#include "RegardsConfigParam.h"
#include <ConvertUtility.h>




std::map<wxString, cv::Mat> CThumbnailBuffer::listPicture;
std::vector<wxString> CThumbnailBuffer::listFile;
PhotosVector *  CThumbnailBuffer::newPhotosVectorList;
std::mutex CThumbnailBuffer::muPictureBuffer;
std::mutex CThumbnailBuffer::muListFile;
std::mutex CThumbnailBuffer::muNewVector;
int  CThumbnailBuffer::vectorSize = 0;

void CThumbnailBuffer::RemovePicture(const wxString& filename)
{
	{
		std::lock_guard<std::mutex> lock(muPictureBuffer);
		auto it = listPicture.find(filename);
		if (it != listPicture.end())
		{
			listPicture.erase(it);
		}
	}

	{
		std::lock_guard<std::mutex> lock(muListFile);
		auto it = std::find(listFile.begin(), listFile.end(), filename);
		if (it != listFile.end())
		{
			listFile.erase(it);
		}
	}
}

PhotosVector* CThumbnailBuffer::GetVectorList()
{
    return newPhotosVectorList;
}

int CThumbnailBuffer::GetVectorSize()
{
    return vectorSize;
}

wxString CThumbnailBuffer::FindPhotoByPath(wxString path)
{
    std::lock_guard<std::mutex> lock(muNewVector);

    auto p = std::find_if(
        newPhotosVectorList->begin(), newPhotosVectorList->end(),
        [&path](const auto& val)
        {
            return static_cast<CPhotos>(val).GetPath() == path;
        }
    );

    if (p != newPhotosVectorList->end())
    {
        return p->GetPath();
    }
    
    return "";
}

wxString CThumbnailBuffer::FindPhotoById(int id)
{
    std::lock_guard<std::mutex> lock(muNewVector);

    auto p = std::find_if(
        newPhotosVectorList->begin(), newPhotosVectorList->end(),
        [id](const auto& val)
        {
            return static_cast<CPhotos>(val).GetId() == id;
        }
    );

    if (p != newPhotosVectorList->end())
    {
        return p->GetPath();
    }
    
    return "";
}

bool CThumbnailBuffer::FindValidFile(wxString localFilename)
{
    std::lock_guard<std::mutex> lock(muNewVector);

    auto p = std::find_if(
        newPhotosVectorList->begin(), newPhotosVectorList->end(),
        [&localFilename](const auto& val)
        {
            return static_cast<CPhotos>(val).GetPath() == localFilename;
        }
    );

    return p != newPhotosVectorList->end();
}

CPhotos CThumbnailBuffer::GetVectorValue(int i)
{
    std::lock_guard<std::mutex> lock(muNewVector);
    
    if (i >= 0 && i < static_cast<int>(newPhotosVectorList->size()))
    {
        return newPhotosVectorList->at(i);
    }
    
    throw std::out_of_range("GetVectorValue: index out of range");
}

void CThumbnailBuffer::InitVectorList(PhotosVector * newVector)
{
    PhotosVector* old = newPhotosVectorList;
    {
        std::lock_guard<std::mutex> lock(muNewVector);
        newPhotosVectorList = newVector;
        vectorSize = newPhotosVectorList->size();
    }
    if (old != nullptr)
    {
        old->clear();
        delete old;
    }
}

cv::Mat CThumbnailBuffer::GetPicture(const wxString& filename)
{
    cv::Mat image;
    int sizeBuffer = 100;
    
    CRegardsConfigParam* param = CParamInit::getInstance();
    if (param != nullptr)
    {
        sizeBuffer = param->GetBufferSize();
    }
    
    if (sizeBuffer <= 0)
    {
        return cv::imread(CConvertUtility::ConvertToStdString(filename), cv::IMREAD_COLOR);
    }

    {
        std::lock_guard<std::mutex> lock(muPictureBuffer);
        auto it = listPicture.find(filename);

        if (it == listPicture.end())
        {
            image = cv::imread(CConvertUtility::ConvertToStdString(filename), cv::IMREAD_COLOR);
            listPicture[filename] = image;
            
            {
                std::lock_guard<std::mutex> fileLock(muListFile);
                listFile.push_back(filename);
            }
        }
        else
        {
            image = it->second;
            
            {
                std::lock_guard<std::mutex> fileLock(muListFile);
                auto fileIt = std::find(listFile.begin(), listFile.end(), filename);
                if (fileIt != listFile.end())
                {
                    listFile.erase(fileIt);
                }
                listFile.push_back(filename);
            }
        }
    }

    {
        std::lock_guard<std::mutex> fileLock(muListFile);
        if (static_cast<int>(listFile.size()) > sizeBuffer)
        {
            wxString firstFile = listFile.front();
            listFile.erase(listFile.begin());
            
            {
                std::lock_guard<std::mutex> picLock(muPictureBuffer);
                auto it = listPicture.find(firstFile);
                if (it != listPicture.end())
                {
                    listPicture.erase(it);
                }
            }
        }
    }

    return image;
}
