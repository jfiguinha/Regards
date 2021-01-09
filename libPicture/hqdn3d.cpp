#include <header.h>
#include "hqdn3d.h"
#include <RegardsBitmap.h>
#include <algorithm>

/*
	HQDN3D 0.11 for Avisynth

	Copyright (C) 2003 Daniel Moreno <comac@comac.darktech.org>
	Avisynth port (C) 2005 Loren Merritt <lorenm@u.washington.edu>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


extern float value[256];

#define ABS(A) ( (A) > 0 ? (A) : -(A) )

static void PrecalcCoefs(int *Ct, double Dist25)
{
	int i;
	double Gamma, Simil, C;

	Gamma = log(0.25) / log(1.0 - Dist25 / 255.0 - 0.00001);

	for (i = -255 * 16; i < 256 * 16; i++)
	{
		Simil = 1.0 - ABS(i) / (16 * 255.0);
		C = pow(Simil, Gamma) * 65536.0 * (double)i / 16.0;
		Ct[16 * 256 + i] = (int)((C < 0) ? (C - 0.5) : (C + 0.5));
	}
}

static inline unsigned int LowPassMul(unsigned int PrevMul, unsigned int CurrMul, int* Coef) {
	//	int dMul= (PrevMul&0xFFFFFF)-(CurrMul&0xFFFFFF);
	int dMul = PrevMul - CurrMul;
	int d = ((dMul + 0x10007FF) / (65536 / 16));
	return CurrMul + Coef[d];
}

void Chqdn3d::deNoise(uint8_t * Frame,  // mpi->planes[x]
	uint8_t * FrameDest,    // dmpi->planes[x]
	unsigned int *LineAnt,       // vf->priv->Line (width bytes)
	uint8_t *FrameAnt,
	int W, int H, int sStride, int dStride,
	int *Horizontal, int *Vertical, int *Temporal)
{
	int X, Y;
	int sLineOffs = 0, dLineOffs = 0;
	unsigned int PixelAnt;
	int PixelDst;

	/* First pixel has no left nor top neightbour. Only previous frame */
	LineAnt[0] = PixelAnt = Frame[0] << 16;
	PixelDst = LowPassMul(FrameAnt[0] << 8, PixelAnt, Temporal);
	FrameAnt[0] = ((PixelDst + 0x1000007F) / 256);
	FrameDest[0] = ((PixelDst + 0x10007FFF) / 65536);

	/* Fist line has no top neightbour. Only left one for each pixel and
		* last frame */
	for (X = 1; X < W; X++) {
		LineAnt[X] = PixelAnt = LowPassMul(PixelAnt, Frame[X] << 16, Horizontal);
		PixelDst = LowPassMul(FrameAnt[X] << 8, PixelAnt, Temporal);
		FrameAnt[X] = ((PixelDst + 0x1000007F) / 256);
		FrameDest[X] = ((PixelDst + 0x10007FFF) / 65536);
	}

	for (Y = 1; Y < H; Y++) {
		unsigned int PixelAnt;
		uint8_t * LinePrev = &FrameAnt[Y*W];
		sLineOffs += sStride, dLineOffs += dStride;
		/* First pixel on each line doesn't have previous pixel */
		PixelAnt = Frame[sLineOffs] << 16;
		LineAnt[0] = LowPassMul(LineAnt[0], PixelAnt, Vertical);
		PixelDst = LowPassMul(LinePrev[0] << 8, LineAnt[0], Temporal);
		LinePrev[0] = ((PixelDst + 0x1000007F) / 256);
		FrameDest[dLineOffs] = ((PixelDst + 0x10007FFF) / 65536);

		for (X = 1; X < W; X++) {
			int PixelDst;
			/* The rest are normal */
			PixelAnt = LowPassMul(PixelAnt, Frame[sLineOffs + X] << 16, Horizontal);
			LineAnt[X] = LowPassMul(LineAnt[X], PixelAnt, Vertical);
			PixelDst = LowPassMul(LinePrev[X] << 8, LineAnt[X], Temporal);
			LinePrev[X] = ((PixelDst + 0x1000007F) / 256);
			FrameDest[dLineOffs + X] = ((PixelDst + 0x10007FFF) / 65536);
		}
	}
}


Chqdn3d::Chqdn3d(const int &w, const int &h, const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp)
{
	double lumSpac = LumSpac;
	double chromSpac = ChromSpac;
	double lumTmp = LumTmp;
	double chromTmp = ChromTmp;


	if (lumSpac < 0)
		lumSpac = 4.0;
	if (chromSpac < 0)
		chromSpac = .75 * lumSpac;
	if (lumTmp < 0)
		lumTmp = 1.5 * lumSpac;
	if (chromTmp < 0) {
		if (lumSpac == 0)
			chromTmp = chromSpac * 1.5;
		else
			chromTmp = lumTmp * chromSpac / lumSpac;
	}
	lumSpac = __min(254.9, lumSpac);
	chromSpac = __min(254.9, chromSpac);
	lumTmp = __min(254.9, lumTmp);
	chromTmp = __min(254.9, chromTmp);

	//if (restart_lap < 0)
	//	restart_lap = __max(2, (int)(1 + __max(LumTmp, ChromTmp)));

	PrecalcCoefs(Coefs[0], lumSpac);
	PrecalcCoefs(Coefs[1], lumTmp);
	PrecalcCoefs(Coefs[2], chromSpac);
	PrecalcCoefs(Coefs[3], chromTmp);
	picture_y = new uint8_t[w*h];
	Frame = new uint8_t[w*h];
	y_out = new uint8_t[w*h];
	Line = new unsigned int[w];
}

Chqdn3d::~Chqdn3d()
{
	if(Line != nullptr)
		delete[] Line;

	if (Frame != nullptr)
		delete[] Frame;

	if (y_out != nullptr)
		delete[] y_out;

	if (picture_y != nullptr)
		delete[] picture_y;

	Line = nullptr;
	Frame = nullptr;
	y_out = nullptr;
	picture_y = nullptr;
}

uint8_t * Chqdn3d::ApplyDenoise3D(uint8_t * picture_y, const int &w, const int &h)
{
	deNoise(picture_y,
		y_out,
		Line, Frame, w, h,
		w,
		w,
		Coefs[0], Coefs[0], Coefs[1]);
	
	return y_out;
}

int Chqdn3d::ApplyDenoise3D(CRegardsBitmap * bitmapIn)
{
	CRegardsBitmap * bitmapOut = new CRegardsBitmap();
	*bitmapOut = *bitmapIn;
	w = bitmapIn->GetBitmapWidth();
	h = bitmapIn->GetBitmapHeight();


	uint8_t* dataIn = bitmapIn->GetPtBitmap();
	int posPicture = 0;
	int posLocal = 0;
	for (int posY = 0; posY < bitmapIn->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmapIn->GetBitmapWidth(); posX++)
		{
			picture_y[posLocal] = 0.257f * value[dataIn[posPicture + 2]] + 0.504f * value[dataIn[posPicture + 1]] + 0.098f * value[dataIn[posPicture]] + 16;
			//picture_v[posLocal] = (0.439 *  value[dataIn[posPicture + 2]]) - (0.368 * value[dataIn[posPicture + 1]]) - (0.071 *  value[dataIn[posPicture]]) + 128;
			//picture_u[posLocal] = -(0.148 *  value[dataIn[posPicture + 2]]) - (0.291 * value[dataIn[posPicture + 1]]) + (0.439 *  value[dataIn[posPicture]]) + 128;

			posLocal++;
			posPicture += 4;
		}
	}

	deNoise(picture_y,
		y_out,
		Line, Frame, w, h,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[0], Coefs[0], Coefs[1]);

	posLocal = 0;
	posPicture = 0;
	uint8_t* data = bitmapOut->GetPtBitmap();

	for (int posY = 0; posY < bitmapOut->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmapOut->GetBitmapWidth(); posX++)
		{
			float Y = y_out[posLocal];
			float V = (0.439 * data[posPicture + 2]) - (0.368 * data[posPicture + 1]) - (0.071 * data[posPicture]) + 128;
			float U = -(0.148 * data[posPicture + 2]) - (0.291 * data[posPicture + 1]) + (0.439 * data[posPicture]) + 128;

			double B = 1.164 * (Y - 16) + 2.018 * (U - 128);
			double G = 1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128);
			double R = 1.164 * (Y - 16) + 1.596 * (V - 128);

			dataIn[posPicture + 2] = min(max(R, 0.0), 255.0);//min(Y + 1.13983f * V, 255.0f); //R
			dataIn[posPicture + 1] = min(max(G, 0.0), 255.0);//min(Y - 0.39465f * U - 0.5806f * V, 255.0f); //G
			dataIn[posPicture] = min(max(B, 0.0), 255.0);//min(Y + 2.03211f * U, 255.0f); //B
			posPicture += 4;
			posLocal++;
		}
	}

	delete bitmapOut;

	return 0;
}