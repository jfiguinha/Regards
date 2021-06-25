#include <header.h>
#include "Interpolation.h"
#include "RegardsBitmap.h"
#include <BitmapYUV.h>

struct myFiltreInterpolationTask
{
	myFiltreInterpolationTask(CInterpolation* filtre, const int& x, const int& y, CRegardsBitmap* In,
	                          CRegardsBitmap* & Out, const wxRect& rectToShow, const int& flipH, const int& flipV,
	                          const int& angle)
	{
		this->x = x;
		this->y = y;
		this->In = In;
		this->Out = Out;
		this->filtre = filtre;
		this->rectToShow = rectToShow;
		this->flipH = flipH;
		this->flipV = flipV;
		this->angle = angle;
	}

	void operator()()
	{
		int32_t width = Out->GetBitmapWidth();
		int32_t height = Out->GetBitmapHeight();

		int32_t widthIn = In->GetBitmapWidth();
		int32_t heightIn = In->GetBitmapHeight();

		float posY = 0;
		float posX = 0;
		CInterpolation::CalculPosition(x, y, widthIn, heightIn, width, height, rectToShow, flipH, flipV, angle, posX,
		                               posY);
		Out->SetColorValue(x, y, In->GetColorValue(posX, posY));
	}

	int x;
	int y;
	CRegardsBitmap* In;
	CRegardsBitmap* Out;
	wxRect rectToShow;
	int flipH;
	int flipV;
	int angle;
	CInterpolation* filtre;
};

CInterpolation::CInterpolation()
{
}


CInterpolation::~CInterpolation()
{
}

inline CRgbaquad GetColorValue(uint8_t* data, const int& m_iWidth, const int& m_iHeight, const int& x, const int& y)
{
	//CRgbaquad color = COLOR_RGBA_BLACK;
	if (data != nullptr && x >= 0 && y >= 0 && x < m_iWidth && y < m_iHeight)
	{
		int i = (x << 2) + (y * (m_iWidth << 2));
		return *((CRgbaquad*)(data + i));
	}
	return COLOR_RGBA_BLACK;
}


//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline CRgbaquad GetColorFromYUV(CBitmapYUV* bmpYUV, const int& x, const int& y)
{
	CRgbaquad color;
	if (x < bmpYUV->width && y < bmpYUV->height && y >= 0 && x >= 0)
	{
		int crv = 104597;
		int cbu = 132201;
		int cgu = 25675;
		int cgv = 53279;
		int positionSrc = x + y * bmpYUV->width;
		int position_uv; //width * height;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{
			if (yModulo == 1)
				position_uv = ((x - 1) / 2) + ((y - 1) / 2) * bmpYUV->uvlinewidth;
			else
				position_uv = ((x - 1) / 2) + (y / 2) * bmpYUV->uvlinewidth;
		}
		else
		{
			if (yModulo == 1)
				position_uv = (x / 2) + ((y - 1) / 2) * bmpYUV->uvlinewidth;
			else
				position_uv = (x / 2) + (y / 2) * bmpYUV->uvlinewidth;
		}

		int v = bmpYUV->dataV[position_uv];
		int u = bmpYUV->dataU[position_uv];

		int c1 = (v - 128) * crv;
		int c2 = (u - 128) * cgu;
		int c3 = (v - 128) * cgv;
		int c4 = (u - 128) * cbu;

		int y1 = 76309 * (bmpYUV->dataY[positionSrc] - 16);
		int r = (y1 + c1) >> 16;
		int g = (y1 - c2 - c3) >> 16;
		int b = (y1 + c4) >> 16;

		if (r < 0)
			r = 0;
		if (r > 255)
			r = 255;

		if (g < 0)
			g = 0;
		if (g > 255)
			g = 255;

		if (b < 0)
			b = 0;
		if (b > 255)
			b = 255;

		color = CRgbaquad(b, g, r, 255);
		/*
		color.x = r;
		color.y = g;
		color.z = b;
		color.w = 255;*/

		return color;
	}
	return color;
}

//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline CRgbaquad GetColorValueFromNV12(uint8_t* dataY, const int& width, const int& height, const int& x, const int& y,
                                       int pitch, int surfaceHeight)
{
	CRgbaquad color;
	if (x < width && y < height && y >= 0 && x >= 0)
	{
		int crv = 104597;
		int cbu = 132201;
		int cgu = 25675;
		int cgv = 53279;
		int positionSrc = x + y * pitch;
		int positionUV = pitch * surfaceHeight;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{
			if (yModulo == 1)
				positionUV = (x - 1) + ((y - 1) / 2) * pitch + positionUV;
			else
				positionUV = (x - 1) + (y / 2) * pitch + positionUV;
		}
		else
		{
			if (yModulo == 1)
				positionUV = x + ((y - 1) / 2) * pitch + positionUV;
			else
				positionUV = x + (y / 2) * pitch + positionUV;
		}

		int v = dataY[positionUV];
		int u = dataY[positionUV + 1];

		int c1 = (v - 128) * crv;
		int c2 = (u - 128) * cgu;
		int c3 = (v - 128) * cgv;
		int c4 = (u - 128) * cbu;

		int y1 = 76309 * (dataY[positionSrc] - 16);
		int r = (y1 + c1) >> 16;
		int g = (y1 - c2 - c3) >> 16;
		int b = (y1 + c4) >> 16;

		if (r < 0)
			r = 0;
		if (r > 255)
			r = 255;

		if (g < 0)
			g = 0;
		if (g > 255)
			g = 255;

		if (b < 0)
			b = 0;
		if (b > 255)
			b = 255;

		color = CRgbaquad(r, g, b, 255);
		/*
		color.x = r;
		color.y = g;
		color.z = b;
		color.w = 255;*/

		return color;
	}
	return color;
}

//----------------------------------------------------
// Conversion du NV12 vers du 32 bits
//----------------------------------------------------
inline CRgbaquad GetColorFromNV12(uint8_t* dataY, uint8_t* dataUV, const int& width, const int& height, const int& x,
                                  const int& y)
{
	CRgbaquad color;
	if (x < width && y < height && y >= 0 && x >= 0)
	{
		int crv = 104597;
		int cbu = 132201;
		int cgu = 25675;
		int cgv = 53279;
		int positionSrc = x + y * width;
		int positionUV = 0; //width * height;

		int yModulo = y % 2;
		int xModulo = x % 2;
		if (xModulo == 1)
		{
			if (yModulo == 1)
				positionUV = (x - 1) + ((y - 1) / 2) * width;
			else
				positionUV = (x - 1) + (y / 2) * width;
		}
		else
		{
			if (yModulo == 1)
				positionUV = x + ((y - 1) / 2) * width + positionUV;
			else
				positionUV = x + (y / 2) * width + positionUV;
		}

		int v = dataUV[positionUV];
		int u = dataUV[positionUV + 1];

		int c1 = (v - 128) * crv;
		int c2 = (u - 128) * cgu;
		int c3 = (v - 128) * cgv;
		int c4 = (u - 128) * cbu;

		int y1 = 76309 * (dataY[positionSrc] - 16);
		int r = (y1 + c1) >> 16;
		int g = (y1 - c2 - c3) >> 16;
		int b = (y1 + c4) >> 16;

		if (r < 0)
			r = 0;
		if (r > 255)
			r = 255;

		if (g < 0)
			g = 0;
		if (g > 255)
			g = 255;

		if (b < 0)
			b = 0;
		if (b > 255)
			b = 255;

		color = CRgbaquad(r, g, b, 255);
		/*
		color.x = r;
		color.y = g;
		color.z = b;
		color.w = 255;*/

		return color;
	}
	return color;
}

wxImage CInterpolation::ExecuteNV12(uint8_t* data, const int& widthIn, const int& heightIn, const int& widthOut,
                                    const int& heightOut, const int& flipH, const int& flipV, const int& angle,
                                    int pitch, int surfaceHeight)
{
	//wxImage * imageout = nullptr;

	int width = widthOut;
	int height = heightOut;

	wxImage imageout(width, height);


	float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);
	if (angle == 90 || angle == 270)
	{
		ratioX = static_cast<float>(widthIn) / static_cast<float>(height);
		ratioY = static_cast<float>(heightIn) / static_cast<float>(width);
	}

	int size = widthOut * heightOut;


	uint8_t* dataOut = imageout.GetData();

#pragma omp parallel for
	for (auto i = 0; i < size; ++i)
	{
		int y = i / width;
		int x = i - (y * width);

		float posY = static_cast<float>(y) * ratioY;
		float posX = static_cast<float>(x) * ratioX;

		if (angle == 90)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posX = widthIn - posX - 1;
		}
		else if (angle == 180)
		{
			posX = widthIn - posX - 1;
			posY = heightIn - posY - 1;
		}
		else if (angle == 270)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posY = heightIn - posY - 1;
		}

		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
		CRgbaquad color = GetColorValueFromNV12(data, widthIn, heightIn, posX, posY, pitch, surfaceHeight);
		memcpy(dataOut + (i * 3), &color, 3);
	}
	return imageout;
}

const wxImage CInterpolation::Execute(uint8_t* data, const int& widthIn, const int& heightIn, const int& widthOut,
                                      const int& heightOut, const int& flipH, const int& flipV, const int& angle)
{
	//wxImage * imageout = nullptr;

	int width = widthOut;
	int height = heightOut;

	wxImage imageout(width, height);


	float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);
	if (angle == 90 || angle == 270)
	{
		ratioX = static_cast<float>(widthIn) / static_cast<float>(height);
		ratioY = static_cast<float>(heightIn) / static_cast<float>(width);
	}

	int size = widthOut * heightOut;


	uint8_t* dataOut = imageout.GetData();

#pragma omp parallel for
	for (auto i = 0; i < size; ++i)
	{
		int y = i / width;
		int x = i - (y * width);

		float posY = static_cast<float>(y) * ratioY;
		float posX = static_cast<float>(x) * ratioX;

		if (angle == 90)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posX = widthIn - posX - 1;
		}
		else if (angle == 180)
		{
			posX = widthIn - posX - 1;
			posY = heightIn - posY - 1;
		}
		else if (angle == 270)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posY = heightIn - posY - 1;
		}

		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
		CRgbaquad color = GetColorValue(data, widthIn, heightIn, posX, posY);
		memcpy(dataOut + (i * 3), &color, 3);
	}
	return imageout;
}


wxImage CInterpolation::ExecuteYUV(CBitmapYUV* bmpYUV, const int& widthOut, const int& heightOut, const int& flipH,
                                   const int& flipV, const int& angle)
{
	int width = widthOut;
	int height = heightOut;

	wxImage imageout(width, height);


	float ratioX = static_cast<float>(bmpYUV->width) / static_cast<float>(width);
	float ratioY = static_cast<float>(bmpYUV->height) / static_cast<float>(height);
	if (angle == 90 || angle == 270)
	{
		ratioX = static_cast<float>(bmpYUV->width) / static_cast<float>(height);
		ratioY = static_cast<float>(bmpYUV->height) / static_cast<float>(width);
	}

	int size = widthOut * heightOut;


	uint8_t* dataOut = imageout.GetData();

#pragma omp parallel for
	for (auto i = 0; i < size; ++i)
	{
		int y = i / width;
		int x = i - (y * width);

		float posY = static_cast<float>(y) * ratioY;
		float posX = static_cast<float>(x) * ratioX;

		if (angle == 90)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posX = bmpYUV->width - posX - 1;
		}
		else if (angle == 180)
		{
			posX = bmpYUV->width - posX - 1;
			posY = bmpYUV->height - posY - 1;
		}
		else if (angle == 270)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posY = bmpYUV->height - posY - 1;
		}

		if (flipH == 1)
		{
			posX = bmpYUV->width - posX - 1;
		}

		if (flipV == 1)
		{
			posY = bmpYUV->height - posY - 1;
		}
		CRgbaquad color = GetColorFromYUV(bmpYUV, posX, posY);
		memcpy(dataOut + (i * 3), &color, 3);
	}
	return imageout;
}


wxImage* CInterpolation::ExecuteNV12(uint8_t* dataY, uint8_t* dataUV, const int& widthIn, const int& heightIn,
                                     const int& widthOut, const int& heightOut, const int& flipH, const int& flipV,
                                     const int& angle)
{
	int width = widthOut;
	int height = heightOut;

	wxImage* imageout = new wxImage(width, height);


	float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);
	if (angle == 90 || angle == 270)
	{
		ratioX = static_cast<float>(widthIn) / static_cast<float>(height);
		ratioY = static_cast<float>(heightIn) / static_cast<float>(width);
	}

	int size = widthOut * heightOut;


	uint8_t* dataOut = imageout->GetData();

#pragma omp parallel for
	for (auto i = 0; i < size; ++i)
	{
		int y = i / width;
		int x = i - (y * width);

		float posY = static_cast<float>(y) * ratioY;
		float posX = static_cast<float>(x) * ratioX;

		if (angle == 90)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posX = widthIn - posX - 1;
		}
		else if (angle == 180)
		{
			posX = widthIn - posX - 1;
			posY = heightIn - posY - 1;
		}
		else if (angle == 270)
		{
			int srcx = posY;
			int srcy = posX;

			posX = srcx;
			posY = srcy;

			posY = heightIn - posY - 1;
		}

		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
		CRgbaquad color = GetColorFromNV12(dataY, dataUV, widthIn, heightIn, posX, posY);
		memcpy(dataOut + (i * 3), &color, 3);
	}
	return imageout;
}

wxImage CInterpolation::Execute(CRegardsBitmap* In, const int& widthOut, const int& heightOut, const int& flipH,
                                const int& flipV, const int& angle)
{
	wxImage imageout(widthOut, heightOut);

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	float ratioX = static_cast<float>(widthIn) / static_cast<float>(widthOut);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(heightOut);
	int size = widthOut * heightOut;

	if (angle == 90 || angle == 270)
	{
		ratioX = static_cast<float>(heightIn) / static_cast<float>(widthOut);
		ratioY = static_cast<float>(widthIn) / static_cast<float>(heightOut);
	}

	uint8_t* dataOut = imageout.GetData();

#pragma omp parallel for
	for (auto i = 0; i < size; ++i)
	{
		int y = i / widthOut;
		int x = i - (y * widthOut);

		float posY = static_cast<float>(y) * ratioY;
		float posX = static_cast<float>(x) * ratioX;

		if (angle == 90)
		{
			int srcx = posY;
			int srcy = heightIn - posX - 1;

			posX = srcx;
			posY = srcy;
		}
		else if (angle == 180)
		{
			posX = widthIn - posX - 1;
			posY = heightIn - posY - 1;
		}
		else if (angle == 270)
		{
			int srcx = widthIn - posY - 1;
			int srcy = posX;

			posX = srcx;
			posY = srcy;
		}

		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 0)
		{
			posY = heightIn - posY - 1;
		}

		CRgbaquad color = In->GetColorValue(posX, posY);

		//int calcul = (height - y - 1) * width + x;
		dataOut[i * 3] = color.GetRed(); // R
		dataOut[i * 3 + 1] = color.GetGreen(); // G
		dataOut[i * 3 + 2] = color.GetBlue(); // B
	}
	return imageout;
}


void CInterpolation::Execute(CRegardsBitmap* In, CRegardsBitmap* & Out, const wxRect& rectToShow, const int& flipH,
                             const int& flipV, const int& angle)
{
	int32_t width = Out->GetBitmapWidth();
	int32_t height = Out->GetBitmapHeight();

	int32_t widthIn = In->GetBitmapWidth();
	int32_t heightIn = In->GetBitmapHeight();

	if (widthIn > 0 && heightIn > 0 && width > 0 && height > 0)
	{
#ifdef USE_TBB
		std::vector<myFiltreInterpolationTask> tasks;

		for (auto y = 0; y < height; y++)
		{
			for (auto x = 0; x < width; x++)
			{
				tasks.push_back(myFiltreInterpolationTask(this, x, y, In, Out, rectToShow, flipH, flipV, angle));
			}
		}

		parallel_for(
			tbb::blocked_range<size_t>(0, tasks.size()),
			[&tasks](const tbb::blocked_range<size_t>& r)
			{
				for (size_t i = r.begin(); i < r.end(); ++i)
					tasks[i]();
			}
		);
#else
		
#pragma omp parallel for
		for (auto y = 0; y < height; y++)
		{


#pragma omp parallel for
			for (auto x = 0; x < width; x++)
			{
				float posY = 0;
				float posX = 0;
				CInterpolation::CalculPosition(x, y, widthIn, heightIn, width, height, rectToShow, flipH, flipV, angle, posX, posY);
				Out->SetColorValue(x, y, In->GetColorValue(posX, posY));
			}
		}
#endif
	}
}


void CInterpolation::Execute(CRegardsBitmap* In, CRegardsBitmap* & Out)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	if (widthIn > 0 && heightIn > 0 && width > 0 && height > 0)
	{
		float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
		float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);

#pragma omp parallel for
		for (auto y = 0; y < height; y++)
		{
#pragma omp parallel for
			for (auto x = 0; x < width; x++)
			{
				float posY = static_cast<float>(y) * ratioY;
				float posX = static_cast<float>(x) * ratioX;
				Out->SetColorValue(x, y, In->GetColorValue(posX, posY));
			}
		}
	}
}

void CInterpolation::CalculPosition(const int& x, const int& y, const int& widthIn, const int& heightIn,
                                    const int& widthOut, const int& heightOut, const wxRect& rectToShow,
                                    const int& flipH, const int& flipV, const int& angle, float& posX, float& posY)
{
	//int width = widthOut;
	//int height = heightOut;

	float ratioX = static_cast<float>(widthIn) / static_cast<float>(rectToShow.width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(rectToShow.height);


	if (angle == 90 || angle == 270)
	{
		ratioX = static_cast<float>(heightIn) / static_cast<float>(rectToShow.width);
		ratioY = static_cast<float>(widthIn) / static_cast<float>(rectToShow.height);
	}

	posX = static_cast<float>(x) * ratioX + rectToShow.x * ratioX;
	posY = static_cast<float>(y) * ratioY + rectToShow.y * ratioY;

	if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
	}
	else if (angle == 180)
	{
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
	}
	else if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}

	if (angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
}


void CInterpolation::CalculPosition(const int& x, const int& y, const int& widthIn, const int& heightIn,
                                    const int& widthOut, const int& heightOut, const int& flipH, const int& flipV,
                                    const int& angle, float& posX, float& posY)
{
	int width = widthOut;
	int height = heightOut;


	float ratioX = static_cast<float>(widthIn) / static_cast<float>(width);
	float ratioY = static_cast<float>(heightIn) / static_cast<float>(height);

	if (angle == 90 || angle == 270)
	{
		ratioX = static_cast<float>(widthIn) / static_cast<float>(height);
		ratioY = static_cast<float>(heightIn) / static_cast<float>(width);
	}


	posY = static_cast<float>(y) * ratioY;
	posX = static_cast<float>(x) * ratioX;

	if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
	}
	else if (angle == 180)
	{
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
	}
	else if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}

	if (angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
}


void CInterpolation::Execute(CRegardsBitmap* In, CRegardsBitmap* & Out, const int& flipH, const int& flipV,
                             const int& angle)
{
	int width = Out->GetBitmapWidth();
	int height = Out->GetBitmapHeight();

	int widthIn = In->GetBitmapWidth();
	int heightIn = In->GetBitmapHeight();

	if (widthIn > 0 && heightIn > 0 && width > 0 && height > 0)
	{
#pragma omp parallel for
		for (auto y = 0; y < height; y++)
		{
#pragma omp parallel for
			for (auto x = 0; x < width; x++)
			{
				float posY = 0;
				float posX = 0;
				CalculPosition(x, y, widthIn, heightIn, width, height, flipH, flipV, angle, posX, posY);
				Out->SetColorValue(x, y, In->GetColorValue(posX, posY));
			}
		}
	}
}
