// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FACERECOGNITION_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FACERECOGNITION_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef WIN32
#ifdef FACERECOGNITION_EXPORTS
#define FACERECOGNITION_API __declspec(dllexport)
#else
#define FACERECOGNITION_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
using namespace std;

class CFaceLoadData;

// This class is exported from the FaceRecognition.dll
class FACERECOGNITION_API CFace {
public:
	CFace();
	~CFace(void);

	int LoadPicture(uint8_t * data, int width, int height, int size, int quality, void * faceLoadData);
	int LoadPictureFromJpegBuffer(const unsigned char * data, const int &size, int quality, void * faceLoadData);
	int LoadPicture(const char * filename, int quality, void * faceLoadData);
	int GetNbFaceFound();
	void GetFaceData(int numFace, long &width, long &height, double &pertinence, uint8_t * data, long &size);
	void GetFaceDescriptor(int numFace, char * data, long &size);
	float IsCompatibleFace(const char * face1, const char * face2);
	void SaveFaceData(const int &numFace, long &width, long &height, double &pertinence, const char * savePath);
private:
	void * faceDetector;
};

class FACERECOGNITION_API CPictureCategorie {
public:
	CPictureCategorie();
	~CPictureCategorie(void);
	std::vector<int> GetCategorieFromPicture(const char * filename, void * pictureData);
	void GetLabel(const int &numLabel, char * labelOut, int size, void * pictureData);

private:
	void * pictureDetector;
};


// This class is exported from the FaceRecognition.dll
class FACERECOGNITION_API CPictureCategorieLoadData {
public:
	CPictureCategorieLoadData();
	~CPictureCategorieLoadData(void);
	bool LoadData(const char * descriptor);
	void * GetPictureCategorieData();
	void * GetCopyData();
	void DeleteCopy(void * copyData);

private:
	void * pictureData;
};

// This class is exported from the FaceRecognition.dll
class FACERECOGNITION_API CFaceLoadData {
public:
	CFaceLoadData();
	~CFaceLoadData(void);
	bool LoadData(const char * descriptor, const char * predictor);
	void * GetFaceData();
	void DeleteCopyFaceData(void * face);
	void * GetCopyFaceData();
private:
	void * faceData;
};

#else

#include <vector>
#include <string>
using namespace std;

class CFaceLoadData;

// This class is exported from the FaceRecognition.dll
class CFace {
public:
	CFace();
	~CFace(void);

	int LoadPicture(uint8_t * data, int width, int height, int size, int quality, void * faceLoadData);
	int LoadPictureFromJpegBuffer(const unsigned char * data, const int &size, int quality, void * faceLoadData);
	int LoadPicture(const char * filename, int quality, void * faceLoadData);
	int GetNbFaceFound();
	void GetFaceData(int numFace, long &width, long &height, double &pertinence, uint8_t * data, long &size);
	void GetFaceDescriptor(int numFace, char * data, long &size);
	float IsCompatibleFace(const char * face1, const char * face2);
	void SaveFaceData(const int &numFace, long &width, long &height, double &pertinence, const char * savePath);
private:
	void * faceDetector;
};


// This class is exported from the FaceRecognition.dll
class CFaceLoadData {
public:
	CFaceLoadData();
	~CFaceLoadData(void);
	bool LoadData(const char * descriptor, const char * predictor);
	void * GetFaceData();
	void DeleteCopyFaceData(void * face);
	void * GetCopyFaceData();
private:
	void * faceData;
};


class CPictureCategorie {
public:
	CPictureCategorie();
	~CPictureCategorie(void);
	std::vector<int> GetCategorieFromPicture(const char * filename, void * pictureData);
	std::vector<string> GetLabelCategorieFromPicture(const char * filename, void * pictureData);
    void GetLabel(const int &numLabel, char * labelOut, int size, void * pictureData);
private:
	void * pictureDetector;
};


// This class is exported from the FaceRecognition.dll
class CPictureCategorieLoadData {
public:
	CPictureCategorieLoadData();
	~CPictureCategorieLoadData(void);
	bool LoadData(const char * descriptor);
	void * GetPictureCategorieData();
    void * GetCopyData();
    void DeleteCopy(void * copyData);
private:
	void * pictureData;
};
#endif