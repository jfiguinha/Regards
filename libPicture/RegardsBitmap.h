// IBitmap.h: interface for the CRegardsBitmap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <RGBAQuad.h>
#define DEPTHDEFO 32
#define SIZE_PATH 1024


class CRegardsBitmap
{
public:
	CRegardsBitmap(const int& iWidth, const int& iHeight, const int& iDepth = 32);
	CRegardsBitmap();
	virtual ~CRegardsBitmap();

	CRegardsBitmap& operator=(const CRegardsBitmap& other);
	void GetYUV420P(uint8_t* & lum, uint8_t* & cb, uint8_t* & cr);
	void SetYUV420P(uint8_t* lum, uint8_t* cb, uint8_t* cr);

	void GetY(uint8_t* & lum);
	void SetY(uint8_t* lum);
	void ApplyRotation(const int& rotation);
	void ReadFile(const wxString& filename);
	void WriteFile(const wxString& filename);

#ifdef WIN32

	void SaveToBmp(const wxString& filename);

#endif

	bool IsValid();

	//Gestion Exif
	bool RotateExif(const int& orientation = 0);
	//bool RotateRawExif(const int& orientation = 0);
	//bool RotateAppleExif(const int & orientation = 0);

	//Rotation
	bool Rotate90();
	bool Rotate180();
	bool Rotate270();

	//Flip
	bool HorzFlipBuf();
	bool VertFlipBuf();

	//wxBitmap GetwxBitmap();

	wxString GetFilename();
	void SetFilename(const wxString& szFilename);

	CRgbaquad GetColorValue(const int& x, const int& y);
	CRgbaquad* GetPtColorValue(const int& x, const int& y);
	void SetColorValue(const int& x, const int& y, const CRgbaquad& color);
	void SetAlphaValue(const int& value);
	void SetBitmap(const int& iWidth, const int& iHeight, const int& iDepth = 32);

	uint8_t* GetPtBitmap();
	long GetBitmapSize();

	const long GetWidthSize();
	const int GetBitmapWidth();
	const int GetBitmapHeight();

	cv::Mat& GetMatrix();
	void SetMatrix(const cv::Mat& matrix, const bool& m_bFlip = false);
	void SetMatrix(const cv::UMat& matrix, const bool& m_bFlip = false);
	int GetBitmapDepth();

	void ConvertToBgr();
	int GetPosition(const int& x, const int& y);

	//Crop
	CRegardsBitmap* CropBitmap(const int& xPos, const int& yPos, const int& width, const int& height);

	//Background Color
	void SetBackgroundColor(const CRgbaquad& m_cValue);

	//Insertion d'une image Bitmap
	int InsertBitmap(CRegardsBitmap* CRegardsBitmap, int xPos, int yPos, const bool& withalpha = true);
	int InsertwxImage(const wxImage& bitmap, int xPos, int yPos);
	int InsertBitmapWithoutAlpha(CRegardsBitmap* picture, int xPos, int yPos);

	//Fusion de deux images Bitmap
	int FusionBitmap(CRegardsBitmap* nextPicture, const float& pourcentage);

	int SetColorTranspBitmap(const CRgbaquad& Transp);
	int SetValueToTranspColor(const CRgbaquad& backgroundValue);

	/*
	const bool GetNeedRotate();
	void SetNeedRotate(const bool & needRotate);
	*/

	int GetOrientation();
	void SetOrientation(const int& orientation);

protected:
	//HBITMAP GetFlipBitmap(const BOOL &bLateral);

	wxString filename;
	cv::Mat bitmapMatrix;
	int orientation;

};
