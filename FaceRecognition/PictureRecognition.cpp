// FaceRecognition.cpp : Defines the exported functions for the DLL application.
//

#include "PictureRecognition.h"
#include "FaceDetector.h"
#include "FaceData.h"
#include "LoadPicture.h"
#include "PictureCategorieData.h"


// This is the constructor of a class that has been exported.
// see FaceRecognition.h for the class definition
CPictureCategorie::CPictureCategorie()
{
}



// This is the constructor of a class that has been exported.
// see FaceRecognition.h for the class definition
CPictureCategorie::~CPictureCategorie()
{
}


std::vector<int> CPictureCategorie::GetCategorieFromJpegBuffer(const unsigned char * data, const int &size, void * pictureData)
{
	if (pictureData != nullptr)
	{
		return ((CPictureCategorieData *)pictureData)->GetCategorie(data, size);

	}
	std::vector<int> categorie;
	return categorie;
}

std::vector<int> CPictureCategorie::GetCategorieFromPicture(const char * filename, void * pictureData)
{
	if (pictureData != nullptr)
	{
		return ((CPictureCategorieData *)pictureData)->GetCategorie(filename);

	}
	std::vector<int> categorie;
	return categorie;
}

void CPictureCategorie::GetLabel(const int &numLabel, char * labelOut, int size, void * pictureData)
{
	if (pictureData != nullptr)
	{
		return ((CPictureCategorieData *)pictureData)->GetLabel(numLabel, labelOut, size);

	}
}

void * CPictureCategorieLoadData::GetCopyData()
{
	CPictureCategorieData * copyData = nullptr;
	if (pictureData != nullptr)
	{
		CPictureCategorieData * faceTemp = (CPictureCategorieData *)pictureData;
		copyData = new CPictureCategorieData();
		*copyData = *faceTemp;
	}
	return copyData;
}

void CPictureCategorieLoadData::DeleteCopy(void * copyData)
{
	if (copyData != nullptr)
	{
		CPictureCategorieData * data = (CPictureCategorieData *)copyData;
		delete data;
		data = nullptr;
	}
}

CPictureCategorieLoadData::CPictureCategorieLoadData()
{
	pictureData = new CPictureCategorieData();
}

CPictureCategorieLoadData::~CPictureCategorieLoadData(void)
{
	if (pictureData != nullptr)
		delete (CPictureCategorieData *)pictureData;
	return;
}

bool CPictureCategorieLoadData::LoadData(const char * descriptor)
{
	if (pictureData != nullptr)
	{
		((CPictureCategorieData *)pictureData)->LoadPredictor(descriptor);
		return true;
	}
	return false;
}

void * CPictureCategorieLoadData::GetPictureCategorieData()
{
	return pictureData;
}

CFaceLoadData::CFaceLoadData()
{
	faceData = new CFaceData();
}

CFaceLoadData::~CFaceLoadData(void)
{
	if (faceData != nullptr)
		delete (CFaceData *)faceData;
	return;
}

bool CFaceLoadData::LoadData(const char * descriptor, const char * predictor)
{
	if (faceData != nullptr)
	{
		((CFaceData *)faceData)->LoadPredictor(predictor);
		((CFaceData *)faceData)->LoadDescriptor(descriptor);
		return true;
	}
	return false;
}

void * CFaceLoadData::GetFaceData()
{
	return faceData;
}

void * CFaceLoadData::GetCopyFaceData()
{
	CFaceData * face = nullptr;
	if (faceData != nullptr)
	{
		CFaceData * faceTemp = (CFaceData *)faceData;
		face = new CFaceData();
		*face = *faceTemp;
	}
	return face;
}

void CFaceLoadData::DeleteCopyFaceData(void * face)
{
	if (face != nullptr)
	{
		CFaceData * faceData = (CFaceData *)face;
		delete faceData;
		faceData = nullptr;
	}
}


// This is the constructor of a class that has been exported.
// see FaceRecognition.h for the class definition
CFace::CFace()
{
	//CFaceDetector * faceDetector = nullptr;
	faceDetector = new CFaceDetector();
}



// This is the constructor of a class that has been exported.
// see FaceRecognition.h for the class definition
CFace::~CFace()
{
	if (faceDetector != nullptr)
		delete (CFaceDetector *)faceDetector;
	return;
}

int CFace::LoadPicture(uint8_t * data, int width, int height, int size, int quality, void * faceLoadData)
{
	if (faceDetector != nullptr && faceLoadData != nullptr)
	{
		array2d<rgb_pixel> img = CLoadPicture::LoadPicture(data, width, height);
		return ((CFaceDetector *)faceDetector)->DetectFace(img, quality, (CFaceData *)faceLoadData);
	}
	return -1;
}

int CFace::LoadPictureFromJpegBuffer(const unsigned char * data, const int &size, int quality, void * faceLoadData)
{
	if (faceDetector != nullptr && faceLoadData != nullptr)
	{
		array2d<rgb_pixel> img = CLoadPicture::LoadPictureFromJpegBuffer(data, size);
		return ((CFaceDetector *)faceDetector)->DetectFace(img, quality, (CFaceData *)faceLoadData);
	}
	return -1;
}

int CFace::LoadPicture(const char * filename, int quality, void * faceLoadData)
{
	if (faceDetector != nullptr && faceLoadData != nullptr)
	{
		array2d<rgb_pixel> img = CLoadPicture::LoadPicture(filename);
		return ((CFaceDetector *)faceDetector)->DetectFace(img, quality, (CFaceData *)faceLoadData);
	}
	return -1;
}

int CFace::GetNbFaceFound()
{
	if (faceDetector != nullptr)
	{
		return ((CFaceDetector *)faceDetector)->GetNbFaceFound();
	}
	return -1;
}

void CFace::GetFaceData(int numFace, long &width, long &height, double &pertinence, uint8_t * data, long &size)
{
	if (faceDetector != nullptr)
	{
		((CFaceDetector *)faceDetector)->GetFaceData(numFace, width, height, pertinence, data, size);
	}
}

void CFace::SaveFaceData(const int &numFace, long &width, long &height, double &pertinence, const char * savePath)
{
	if (faceDetector != nullptr)
	{
		((CFaceDetector *)faceDetector)->SaveFaceData(numFace, width, height, pertinence, savePath);
	}
}

void CFace::GetFaceDescriptor(int numFace, char * data, long &size)
{
	if (faceDetector != nullptr)
	{
		string face = ((CFaceDetector *)faceDetector)->GetFaceDescriptor(numFace);
		if (size == 0)
			size = face.length();
		else
		{
			memcpy(data, face.c_str(), size);
		}
	}
}

float CFace::IsCompatibleFace(const char * face1, const char * face2)
{
	if (faceDetector != nullptr)
	{
		return ((CFaceDetector *)faceDetector)->IsCompatibleFace(face1, face2);
	}
	return 0;
}

