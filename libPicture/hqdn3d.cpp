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
#define HQDN3D_SPATIAL_LUMA_DEFAULT    4.0f
#define HQDN3D_SPATIAL_CHROMA_DEFAULT  3.0f
#define HQDN3D_TEMPORAL_LUMA_DEFAULT   6.0f

extern float value[256];

#define ABS(A) ( (A) > 0 ? (A) : -(A) )

unsigned int Chqdn3d::hqdn3d_lowpass_mul(int prev_mul,
	int curr_mul,
	short * coef)
{
	int d = (prev_mul - curr_mul) >> 4;
	return curr_mul + coef[d];
}

void Chqdn3d::hqdn3d_precalc_coef(short * ct,
	double dist25)
{
	int i;
	double gamma, simil, c;

	gamma = log(0.25) / log(1.0 - min(dist25, 252.0) / 255.0 - 0.00001);

	for (i = -255 * 16; i <= 255 * 16; i++)
	{
		/* hqdn3d_lowpass_mul() truncates (not rounds) the diff, use +15/32 as midpoint */
		double f = (i + 15.0 / 32.0) / 16.0;
		simil = 1.0 - ABS(f) / 255.0;
		c = pow(simil, gamma) * 256.0 * f;
		ct[16 * 256 + i] = (c < 0) ? (c - 0.5) : (c + 0.5);
	}

	ct[0] = (dist25 != 0);
}

void Chqdn3d::hqdn3d_denoise_temporal(unsigned char * frame_src,
	unsigned char * frame_dst,
	unsigned short * frame_ant,
	int w, int h,
	short * temporal)
{
	int x, y;
	unsigned int tmp;

	temporal += 0x1000;

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x],
				frame_src[x] << 8,
				temporal);
			frame_dst[x] = (tmp + 0x7F) >> 8;
		}

		frame_src += w;
		frame_dst += w;
		frame_ant += w;
	}
}



void Chqdn3d::hqdn3d_denoise_spatial(unsigned char * frame_src,
	unsigned char * frame_dst,
	unsigned short * line_ant,
	unsigned short * frame_ant,
	int w, int h,
	short * spatial,
	short * temporal)
{
	int x, y;
	unsigned int pixel_ant;
	unsigned int tmp;

	spatial += 0x1000;
	temporal += 0x1000;

	/* First line has no top neighbor. Only left one for each tmp and last frame */
	pixel_ant = frame_src[0] << 8;

	for (x = 0; x < w; x++)
	{
		line_ant[x] = tmp = pixel_ant = hqdn3d_lowpass_mul(pixel_ant,
			frame_src[x] << 8,
			spatial);
		frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x],
			tmp,
			temporal);
		frame_dst[x] = (tmp + 0x7F) >> 8;
	}

	for (y = 1; y < h; y++)
	{
		frame_src += w;
		frame_dst += w;
		frame_ant += w;
		pixel_ant = frame_src[0] << 8;

		for (x = 0; x < w - 1; x++)
		{
			line_ant[x] = tmp = hqdn3d_lowpass_mul(line_ant[x],
				pixel_ant,
				spatial);
			pixel_ant = hqdn3d_lowpass_mul(pixel_ant,
				frame_src[x + 1] << 8,
				spatial);
			frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x],
				tmp,
				temporal);
			frame_dst[x] = (tmp + 0x7F) >> 8;
		}
		line_ant[x] = tmp = hqdn3d_lowpass_mul(line_ant[x],
			pixel_ant,
			spatial);
		frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x],
			tmp,
			temporal);
		frame_dst[x] = (tmp + 0x7F) >> 8;
	}
}

void Chqdn3d::hqdn3d_denoise(unsigned char * frame_src,
	unsigned char * frame_dst,
	unsigned short * line_ant,
	unsigned short ** frame_ant_ptr,
	int w,
	int h,
	short * spatial,
	short * temporal)
{
	int x, y;
	unsigned short* frame_ant = (*frame_ant_ptr);

	if (!frame_ant)
	{
		unsigned char * src = frame_src;
		(*frame_ant_ptr) = frame_ant = (unsigned short *)malloc(w*h * sizeof(unsigned short));

		for (y = 0; y < h; y++, frame_src += w, frame_ant += w)
		{
			for (x = 0; x < w; x++)
			{
				frame_ant[x] = frame_src[x] << 8;
			}
		}
		frame_src = src;
		frame_ant = *frame_ant_ptr;
	}

	/* If no spatial coefficients, do temporal denoise only */
	if (spatial[0])
	{
		hqdn3d_denoise_spatial(frame_src,
			frame_dst,
			line_ant,
			frame_ant,
			w, h,
			spatial,
			temporal);
	}
	else
	{
		hqdn3d_denoise_temporal(frame_src,
			frame_dst,
			frame_ant,
			w, h,
			temporal);
	}
}


Chqdn3d::Chqdn3d(const int &w, const int &h, const double & LumSpac, const double & LumTmp)
{
	double spatial_luma = LumSpac;
	double temporal_luma = HQDN3D_TEMPORAL_LUMA_DEFAULT * spatial_luma / HQDN3D_SPATIAL_LUMA_DEFAULT;

	this->h = h;
	this->w = w;

	//if (restart_lap < 0)
	//	restart_lap = __max(2, (int)(1 + __max(LumTmp, ChromTmp)));

	hqdn3d_precalc_coef(hqdn3d_coef[0], spatial_luma);
	hqdn3d_precalc_coef(hqdn3d_coef[1], temporal_luma);

	picture_y = new uint8_t[w*h];
	y_out = new uint8_t[w*h];

	Frame = new unsigned short[w*h];
	Line = new  unsigned short[w];
}

Chqdn3d::~Chqdn3d()
{
	if (Line != nullptr)
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
	hqdn3d_denoise(picture_y, y_out, Line, &Frame, w, h, hqdn3d_coef[0], hqdn3d_coef[1]);
	/*
	deNoise(picture_y,
		y_out,
		Line, Frame, w, h,
		w,
		w,
		hqdn3d_coef[0], hqdn3d_coef[0], hqdn3d_coef[1]);
	*/
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
			posLocal++;
			posPicture += 4;
		}
	}

	hqdn3d_denoise(picture_y, y_out, Line, &Frame, w, h, hqdn3d_coef[0], hqdn3d_coef[1]);
	/*
	deNoise(picture_y,
		y_out,
		Line, Frame, w, h,
		bitmapIn->GetBitmapWidth(),
		bitmapIn->GetBitmapWidth(),
		Coefs[0], Coefs[0], Coefs[1]);
	*/
	posLocal = 0;
	posPicture = 0;
	uint8_t* data = bitmapOut->GetPtBitmap();

	for (int posY = 0; posY < bitmapOut->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmapOut->GetBitmapWidth(); posX++)
		{
			float Y = y_out[posLocal];
			float cr = (0.439 * data[posPicture + 2]) - (0.368 * data[posPicture + 1]) - (0.071 * data[posPicture]) + 128;
			float cb = -(0.148 * data[posPicture + 2]) - (0.291 * data[posPicture + 1]) + (0.439 * data[posPicture]) + 128;

			double B = 1.164 * (Y - 16) + 2.018 * (cb - 128);
			double G = 1.164 * (Y - 16) - 0.391 * (cb - 128) - 0.813 * (cr - 128);
			double R = 1.164 * (Y - 16) + 1.596 * (cr - 128);

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