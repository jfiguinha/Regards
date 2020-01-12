// IBitmap.h: interface for the CRegardsBitmap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CRegardsFloatBitmap
{
public:

	CRegardsFloatBitmap(const int &iWidth, const int &iHeight);
	CRegardsFloatBitmap();
	virtual ~CRegardsFloatBitmap();

	bool IsValid();
	int GetPosition(const int &x, const int &y);
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
	
   
	int m_iWidth;
	int m_iHeight;
	long m_lSize;
	

};


