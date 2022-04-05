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

inline unsigned int hqdn3d_lowpass_mul(int prev_mul, int curr_mul, short* coef)
{
	int d = (prev_mul - curr_mul) >> 4;
	return curr_mul + coef[d];
}

void Chqdn3d::hqdn3d_precalc_coef(short* ct,
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

void Chqdn3d::hqdn3d_denoise_temporal(unsigned char* frame_src,
                                      unsigned char* frame_dst,
                                      unsigned short* frame_ant,
                                      int w, int h,
                                      short* temporal)
{
	int x, y;
	unsigned int tmp;

	temporal += 0x1000;

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x], frame_src[x] << 8, temporal);
			frame_dst[x] = (tmp + 0x7F) >> 8;
		}

		frame_src += w;
		frame_dst += w;
		frame_ant += w;
	}
}


void Chqdn3d::hqdn3d_denoise_spatial(unsigned char* frame_src,
                                     unsigned char* frame_dst,
                                     unsigned short* line_ant,
                                     unsigned short* frame_ant,
                                     int w, int h,
                                     short* spatial,
                                     short* temporal)
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
		line_ant[x] = tmp = pixel_ant = hqdn3d_lowpass_mul(pixel_ant,frame_src[x] << 8,spatial);
		frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x],tmp,temporal);
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
			line_ant[x] = tmp = hqdn3d_lowpass_mul(line_ant[x],pixel_ant,spatial);
			pixel_ant = hqdn3d_lowpass_mul(pixel_ant,frame_src[x + 1] << 8,spatial);
			frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x],tmp,temporal);
			frame_dst[x] = (tmp + 0x7F) >> 8;
		}
		line_ant[x] = tmp = hqdn3d_lowpass_mul(line_ant[x],pixel_ant,spatial);
		frame_ant[x] = tmp = hqdn3d_lowpass_mul(frame_ant[x],tmp,temporal);
		frame_dst[x] = (tmp + 0x7F) >> 8;
	}
}

void Chqdn3d::hqdn3d_denoise(unsigned char* frame_src,
                             unsigned char* frame_dst,
                             unsigned short* line_ant,
                             unsigned short** frame_ant_ptr,
                             int w,
                             int h,
                             short* spatial,
                             short* temporal)
{
	int x, y;
	unsigned short* frame_ant = (*frame_ant_ptr);

	if (!frame_ant)
	{
		unsigned char* src = frame_src;
		(*frame_ant_ptr) = frame_ant = static_cast<unsigned short*>(malloc(w * h * sizeof(unsigned short)));

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
	hqdn3d_denoise_spatial(frame_src,
		frame_dst,
		line_ant,
		frame_ant,
		w, h,
		spatial,
		temporal);

}


Chqdn3d::Chqdn3d(const int& w, const int& h, const double& LumSpac, const double& LumTmp)
{
	double spatial_luma = LumSpac;
	double temporal_luma = HQDN3D_TEMPORAL_LUMA_DEFAULT * spatial_luma / HQDN3D_SPATIAL_LUMA_DEFAULT;

	this->h = h;
	this->w = w;

	//if (restart_lap < 0)
	//	restart_lap = __max(2, (int)(1 + __max(LumTmp, ChromTmp)));

	hqdn3d_precalc_coef(hqdn3d_coef[0], spatial_luma);
	hqdn3d_precalc_coef(hqdn3d_coef[1], temporal_luma);

	picture_y = new uint8_t[w * h];
	y_out = new uint8_t[w * h];

	Frame = new unsigned short[w * h];
	Line = new unsigned short[w];
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

uint8_t* Chqdn3d::ApplyDenoise3D(uint8_t* picture_y, const int& w, const int& h)
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

int Chqdn3d::ApplyDenoise3D(cv::Mat& bitmapIn)
{
	w = bitmapIn.size().width;
	h = bitmapIn.size().height;

	cv::Mat ycbcr;
	cv::Mat yChannel;

	cvtColor(bitmapIn, ycbcr, cv::COLOR_BGR2YCrCb);
	
	cv::extractChannel(ycbcr, yChannel, 0);

	memcpy(picture_y, yChannel.data, bitmapIn.cols * bitmapIn.rows);

	hqdn3d_denoise(picture_y, y_out, Line, &Frame, w, h, hqdn3d_coef[0], hqdn3d_coef[1]);

	memcpy(yChannel.data, y_out, bitmapIn.cols * bitmapIn.rows);

	// Merge the the color planes back into an Lab image
	cv::insertChannel(yChannel, ycbcr, 0);

	// convert back to RGB
	cv::cvtColor(ycbcr, bitmapIn, cv::COLOR_YCrCb2BGR);

	yChannel.release();
	ycbcr.release();

	return 0;
}
