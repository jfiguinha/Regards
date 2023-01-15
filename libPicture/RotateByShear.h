#pragma once

#ifndef _PROGRESS_AND_ABORT_CALL_BACK
#define _PROGRESS_AND_ABORT_CALL_BACK

using ProgressAnbAbortCallBack = bool(*)(uint8_t bPercentComplete);

#endif  // _PROGRESS_AND_ABORT_CALL_BACK

template <class CPxl>
class CRotateByShear
{
public:
	CRotateByShear(ProgressAnbAbortCallBack callback = nullptr) :
		m_Callback(callback)
	{
	}

	virtual ~CRotateByShear()
	{
	}

	CPxl* AllocAndRotate(
		CPxl* pSrc, // Pointer to source image
		wxSize sSrc, // Size of source image
		float dAngle, // Rotation angle
		wxSize* psDst, // Pointer to destination image size
		CRgbaquad clrBack); // Background color

protected:
	//
	// You must virtualthese 5 function in the derived class and implement 
	// specific pixel access functions according to the bitmap type you use.
	//

	// Get RGB value at given pixel coordinates
	virtual CRgbaquad GetRGB(CPxl* pImage, // Pointer to image
	                         wxSize sImage, // Size of image
	                         int x, // X coordinate
	                         int y // Y coordinate
	) = 0;

	// Set RGB value at given pixel coordinates
	virtual void SetRGB(CPxl* pImage, // Pointer to image
	                    wxSize sImage, // Size of image
	                    int x, // X coordinate
	                    int y, // Y coordinate
	                    CPxl clr // New color to set
	) = 0;

	// Create a new bitmap, given a bitmap dimensions
	virtual CPxl* CreateNewBitmap(wxSize sImage // Size of image
	) = 0;

	// Create a new bitmap which is an identical copy of the source bitmap
	virtual CPxl* CopyBitmap(CPxl* pImage, // Pointer to source image
	                         wxSize sImage // Size of source (and destination) image
	) = 0;

	// Destroy a bitmap previously created in call to CreateNewBitmap(..)
	// or to CopyBitmap (...)
	virtual void DestroyBitmap(CPxl* pImage, // Pointer to image
	                           wxSize sImage // Size of image
	) = 0;

private:
	ProgressAnbAbortCallBack m_Callback;

	void HorizSkew(CPxl* pSrc,
	               wxSize sSrc,
	               CPxl* pDst,
	               wxSize sDst,
	               int uRow,
	               int iOffset,
	               float dWeight,
	               CRgbaquad clrBack);

	void VertSkew(CPxl* pSrc,
	              wxSize sSrc,
	              CPxl* pDst,
	              wxSize sDst,
	              int uCol,
	              int iOffset,
	              float dWeight,
	              CRgbaquad clrBack);

	CPxl* Rotate90(CPxl* pSrc, wxSize sSrc, wxSize* psDst);
	CPxl* Rotate180(CPxl* pSrc, wxSize sSrc, wxSize* psDst);
	CPxl* Rotate270(CPxl* pSrc, wxSize sSrc, wxSize* psDst);

	CPxl* Rotate45(
		CPxl* pSrc,
		wxSize sSrc,
		wxSize* psDst,
		float dAngle,
		CRgbaquad clrBack,
		bool bMidImage);
}; // CRotateByShear


#ifdef ROTATE_PI
#undef ROTATE_PI
#endif // ROTATE_PI

#define ROTATE_PI  float (3.1415926535897932384626433832795)


template <class CPxl>
void
CRotateByShear<CPxl>::HorizSkew(
	CPxl* pSrc,
	wxSize sSrc,
	CPxl* pDst,
	wxSize sDst,
	int uRow,
	int iOffset,
	float dWeight,
	CRgbaquad clrBack)
/*------------------------------------------------------------------------------

  Function: HorizSkew

  Purpose:  Skews a row horizontally (with filtered weights)

  Input:    Image to skew (+ dimensions)
            Destination of skewed image (+ dimensions)
            Row index
            Skew offset
            Relative weight of right pixel
            Background color

  Output:   None.

  Remarks:  Limited to 45 degree skewing only. Filters two adjacent pixels.

------------------------------------------------------------------------------*/
{
	for (auto i = 0; i < iOffset; i++)
	{
		// Fill gap left of skew with background
		SetRGB(pDst, sDst, i, uRow, clrBack);
	}

	CRgbaquad pxlOldLeft = clrBack;
	int i = 0;
	for (i = 0; i < sSrc.x; i++)
	{
		// Loop through row pixels
		CRgbaquad pxlSrc = GetRGB(pSrc, sSrc, i, uRow);
		// Calculate weights
		CRgbaquad pxlLeft = pxlSrc;
		pxlLeft.Mul(dWeight);

		CRgbaquad temp = pxlLeft;
		temp.Sub(pxlOldLeft);
		pxlSrc.Sub(temp);

		// Check boundries 
		if ((i + iOffset >= 0) && (i + iOffset < sDst.x))
		{
			SetRGB(pDst, sDst, i + iOffset, uRow, pxlSrc);
		}
		// Save leftover for next pixel in scan
		pxlOldLeft = pxlLeft;
	}
	// Go to rightmost point of skew
	i += iOffset;
	if (i < sDst.x)
	{
		// If still in image bounds, put leftovers there
		SetRGB(pDst, sDst, i, uRow, pxlOldLeft);
	}
	while (++i < sDst.x)
	{
		// Clear to the right of the skewed line with background
		SetRGB(pDst, sDst, i, uRow, clrBack);
	}
} // CRotateByShear::HorizSkew


template <class CPxl>
void
CRotateByShear<CPxl>::VertSkew(
	CPxl* pSrc,
	wxSize sSrc,
	CPxl* pDst,
	wxSize sDst,
	int uCol,
	int iOffset,
	float dWeight,
	CRgbaquad clrBack)
/*------------------------------------------------------------------------------

  Function: VertSkew

  Purpose:  Skews a column vertically (with filtered weights)

  Input:    Image to skew (+dimensions)
            Destination of skewed image (+dimensions)
            Column index
            Skew offset
            Relative weight of upper pixel
            Background color

  Output:   None.

  Remarks:  Limited to 45 degree skewing only. Filters two adjacent pixels.

------------------------------------------------------------------------------*/
{
	for (auto i = 0; i < iOffset; i++)
	{
		// Fill gap above skew with background
		SetRGB(pDst, sDst, uCol, i, clrBack);
	}

	CRgbaquad pxlOldLeft = clrBack;

	int iYPos;
	for (auto i = 0; i < sSrc.y; i++)
	{
		// Loop through column pixels
		CRgbaquad pxlSrc = GetRGB(pSrc, sSrc, uCol, i);
		iYPos = i + iOffset;

		CRgbaquad pxlLeft = pxlSrc;
		pxlLeft.Mul(dWeight);

		CRgbaquad temp = pxlLeft;
		temp.Sub(pxlOldLeft);
		pxlSrc.Sub(temp);

		// Check boundries
		if ((iYPos >= 0) && (iYPos < sDst.y))
		{
			SetRGB(pDst, sDst, uCol, iYPos, pxlSrc);
		}
		// Save leftover for next pixel in scan
		pxlOldLeft = pxlLeft;
	}
	// Go to bottom point of skew
	int i = iYPos;
	if (i < sDst.y)
	{
		// If still in image bounds, put leftovers there
		SetRGB(pDst, sDst, uCol, i, pxlOldLeft);
	}
	while (++i < sDst.y)
	{
		// Clear below skewed line with background
		SetRGB(pDst, sDst, uCol, i, clrBack);
	}
} // CRotateByShear::VertSkew


template <class CPxl>
CPxl*
CRotateByShear<CPxl>::Rotate90(CPxl* pSrc, wxSize sSrc, wxSize* psDst)
/*------------------------------------------------------------------------------

  Function: Rotate90

  Purpose:  Rotates an image by 90 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{
	(*psDst).x = sSrc.y;
	(*psDst).y = sSrc.x;

	CPxl* pDst = CreateNewBitmap(*psDst);
	if (nullptr == pDst)
	{
		return nullptr;
	}
	for (auto uY = 0; uY < sSrc.y; uY++)
	{
		for (auto uX = 0; uX < sSrc.x; uX++)
		{
			SetRGB(pDst, *psDst, uY, (*psDst).y - uX - 1, GetRGB(pSrc, sSrc, uX, uY));
		}
		if (m_Callback)
		{
			// Report progress
			if (!m_Callback(
				static_cast<uint8_t>(static_cast<float>(uY) / static_cast<float>(sSrc.y) * static_cast<float>(50.0))))
			{
				// Operation cancelled
				DestroyBitmap(pDst, *psDst);
				return nullptr;
			}
		}
	}
	return pDst;
} // CRotateByShear::Rotate90

template <class CPxl>
CPxl*
CRotateByShear<CPxl>::Rotate180(CPxl* pSrc, wxSize sSrc, wxSize* psDst)
/*------------------------------------------------------------------------------

  Function: Rotate180

  Purpose:  Rotates an image by 180 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{
	*psDst = sSrc;

	CPxl* pDst = CreateNewBitmap(*psDst);
	if (nullptr == pDst)
	{
		return nullptr;
	}

	for (auto uY = 0; uY < sSrc.y; uY++)
	{
		for (auto uX = 0; uX < sSrc.x; uX++)
		{
			SetRGB(pDst, *psDst, (*psDst).x - uX - 1, (*psDst).y - uY - 1, GetRGB(pSrc, sSrc, uX, uY));
		}
		if (m_Callback)
		{
			// Report progress
			if (!m_Callback(
				static_cast<uint8_t>(static_cast<float>(uY) / static_cast<float>(sSrc.y) * static_cast<float>(50.0))))
			{
				// Operation cancelled
				DestroyBitmap(pDst, *psDst);
				return nullptr;
			}
		}
	}
	return pDst;
} // CRotateByShear::Rotate180

template <class CPxl>
CPxl*
CRotateByShear<CPxl>::Rotate270(CPxl* pSrc, wxSize sSrc, wxSize* psDst)
/*------------------------------------------------------------------------------

  Function: Rotate270

  Purpose:  Rotates an image by 270 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{
	(*psDst).x = sSrc.y;
	(*psDst).y = sSrc.x;

	CPxl* pDst = CreateNewBitmap(*psDst);
	if (nullptr == pDst)
	{
		return nullptr;
	}

	for (auto uY = 0; uY < sSrc.y; uY++)
	{
		for (auto uX = 0; uX < sSrc.x; uX++)
		{
			SetRGB(pDst, *psDst, (*psDst).x - uY - 1, uX, GetRGB(pSrc, sSrc, uX, uY));
		}
		if (m_Callback)
		{
			// Report progress
			if (!m_Callback(
				static_cast<uint8_t>(static_cast<float>(uY) / static_cast<float>(sSrc.y) * static_cast<float>(50.0))))
			{
				// Operation cancelled
				DestroyBitmap(pDst, *psDst);
				return nullptr;
			}
		}
	}
	return pDst;
} // CRotateByShear::Rotate270

template <class CPxl>
CPxl*
CRotateByShear<CPxl>::Rotate45(
	CPxl* pSrc,
	wxSize sSrc,
	wxSize* psDst,
	float dAngle,
	CRgbaquad clrBack,
	bool bMidImage)
/*------------------------------------------------------------------------------

  Function: Rotate45

  Purpose:  Rotates an image by a given degree in range [-45.0 .. +45.0]
            (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size
            Degree of rotation
            Background color
            Was middle image used (for correct progress report)

  Output:   Pointer to newly allocated rotated image

  Remarks:  Using the 3-shear technique.

------------------------------------------------------------------------------*/
{
	if (0.0 == dAngle)
	{
		// No rotation at all
		(*psDst) = sSrc;
		return CopyBitmap(pSrc, sSrc);
	}

	float dRadAngle = dAngle * ROTATE_PI / static_cast<float>(180); // Angle in radians
	float dSinE = sin(dRadAngle);
	float dTan = tan(dRadAngle / 2.0);

	// Calc first shear (horizontal) destination image dimensions 
	wxSize sDst1;
	sDst1.x = sSrc.x + static_cast<int>(static_cast<float>(sSrc.y) * fabs(dTan));
	sDst1.y = sSrc.y;


	/******* Perform 1st shear (horizontal) ******/

	// Allocate image for 1st shear
	CPxl* pDst1 = CreateNewBitmap(sDst1);
	if (nullptr == pDst1)
	{
		return nullptr;
	}
	for (auto u = 0; u < sDst1.y; u++)
	{
		float dShear;

		if (dTan >= 0.0)
		{
			// Positive angle
			dShear = (static_cast<float>(u) + 0.5) * dTan;
		}
		else
		{
			// Negative angle
			dShear = (static_cast<float>(u - sDst1.y) + 0.5) * dTan;
		}
		int iShear = static_cast<int>(floor(dShear));
		HorizSkew(pSrc,
		          sSrc,
		          pDst1,
		          sDst1,
		          u,
		          iShear,
		          dShear - static_cast<float>(iShear),
		          clrBack);
		if (m_Callback)
		{
			// Report progress
			uint8_t bProgress = bMidImage
				                    ? 50 + static_cast<uint8_t>(static_cast<float>(u) / static_cast<float>(sDst1.y) *
					                    static_cast<float>(50.0 / 3.0))
				                    : static_cast<uint8_t>(static_cast<float>(u) / static_cast<float>(sDst1.y) *
					                    static_cast<float>(33.33333));
			if (!m_Callback(bProgress))
			{
				// Operation cancelled
				DestroyBitmap(pDst1, sDst1);
				return nullptr;
			}
		}
	}

	/******* Perform 2nd shear  (vertical) ******/

	// Calc 2nd shear (vertical) destination image dimensions
	wxSize sDst2;
	sDst2.x = sDst1.x;
	sDst2.y = static_cast<int>(static_cast<float>(sSrc.x) * fabs(dSinE) + static_cast<float>(sSrc.y) * cos(dRadAngle)) +
		1;
	// Allocate image for 2nd shear
	CPxl* pDst2 = CreateNewBitmap(sDst2);
	if (nullptr == pDst2)
	{
		DestroyBitmap(pDst1, sDst1);
		return nullptr;
	}

	float dOffset; // Variable skew offset
	if (dSinE > 0.0)
	{
		// Positive angle
		dOffset = static_cast<float>(sSrc.x - 1) * dSinE;
	}
	else
	{
		// Negative angle
		dOffset = -dSinE * static_cast<float>(sSrc.x - sDst2.x);
	}

	for (long u = 0; u < sDst2.x; u++, dOffset -= dSinE)
	{
		int iShear = static_cast<int>(floor(dOffset));
		VertSkew(pDst1,
		         sDst1,
		         pDst2,
		         sDst2,
		         u,
		         iShear,
		         dOffset - static_cast<float>(iShear),
		         clrBack);
		if (m_Callback)
		{
			// Report progress
			uint8_t bProgress = bMidImage
				                    ? 66 + static_cast<uint8_t>(static_cast<float>(u) / static_cast<float>(sDst2.y) *
					                    static_cast<float>(50.0 / 3.0))
				                    : 33 + static_cast<uint8_t>(static_cast<float>(u) / static_cast<float>(sDst2.y) *
					                    static_cast<float>(33.33333));
			if (!m_Callback(bProgress))
			{
				// Operation cancelled
				DestroyBitmap(pDst1, sDst1);
				DestroyBitmap(pDst2, sDst2);
				return nullptr;
			}
		}
	}

	/******* Perform 3rd shear (horizontal) ******/

	// Free result of 1st shear
	DestroyBitmap(pDst1, sDst1);

	// Calc 3rd shear (horizontal) destination image dimensions
	(*psDst).x = static_cast<int>(static_cast<float>(sSrc.y) * fabs(dSinE) + static_cast<float>(sSrc.x) *
		cos(dRadAngle)) + 1;
	(*psDst).y = sDst2.y;

	// Allocate image for 3rd shear
	CPxl* pDst3 = CreateNewBitmap(*psDst);

	if (nullptr == pDst3)
	{
		DestroyBitmap(pDst2, sDst2);
		return nullptr;
	}

	if (dSinE >= 0.0)
	{
		// Positive angle
		dOffset = static_cast<float>(sSrc.x - 1) * dSinE * -dTan;
	}
	else
	{
		// Negative angle
		dOffset = dTan * (static_cast<float>(sSrc.x - 1) * -dSinE + static_cast<float>(1 - (*psDst).y));
	}
	for (auto u = 0; u < (*psDst).y; u++, dOffset += dTan)
	{
		int iShear = static_cast<int>(floor(dOffset));
		HorizSkew(pDst2,
		          sDst2,
		          pDst3,
		          (*psDst),
		          u,
		          iShear,
		          dOffset - static_cast<float>(iShear),
		          clrBack
		);
		if (m_Callback)
		{
			// Report progress
			uint8_t bProgress = bMidImage
				                    ? 83 + static_cast<uint8_t>(static_cast<float>(u) / static_cast<float>((*psDst).y) *
					                    static_cast<float>(50.0 / 3.0))
				                    : 66 + static_cast<uint8_t>(static_cast<float>(u) / static_cast<float>((*psDst).y) *
					                    static_cast<float>(33.33333));
			if (!m_Callback(bProgress))
			{
				// Operation cancelled
				DestroyBitmap(pDst2, sDst2);
				DestroyBitmap(pDst3, *psDst);
				return nullptr;
			}
		}
	}
	// Free result of 2nd shear    
	DestroyBitmap(pDst2, sDst2);
	// Return result of 3rd shear
	return pDst3;
} // CRotateByShear::Rotate45


template <class CPxl>
CPxl*
CRotateByShear<CPxl>::AllocAndRotate(
	CPxl* pSrc,
	wxSize sSrc,
	float dAngle,
	wxSize* psDst,
	CRgbaquad clrBack
)
/*------------------------------------------------------------------------------

  Function: AllocAndRotate

  Purpose:  Rotates an image by a given degree

  Input:    Image to rotate (+dimensions)
            Angle of rotation
            Pointers to dimensions of rotated image
            Background color

  Output:   Pointer to newly allocated rotated image

  Remarks:  Angle is unlimited. 3-shears technique is used.

------------------------------------------------------------------------------*/
{
	CPxl* pMidImg = pSrc;
	wxSize sMidImg = sSrc;

	if (nullptr == pSrc)
	{
		return nullptr;
	}

	while (dAngle >= 360.0)
	{
		// Bring angle to range of (-INF .. 360.0)
		dAngle -= 360.0;
	}
	while (dAngle < 0.0)
	{
		// Bring angle to range of [0.0 .. 360.0) 
		dAngle += 360.0;
	}
	if ((dAngle > 45.0) && (dAngle <= 135.0))
	{
		// Angle in (45.0 .. 135.0] 
		// Rotate image by 90 degrees into temporary image,
		// so it requires only an extra rotation angle 
		// of -45.0 .. +45.0 to complete rotation.
		pMidImg = Rotate90(pSrc, sSrc, &sMidImg);
		dAngle -= 90.0;
	}
	else if ((dAngle > 135.0) && (dAngle <= 225.0))
	{
		// Angle in (135.0 .. 225.0] 
		// Rotate image by 180 degrees into temporary image,
		// so it requires only an extra rotation angle 
		// of -45.0 .. +45.0 to complete rotation.
		pMidImg = Rotate180(pSrc, sSrc, &sMidImg);
		dAngle -= 180.0;
	}
	else if ((dAngle > 225.0) && (dAngle <= 315.0))
	{
		// Angle in (225.0 .. 315.0] 
		// Rotate image by 270 degrees into temporary image,
		// so it requires only an extra rotation angle 
		// of -45.0 .. +45.0 to complete rotation.
		pMidImg = Rotate270(pSrc, sSrc, &sMidImg);
		dAngle -= 270.0;
	}

	// If we got here, angle is in (-45.0 .. +45.0]

	if (nullptr == pMidImg)
	{
		// Failed to allocate middle image
		return nullptr;
	}

	CPxl* pDst = Rotate45(pMidImg,
	                      sMidImg,
	                      psDst,
	                      dAngle,
	                      clrBack,
	                      pSrc != pMidImg);

	if (pSrc != pMidImg)
	{
		// Middle image was required, free it now.
		DestroyBitmap(pMidImg, sMidImg);
	}
	return pDst;
} // AllocAndRotate
