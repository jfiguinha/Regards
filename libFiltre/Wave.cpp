#include <header.h>
#include "Wave.h"
#include "RegardsBitmap.h"
using namespace Regards::FiltreEffet;

#ifndef PI
#define PI 3.141592653589793f
#endif

CWaveFilter::CWaveFilter(): _waveWidth(0), _waveHeight(0), _weHaveWaves(false), _scale(0)
{
	_activeBuffer = 0;
	_waves = nullptr;
}


CWaveFilter::~CWaveFilter()
{
	if(_waves != nullptr)
		delete[] _waves;
}

void CWaveFilter::ProcessEffect(CRegardsBitmap * bitmap, int x, int y, short height, int radius, int scale)
{
	if (_waves != nullptr)
		delete[] _waves;

	_scale = 2;
	_waveWidth = bitmap->GetBitmapWidth() >> _scale;
	_waveHeight = bitmap->GetBitmapHeight() >> _scale;
	int size = _waveWidth * _waveWidth * _waveHeight * 2;
	_waves = new short[size+1];
	memset(_waves, 0, size * sizeof(short));

	int realX = (int)((x / (double)bitmap->GetBitmapWidth())*_waveWidth);
	int realY = (int)((y / (double)bitmap->GetBitmapHeight())*_waveHeight);

	PutDrop(realX, realY, height, radius);

	if (_weHaveWaves)
	{
		for (int i = 0; i < scale; i++)
			ProcessWaves();
		WaterEffect(bitmap);
	}
}

void CWaveFilter::ProcessWaves()
{
	int newBuffer = (_activeBuffer == 0) ? 1 : 0;
	bool wavesFound = false;

#pragma omp parallel for
	for (int x = 1; x<_waveWidth - 1; x++)
	{
#pragma omp parallel for
		for (int y = 1; y<_waveHeight - 1; y++)
		{
			short value = (short)(
				((GetWaveData(x - 1, y - 1, _activeBuffer) +
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

void CWaveFilter::WaterEffect(CRegardsBitmap * bitmap)
{
    if(bitmap != nullptr)
    {
        uint8_t * data = bitmap->GetPtBitmap();
        uint8_t * copy = new uint8_t[bitmap->GetBitmapSize()];
        memcpy(copy, data, bitmap->GetBitmapSize());

        int _bmpWidth = bitmap->GetBitmapWidth();
        int _bmpHeight = bitmap->GetBitmapHeight();
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
                    xOffset = (GetWaveData(waveX - 1, waveY, _activeBuffer) - GetWaveData(waveX + 1, waveY, _activeBuffer)) >> 3;
                    yOffset = (GetWaveData(waveX, waveY - 1, _activeBuffer) - GetWaveData(waveX, waveY + 1, _activeBuffer)) >> 3;

                    if ((xOffset != 0) || (yOffset != 0))
                    {
                        //check bounds
                        if (x + xOffset >= _bmpWidth - 1)	xOffset = _bmpWidth - x - 1;
                        if (y + yOffset >= _bmpHeight - 1)	yOffset = _bmpHeight - y - 1;
                        if (x + xOffset < 0)	xOffset = -x;
                        if (y + yOffset < 0)	yOffset = -y;

                        //generate alpha
                        alpha = (short)(200 - xOffset);
                        if (alpha < 0) alpha = 0;
                        if (alpha > 255) alpha = 254;

                        //set colors
                        copy[4 * (x + y*_bmpWidth)] = data[4 * (x + xOffset + (y + yOffset)*_bmpWidth)];
                        copy[4 * (x + y*_bmpWidth) + 1] = data[4 * (x + xOffset + (y + yOffset)*_bmpWidth) + 1];
                        copy[4 * (x + y*_bmpWidth) + 2] = data[4 * (x + xOffset + (y + yOffset)*_bmpWidth) + 2];
                        copy[4 * (x + y*_bmpWidth) + 3] = alpha;

                    }

                }
            }
        }

        memcpy(data, copy, bitmap->GetBitmapSize());

        delete[] copy;     
    }

}

void CWaveFilter::PutDrop(int x, int y, short height, int radius)
{


	_weHaveWaves = true;
	//int radius = 20;
	double dist;
#pragma omp parallel for
	for (int i = -radius; i <= radius; i++)
	{
#pragma omp parallel for
		for (int j = -radius; j <= radius; j++)
		{
			if (((x + i >= 0) && (x + i<_waveWidth - 1)) && ((y + j >= 0) && (y + j<_waveHeight - 1)))
			{
				dist = sqrt(i*i + j*j);
				if (dist < radius)
				{
					SetWaveData(x + i, y + j, _activeBuffer, (short)(cos(dist*PI / radius) * height));
				}
			}
		}
	}
}

void CWaveFilter::SetWaveData(int x, int y, int buffer, short data)
{
	_waves[(x * _waveWidth + y) + (buffer * _waveWidth * _waveWidth * _waveHeight)] = data;
}

short CWaveFilter::GetWaveData(int x, int y, int buffer)
{
	return _waves[(x * _waveWidth + y) + (buffer * _waveWidth * _waveWidth * _waveHeight)];
}
