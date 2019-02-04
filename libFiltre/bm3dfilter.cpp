#include "bm3dfilter.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string.h>
#include <thread>
#include <RegardsBitmap.h>
#include "bm3d.h"
#include "utilities.h"
#include "window_id.h"
#include <StatusText.h>

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

/**
* @file   main.cpp
* @brief  Main executable file. Do not use lib_fftw to
*         process DCT.
*
* @author MARC LEBRUN  <marc.lebrun@cmla.ens-cachan.fr>
*/

class DecodeBlocPicture
{
public:

	DecodeBlocPicture() {};
	~DecodeBlocPicture() {};

	int size;
	int chnls;
	int width;
	int height;
	int x;
	int y;
	CRegardsBitmap * bitmap;
	float fSigma;
	bool useSD_1;
	bool useSD_2;
	unsigned tau_2D_hard;
	unsigned tau_2D_wien;
	unsigned color_space;
	int patch_size;
	int nb_threads;
	bool verbose;
	int numloop;
	int nbloop;
    int numThread;
    wxDialog * dialog = nullptr;
};

void CBm3DFilter::DecodeFrame(void * dataToDecode)
{
	DecodeBlocPicture * block = (DecodeBlocPicture *)dataToDecode;
	int size = block->size;
	int chnls = block->chnls;
	int width = block->width;
	int height = block->height;
	int x = block->x;
	int y = block->y;
	CRegardsBitmap * bitmap = block->bitmap;
	int posLocal = 0;

	vector<float> picture;
	vector<float> picturedenoised;
	vector<float> picturebasic;
	picture.resize(size * size * chnls, 0);
	picturedenoised.resize(size * size * chnls, 0);
	picturebasic.resize(size * size * chnls, 0);
	//memset(&picture->data, 0, size * size * 4);
	int xMax = (x + 1) * size;
	if (xMax > width)
		xMax = (width - (x * size));
	else
		xMax = size;

	int yMax = (y + 1) * size;
	if (yMax > height)
		yMax = height - (y *size);
	else
		yMax = size;

	cout << "xMax = " << xMax << endl;
	cout << "yMax = " << yMax << endl;

	printf("Execution of loop %d of %d \n", block->numloop, block->nbloop);

	uint8_t * data = bitmap->GetPtBitmap();

	for (int posY = 0; posY < yMax; posY++)
	{
		for (int posX = 0; posX < xMax; posX++)
		{
			int posPicture = bitmap->GetPosition(posX + x * size, posY + y * size);
			//posPicture = ((posY + y * size) + (posX + x * size)) * chnls;
			for (int i = 0; i < chnls; i++)
			{
				int posLocal = (posY * size + posX) + i * size * size;
				
				//CRgbaquad color = bitmap->GetColorValue(posX + x * size, posY + y * size);
				picture[posLocal] = value[data[posPicture + i]];
			}
		}
	}

	//float fSigma = atof(argv[2]);


	//if (save_image("img_test_lo.png", picture, size, size, chnls) != EXIT_SUCCESS)
	//    return EXIT_FAILURE;
	/*
	exit(0);
	*/
	//! Denoising
	if (run_bm3d(block->fSigma, picture, picturebasic, picturedenoised, size, size, chnls,
		block->useSD_1, block->useSD_2, block->tau_2D_hard, block->tau_2D_wien, block->color_space, block->patch_size,
		block->nb_threads, block->verbose)
		!= EXIT_SUCCESS)
		return;

	for (int posY = 0; posY < yMax; posY++)
	{
		for (int posX = 0; posX < xMax; posX++)
		{
			int posPicture = bitmap->GetPosition(posX + x * size, posY + y * size);
			for (int i = 0; i < chnls; i++)
			{
				int posLocal = (posY * size + posX) + i * size * size;
				
				data[posPicture + i] = picturedenoised[posLocal];
			}
		}
	}
    
    if(block->dialog != nullptr)
    {
        wxCommandEvent * event = new wxCommandEvent(EVENT_ENDNEWPICTURETHREAD);
        event->SetInt(block->numThread);
        wxQueueEvent(block->dialog, event);
    }
	delete block;
    
}

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
    
	//! Declarations
	//vector<float> img_noisy, img_basic, img_denoised;
	
	width = bitmap->GetBitmapWidth();
	height = bitmap->GetBitmapHeight();

	//size = 256;//6;
				   //Découpage de l'image en bloc de 512x512
	
	int nbPictureX = width / size;
	int nbPictureY = height / size;
	if (nbPictureX * size < width)
		nbPictureX++;
	if (nbPictureY * size < height)
		nbPictureY++;

	int posPicture = 0;

	nbloop = nbPictureY * nbPictureX;
	numloop = 0;
	//float fSigma = atof(argv[2]);

	for (int y = 0; y < nbPictureY; y++)
	{
		for (int x = 0; x < nbPictureX; x++)
		{
			wxPoint posPicture;
			posPicture.x = x;
			posPicture.y = y;
			listPicture.push_back(posPicture);
		}
	}


	int _nbProcess = nbProcess;//thread::hardware_concurrency();;
    if(_nbProcess > nbloop)
    {
        _nbProcess = nbloop;
    }
    else
        nb_threads = 1;


	for (int j = 0; j < _nbProcess; j++)
	{
		listThread.push_back(nullptr);
	}
   
    return 0;
}

CBm3DFilter::CBm3DFilter(CRegardsBitmap * & bitmap, const float &sigma)
{
    this->bitmap = bitmap;
    this->fSigma = sigma;
}

int CBm3DFilter::GetNbLoop()
{
   return nbloop;
}

int CBm3DFilter::GetNumLoop()
{
    return numloop;
}

int CBm3DFilter::EndThread(const int &numThread)
{
    if (listThread[numThread] != nullptr)
    {
        listThread[numThread]->join();
        delete listThread[numThread];
        listThread[numThread] = nullptr;
    }
}

int CBm3DFilter::NextStep(wxDialog * dialog)
{
	//while (numloop < listPicture.size())
//	{
        /*
		for (int j = 0; j < nbProcess; j++)
		{
			if (listThread[j] != nullptr)
			{
				listThread[j]->join();
				delete listThread[j];
				listThread[j] = nullptr;
			}
		}
        */
		for (int j = 0; j < listThread.size(); j++)
		{
			if (listThread[j] == nullptr)
			{
				DecodeBlocPicture * block = new DecodeBlocPicture();
				block->size = size;
				block->chnls = chnls;
				block->width = width;
				block->height = height;
				block->x = listPicture[numloop].x;
				block->y = listPicture[numloop].y;
				block->bitmap = bitmap;
				block->fSigma = fSigma;
				block->useSD_1 = useSD_1;
				block->useSD_2 = useSD_2;
				block->tau_2D_hard = tau_2D_hard;
				block->tau_2D_wien = tau_2D_wien;
				block->color_space = color_space;
				block->patch_size = patch_size;
				block->nb_threads = thread::hardware_concurrency();// / listPicture.size();
				block->verbose = verbose;
				block->numloop = numloop;
				block->nbloop = nbloop;
                block->numThread = j;
                block->dialog = dialog;
				numloop++;

				listThread[j] = new thread(DecodeFrame, block);
			}
		}
	//}
}

CBm3DFilter::~CBm3DFilter()
{
	for (int j = 0; j < listThread.size(); j++)
	{
		if (listThread[j] != nullptr)
		{
			listThread[j]->join();
			delete listThread[j];
			listThread[j] = nullptr;
		}
	}
}

int CBm3DFilter::ExecuteFilter()
{
    while (numloop < listPicture.size())
    {
		for (int j = 0; j < listThread.size(); j++)
		{
			if (listThread[j] != nullptr)
			{
				listThread[j]->join();
				delete listThread[j];
				listThread[j] = nullptr;
			}
		}
        NextStep(nullptr);
    }

	return EXIT_SUCCESS;
}