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

#pragma once
#include <ImageLoadingFormat.h>


class CRotDetect
{
public:

	CRotDetect(const float & opt_max_false = 0.20,const float & opt_min_true = 0.44)
	{
		this->opt_max_false = opt_max_false;
		this->opt_min_true = opt_min_true;
	};

	~CRotDetect() {};

	/* Different result states for images */
	enum rotation
	{
		NOT_ROTATED,	/* source image does not need to be rotated 	*/
		ROTATED90CW,	/* source image is 90 degrees clockwise rotated	*/
		ROTATED90CCW,	/* source image is 90 degrees counterclockwise rotated */
		UNKNOWN		/* orientation of the source image is unknown	*/
	};

	enum tristate { VRAI, FAUX, UNDEF };


	tristate defuzzy(float value)
	{
		if (value <= opt_max_false)
			return FAUX;

		if (value >= opt_min_true)
			return VRAI;

		return UNDEF;
	}

	/*
		Analyzes the result of rotdetect (see rotdetect.h) and determines
		the orientation of the source image.

		Parameter:
			result - output activations returned from the neuronal network
		Returns:
			Decision about the orientation.
	*/
	rotation analyzeResult(float * result);

	/*
		Feeds the source image into the detector network and
		detect its orientation.

		Parameter:
			image  - Image to analyze
			result - Pointer to the result vector. The results will have
					 the following meaning:
				 result[0]: indicator for "image is not rotated"
				 result[1]: indicator for "image is 90° cw rotated"
				 result[2]: indicator for "image is 90° ccw rotated"

			The indicators are typically in the range [0,1].
			However, this is not guaranteed.
	*/
	void rotdetect(CImageLoadingFormat *image, float *result, const bool &opt_verify);


private:

	void runANN(CRegardsFloatBitmap *image, float *result, float ratio);
	float opt_max_false = 0.20;
	float opt_min_true = 0.90;

};
