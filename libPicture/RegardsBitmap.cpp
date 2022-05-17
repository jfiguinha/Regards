#include <header.h>
// IBitmap.cpp: implementation of the CRegardsBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "RegardsBitmap.h"
#include <FaceRect.h>
#include <fstream>
extern float value[256];
extern float clamp(float val, float minval, float maxval);

auto CRegardsBitmap::operator=(const CRegardsBitmap& other) -> CRegardsBitmap&
{
	filename = other.filename;
	orientation = other.orientation;
	other.bitmapMatrix.copyTo(bitmapMatrix);
	return *this;
}


void CRegardsBitmap::GetY(uint8_t* & lum)
{
	if (!bitmapMatrix.empty())
	{
		cv::Mat cvDest;
		cv::Mat ycbcr;
		cv::Mat yChannel;

		cv::cvtColor(bitmapMatrix, cvDest, cv::COLOR_BGRA2BGR);
		cvtColor(cvDest, ycbcr, cv::COLOR_BGR2YCrCb);
		cv::extractChannel(ycbcr, yChannel, 0);
		memcpy(lum, yChannel.data, bitmapMatrix.cols * bitmapMatrix.rows);

		yChannel.release();
		ycbcr.release();
	}
}

void CRegardsBitmap::SetY(uint8_t* lum)
{
	if (!bitmapMatrix.empty())
	{
		cv::Mat cvDest;
		cv::Mat ycbcr;
		cv::Mat yChannel;

		cv::cvtColor(bitmapMatrix, cvDest, cv::COLOR_BGRA2BGR);
		cvtColor(cvDest, ycbcr, cv::COLOR_BGR2YCrCb);

		// Extract the Y channel
		cv::extractChannel(ycbcr, yChannel, 0);

		memcpy(yChannel.data, lum, bitmapMatrix.cols * bitmapMatrix.rows);

		// Merge the the color planes back into an Lab image
		cv::insertChannel(yChannel, ycbcr, 0);

		// convert back to RGB
		cv::cvtColor(ycbcr, cvDest, cv::COLOR_YCrCb2BGR);
		cv::cvtColor(cvDest, bitmapMatrix, cv::COLOR_BGR2BGRA);
		// Temporary Mat not reused, so release from memory.
		yChannel.release();
		ycbcr.release();
	}
}

int CRegardsBitmap::GetOrientation()
{
	return orientation;
}

void CRegardsBitmap::SetOrientation(const int& orientation)
{
	this->orientation = orientation;
}

void CRegardsBitmap::ApplyRotation(const int& rotation)
{
	if (rotation == 90)
		Rotate90();
	else if (rotation == -90)
		Rotate270();
	else if (rotation == -180)
		Rotate180();
	else if (rotation == 180)
		Rotate180();
	else if (rotation == -270)
		Rotate90();
	else if (rotation == 270)
		Rotate270();
}

bool CRegardsBitmap::IsValid()
{
	if (bitmapMatrix.empty() || GetBitmapHeight() == 0 || GetBitmapWidth() == 0 || GetBitmapSize() == 0)
		return false;

	return true;
}

#ifdef WIN32

#include <Windows.h>

HBITMAP GenerateHBitmap(const uint8_t* pbyData, const int& nWidth, const int& nHeight)
{
	BITMAPINFO bmiInfos;
	memset(&bmiInfos, 0, sizeof(BITMAPINFO));
	bmiInfos.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiInfos.bmiHeader.biWidth = nWidth;
	bmiInfos.bmiHeader.biHeight = nHeight;
	bmiInfos.bmiHeader.biPlanes = 1;
	bmiInfos.bmiHeader.biCompression = BI_RGB;
	bmiInfos.bmiHeader.biBitCount = 32;
	bmiInfos.bmiHeader.biSizeImage = nWidth * nHeight << 2;
	//((iTaille * bitmap.bmWidth + iTaille) & ~iTaille) * bitmap.bmHeight;
	const HBITMAP hBitmapRender = CreateDIBitmap(GetDC(nullptr),
	                                             &bmiInfos.bmiHeader,
	                                             CBM_INIT,
	                                             pbyData,
	                                             &bmiInfos,
	                                             DIB_RGB_COLORS);

	return hBitmapRender;
}

void CRegardsBitmap::SaveToBmp(const wxString& filename)
{
	const HBITMAP hBitmap = GenerateHBitmap(bitmapMatrix.data, bitmapMatrix.cols, bitmapMatrix.rows);
	HDC hDC = GetDC(nullptr);
	WORD wBitCount;
	DWORD dwPaletteSize = 0, dwWritten = 0;
	BITMAP Bitmap0;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal, hOldPal2 = nullptr;
	wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap0), &Bitmap0);
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
	const DWORD dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
		* Bitmap0.bmHeight;
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = static_cast<LPBITMAPINFOHEADER>(GlobalLock(hDib));
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(nullptr);
		hOldPal2 = SelectPalette(hDC, static_cast<HPALETTE>(hPal), FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, hBitmap, 0, static_cast<UINT>(Bitmap0.bmHeight), (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
	          + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	if (hOldPal2)
	{
		SelectPalette(hDC, static_cast<HPALETTE>(hOldPal2), TRUE);
		RealizePalette(hDC);
		ReleaseDC(nullptr, hDC);
	}

	fh = CreateFile(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
	                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);

	if (fh == INVALID_HANDLE_VALUE)
		return;

	bmfHdr.bfType = 0x4D42; // "BM"
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER)) + static_cast<DWORD>(sizeof(BITMAPINFOHEADER)) +
		dwPaletteSize;

	::WriteFile(fh, &bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, nullptr);

	::WriteFile(fh, lpbi, dwDIBSize, &dwWritten, nullptr);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
}

#endif

#define SCALEBITS            8
#define ONE_HALF             (1 << (SCALEBITS - 1))
#define FIX(x)               ((int) ((x) * (1L<<SCALEBITS) + 0.5))

void CRegardsBitmap::GetYUV420P(uint8_t* & lum, uint8_t* & cb, uint8_t* & cr)
{
	if (bitmapMatrix.empty())
		return;

	int width_middle = bitmapMatrix.cols / 2;
	int height_middle = bitmapMatrix.rows / 2;

	for (int y = 0; y < height_middle; y++)
	{

		for (int x = 0; x < width_middle; x++)
		{
			float r1 = 0;
			float g1 = 0;
			float b1 = 0;
			int cr_position = x + y * width_middle;
			int lum_position = (x * 2) + (y * 2) * bitmapMatrix.cols;
			int position = (x * 2) * 4 + (y * 2) * bitmapMatrix.cols * 4;
			for (int i = 0; i < 2; i++)
			{
				float r = value[bitmapMatrix.data[position + 2]];
				float g = value[bitmapMatrix.data[position + 1]];
				float b = value[bitmapMatrix.data[position + 0]];
				r1 += r;
				g1 += g;
				b1 += b;
				lum[lum_position] = static_cast<int>((FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b +
					ONE_HALF)) >> SCALEBITS;
				r = value[bitmapMatrix.data[position + 6]];
				g = value[bitmapMatrix.data[position + 5]];
				b = value[bitmapMatrix.data[position + 4]];
				r1 += r;
				g1 += g;
				b1 += b;
				lum[lum_position + 1] = static_cast<int>((FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b +
					ONE_HALF)) >> SCALEBITS;

				position += bitmapMatrix.cols * 4;
				lum_position += bitmapMatrix.cols;
			}

			cb[cr_position] = ((static_cast<int>(-FIX(0.16874) * r1 - FIX(0.33126) * g1 + FIX(0.50000) * b1 + 4 *
				ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
			cr[cr_position] = ((static_cast<int>((FIX(0.50000) * r1 - FIX(0.41869) * g1 - FIX(0.08131) * b1 + 4 *
				ONE_HALF - 1)) >> (SCALEBITS + 2)) + 128);
		}
	}
}

void CRegardsBitmap::SetYUV420P(uint8_t* lum, uint8_t* cb, uint8_t* cr)
{
	if (bitmapMatrix.empty())
		return;
	for (int y = 0; y < bitmapMatrix.rows; y++)
	{

		for (int x = 0; x < bitmapMatrix.cols; x++)
		{
			const int positionSrc = x + y * bitmapMatrix.cols;
			int position_uv;
			if (x & 1)
			{
				if (y & 1)
					position_uv = ((x - 1) / 2) + ((y - 1) / 2) * (bitmapMatrix.cols / 2);
				else
					position_uv = ((x - 1) / 2) + (y / 2) * (bitmapMatrix.cols / 2);
			}
			else
			{
				if (y & 1)
					position_uv = (x / 2) + ((y - 1) / 2) * (bitmapMatrix.cols / 2);
				else
					position_uv = (x / 2) + (y / 2) * (bitmapMatrix.cols / 2);
			}
			float uComp = cb[position_uv];
			float vComp = cr[position_uv];
			float yComp = lum[positionSrc];
			// RGB conversion

			float b = (1.164 * (yComp - 16) + 1.596 * (vComp - 128));
			float g = (1.164 * (yComp - 16) - 0.391 * (uComp - 128) - 0.813 * (vComp - 128));
			float r = (1.164 * (yComp - 16) + 2.018 * (uComp - 128));
			float a = 255.0f;

			float minimal = 0.0;
			float maximal = 255.0;

			int position = x * 4 + y * bitmapMatrix.cols * 4;
			bitmapMatrix.data[position] = clamp(r, minimal, maximal);
			bitmapMatrix.data[position + 1] = clamp(g, minimal, maximal);
			bitmapMatrix.data[position + 2] = clamp(b, minimal, maximal);
			bitmapMatrix.data[position + 3] = clamp(a, minimal, maximal);
		}
	}
}

void CRegardsBitmap::ReadFile(const wxString& filename)
{
	bitmapMatrix = cv::imread(filename.ToStdString());
    if(!bitmapMatrix.empty())
        cv::cvtColor(bitmapMatrix, bitmapMatrix, cv::COLOR_BGR2BGRA);
}

void CRegardsBitmap::WriteFile(const wxString& filename)
{
    if(!bitmapMatrix.empty())
        cv::imwrite(filename.ToStdString(), bitmapMatrix);
}


void CRegardsBitmap::SetBitmap(const int& iWidth, const int& iHeight, const int& iDepth)
{
	bitmapMatrix.release();
	bitmapMatrix = cv::Mat(iHeight, iWidth, CV_8UC4);
}


CRegardsBitmap::CRegardsBitmap(const int& iWidth, const int& iHeight, const int& iDepth)
{

	bitmapMatrix.cols = iWidth;
	bitmapMatrix.rows = iHeight;
	bitmapMatrix = cv::Mat(iHeight, iWidth, CV_8UC4);
	orientation = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRegardsBitmap::CRegardsBitmap()
{
	bitmapMatrix.cols = 0;
	bitmapMatrix.rows = 0;
	orientation = 0;
}

int CRegardsBitmap::GetBitmapDepth()
{
	return 32;
}

cv::Mat& CRegardsBitmap::GetMatrix()
{
	return bitmapMatrix;
}

void CRegardsBitmap::SetMatrix(const cv::UMat& matrix, const bool& m_bFlip)
{
	if (matrix.channels() == 3)
		cvtColor(matrix, bitmapMatrix, cv::COLOR_BGR2BGRA);
	else if (matrix.channels() == 1)
		cvtColor(matrix, bitmapMatrix, cv::COLOR_GRAY2BGRA);
	else
		matrix.copyTo(bitmapMatrix);

	if (m_bFlip)
	{
		VertFlipBuf();
	}
}

void CRegardsBitmap::SetMatrix(const cv::Mat& matrix, const bool& m_bFlip)
{
	if (matrix.channels() == 3)
		cvtColor(matrix, bitmapMatrix, cv::COLOR_BGR2BGRA);
	else if (matrix.channels() == 1)
		cvtColor(matrix, bitmapMatrix, cv::COLOR_GRAY2BGRA);
	else
		matrix.copyTo(bitmapMatrix);

	if (m_bFlip)
	{
		VertFlipBuf();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
CRegardsBitmap::~CRegardsBitmap()
{
	bitmapMatrix.release();
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsBitmap::HorzFlipBuf()
{
	if (!bitmapMatrix.empty())
		cv::flip(bitmapMatrix, bitmapMatrix, 1);

	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsBitmap::Rotate90()
{
	if (bitmapMatrix.empty())
		return false;

	cv::rotate(bitmapMatrix, bitmapMatrix, cv::ROTATE_90_COUNTERCLOCKWISE);
	return true;
}

bool CRegardsBitmap::Rotate270()
{
	if (bitmapMatrix.empty())
		return false;

	cv::rotate(bitmapMatrix, bitmapMatrix, cv::ROTATE_90_CLOCKWISE);
	return true;
}

bool CRegardsBitmap::Rotate180()
{
	if (bitmapMatrix.empty())
		return false;

	cv::rotate(bitmapMatrix, bitmapMatrix, cv::ROTATE_180);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRegardsBitmap::VertFlipBuf()
{
	if (bitmapMatrix.empty())
		return false;
	
	cv::flip(bitmapMatrix, bitmapMatrix, 0);

	return true;
}

/*
bool CRegardsBitmap::RotateRawExif(const int& orientation)
{
	bool ret = true;
	switch (orientation)
	{
	case 1: // top left side
		break;
	case 2: // top right side
		this->HorzFlipBuf();
		break;
	case 3: // bottom right side
		this->Rotate90();
		this->VertFlipBuf();
		break;
	case 4: // bottom left side
		this->VertFlipBuf();
		break;
	case 5: //left side top
		this->Rotate90();
		//this->VertFlipBuf();
		break;
	case 6: // right side top
		this->Rotate90();
		//this->VertFlipBuf();
		this->HorzFlipBuf();
		break;
	case 7: // right side bottom
		this->Rotate90();
		this->HorzFlipBuf();
		this->VertFlipBuf();
		break;
	case 8: // left side bottom
		this->Rotate90();
		this->VertFlipBuf();
		break;
	default:;
	}
	return ret;
}
*/
bool CRegardsBitmap::RotateExif(const int& orientation)
{
	bool ret = true;
	switch (orientation)
	{
	case 1: // top left side
		break;
	case 2: // top right side
		this->HorzFlipBuf();
		break;
	case 3: // bottom right side
		this->HorzFlipBuf();
		this->VertFlipBuf();
		break;
	case 4: // bottom left side
		this->VertFlipBuf();
		break;
	case 5: //left side top
		this->Rotate90();
		this->HorzFlipBuf();
		break;
	case 6: // right side top
		this->Rotate90();
		break;
	case 7: // right side bottom
		this->Rotate90();
		this->VertFlipBuf();
		break;
	case 8: // left side bottom
		this->Rotate90();
		this->HorzFlipBuf();
		this->VertFlipBuf();
		break;
	default: ;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

uint8_t* CRegardsBitmap::GetPtBitmap()
{
	return bitmapMatrix.data;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const int CRegardsBitmap::GetBitmapWidth()
{
	return bitmapMatrix.cols;
}

const int CRegardsBitmap::GetBitmapHeight()
{
	return bitmapMatrix.rows;
}

int CRegardsBitmap::GetPosition(const int& x, const int& y)
{
	return (x << 2) + (y * (bitmapMatrix.cols << 2));
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void CRegardsBitmap::SetFilename(const wxString& szFilename)
{
	filename = szFilename;
}

wxString CRegardsBitmap::GetFilename()
{
	return filename;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CRgbaquad CRegardsBitmap::GetColorValue(const int& x, const int& y)
{
	//CRgbaquad color = COLOR_RGBA_BLACK;
	if (!bitmapMatrix.empty() && x >= 0 && y >= 0 && x < bitmapMatrix.cols && y < bitmapMatrix.rows)
	{
		int i = GetPosition(x, y);
		return *((CRgbaquad*)(bitmapMatrix.data + i));
	}
	return COLOR_RGBA_BLACK;
}


CRgbaquad* CRegardsBitmap::GetPtColorValue(const int& x, const int& y)
{
	CRgbaquad* color = nullptr;
	if (!bitmapMatrix.empty() && x >= 0 && y >= 0 && x < bitmapMatrix.cols && y < bitmapMatrix.rows)
	{
		int i = GetPosition(x, y);
		color = ((CRgbaquad*)(bitmapMatrix.data + i));
	}
	return color;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void CRegardsBitmap::SetColorValue(const int& x, const int& y, const CRgbaquad& color)
{
	if (!bitmapMatrix.empty() && x >= 0 && y >= 0 && x < bitmapMatrix.cols && y < bitmapMatrix.rows)
	{
		int i = GetPosition(x, y);
		memcpy(bitmapMatrix.data + i, &color, sizeof(CRgbaquad));
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
long CRegardsBitmap::GetBitmapSize()
{
	return bitmapMatrix.cols * 4 * bitmapMatrix.rows;
}

const long CRegardsBitmap::GetWidthSize()
{
	return bitmapMatrix.cols * 4;
}

void CRegardsBitmap::SetAlphaValue(const int& value)
{
	uint8_t alphaValue = (static_cast<float>(value) / 100.0f) * 255;
	if (!bitmapMatrix.empty())
	{
		vector<cv::Mat> bgr_planes;
		split(bitmapMatrix, bgr_planes);
		bgr_planes[3] = cv::Scalar(alphaValue);
		cv::merge(bgr_planes, bitmapMatrix);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int CRegardsBitmap::SetValueToTranspColor(const CRgbaquad& backgroundValue)
{
	if (!bitmapMatrix.empty())
	{

		for (auto y = 0; y < GetBitmapHeight(); y++)
		{

			for (auto x = 0; x < bitmapMatrix.cols; x++)
			{
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
	if (!bitmapMatrix.empty())
	{
		cvtColor(bitmapMatrix, bitmapMatrix, cv::COLOR_RGBA2BGRA);
	}
}

int CRegardsBitmap::InsertBitmap(CRegardsBitmap* bitmap, int xPos, int yPos, const bool& withalpha)
{
	if (!bitmapMatrix.empty() && bitmap != nullptr)
	{
		int yEnd = yPos + bitmap->GetBitmapHeight();
		int xEnd = xPos + bitmap->GetBitmapWidth();

		if (yEnd > bitmapMatrix.rows)
			yEnd = bitmapMatrix.rows;

		if (xEnd > bitmapMatrix.cols)
			xEnd = bitmapMatrix.cols;


		for (auto y = yPos; y < yEnd; y++)
		{

			for (auto x = xPos; x < xEnd; x++)
			{
				CRgbaquad* colorSrc = GetPtColorValue(x, y);
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


int CRegardsBitmap::InsertwxImage(const wxImage& bitmap, int xPos, int yPos)
{
	if (!bitmapMatrix.empty() && bitmap.IsOk())
	{
		int withwxImage = bitmap.GetWidth();
		int yEnd = yPos + bitmap.GetHeight();
		int xEnd = xPos + bitmap.GetWidth();

		if (yEnd > bitmapMatrix.rows)
			yEnd = bitmapMatrix.rows;

		if (xEnd > bitmapMatrix.cols)
			xEnd = bitmapMatrix.cols;

		uint8_t* data = bitmap.GetData();
		uint8_t* alpha = bitmap.GetAlpha();


		tbb::parallel_for(yPos, yEnd, 1, [=](int y)
			//for (auto y = yPos; y < yEnd; y++)
			{

				for (auto x = xPos; x < xEnd; x++)
				{
					int i = (y - yPos) * withwxImage + (x - xPos);
					CRgbaquad* colorSrc = GetPtColorValue(x, y);
					if (colorSrc != nullptr)
					{
						auto color = CRgbaquad(data[i * 3], data[i * 3 + 1], data[i * 3 + 2], alpha[i]);
						float value = color.GetFAlpha() / 255.0f;
						float alphaDiff = 1.0f - value;
						if (alphaDiff < 1.0f)
						{
							colorSrc->Mul(alphaDiff);
							color.Mul(value);
							colorSrc->Add(color);
						}
					}
				}
			});
	}

	return 0;
}

CRegardsBitmap* CRegardsBitmap::CropBitmap(const int& xPos, const int& yPos, const int& width, const int& height)
{
	auto bitmap = new CRegardsBitmap();

	if (xPos < 0 || xPos > this->GetBitmapWidth() || yPos < 0 || yPos > this->GetBitmapHeight())
		return bitmap;

	if (width > this->GetBitmapWidth() || height > this->GetBitmapHeight())
		return bitmap;

	if (!bitmapMatrix.empty())
	{
		int yEnd = yPos + height;
		int xEnd = xPos + width;

		if (yEnd > this->GetBitmapHeight())
			yEnd = this->GetBitmapHeight();

		if (xEnd > this->GetBitmapWidth())
			xEnd = this->GetBitmapWidth();

		cv::Rect rect(xPos, yPos, (xPos - xEnd), (yEnd - yPos));
		VerifRectSize(rect, bitmapMatrix);
		//bool is_inside = (rect & cv::Rect(0, 0, bitmapMatrix.cols, bitmapMatrix.rows)) == rect;
		cv::Mat crop;
		bitmapMatrix(rect).copyTo(crop);
		bitmap->SetMatrix(crop);
	}
	return bitmap;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int CRegardsBitmap::SetColorTranspBitmap(const CRgbaquad& Transp)
{
	if (!bitmapMatrix.empty())
	{

		for (auto y = 0; y < bitmapMatrix.rows; y++)
		{
			for (auto x = 0; x < bitmapMatrix.cols; x++)
			{
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

void CRegardsBitmap::SetBackgroundColor(const CRgbaquad& m_cValue)
{
	if (!bitmapMatrix.empty())
		bitmapMatrix = cv::Scalar(m_cValue.GetBlue(), m_cValue.GetGreen(), m_cValue.GetRed());

}


int CRegardsBitmap::InsertBitmapWithoutAlpha(CRegardsBitmap* picture, int xPos, int yPos)
{
	uint8_t* pictureData = picture->GetPtBitmap();
	int pictureWidth = picture->GetBitmapWidth();

	if (!bitmapMatrix.empty() && pictureData != nullptr)
	{
		const int copySize = pictureWidth << 2;
		int y_end = yPos + picture->GetBitmapHeight();

		if (y_end > bitmapMatrix.rows)
			y_end = bitmapMatrix.rows;

		if (yPos < 0)
			yPos = 0;

		if (xPos < 0)
			xPos = 0;


		for (auto y = yPos; y < y_end; y++)
		{
			memcpy(bitmapMatrix.data + GetPosition(xPos, y), pictureData + picture->GetPosition(0, y - yPos), copySize);
		}
	}

	return 0;
}


int CRegardsBitmap::FusionBitmap(CRegardsBitmap* nextPicture, const float& pourcentage)
{
	if (!bitmapMatrix.empty())
	{
		cv::Mat dst;
		float beta = (1.0 - pourcentage);
		cv::addWeighted(bitmapMatrix, pourcentage, nextPicture->GetMatrix(), beta, 0.0, dst);
		dst.copyTo(bitmapMatrix);
	}
	return 0;
}


