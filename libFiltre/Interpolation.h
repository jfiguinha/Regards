#pragma once



class CBitmapYUV;
class CRegardsBitmap;

class CInterpolation
{
public:

	CInterpolation();
	~CInterpolation();
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out);
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const int &flipH, const int &flipV, const int &angle);
	void Execute(CRegardsBitmap * In, CRegardsBitmap * & Out, const wxRect &rectToShow);
	static wxImage * ExecuteNV12(uint8_t * dataY, uint8_t * dataUV, const int &widthIn, const int &heightIn, const int &widthOut, const int &heightOut, const int &flipH, const int &flipV, const int &angle);
	static wxImage ExecuteNV12(uint8_t * data, const int &widthIn, const int &heightIn, const int &widthOut, const int &heightOut, const int &flipH, const int &flipV, const int &angle, int pitch, int surfaceHeight);
	static wxImage ExecuteYUV(CBitmapYUV * bmpYUV, const int &widthOut, const int &heightOut, const int &flipH, const int &flipV, const int &angle);
	static wxImage Execute(CRegardsBitmap * In, const int &widthOut, const int &heightOut, const int &flipH, const int &flipV, const int &angle);
	static const wxImage Execute(uint8_t * data, const int &widthIn, const int &heightIn, const int &widthOut, const int &heightOut, const int &flipH, const int &flipV, const int &angle);

	static inline void CalculPosition(const int &x, const int &y, const int &widthIn, const int &heightIn, const int &widthOut, const int &heightOut, const int &flipH, const int &flipV, const int &angle, float &posX, float &posY);
};


