//    Copyright (C) 2010 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#include "header.h"
#include "cximagewriter.h"
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <RegardsBitmap.h>
using namespace std;

namespace ffmpegthumbnailer
{
	CxImageWriter::CxImageWriter(const std::string& outputFile)
	{
		wX = nullptr;
		wY = nullptr;
		image = nullptr;
	}

	CxImageWriter::CxImageWriter(std::vector<uint8_t>& outputBuffer)
	{
		image = nullptr;
		wX = nullptr;
		wY = nullptr;
	}

	CxImageWriter::CxImageWriter(CRegardsBitmap * image) : ImageWriter()
	{
		init();
		this->image = image;
		wX = nullptr;
		wY = nullptr;
	}

	CxImageWriter::~CxImageWriter()
	{
	}

	void CxImageWriter::init()
	{
	}

	void CxImageWriter::setText(const string& /*key*/, const string& /*value*/)
	{
	}

	double CxImageWriter::Filter(const double &f)
	{
	
		return (f < -2.0 || f > 2.0) ? 0.0 : (
			(f < -1.0) ? (2.0 + f)*(2.0 + f)*(2.0 + f) / 6.0 : (
			(f < 0.0) ? (4.0 + f*f*(-6.0 - 3.0*f)) / 6.0 : (
			(f < 1.0) ? (4.0 + f*f*(-6.0 + 3.0*f)) / 6.0 : (2.0 - f)*(2.0 - f)*(2.0 - f) / 6.0)));
	
	}

	void CxImageWriter::CalculWeight(const int32_t &width, const int32_t &height, const float &ratioY, const float &ratioX, const float &posTop, const float &posLeft)
	{
		wX = new weightX[width];
		wY = new weightX[height];

	#pragma omp parallel for
		for (auto y = 0; y < height; y++)
		{
			float posY = (float)y * ratioY + posTop;
			int valueB = (int)posY;
			float realB = posY - valueB;
			wY[y].tabF[0] = Filter(-(-1.0f - realB));
			wY[y].tabF[1] = Filter(-(0.0f - realB));
			wY[y].tabF[2] = Filter(-(1.0f - realB));
			wY[y].tabF[3] = Filter(-(2.0f - realB));
		}
	#pragma omp parallel for
		for (auto x = 0; x < width; x++)
		{
			float posX = (float)x * ratioX + posLeft;
			int valueA = (int)posX;
			float realA = posX - valueA;
			wX[x].tabF[0] = Filter((-1.0f - realA));
			wX[x].tabF[1] = Filter((0.0f - realA));
			wX[x].tabF[2] = Filter((1.0f - realA));
			wX[x].tabF[3] = Filter((2.0f - realA));
		}
	}

	void CxImageWriter::writeFrame(std::vector<uint8_t> & rgbData, int lineSize, int width, int height, int quality, int thumbnailWidth, int thumbnailHeight, int rotation)
	{
		if(image == nullptr)
			return;

		//int widthSize = ((((width * 24) + 31) & ~31) >> 3);

		//image->Create(width, height, 24);
		//image->RGBtoBGR(*rgbData, width*height * 4);
		//image->CreateFromArray(&rgbData.at(0), rgbData.size(), width, height, 24, lineSize, true, true);

		if(thumbnailWidth == 0 || thumbnailHeight == 0)
		{
			image->SetBitmap(width, height);
#pragma omp parallel for
			for (int y = 0; y < height; y++)
			{
#pragma omp parallel for
				for (int x = 0; x < width; x++)
				{
					uint8_t * data = image->GetPtBitmap();
					int position = (y << 2) * width + (x << 2);
					int srcPos = y * lineSize + x * 3;
					data[position] = rgbData[srcPos];
					data[position+1] = rgbData[srcPos+1];
					data[position+2] = rgbData[srcPos+2];
				}
			}

			if (rotation == 90)
			{
				image->Rotation90();
				image->VertFlipBuf();
			}
			else if (rotation == 270)
			{
				image->Rotation90();
			}
			else if (rotation == 180)
			{
				image->VertFlipBuf();
				image->HorzFlipBuf();
			}
		}
		else
		{
			int widthThumbnail = thumbnailWidth;
			int heightThumbnail = thumbnailHeight;

			if(rotation == 90 || rotation == 270)
			{
				widthThumbnail = thumbnailHeight;
				heightThumbnail = thumbnailWidth;
				image->SetBitmap(thumbnailHeight, thumbnailWidth);
				//image = new CRegardsBitmap(thumbnailHeight, thumbnailWidth);
			}
			else
				image->SetBitmap(thumbnailWidth, thumbnailHeight);
			
			//image = new CRegardsBitmap(thumbnailWidth, thumbnailHeight);


			int widthIn = width;
			int heightIn = height;

			float ratioX = (float)widthIn / (float)widthThumbnail;
			float ratioY = (float)heightIn / (float)heightThumbnail;

			if (rotation == 90 || rotation == 270)
			{
				ratioX = (float)heightIn / (float)widthThumbnail;
				ratioY = (float)widthIn / (float)heightThumbnail;
			}

			uint8_t * data = image->GetPtBitmap();

			CalculWeight(widthThumbnail, heightThumbnail, ratioY, ratioX, 0.0f, 0.0f);
	
		#pragma omp parallel for
			for (auto y = 0; y < heightThumbnail; y++)
			{
		#pragma omp parallel for
				for (auto x = 0; x < widthThumbnail; x++)
				{
					float posY = float(y) * ratioY;
					float posX = float(x) * ratioX;

					if (rotation == 90)
					{
						int srcx = posY;
						int srcy = height - posX - 1;

						posX = srcx;
						posY = srcy;
					}
					else if (rotation == 180)
					{
						posX = width - posX - 1;
						posY = height - posY - 1;
					}
					else if (rotation == 270)
					{
						int srcx = width - posY - 1;
						int srcy = posX;

						posX = srcx;
						posY = srcy;
					}


					CRgbaquad color;// = Out->GetPtColorValue(x, y);
					Bicubic(color, rgbData, width, height, lineSize, posX, posY, wY[y].tabF, wX[x].tabF);
					//Out->SetColorValue(x,y,color);
					int i =  (x << 2) + (y * (widthThumbnail << 2));// int i = Out->GetPosition(x, y);
					memcpy(data + i, &color, sizeof(CRgbaquad));
				}
			}
			delete[] wX;
			delete[] wY;
			wX = nullptr;
			wY = nullptr;
		}
	}

	void CxImageWriter::Bicubic(CRgbaquad & data, std::vector<uint8_t> & rgbData, int width, int height, int effwidth, const float &x, const float &y, float * tabF1, float * tabF)
	{
		float nDenom = 0.0;
		int valueA = (int)x;
		int valueB = (int)y;

		float r = 0.0, g = 0.0, b = 0.0, a = 0.0;

		int debutN = -1;
		int finN = 2;
		//Calcul démarrage du y;
		if (valueB == 0)
		{
			debutN = 0;
		}

		if (valueB == height - 2)
		{
			finN = 1;
		}

		if (valueB == height - 1)
		{
			finN = 0;
		}

		int debutM = -1;
		int finM = 2;
		//Calcul démarrage du y;
		if (valueA == 0)
		{
			debutM = 0;
		}

		if (valueA == width - 2)
		{
			finM = 1;
		}

		if (valueA == width - 1)
		{
			finM = 0;
		}

		int posX = valueA + debutM;
		if (valueA == 1)
			posX = valueA;

		int posY = valueB + debutN;
		if (valueB == 1)
			posY = valueB;

		for (auto n = debutN; n <= finN; n++)
		{
			for (auto m = debutM; m <= finM; m++)
			{
				int position = (posX + m) * 3 + (posY + n) * effwidth;
				float f = tabF1[n + 1] * tabF[m + 1];
				nDenom += f;
				r += rgbData[position+2] * f;
				g += rgbData[position+1] * f;
				b += rgbData[position] * f;
				a += 0;
			}
		}


		data.SetColor(uint8_t(r / nDenom), uint8_t(g / nDenom), uint8_t(b / nDenom), uint8_t(a / nDenom));
	}
}
