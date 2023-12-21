#pragma once
#include "Photos.h"
class CThumbnailBuffer
{

public:

	static wxImage GetPicture(const wxString& filename);
	static void RemovePicture(const wxString& filename);
	static void InitVectorList(PhotosVector * newVector);
	static CPhotos GetVectorValue(int i);
	static int GetVectorSize();
	static bool FindValidFile(wxString localFilename);
	static wxString FindPhotoById(int id);
	static wxString FindPhotoByPath(wxString path);

private:

	static std::map<wxString, wxImage> listPicture;
	static std::vector<wxString> listFile;
	static std::mutex muPictureBuffer;
	static std::mutex muListFile;
	static std::mutex muNewVector;
	static int vectorSize;
	static PhotosVector * newPhotosVectorList;
};