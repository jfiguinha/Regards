#include <header.h>
#include "bm3dfilter.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <thread>
#include <RegardsBitmap.h>
#include "bm3d.h"

#define YUV       0
#define YCBCR     1
#define OPP       2
#define RGB       3
#define DCT       4
#define BIOR      5
#define HADAMARD  6
#define NONE      7

using namespace std;

extern float value[256];


int CBm3DFilter::DetermineData(const int &nbProcess)
{
	//! Check parameters
	tau_2D_hard = (strcmp(_tau_2D_hard, "dct") == 0 ? DCT :
		(strcmp(_tau_2D_hard, "bior") == 0 ? BIOR : NONE));

	tau_2D_wien = (strcmp(_tau_2D_wien, "dct") == 0 ? DCT :
		(strcmp(_tau_2D_wien, "bior") == 0 ? BIOR : NONE));

	color_space = (strcmp(_color_space, "rgb") == 0 ? RGB :
		(strcmp(_color_space, "yuv") == 0 ? YUV :
		(strcmp(_color_space, "ycbcr") == 0 ? YCBCR :
			(strcmp(_color_space, "opp") == 0 ? OPP : NONE))));

	patch_size = atoi(_patch_size);
	if (patch_size < 0)
	{
		cout << "The patch_size parameter must not be negative." << endl;
		return EXIT_FAILURE;
	}

	nb_threads = thread::hardware_concurrency();
	if (nb_threads < 0)
	{
		cout << "The nb_threads parameter must not be negative." << endl;
		return EXIT_FAILURE;
	}
      
    return 0;
}

CRegardsBitmap* CBm3DFilter::GetResult()
{
	return bitmap;
}

CBm3DFilter::CBm3DFilter(CRegardsBitmap * bitmapSource, const float &sigma): patch_size(0), nb_threads(0),
                                                                             tau_2D_wien(0),
                                                                             tau_2D_hard(0),
                                                                             color_space(0)
{
	this->bitmap = new CRegardsBitmap(bitmapSource->GetBitmapWidth(), bitmapSource->GetBitmapHeight());
	memcpy(this->bitmap->GetPtBitmap(), bitmapSource->GetPtBitmap(), bitmapSource->GetBitmapSize());
	this->fSigma = sigma;
}


CBm3DFilter::~CBm3DFilter()
{
	if (bitmap != nullptr)
		delete bitmap;
}

int CBm3DFilter::ExecuteFilter()
{
	vector<float> picture;
	vector<float> picturedenoised;
	vector<float> picturebasic;
	picture.resize(bitmap->GetBitmapHeight() * bitmap->GetBitmapWidth() * chnls, 0);
	picturedenoised.resize(bitmap->GetBitmapHeight() * bitmap->GetBitmapWidth() * chnls, 0);
	picturebasic.resize(bitmap->GetBitmapHeight() * bitmap->GetBitmapWidth() * chnls, 0);

	uint8_t* data = bitmap->GetPtBitmap();
	int posPicture = 0; 
	int posLocal = 0;
	for (int posY = 0; posY < bitmap->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmap->GetBitmapWidth(); posX++)
		{
			picture[posLocal++] = 0.257f * value[data[posPicture + 2]] + 0.504f * value[data[posPicture + 1]] + 0.098f * value[data[posPicture]] + 16;
			posPicture += 4;
		}
	}

	//! Denoising
	if (run_bm3d(fSigma, picture, picturebasic, picturedenoised, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), chnls,
		useSD_1, useSD_2, tau_2D_hard, tau_2D_wien, color_space, patch_size,
		nb_threads, verbose)
		!= EXIT_SUCCESS)
		return EXIT_FAILURE;

	posLocal = 0;
	posPicture = 0;
	for (int posY = 0; posY < bitmap->GetBitmapHeight(); posY++)
	{
		for (int posX = 0; posX < bitmap->GetBitmapWidth(); posX++)
		{
			float Y = picturedenoised[posLocal++];
			float V = (0.439 * data[posPicture + 2]) - (0.368 * data[posPicture + 1]) - (0.071 * data[posPicture]) + 128;
			float U = -(0.148 * data[posPicture + 2]) - (0.291 * data[posPicture + 1]) + (0.439 * data[posPicture]) + 128;

			float B = 1.164 * (Y - 16) + 2.018 * (U - 128);
			float G = 1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128);
			float R = 1.164 * (Y - 16) + 1.596 * (V - 128);

			data[posPicture + 2] = R;//min(Y + 1.13983f * V, 255.0f); //R
			data[posPicture + 1] = G;//min(Y - 0.39465f * U - 0.5806f * V, 255.0f); //G
			data[posPicture] = B;//min(Y + 2.03211f * U, 255.0f); //B
			posPicture += 4;
		}
	}

	return EXIT_SUCCESS;
}