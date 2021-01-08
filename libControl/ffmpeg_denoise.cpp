#include <header.h>
#include "ffmpeg_denoise.h"
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
enum type_yuv { TYPE_Y, TYPE_U, TYPE_V };

struct myDenoisetask {
	myDenoisetask(CFFmpegDenoise * denoise, unsigned short * Frame,  // mpi->planes[x]
		unsigned char * FrameDest,    // dmpi->planes[x]
		unsigned short *FrameAnt,
		int W, int H, int sStride, int dStride,
		int *Horizontal, int *Vertical, int *Temporal){

		this->denoise = denoise;
		this->Frame = Frame;
		this->FrameDest = FrameDest;
		this->FrameAnt = FrameAnt;
		this->W = W;
		this->H = H;
		this->sStride = sStride;
		this->dStride = dStride;
		this->Horizontal = Horizontal;
		this->Vertical = Vertical;
		this->Temporal = Temporal;
	}

	void operator()()
	{
		unsigned int * Line = new unsigned int[W];

		denoise->deNoise(Frame,  // mpi->planes[x]
			FrameDest,    // dmpi->planes[x]
			Line,       // vf->priv->Line (width bytes)
			FrameAnt,
			W, H, sStride, dStride,
			Horizontal, Vertical, Temporal);

		delete[] Line;
	}


	void Free()
	{
	}

	CFFmpegDenoise * denoise;
	unsigned short * Frame;
	unsigned char * FrameDest;
	unsigned short *FrameAnt;
	int W;
	int H;
	int sStride;
	int dStride;
	int *Horizontal;
	int *Vertical;
	int *Temporal;
	
};



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

void CFFmpegDenoise::deNoise(unsigned short * Frame,  // mpi->planes[x]
	unsigned char * FrameDest,    // dmpi->planes[x]
	unsigned int *LineAnt,       // vf->priv->Line (width bytes)
	unsigned short *FrameAnt,
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
		unsigned short* LinePrev = &FrameAnt[Y*W];
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


CFFmpegDenoise::CFFmpegDenoise()
{

}

CFFmpegDenoise::~CFFmpegDenoise()
{

}

#define rgbtoy(b, g, r, y) \
    y=(unsigned char)(((int)(30*r) + (int)(59*g) + (int)(11*b))/100)

#define rgbtoyuv(b, g, r, y, u, v) \
    rgbtoy(b, g, r, y); \
    u=(unsigned char)(((int)(-17*r) - (int)(33*g) + (int)(50*b)+12800)/100); \
    v=(unsigned char)(((int)(50*r) - (int)(42*g) - (int)(8*b)+12800)/100)

static void RGBtoYUV420PSameSize(const unsigned char * rgb,
	unsigned char * yplane, unsigned char * uplane, unsigned char * vplane,
	unsigned rgbIncrement,
	unsigned char flip,
	int srcFrameWidth, int srcFrameHeight)
{
	unsigned int planeSize;
	unsigned int halfWidth;

	//unsigned char * yplane;
	//unsigned char * uplane;
	//unsigned char * vplane;
	const unsigned char * rgbIndex;

	int x, y;
	unsigned char * yline;
	unsigned char * uline;
	unsigned char * vline;

	planeSize = srcFrameWidth * srcFrameHeight;
	halfWidth = srcFrameWidth >> 1;

	// get pointers to the data
	//yplane = yuv;
	//uplane = yuv + planeSize;
	//vplane = yuv + planeSize + (planeSize >> 2);
	rgbIndex = rgb;

	for (y = 0; y < srcFrameHeight; y++)
	{
		yline = yplane + (y * srcFrameWidth);
		uline = uplane + ((y >> 1) * halfWidth);
		vline = vplane + ((y >> 1) * halfWidth);

		if (flip)
			rgbIndex = rgb + (srcFrameWidth*(srcFrameHeight - 1 - y)*rgbIncrement);

		for (x = 0; x < (int)srcFrameWidth; x += 2)
		{
			rgbtoyuv(rgbIndex[0], rgbIndex[1], rgbIndex[2], *yline, *uline, *vline);
			rgbIndex += rgbIncrement;
			yline++;
			rgbtoyuv(rgbIndex[0], rgbIndex[1], rgbIndex[2], *yline, *uline, *vline);
			rgbIndex += rgbIncrement;
			yline++;
			uline++;
			vline++;
		}
	}
}

CRegardsBitmap * CFFmpegDenoise::ApplyDenoise3D(double LumSpac, double ChromSpac, double LumTmp, double ChromTmp, CRegardsBitmap * bitmapIn)
{

	CRegardsBitmap * bitmapOut = new CRegardsBitmap(bitmapIn->GetBitmapWidth(), bitmapIn->GetBitmapHeight());



	if (LumSpac < 0)
		LumSpac = 4.0;
	if (ChromSpac < 0)
		ChromSpac = .75 * LumSpac;
	if (LumTmp < 0)
		LumTmp = 1.5 * LumSpac;
	if (ChromTmp < 0) {
		if (LumSpac == 0)
			ChromTmp = ChromSpac * 1.5;
		else
			ChromTmp = LumTmp * ChromSpac / LumSpac;
	}
	LumSpac = __min(254.9, LumSpac);
	ChromSpac = __min(254.9, ChromSpac);
	LumTmp = __min(254.9, LumTmp);
	ChromTmp = __min(254.9, ChromTmp);

	if (restart_lap < 0)
		restart_lap = __max(2, (int)(1 + __max(LumTmp, ChromTmp)));

	PrecalcCoefs(Coefs[0], LumSpac);
	PrecalcCoefs(Coefs[1], LumTmp);
	PrecalcCoefs(Coefs[2], ChromSpac);
	PrecalcCoefs(Coefs[3], ChromTmp);

	w = bitmapIn->GetBitmapWidth();
	h = bitmapIn->GetBitmapHeight();
	cw = w >> 1;
	ch = h >> 1;


	unsigned short * picture_y = new unsigned short[bitmapIn->GetBitmapWidth() * bitmapIn->GetBitmapHeight()];
	unsigned short * picture_u = new unsigned short[bitmapIn->GetBitmapWidth() * bitmapIn->GetBitmapHeight()];
	unsigned short * picture_v = new unsigned short[bitmapIn->GetBitmapWidth() * bitmapIn->GetBitmapHeight()];

	
	Frame[0] = new unsigned short[w*h];
	Frame[1] = new unsigned short[w*h];
	Frame[2] = new unsigned short[w*h];

	uint8_t * y_out = new uint8_t[w*h];
	uint8_t * u_out = new uint8_t[w*h];
	uint8_t * v_out = new uint8_t[w*h];

	uint8_t* dataIn = bitmapIn->GetPtBitmap();
	int posPicture = 0;
	int posLocal = 0;
	for (int posY = 0; posY < bitmapIn->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmapIn->GetBitmapWidth(); posX++)
		{
			picture_y[posLocal] = 0.257f * value[dataIn[posPicture + 2]] + 0.504f * value[dataIn[posPicture + 1]] + 0.098f * value[dataIn[posPicture]] + 16;
			picture_v[posLocal] = (0.439 * dataIn[posPicture + 2]) - (0.368 * dataIn[posPicture + 1]) - (0.071 * dataIn[posPicture]) + 128;
			picture_u[posLocal] = -(0.148 * dataIn[posPicture + 2]) - (0.291 * dataIn[posPicture + 1]) + (0.439 * dataIn[posPicture]) + 128;

			posLocal++;
			posPicture += 4;
		}
	}


	unsigned int * Line = new unsigned int[w];
#ifdef USE_TBB_ONLY
	std::vector<myDenoisetask> tasks;
	tasks.push_back(myDenoisetask(this, picture_y,
		y_out,
		Frame[0], w, h,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[0], Coefs[0], Coefs[1]));

	tasks.push_back(myDenoisetask(this, picture_u,
		u_out,
		Frame[1], w, h,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[2], Coefs[2], Coefs[3]));

	tasks.push_back(myDenoisetask(this, picture_v,
		v_out,
		Frame[2], w, h,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[2], Coefs[2], Coefs[3]));


	tbb::parallel_for(
		tbb::blocked_range<size_t>(0, tasks.size()),
		[&tasks](const tbb::blocked_range<size_t>& r)
	{
		for (size_t i = r.begin(); i < r.end(); ++i)
			tasks[i]();
	}
	);
	
#else
	
	deNoise(picture_y,
		y_out,
		Line, Frame[0], w, h,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[0], Coefs[0], Coefs[1]);

	deNoise(picture_u,
		u_out,
		Line, Frame[1], cw, ch,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[2], Coefs[2], Coefs[3]);

	deNoise(picture_v,
		v_out,
		Line, Frame[2],cw, ch,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[2], Coefs[2], Coefs[3]);
	

#endif
	posLocal = 0;
	posPicture = 0;
	uint8_t* data = bitmapOut->GetPtBitmap();

	for (int posY = 0; posY < bitmapOut->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmapOut->GetBitmapWidth(); posX++)
		{
			float Y = y_out[posLocal];
			//float V = v_out[posLocal];
			//float U = u_out[posLocal];
			float V = (0.439 * dataIn[posPicture + 2]) - (0.368 * dataIn[posPicture + 1]) - (0.071 * dataIn[posPicture]) + 128;
			float U = -(0.148 * dataIn[posPicture + 2]) - (0.291 * dataIn[posPicture + 1]) + (0.439 * dataIn[posPicture]) + 128;


			double B = 1.164 * (Y - 16) + 2.018 * (U - 128);
			double G = 1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128);
			double R = 1.164 * (Y - 16) + 1.596 * (V - 128);

			data[posPicture + 2] = min(max(R,0.0),255.0);//min(Y + 1.13983f * V, 255.0f); //R
			data[posPicture + 1] = min(max(G, 0.0), 255.0);//min(Y - 0.39465f * U - 0.5806f * V, 255.0f); //G
			data[posPicture] = min(max(B, 0.0), 255.0);//min(Y + 2.03211f * U, 255.0f); //B
			posPicture += 4;
			posLocal++;
		}
	}

	delete[] picture_y;
	delete[] picture_u;
	delete[] picture_v;

	delete[] Frame[0];
	delete[] Frame[1];
	delete[] Frame[2];

	delete[] y_out;
	delete[] u_out;
	delete[] v_out;

	delete[] Line;

	return bitmapOut;
}