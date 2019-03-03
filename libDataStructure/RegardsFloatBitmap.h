// IBitmap.h: interface for the CRegardsBitmap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <xmmintrin.h>  

union SSEfloatUnion{
    __m128 v;    // SSE 4 x float vector
    float a[4];  // scalar array of 4 floats
};

class CRegardsFloatBitmap
{
public:

	CRegardsFloatBitmap(const int &iWidth, const int &iHeight);
	CRegardsFloatBitmap();
	virtual ~CRegardsFloatBitmap();

	bool IsValid();

	//Flip
	bool HorzFlipBuf();
	bool VertFlipBuf();
    float * GetColorValue(const int &x, const int &y);
	float * GetData();
	long GetSize();

	const int GetWidth();
	const int GetHeight();

    float * data = nullptr;

protected:
	
    inline int GetPosition(const int &x, const int &y) const;
	int m_iWidth;
	int m_iHeight;
	long m_lSize;
	

};


