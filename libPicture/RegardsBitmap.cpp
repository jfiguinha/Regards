#include <header.h>
// IBitmap.cpp: implementation of the CRegardsBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "RegardsBitmap.h"
#include <iostream>
#include <fstream>

CRegardsBitmap& CRegardsBitmap::operator=(const CRegardsBitmap& other)
{
	filename = other.filename;
	if (other.data != nullptr)
		this->SetBitmap(other.data, other.m_iWidth, other.m_iHeight);
	return *this;
}

int CRegardsBitmap::GetOrientation()
{
	return orientation;
}

void CRegardsBitmap::SetOrientation(const int &orientation)
{
	this->orientation = orientation;
}

bool CRegardsBitmap::IsValid()
{
	if(data == nullptr || GetBitmapHeight() == 0 || GetBitmapWidth() == 0 || GetBitmapSize() == 0)
		return false;

	return true;
}

#ifdef WIN32

#include <Windows.h>

HBITMAP GenerateHBitmap(const uint8_t * pbyData, const int &nWidth, const int &nHeight)
{
	HBITMAP hBitmapRender;
	BITMAPINFO bmiInfos;
	memset(&bmiInfos, 0, sizeof(BITMAPINFO));
	bmiInfos.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiInfos.bmiHeader.biWidth = nWidth;
	bmiInfos.bmiHeader.biHeight = nHeight;
	bmiInfos.bmiHeader.biPlanes = 1;
	bmiInfos.bmiHeader.biCompression = BI_RGB;
	bmiInfos.bmiHeader.biBitCount = 32;
	bmiInfos.bmiHeader.biSizeImage = nWidth * nHeight << 2; //((iTaille * bitmap.bmWidth + iTaille) & ~iTaille) * bitmap.bmHeight;
	hBitmapRender = CreateDIBitmap(GetDC(nullptr),
		&bmiInfos.bmiHeader,
		CBM_INIT,
		pbyData,
		(LPBITMAPINFO)&bmiInfos,
		DIB_RGB_COLORS);

	return hBitmapRender;
}

void CRegardsBitmap::SaveToBmp(const wxString &filename)
{
	HBITMAP hBitmap = GenerateHBitmap(data, m_iWidth, m_iHeight);
	HDC hDC = GetDC(nullptr);
	WORD wBitCount;
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	BITMAP Bitmap0;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal, hOldPal2 = nullptr;
	wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap0.bmWidth;
	bi.biHeight = -Bitmap0.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 256;
	dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
		* Bitmap0.bmHeight;
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(nullptr);
		hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if (hOldPal2)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
		RealizePalette(hDC);
		ReleaseDC(nullptr, hDC);
	}

	fh = CreateFile(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);

	if (fh == INVALID_HANDLE_VALUE)
		return;

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	::WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, nullptr);

	::WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, nullptr);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

}

#endif

void CRegardsBitmap::ReadFile(const wxString &filename)
{
	if (data != nullptr)
		delete[] data;

	FILE *file = nullptr;
	file = fopen(filename.c_str(), "rb");      // File pointer
	if (file != nullptr)
	{
		fread(&m_iWidth, sizeof(int), sizeof(m_iWidth), file);
		fread(&m_iHeight, sizeof(int), sizeof(m_iHeight), file);
		fread(&orientation, sizeof(int), sizeof(orientation), file);
		long size = m_iWidth *m_iHeight * 4;
		data = new uint8_t[size];
		fread(data, sizeof(uint8_t), size, file);
		fclose(file);
	}
	m_sDepth = 32;
}

void CRegardsBitmap::WriteFile(const wxString &filename)
{
	long size = m_iWidth *m_iHeight * 4;
	FILE *file = nullptr;
	file = fopen(filename.c_str(), "wb");      // File pointer
	if (file != nullptr){
		fwrite((const void *)&m_iWidth, sizeof(int), sizeof(m_iWidth), file);
		fwrite((const void *)&m_iHeight, sizeof(int), sizeof(m_iHeight), file);
		fwrite((const void *)&orientation, sizeof(int), sizeof(orientation), file);
		fwrite(data, sizeof(uint8_t), size, file);
		fclose(file);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void CRegardsBitmap::SetBitmap(CRgbaquad * m_bBuffer, const unsigned int &bmWidth, const unsigned int &bmHeight, const bool &m_bFlip)
{
	m_sDepth = 32;

	if (data != nullptr)
		delete[] data;

	m_lSize = bmWidth * bmHeight * 4;

	data = new uint8_t[m_lSize];

	if (m_bBuffer != nullptr)
		memcpy(data, m_bBuffer, m_lSize);

	if (m_bFlip)
		VertFlipBuf();

	m_iWidth = bmWidth;
	m_iHeight = bmHeight;



}

void CRegardsBitmap::SetBitmap(const int &iWidth, const int &iHeight, const int &iDepth)
{
	if (m_iWidth != iWidth || m_iHeight != iHeight)
	{
		m_sDepth = 32;

		if (data != nullptr)
			delete[] data;

		m_lSize = iWidth * iHeight * 4;

		data = new uint8_t[m_lSize];

		m_iWidth = iWidth;
		m_iHeight = iHeight;
	}
}


CRegardsBitmap::CRegardsBitmap(const int &iWidth, const int &iHeight, const int &iDepth)
{
	data = nullptr;
	m_sDepth = iDepth;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_lSize = iWidth*iHeight * 4;
	//needRotate = false;
	data = new uint8_t[m_lSize];
	orientation = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRegardsBitmap::CRegardsBitmap()
{
	//needRotate = false;
	data = nullptr;
	m_sDepth = 32;
	m_iWidth = 0;
	m_iHeight = 0;
	m_lSize = 0;
	orientation = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
CRegardsBitmap::~CRegardsBitmap()
{
	if (keepMem)
	{
		if (data != nullptr)
			delete[] data;

		data = nullptr;
	}
}

void CRegardsBitmap::DeleteMemory(const bool &keepMem)
{
	this->keepMem = keepMem;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsBitmap::HorzFlipBuf()
{
	if (data == nullptr)
		return false;

	int MiddleX = (m_iWidth >> 1) << 2;
	
	int iWidth4 = m_iWidth << 2;

	for (auto y = 0; y < m_iHeight; y++)
	{
		for (auto x = 0; x < MiddleX; x += 4)
		{
			uint8_t m_bDataBuffer[4];
			int iPos = y * iWidth4;
			int iPos2 = iPos + iWidth4 - 4;
			memcpy(m_bDataBuffer, data + iPos + x, 4);
			memcpy(data + iPos + x, data + iPos2 - x, 4);
			memcpy(data + iPos2 - x, m_bDataBuffer, 4);
		}
	}


	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsBitmap::Rotation90()
{
	if (data == nullptr)
		return false;

	CRegardsBitmap * out = new CRegardsBitmap(m_iHeight, m_iWidth);

#pragma omp parallel for
	for (auto y = 0; y < m_iHeight; y++)
	{
#pragma omp parallel for
		for (auto x = 0; x < m_iWidth; x++)
		{
			memcpy(out->GetPtBitmap() + out->GetPosition(y, x), data + GetPosition(x, y), 4);
		}
	}

	SetBitmap(out->GetPtBitmap(), out->GetBitmapWidth(), out->GetBitmapHeight(), false);

	delete out;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsBitmap::VertFlipBuf()
{
	uint8_t  *tb1;
	uint8_t  *tb2;

	if (data == nullptr)
		return false;

	int bufsize;

	bufsize = (int)GetWidthSize();

	tb1 = (uint8_t *)new uint8_t[bufsize];
	if (tb1 == nullptr) {
		return false;
	}

	tb2 = (uint8_t *)new uint8_t[bufsize];
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



//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
void CRegardsBitmap::SetBitmap(uint8_t * m_bBuffer, const unsigned int &bmWidth, const unsigned int &bmHeight, const bool &m_bFlip, const bool &copy)
{
	if (data != nullptr)
		delete[] data;

	data = nullptr;

	if(m_bBuffer != nullptr)
	{
		m_lSize = bmWidth * bmHeight * 4;
		m_iWidth = bmWidth;
		m_iHeight = bmHeight;

		if (m_bBuffer != nullptr && copy)
		{
			data = new uint8_t[m_lSize];
			memcpy(data, m_bBuffer, m_lSize);
		}
		else
		{
			data = m_bBuffer;
		}

		if(m_bFlip)
			VertFlipBuf();
	}
}

bool CRegardsBitmap::RotateRawExif(const int & orientation)
{
	bool ret = true;
	switch (orientation)
	{
	case 1:// top left side
		break;
	case 2:// top right side
		this->HorzFlipBuf();
		break;
	case 3:// bottom right side
		this->HorzFlipBuf();
		this->VertFlipBuf();
		break;
	case 4:// bottom left side
		this->VertFlipBuf();
		break;
	case 5://left side top
        this->Rotation90();
        //this->VertFlipBuf();
		break;
	case 6:// right side top
        this->Rotation90();
		//this->VertFlipBuf();
        this->HorzFlipBuf();
		break;
	case 7:// right side bottom
        this->Rotation90();
		this->HorzFlipBuf();
		this->VertFlipBuf();
		break;
	case 8:// left side bottom
		this->Rotation90();
		this->VertFlipBuf();
		break;
	}
	return ret;
}

bool CRegardsBitmap::RotateExif(const int & orientation)
{
    bool ret = true;
    switch (orientation)
    {
        case 1:// top left side
            break;
        case 2:// top right side
            this->HorzFlipBuf();
            break;
        case 3:// bottom right side
            this->HorzFlipBuf();
            this->VertFlipBuf();
            break;
        case 4:// bottom left side
            this->VertFlipBuf();
            break;
        case 5://left side top
            this->Rotation90();
            this->VertFlipBuf();
            this->HorzFlipBuf();
            break;
        case 6:// right side top
            this->Rotation90();
            this->VertFlipBuf();
            break;
        case 7:// right side bottom
            this->Rotation90();
            break;
        case 8:// left side bottom
            this->Rotation90();
            this->HorzFlipBuf();
            break;
    }
    return ret;
}

/*
 bool CRegardsBitmap::RotateAppleExif(const int & orientation)
{
    bool ret = true;
    switch (orientation)
    {
        case 1:// top left side
            break;
        case 2:// top right side
            this->HorzFlipBuf();
            break;
        case 3:// bottom right side
            this->HorzFlipBuf();
            this->VertFlipBuf();
            break;
        case 4:// bottom left side
            this->VertFlipBuf();
            break;
        case 5://left side top
            this->Rotation90();
            this->VertFlipBuf();
            this->HorzFlipBuf();
            break;
        case 6:// right side top
            this->Rotation90();
            this->VertFlipBuf();
            break;
        case 7:// right side bottom
            this->Rotation90();
            break;
        case 8:// left side bottom
            this->Rotation90();
            this->HorzFlipBuf();
            break;
    }
    return ret;
}

bool CRegardsBitmap::RotateExif(const int & orientation)
{
	bool ret = true;
	switch (orientation)
	{
	case 1:// top left side
		break;
	case 2:// top right side
		this->HorzFlipBuf();
		break;
	case 3:// bottom right side
		this->HorzFlipBuf();
		this->VertFlipBuf();
		break;
	case 4:// bottom left side
		this->VertFlipBuf();
		break;
	case 5://left side top
        this->Rotation90();
        this->VertFlipBuf();
		break;
	case 6:// right side top
        this->Rotation90();
		this->VertFlipBuf();
        this->HorzFlipBuf();
		break;
	case 7:// right side bottom
        this->Rotation90();
		this->HorzFlipBuf();
		break;
	case 8:// left side bottom
		this->Rotation90();
		break;
	}
	return ret;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

uint8_t * CRegardsBitmap::GetPtBitmap()
{
	return data;
}

/*
const bool CRegardsBitmap::GetNeedRotate()
{
	return needRotate;
}

void CRegardsBitmap::SetNeedRotate(const bool &needRotate)
{
	this->needRotate = needRotate;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const int CRegardsBitmap::GetBitmapWidth()
{
	return m_iWidth;
}

const int CRegardsBitmap::GetBitmapHeight()
{
	return m_iHeight;
}

const short CRegardsBitmap::GetBitmapDepth()
{
	return m_sDepth;
}

int CRegardsBitmap::GetPosition(const int &x, const int &y)
{
	return (x << 2) + (y * (m_iWidth << 2));
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void CRegardsBitmap::SetFilename(const wxString & szFilename)
{
	filename = szFilename;
}

wxString CRegardsBitmap::GetFilename()
{
	return filename;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CRgbaquad CRegardsBitmap::GetColorValue(const int &x, const int &y)
{
	//CRgbaquad color = COLOR_RGBA_BLACK;
	if (data != nullptr && x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight)
	{
		int i = GetPosition(x, y);
		return *((CRgbaquad *)(data + i));
	}
	return COLOR_RGBA_BLACK;
}


CRgbaquad * CRegardsBitmap::GetPtColorValue(const int &x, const int &y)
{
	CRgbaquad * color = nullptr;
	if (data != nullptr && x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight)
	{
		int i = GetPosition(x, y);
		color = ((CRgbaquad *)(data + i));
	}
	return color;
}
 

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void CRegardsBitmap::SetColorValue(const int &x, const int &y, const CRgbaquad &color)
{
	if (data != nullptr && x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight)
	{
		int i = GetPosition(x, y);
		memcpy(data + i, &color, sizeof(CRgbaquad));
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
long CRegardsBitmap::GetBitmapSize()
{
	return m_lSize;
}

const long CRegardsBitmap::GetWidthSize()
{
	if(m_sDepth == 24)
		return ((((m_iWidth * 24) + 31) & ~31) >> 3);

	return m_iWidth * 4;
}
void CRegardsBitmap::SetAlphaValue(const int &value)
{
    uint8_t alphaValue = ((float)value / 100.0f) * 255;
 	if (data != nullptr)
	{
#pragma omp parallel for
		for (auto y = 0; y < GetBitmapHeight(); y++)
		{
#pragma omp parallel for
			for (auto x = 0; x < m_iWidth; x++)
			{
				CRgbaquad * colorSrc = GetPtColorValue(x, y);
				colorSrc->SetAlpha(alphaValue);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int CRegardsBitmap::SetValueToTranspColor(const CRgbaquad & backgroundValue)
{
	if (data != nullptr)
	{
#pragma omp parallel for
		for (auto y = 0; y < GetBitmapHeight(); y++)
		{
#pragma omp parallel for
			for (auto x = 0; x < m_iWidth; x++)
			{
                /*
				CRgbaquad * colorSrc = GetPtColorValue(x, y);
				if (colorSrc->GetAlpha() == 0)
				{
					*colorSrc = backgroundValue;
				}
                 */
                CRgbaquad colorSrc = GetColorValue(x, y);
                if (colorSrc.GetAlpha() == 0)
                {
                    SetColorValue(x, y, backgroundValue);
                }
			}
		}
	}
	return 0;
}


void CRegardsBitmap::ConvertToBgr()
{
	if (data != nullptr)
	{
    #pragma omp parallel for
            for (auto y = 0; y < GetBitmapHeight(); y++)
            {
    #pragma omp parallel for
                for (auto x = 0; x < m_iWidth; x++)
                {
                    
                    CRgbaquad * colorSrc = GetPtColorValue(x, y);
                    if (colorSrc != nullptr)
                    {
                        uint8_t blue = colorSrc->GetBlue();
                        colorSrc->SetBlue(colorSrc->GetRed());
                        colorSrc->SetRed(blue);
                    }
                }
            }
	}
}

int CRegardsBitmap::InsertBitmap(CRegardsBitmap * bitmap, int xPos, int yPos, const bool &withalpha)
{
	if (data != nullptr && bitmap != nullptr)
	{
		int yEnd = yPos + bitmap->GetBitmapHeight();
		int xEnd = xPos + bitmap->GetBitmapWidth();

		if (yEnd > m_iHeight)
			yEnd = m_iHeight;

		if (xEnd > m_iWidth)
			xEnd = m_iWidth;

#pragma omp parallel for
		for (auto y = yPos; y < yEnd; y++)
		{
#pragma omp parallel for
			for (auto x = xPos; x < xEnd; x++)
			{
                CRgbaquad * colorSrc = GetPtColorValue(x, y);
				if (colorSrc != nullptr)
				{
					if (withalpha)
					{
						CRgbaquad color = bitmap->GetColorValue(x - xPos, y - yPos);
						float alpha = color.GetFAlpha() / 255.0f;
						float alphaDiff = 1.0f - alpha;
						if (alphaDiff < 1.0f)
						{
							colorSrc->Mul(alphaDiff);
							color.Mul(alpha);
							colorSrc->Add(color);
							//SetColorValue(x, y, colorSrc);
						}
					}
					else
					{
						CRgbaquad color = bitmap->GetColorValue(x - xPos, y - yPos);
						*colorSrc = color;
					}
				}
			}
		}
	}

	return 0;
}


int CRegardsBitmap::InsertwxImage(const wxImage & bitmap, int xPos, int yPos)
{
	if (data != nullptr && bitmap.IsOk())
	{
		int withwxImage = bitmap.GetWidth();
		int yEnd = yPos + bitmap.GetHeight();
		int xEnd = xPos + bitmap.GetWidth();

		if (yEnd > m_iHeight)
			yEnd = m_iHeight;

		if (xEnd > m_iWidth)
			xEnd = m_iWidth;

		uint8_t * data = bitmap.GetData();
		uint8_t * alpha = bitmap.GetAlpha();
		
#pragma omp parallel for
		for (auto y = yPos; y < yEnd; y++)
		{
	#pragma omp parallel for
			for (auto x = xPos; x < xEnd; x++)	
			{
				int i = (y - yPos) * withwxImage + (x - xPos);
				CRgbaquad * colorSrc = GetPtColorValue(x, y);
				if (colorSrc != nullptr)
				{
					CRgbaquad color = CRgbaquad(data[i * 3], data[i * 3 + 1], data[i * 3 + 2], alpha[i]);
					float alpha = color.GetFAlpha() / 255.0f;
					float alphaDiff = 1.0f - alpha;
					if (alphaDiff < 1.0f)
					{
						colorSrc->Mul(alphaDiff);
						color.Mul(alpha);
						colorSrc->Add(color);
						//SetColorValue(x, y, colorSrc);
					}
				}
			}
		}
	}

	return 0;
}

CRegardsBitmap * CRegardsBitmap::CropBitmap(const int &xPos, const int &yPos, const int &width, const int &height)
{
	CRegardsBitmap * bitmap = new CRegardsBitmap(width, height);
	//bitmap->SetBackgroundColor(COLOR_RGBA_BLACK);
	//uint8_t * dataPicture = bitmap->GetPtBitmap();

	if (xPos < 0 || xPos > this->GetBitmapWidth() || yPos < 0 || yPos > this->GetBitmapHeight())
		return bitmap;

	if (width > this->GetBitmapWidth() || height > this->GetBitmapHeight())
		return bitmap;

	if (data != nullptr)
	{
		int yEnd = yPos + height;
		int xEnd = xPos + width;

		if (yEnd > this->GetBitmapHeight())
			yEnd = this->GetBitmapHeight();

		if (xEnd > this->GetBitmapWidth())
			xEnd = this->GetBitmapWidth();

#pragma omp parallel for
		for (auto y = yPos; y < yEnd; y++)
		{
#pragma omp parallel for
			for (auto x = xPos; x < xEnd; x++)
			{
				//CRgbaquad * color = bitmap->GetPtColorValue(x - xPos, y - yPos);
				//*color = GetColorValue(x, y);
                bitmap->SetColorValue(x - xPos, y - yPos, GetColorValue(x, y));
			}
		}
	}
	return bitmap;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int CRegardsBitmap::SetColorTranspBitmap(const CRgbaquad &Transp)
{
	if (data != nullptr)
	{
#pragma omp parallel for
		for (auto y = 0; y < m_iHeight; y++)
		{
#pragma omp parallel for
			for (auto x = 0; x < m_iWidth; x++)

			{
                /*
				CRgbaquad * color = GetPtColorValue(x, y);
				if (*color == Transp)
					color->SetAlpha(0);
				else
					color->SetAlpha(255);
                */
                CRgbaquad color = GetColorValue(x, y);
                if (color == Transp)
                    color.SetAlpha(0);
                else
                    color.SetAlpha(255);
                
                SetColorValue(x, y, color);
			}
		}
	}
	return 0;
}

void CRegardsBitmap::SetBackgroundColor(const CRgbaquad &m_cValue)
{
	int size = m_iWidth << 2;
	uint8_t * buffer = new uint8_t[size];

#pragma omp parallel for
	for (auto x = 0; x < m_iWidth; x++)
	{
		int position = x << 2;
		memcpy(buffer + position, &m_cValue, sizeof(CRgbaquad));
	}

	if (data != nullptr)
	{
#pragma omp parallel for
		for (auto y = 0; y < m_iHeight; y++)
		{
			int position = GetPosition(0, y);
			memcpy(data + position, buffer, size);
		}

	}

	delete[] buffer;
}


int CRegardsBitmap::InsertBitmapWithoutAlpha(CRegardsBitmap * picture, int xPos, int yPos)
{
	uint8_t * pictureData = picture->GetPtBitmap();
	int pictureWidth = picture->GetBitmapWidth();
	//int pictureHeight = picture->GetBitmapHeight();
	if (data != nullptr && pictureData != nullptr)
	{
		int copySize = pictureWidth << 2;
		int yEnd = yPos + picture->GetBitmapHeight();
		int xEnd = xPos + pictureWidth;

		if (yEnd > m_iHeight)
			yEnd = m_iHeight;

		if (xEnd > m_iWidth)
			xEnd = m_iWidth;

		if (yPos < 0)
			yPos = 0;

		if (xPos < 0)
			xPos = 0;

#pragma omp parallel for
		for (auto y = yPos; y < yEnd; y++)
		{
			memcpy(data + GetPosition(xPos, y), pictureData + picture->GetPosition(0, y - yPos), copySize);
		}
	}

	return 0;
}


int CRegardsBitmap::FusionBitmap(CRegardsBitmap * nextPicture, const float &pourcentage)
{
	//uint8_t * dest = nextPicture->GetPtBitmap();

	if (data != nullptr)
	{
		float diff = 1.0f - pourcentage;
#pragma omp parallel for
		for (auto y = 0; y < m_iHeight; y++)
		{
#pragma omp parallel for
			for (auto x = 0; x < m_iWidth; x++)
			{
                /*
				CRgbaquad color1 = nextPicture->GetColorValue(x,y);
				CRgbaquad * color2 = GetPtColorValue(x, y);
				color1.Mul(pourcentage);
				color2->Mul(diff);
				color2->Add(color1);
                */
                
                CRgbaquad color1 = nextPicture->GetColorValue(x,y);
                CRgbaquad color2 = GetColorValue(x, y);
                color1.Mul(pourcentage);
                color2.Mul(diff);
                color2.Add(color1);
                SetColorValue(x, y, color2);
			}
		}
	}
	return 0;
}

void CRegardsBitmap::SetBackgroundBitmap(CRegardsBitmap * background, const int &xStart, const int &yStart)
{
	//uint8_t * m_bBackgound = background->GetPtBitmap();

	if (data != nullptr)
	{
#pragma omp parallel for
		for (auto y = 0; y < m_iHeight; y++)
		{
#pragma omp parallel for
			for (auto x = 0; x < m_iWidth; x++)
			{
                /*
				CRgbaquad * color2 = GetPtColorValue(x, y);
				if (color2->GetAlpha() == 0)
				{
					*color2 = background->GetColorValue(x + xStart, y + yStart);
				}
                */
                
                CRgbaquad color2 = GetColorValue(x, y);
                if (color2.GetAlpha() == 0)
                    SetColorValue(x, y, background->GetColorValue(x + xStart, y + yStart));
			}
		}

	}
}
