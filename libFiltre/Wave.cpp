#include <header.h>
#include "Wave.h"

using namespace Regards::FiltreEffet;

#ifndef PI
#define PI 3.141592653589793f
#endif

CWaveFilter::CWaveFilter(): _waveWidth(0), _waveHeight(0), _weHaveWaves(false), _scale(0)
{
	_activeBuffer = 0;
}


CWaveFilter::~CWaveFilter()
{
}

void CWaveFilter::ProcessEffect(cv::Mat& image, int x, int y, short height, int radius, int scale)
{
	_waves.clear();

	_scale = 2;
	_waveWidth = image.size().width >> _scale;
	_waveHeight = image.size().height >> _scale;
	int size = _waveWidth * _waveWidth * _waveHeight * 2;

	int realX = static_cast<int>((x / static_cast<double>(image.size().width)) * _waveWidth);
	int realY = static_cast<int>((y / static_cast<double>(image.size().height)) * _waveHeight);

	PutDrop(realX, realY, height, radius);

	if (_weHaveWaves)
	{
		for (int i = 0; i < scale; i++)
			ProcessWaves();
		WaterEffect(image);
	}
}

void CWaveFilter::ProcessWaves()
{
	int newBuffer = (_activeBuffer == 0) ? 1 : 0;
	bool wavesFound = false;


	for (int x = 1; x < _waveWidth - 1; x++)
	{
		for (int y = 1; y < _waveHeight - 1; y++)
		{
			short value = static_cast<short>(((GetWaveData(x - 1, y - 1, _activeBuffer) +
				GetWaveData(x, y - 1, _activeBuffer) +
				GetWaveData(x + 1, y - 1, _activeBuffer) +
				GetWaveData(x - 1, y, _activeBuffer) +
				GetWaveData(x + 1, y, _activeBuffer) +
				GetWaveData(x - 1, y + 1, _activeBuffer) +
				GetWaveData(x, y + 1, _activeBuffer) +
				GetWaveData(x + 1, y + 1, _activeBuffer)) >> 2) - GetWaveData(x, y, newBuffer));

			SetWaveData(x, y, newBuffer, value);

			//damping
			if (GetWaveData(x, y, newBuffer) != 0)
			{
				short data = GetWaveData(x, y, newBuffer);
				data -= static_cast<short>(GetWaveData(x, y, newBuffer) >> 4);
				SetWaveData(x, y, newBuffer, data);
				wavesFound = true;
			}
		}
	}

	_weHaveWaves = wavesFound;
	_activeBuffer = newBuffer;
}

void CWaveFilter::WaterEffect(cv::Mat& image)
{
	cv::Mat copyMat;
	uint8_t* data = image.data;
	image.copyTo(copyMat);
	uint8_t* copy = copyMat.data;


	int _bmpWidth = image.size().width;
	int _bmpHeight = image.size().height;
	int xOffset, yOffset;
	short alpha;

	if (_weHaveWaves)
	{
		for (int x = 1; x < _bmpWidth - 1; x++)
		{
			for (int y = 1; y < _bmpHeight - 1; y++)
			{
				int waveX = x >> _scale;
				int waveY = y >> _scale;

				//check bounds
				if (waveX <= 0) waveX = 1;
				if (waveY <= 0) waveY = 1;
				if (waveX >= _waveWidth - 1) waveX = _waveWidth - 2;
				if (waveY >= _waveHeight - 1) waveY = _waveHeight - 2;

				//this gives us the effect of water breaking the light
				xOffset = (GetWaveData(waveX - 1, waveY, _activeBuffer) - GetWaveData(waveX + 1, waveY, _activeBuffer))
					>> 3;
				yOffset = (GetWaveData(waveX, waveY - 1, _activeBuffer) - GetWaveData(waveX, waveY + 1, _activeBuffer))
					>> 3;

				if ((xOffset != 0) || (yOffset != 0))
				{
					//check bounds
					if (x + xOffset >= _bmpWidth - 1) xOffset = _bmpWidth - x - 1;
					if (y + yOffset >= _bmpHeight - 1) yOffset = _bmpHeight - y - 1;
					if (x + xOffset < 0) xOffset = -x;
					if (y + yOffset < 0) yOffset = -y;

					//generate alpha
					alpha = static_cast<short>(200 - xOffset);
					if (alpha < 0) alpha = 0;
					if (alpha > 255) alpha = 254;

					//set colors
					copy[3 * (x + y * _bmpWidth)] = data[3 * (x + xOffset + (y + yOffset) * _bmpWidth)];
					copy[3 * (x + y * _bmpWidth) + 1] = data[3 * (x + xOffset + (y + yOffset) * _bmpWidth) + 1];
					copy[3 * (x + y * _bmpWidth) + 2] = data[3 * (x + xOffset + (y + yOffset) * _bmpWidth) + 2];
				}
			}
		}
	}
	copyMat.copyTo(image);
}

void CWaveFilter::PutDrop(int x, int y, short height, int radius)
{
	_weHaveWaves = true;
	//int radius = 20;
	double dist;

	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			if (((x + i >= 0) && (x + i < _waveWidth - 1)) && ((y + j >= 0) && (y + j < _waveHeight - 1)))
			{
				dist = sqrt(i * i + j * j);
				if (dist < radius)
				{
					SetWaveData(x + i, y + j, _activeBuffer, static_cast<short>(cos(dist * PI / radius) * height));
				}
			}
		}
	}
}

int CWaveFilter::CalculPosition(int x, int y, int buffer)
{
	return (x * _waveWidth + y) + (buffer * _waveWidth * _waveWidth * _waveHeight);
}

void CWaveFilter::SetWaveData(int x, int y, int buffer, short data)
{
	_waves[CalculPosition(x, y, buffer)] = data;
}

short CWaveFilter::GetWaveData(int x, int y, int buffer)
{
	int position = CalculPosition(x, y, buffer);
	std::map<int, short>::iterator it;
	it = _waves.find(position);
	if (it != _waves.end())
		return _waves[position];
	return 0;
}
