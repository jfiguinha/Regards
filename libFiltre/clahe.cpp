#include "clahe.h"
#include <RegardsBitmap.h>
#include <math.h>

using namespace Regards::FiltreEffet;

CClahe::CClahe()
{
}

CClahe::~CClahe()
{
}

double CClahe::round(double d)
{
  return floor(d + 0.5);
}

CRegardsBitmap * CClahe::ClaheFilter(CRegardsBitmap * image_in, int nBins, float clipLevel, int windowSize)
{
	//float alpha;

	this->nBins = nBins;
	this->clipLevel =clipLevel;
	this->windowSize = windowSize; // to be decided later based on thw window size

	w = (int)image_in->GetBitmapWidth();
	h = (int)image_in->GetBitmapHeight();
  
	if (windowSize == -1) {
		this->windowSize = (int)round(((w < h) ? w : h) / 25);
	}

	range = 256.f;
	binSize = ceil(range / this->nBins);
	

	return Clahe(image_in);
}

inline void CClahe::ClaheChannel(int * count, int channel, int pos, uint8_t * colorPt, uint8_t * colorPtOut, int * * bin, CRegardsBitmap * image_in, int i, int j)
{
	float* binFreq = new float[nBins];

	int position = pos + channel;

	float pixVal = colorPt[position]; 

	// create the histogram
	histogram(image_in, i, j, channel, count[channel], bin[channel], binFreq);	  

	// find the bin location for the center pixel
	int binLoc =(int)(pixVal / binSize);

	// to accomodate the max value in the range
	if (binLoc == nBins)
		binLoc--;

	if (binLoc > (nBins - 1)) {
		//cout << "Error in calculating binLoc\n";
		throw("Error in calculating binLoc\n");
		//exit(-1);
	}	

	float cdf = 0.f;
	for (auto k = 0; k <= binLoc; k++) {
		cdf += binFreq[k];
	}

	float newPixVal = round(cdf * 255.f);

	//image_out->poke(i, j) = newPixVal;
	colorPtOut[position] = newPixVal;

	delete[] binFreq;
}

CRegardsBitmap * CClahe::Clahe(CRegardsBitmap * image_in) 
{
	CRegardsBitmap * image_out = new CRegardsBitmap(image_in->GetBitmapWidth(), image_in->GetBitmapHeight());

	uint8_t * colorPt = image_in->GetPtBitmap();
	uint8_t * colorPtOut = image_out->GetPtBitmap();

#pragma omp parallel for
	for (auto i = 0; i < w; i++) 
	{
		int count[4];
		int** bin = new int*[4];
		for(int k = 0;k < 4;k++)
			bin[k] = new int[nBins];

		memset(count, 0, sizeof(int) * 4);

		for(int k = 0;k < 4;k++)
			memset(bin[k], 0, sizeof(int) * nBins);

		for (auto j = 0; j < h; j++) 
		{
			int position = i * 4 + (j * w) * 4;
			
		#pragma omp parallel for
			for(int channel = 0;channel < 4;channel++)
			{
				ClaheChannel(count, channel, position, colorPt, colorPtOut, bin, image_in, i, j);
			}
		}

		for(int k = 0;k < 4;k++)
			delete[] bin[k];

		delete[] bin;
	}

	return image_out;
}  


void CClahe::histogram(CRegardsBitmap * bitmap, int origX,  int origY, int channel, int & count, int * bin, float * binFreq) 
{
	int startX = origX - (windowSize / 2);
	int startY = origY - (windowSize / 2);

	float pixVal;
	int a, b, binLoc; 

	if (origY == 0)
	{
		for (auto i = 0; i < windowSize; i++) 
		{
			for (auto j = 0; j < windowSize; j++)
			{
				a = startX + i;
				b = startY + j;

				if ((a >= 0 && a < w) && (b >= 0 && b < h))
				{
					uint8_t * color = bitmap->GetPtBitmap();
					pixVal = color[a * 4 + b * bitmap->GetBitmapWidth() * 4 + channel];  
				}
				else 
				{
					pixVal = 0.f; // has to be decided
				}

				// alternate logic, takes less iterations 
				binLoc = (int)(pixVal / binSize);

				// to accomodate the max value in the range
				if (binLoc == nBins)
					binLoc--;

				bin[binLoc] += 1;
				count++;
			}
		}
	}
	else
	{
		// remove the old row
		b = startY - 1;
		for (auto j = 0; j < windowSize; j++) 
		{
			a = startX + j;

			if ((a >= 0 && a < w) && (b >= 0 && b < h)) 
			{
				uint8_t * color = bitmap->GetPtBitmap();
				pixVal = color[a * 4 + b * bitmap->GetBitmapWidth() * 4 + channel]; 
				//pixVal = image.peek(a, b);  
			}
			else 
			{
				pixVal = 0.f; // has to be decided
			}
	
			binLoc = (int)(pixVal / binSize);

			// to accomodate the max value in the range
			if (binLoc == nBins)
				binLoc--;

			bin[binLoc] -= 1;
		}

		// add the new row
		b = startY + windowSize - 1;
		for (auto j = 0; j < windowSize; j++) 
		{
			a = startX + j;

			if ((a >= 0 && a < w) && (b >= 0 && b < h))
			{
				uint8_t * color = bitmap->GetPtBitmap();
				pixVal = color[a * 4 + b * bitmap->GetBitmapWidth() * 4 + channel]; 
				//pixVal = image.peek(a, b);  
			}
			else 
			{
				pixVal = 0.f; // has to be decided
			}
	
			binLoc = (int)(pixVal / binSize);

			// to accomodate the max value in the range
			if (binLoc == nBins)
				binLoc--;

			bin[binLoc] += 1;
		}
	}

	
	float extraAmount = 0.f;

	// calculating the relative frequency
	// and the total extra amount > clip level
	for (auto i = 0; i < nBins; i++)
	{
		binFreq[i] = ((float)bin[i] / (float)count);
		if (binFreq[i] > clipLevel)
		{
			extraAmount += (binFreq[i] - clipLevel);
			binFreq[i] = clipLevel;
		}
	}

	// adjusting the total extra amount
	if (extraAmount > 0.f) 
	{
		float extraAmountPerBin = float(extraAmount / nBins);
		for (auto i = 0; i < nBins; i++) {
			binFreq[i] += extraAmountPerBin;
		}
	}		     
}
