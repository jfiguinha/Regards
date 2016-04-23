#pragma once
#include <RGBAQuad.h>
#include "RotateByShear.h"

//
// Sample implementation :
// -----------------------
// This implementation uses CRgbaquad for a pixel.
// This implementation fits the DIB data structure.
//
class CRotateByShearRGB : public CRotateByShear<CRgbaquad>
{
public: 

	static bool Progress (uint8_t bPercentComplete)
	{
		//printf ("%d percent completed...\r", bPercentComplete);
		return true;
		// If we return FALSE here, the rotation will be immediately aborted
	}

    CRotateByShearRGB (ProgressAnbAbortCallBack callback = nullptr) : 
        CRotateByShear<CRgbaquad> (callback) {}

    virtual ~CRotateByShearRGB() {}

    CRgbaquad GetRGB (CRgbaquad *pImage,      // Pointer to image
                     wxSize  sImage,      // Size of image
                     int  x,           // X coordinate
                     int  y            // Y coordinate
                    )
    {
        return pImage [x + y * sImage.x];
    }

        // Set RGB value at given pixel coordinates
    void SetRGB (CRgbaquad  *pImage,   // Pointer to image
		wxSize       sImage,   // Size of image
                 int       x,        // X coordinate
                 int       y,        // Y coordinate
                 CRgbaquad   clr       // New color to set
                )
    {
        pImage [x + y * sImage.x] = clr;
    }

        // Create a new bitmap, given a bitmap dimensions
    CRgbaquad *CreateNewBitmap (wxSize  sImage) // Size of image
    {
        return new CRgbaquad [sImage.x * sImage.y];
    }

        // Create a new bitmap which is an identical copy of the source bitmap
    CRgbaquad *CopyBitmap (CRgbaquad *pImage,     // Pointer to source image
                          wxSize      sImage      // Size of source (and destination) image
                         )
    {
        CRgbaquad *pDst =  CreateNewBitmap (sImage);
        if (nullptr != pDst)
        {
			memcpy(pDst, pImage, sizeof (CRgbaquad) * sImage.x * sImage.y);
        }
        return pDst;
    }          

        // Destroy a bitmap previously created in call to CreateNewBitmap(..)
        // or to CopyBitmap (...)
    void DestroyBitmap (CRgbaquad *pImage,   // Pointer to image
                        wxSize                // Size of image
                       )
    {
        delete [] pImage;
    }

};

