#include "header.h"
#include "ThumbnailBuffer.h"
#include <ParamInit.h>
#include "RegardsConfigParam.h"
#include <ConvertUtility.h>
#include <FileUtility.h>
#include <iostream>
#include <fstream>


std::map<wxString, cv::Mat> CThumbnailBuffer::listPicture;
std::vector<wxString> CThumbnailBuffer::listFile;
PhotosVector *  CThumbnailBuffer::newPhotosVectorList;
std::mutex CThumbnailBuffer::muPictureBuffer;
std::mutex CThumbnailBuffer::muListFile;
std::mutex CThumbnailBuffer::muNewVector;
int  CThumbnailBuffer::vectorSize = 0;


static cv::Mat GetMatrixFromFile(const wxString& szFilePath)
{
    
    cv::Mat image;
    if(wxFileExists(szFilePath))
    {
        std::ifstream infile(CConvertUtility::ConvertToStdString(szFilePath)); // and since you want bytes rather than
                                        // characters, strongly consider opening the
                                        // File in binary mode with std::ios_base::binary
        // Get length of file
        infile.seekg(0, std::ios::end);
        size_t length = infile.tellg();
        infile.seekg(0, std::ios::beg);
        image = cv::Mat( 1, length, CV_8UC1);
        
        printf("GetMatrixFromFile File : %s Size : %d \n", CConvertUtility::ConvertToStdString(szFilePath).c_str(), length);
        // Read file
        infile.read((char *)image.data, length);
        
    }
    return image;
}


void CThumbnailBuffer::RemovePicture(const wxString& filename)
{
	{
		std::lock_guard<std::mutex> lock(muPictureBuffer);
		auto it = listPicture.find(filename);
		if (it != listPicture.end())
		{
            cv::Mat picture = it->second;
            picture.release();
			listPicture.erase(it);
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
    else
    {
        std::lock_guard<std::mutex> lock(muPictureBuffer);
        {
            cv::Mat rawData;
            if (static_cast<int>(listPicture.size()) > sizeBuffer)
            {
                auto it = listPicture.begin();
                if (it != listPicture.end())
                {
                    cv::Mat picture = it->second;
                    picture.release();
                    listPicture.erase(it);
                }
            }

            auto it = listPicture.find(filename);
            if (it == listPicture.end())
            {
                rawData = GetMatrixFromFile(filename);
                listPicture[filename] = rawData;
            }
            else
            {
                rawData = listPicture[filename];
            }
            
            image = cv::imdecode(rawData, cv::IMREAD_COLOR);
        }
    }

    return image;
}
