#pragma once
#include "Photos.h"


class CThumbnailBuffer
{

public:
    static cv::Mat GetPicture(const wxString& filename);
	//static wxImage GetPicture(const wxString& filename);
	static void RemovePicture(const wxString& filename);
	static void InitVectorList(PhotosVector * newVector);
	static CPhotos GetVectorValue(int i);
	static int GetVectorSize();
	static bool FindValidFile(wxString localFilename);
	static wxString FindPhotoById(int id);
	static wxString FindPhotoByPath(wxString path);
	static PhotosVector * GetVectorList();

private:

	static std::map<wxString, cv::Mat> listPicture;
	static std::vector<wxString> listFile;
	static std::mutex muPictureBuffer;
	static std::mutex muListFile;
	static std::mutex muNewVector;
	static int vectorSize;
	static PhotosVector * newPhotosVectorList;
};