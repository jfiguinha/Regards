/***************************************************************************
 *   Copyright (C) 2005 by Johann Uhrmann                                  *
 *   johann.uhrmann@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "header.h"
#include "rotdetect.h"
extern "C"
{
	#include "detector.h"
}
#include <RegardsFloatBitmap.h>
#include "InterpolationFloatBicubic.h"


CRotDetect::rotation CRotDetect::analyzeResult(float * result)
{
	enum CRotDetect::tristate not, cw90, ccw90;

	
	if(result[0] > result[1] && result[0] > result[2])
		return NOT_ROTATED;
	if (result[1] > result[0] && result[1] > result[2])
		return ROTATED90CW;
	if (result[2] > result[0] && result[2] > result[1])
		return ROTATED90CCW;
	
	/* defuzzy result activations */
	not = defuzzy(result[0]);
	cw90 = defuzzy(result[1]);
	ccw90 = defuzzy(result[2]);

	/* check for clear results */
	if (not == VRAI && cw90 == FAUX && ccw90 == FAUX)
		return NOT_ROTATED;

	if (not == FAUX && cw90 == VRAI && ccw90 == FAUX)
		return ROTATED90CW;

	if (not== FAUX && cw90 == FAUX && ccw90 == VRAI)
		return ROTATED90CCW;

	/* no clear result -> give up */
	return UNKNOWN;
}


/*
	Internal Method that runs the artifical neuronal network

	Parameter:
		image 	- the already scaled down image for the ANN
		result	- pointer to the result unit activation array
		ratio	- the source image ratio
*/
void CRotDetect::runANN(CRegardsFloatBitmap *image, float *result, float ratio)
{
	/* Image pixel data in RGB order */
	//float pixels[1200];
	
	/* ANN input unit activations */
	float * inputUnits;
	
	int i;
	int c = 0;
	

//	bool status;
	//CRegardsFloatBitmap * bitmap = image->GetFloatBitmap(true);
	float * pixels = image->GetData();
	inputUnits = new float[image->GetWidth() * image->GetHeight() * 3 + 2];

	//FloatPixel

	//status = MagickGetImagePixels(image, 0, 0, 20, 20, "RGB", FloatPixel, pixels);
	//if (status == false)
	//	throwException(image);
	
	/* source image ratio */
	inputUnits[c++] = ratio;
	
	/* set red values */
	for (i = 0; i < image->GetWidth() * image->GetHeight(); ++i)
	{
		inputUnits[c++] = pixels[i * 4];
		inputUnits[c++] = pixels[i * 4 + 1];
		inputUnits[c++] = pixels[i * 4 + 2];
	}
	
	if (detector_h(inputUnits, result, 0))
	{
		//fprintf(stderr, "Failure in artifical neuronal network");
		throw("Failure in artifical neuronal network");
		//exit(EXIT_ANN_ERROR);
	}

	delete[] inputUnits;
}

void CRotDetect::rotdetect(CImageLoadingFormat *image, float *result, const bool &opt_verify)
{
	CRegardsFloatBitmap * annImage;
	//CRegardsFloatBitmap * outImage = new CRegardsFloatBitmap(20,20);
	//PixelWand * pwand;
	bool status;
	float verificationResult[3];
	float ratio;
	
	
	/* The source image should not be destroyed.
	   Therefore, we need a working copy.
	*/
	annImage = image->GetFloatBitmap();
	
	/* Scale down to neuronal network input size */
	//MagickResetIterator(annImage);
	//CInterpolationFloatLanczos interpolation(1.0);
	//interpolation.Execute(annImage, outImage);
	//MagickResizeImage(annImage,20,20,LanczosFilter,1.0);

	ratio = (float)annImage->GetWidth() / (float)annImage->GetHeight();
	runANN(annImage, result, ratio);
	
	/* If result verification is requested, then rotate the
	   scaled-down image and rerun the ANN.
	*/
	if (opt_verify)
	{
		/* rotate 90 degrees and verify */
		annImage->Rotate90();
		/*
		pwand = NewPixelWand();
		status = MagickRotateImage(annImage, pwand, 90);
		if (status == MagickFalse)
			throwException(annImage);
		*/
		runANN(annImage, verificationResult, 1.0 / ratio);
		
		/* add up results (note: indizes are shifted) */
		result[0] += verificationResult[1];
		result[2] += verificationResult[0];

		annImage->Rotate180();

		/* rotate another 180 degrees and verify */
		/*
		status = MagickRotateImage(annImage, pwand, 180);
		if (status == MagickFalse)
			throwException(annImage);
		*/
		
		runANN(annImage, verificationResult, 1.0 / ratio);
				
		/* add up results again */
		result[0] += verificationResult[2];
		result[1] += verificationResult[0];

		/* normalize the result */
		result[0] /= 3;
		result[1] /= 2;
		result[2] /= 2;

		//DestroyPixelWand(pwand);
	}

	delete annImage;
	//delete outImage;
	//DestroyMagickWand(annImage);
}
