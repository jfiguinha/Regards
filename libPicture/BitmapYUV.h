#pragma once
class CBitmapNV12DXVA2
{
public:
	CBitmapNV12DXVA2()
	{
		data = nullptr;
		dataSize = 0;
		width = 0;
		height = 0;
		pitch = 0;
		heightPitch = 0;
		widthVideo = 0;
		heightVideo = 0;
	}

	~CBitmapNV12DXVA2()
	{
		if(data != nullptr)
			delete[] data;
	}


	uint8_t * data;
	int dataSize;
	int width;
	int height;
	int pitch;
	int heightPitch;
	int widthVideo;
	int heightVideo;

};


class CBitmapNV12
{
public:
	CBitmapNV12()
	{
		dataY = nullptr;
		dataUV = nullptr;
		dataYSize = 0;
		dataUVSize = 0;
	}

	~CBitmapNV12()
	{
		if(dataY != nullptr)
			delete[] dataY;

		if(dataUV != nullptr)
			delete[] dataUV;

		dataYSize = 0;
		dataUVSize = 0;
	}


	uint8_t * dataY;
	int dataYSize;
	uint8_t * dataUV;
	int dataUVSize;
	int width;
	int height;
	int widthVideo;
	int heightVideo;

};

class CBitmapYUV
{
public:
	CBitmapYUV()
	{
		dataY = nullptr;
		dataU = nullptr;
		dataV = nullptr;
		dataYSize = 0;
		dataUSize = 0;
		dataVSize = 0;
		isOk = false;
	}

	~CBitmapYUV()
	{
		if(dataY != nullptr)
			delete[] dataY;

		if(dataU != nullptr)
			delete[] dataU;

		if(dataV != nullptr)
			delete[] dataV;

		dataYSize = 0;
		dataUSize = 0;
		dataVSize = 0;
	}

	bool isOk;
	uint8_t * dataY;
	int dataYSize;
	uint8_t * dataU;
	int dataUSize;
	uint8_t * dataV;
	int dataVSize;
	int uvlinewidth;
	int width;
	int height;

};