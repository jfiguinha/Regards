#include <header.h>
// IBitmap.cpp: implementation of the CRegardsFloatBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "RegardsFloatBitmap.h"
#include <iostream>
#include <fstream>
#include <memory.h>


bool CRegardsFloatBitmap::IsValid()
{
	if(data == nullptr || GetHeight() == 0 || GetWidth() == 0 || GetSize() == 0)
		return false;

	return true;
}

CRegardsFloatBitmap::CRegardsFloatBitmap(const int &iWidth, const int &iHeight)
{
	data = nullptr;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_lSize = iWidth * iHeight * 4;
	//needRotate = false;
	data = new float[m_lSize];
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRegardsFloatBitmap::CRegardsFloatBitmap()
{
	//needRotate = false;
	data = nullptr;
	m_iWidth = 0;
	m_iHeight = 0;
	m_lSize = 0;
}

int CRegardsFloatBitmap::GetPosition(const int &x, const int &y) const
{
	return (x << 2) + (y * (m_iWidth << 2));
}

float * CRegardsFloatBitmap::GetColorValue(const int &x, const int &y)
{
	if (data != nullptr && x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight)
	{
		int i = GetPosition(x, y);
		return (data + i);
	}
	return nullptr;    
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
CRegardsFloatBitmap::~CRegardsFloatBitmap()
{
	if (data != nullptr)
		delete[] data;

	data = nullptr;

}



//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsFloatBitmap::HorzFlipBuf()
{
	if (data == nullptr)
		return false;

	int MiddleX = (m_iWidth >> 1);
	
#pragma omp parallel for
	for (auto y = 0; y < m_iHeight; y++)
	{
#pragma omp parallel for
		for (auto x = 0; x < MiddleX; x++)
		{
			float m_bDataBuffer;
			int iPos = y * m_iWidth;
			int iPos2 = iPos + m_iWidth;
			m_bDataBuffer = data[iPos+x];
			data[iPos+x] = data[iPos2-x];
			data[iPos2-x] = m_bDataBuffer;
		}
	}


	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsFloatBitmap::VertFlipBuf()
{
	float  *tb1;
	float  *tb2;

	if (data == nullptr)
		return false;

	int bufsize = m_iWidth * sizeof(float);

	tb1 = new float[bufsize];
	if (tb1 == nullptr) {
		return false;
	}

	tb2 = new float[bufsize];
	if (tb2 == nullptr) {
		delete[] tb1;
		return false;
	}

	int endValue = (m_iHeight + 1) / 2;

	for (auto row_cnt = 0; row_cnt< endValue; row_cnt++)
	{
		int off1 = row_cnt*bufsize;
		int off2 = ((m_iHeight - 1) - row_cnt)*bufsize;
		memcpy(tb1, data + off1, bufsize);
		memcpy(tb2, data + off2, bufsize);
		memcpy(data + off1, tb2, bufsize);
		memcpy(data + off2, tb1, bufsize);
	}


	delete[] tb1;
	delete[] tb2;

	return true;
}

long CRegardsFloatBitmap::GetSize()
{
	return m_iWidth * m_iHeight * 4 * sizeof(float);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

float * CRegardsFloatBitmap::GetData()
{
	return data;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const int CRegardsFloatBitmap::GetWidth()
{
	return m_iWidth;
}

const int CRegardsFloatBitmap::GetHeight()
{
	return m_iHeight;
}

