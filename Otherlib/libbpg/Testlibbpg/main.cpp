// mainBpg.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <tchar.h>
#include <string>
#include <DllBpg.h>
using namespace std;

void  writefile(const string & fileName, uint8_t * data, size_t & size)
{
	FILE *file = NULL;
	if ((file = fopen(fileName.c_str(), "wb")) == NULL)
		printf("File Failed To Load\n");
	else
	{
		fwrite(data, size, 1, file);
		fclose(file);
	}
}


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

void SaveToBmp(const wstring &filename, const uint8_t * data, const int &m_iWidth, const int &m_iHeight)
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

uint8_t * readfile(const string & fileName, size_t &_fileSize)
{
	uint8_t * _compressedImage = nullptr;
	FILE *file = NULL;
	if ((file = fopen(fileName.c_str(), "rb")) == NULL)
		printf("File Failed To Load\n");
	else
	{
		long prev = ftell(file);
		fseek(file, 0L, SEEK_END);
		_fileSize = ftell(file);
		fseek(file, prev, SEEK_SET);

		//Creating a buffer and saving it back
		_compressedImage = new uint8_t[_fileSize];
		//cout << "fileSize" << fileSize;
		fread(_compressedImage, _fileSize, 1, file);
		fclose(file);
	}
	return _compressedImage;
}

int _tmain(int argc, _TCHAR* argv[])
{
	size_t filesize = 0;
	size_t filesizeoutput = 0;
	uint8_t * output = nullptr;
	int width = 0, height = 0;
	
	/*
	uint8_t * data = readfile("c:\\developpement\\out.png", filesize);
	int returnValue = BPG_SavePNGPicture(data, filesize,"c:\\developpement\\out.bpg");
	delete[] data;
	*/
	
	uint8_t * data = readfile("c:\\developpement\\005.bpg", filesize);

	BPG_GetDimensions(data, filesize, width, height);
	
	filesizeoutput = width * height * 4;
	output = new uint8_t[filesizeoutput];
	int returnValue = BPG_GetPictureRGBA(data, filesize, output, filesizeoutput, width, height, true);
	
	SaveToBmp(L"c:\\developpement\\out.bmp", output, width, height);
	
	delete[] output;
	/*
	int bit_depth = 8;
	output = BPG_GetPNGPicture(data, filesize, filesizeoutput, bit_depth);
	writefile("c:\\developpement\\test3.png",output, filesizeoutput);
	delete[] data;
	free(output);
	*/
	printf("Taille : %d et %d \n", width, height);
	
	return 0;
}

