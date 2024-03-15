#include <header.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include "subimage.h"

using namespace cv;

SubImage::SubImage(const cv::Mat_<cv::Vec3b> colors, const cv::Mat_<unsigned char> mask, int opaque_intensity) 
	: colors(colors), mask(mask), opaque_intensity(opaque_intensity)
{

//	colors /= 255.0;
//	mask /= 255.0;
	
	/* enlarge mask to BGR for simpler operation */
	std::vector<Mat> marr(3, mask);
	merge(marr, maskBGR);

	/* hard threshold & erode mask */
	threshold(mask, thrmask, opaque_intensity, 255, THRESH_BINARY);

	Mat_<unsigned char> element(5, 5, 255);
	erode(thrmask, thrmask, element);
}


void SubImage::show() const {
	imshow("Piece", colors);
	imshow("Mask", mask);
	imshow("ThrMask", thrmask);
//	imshow("Applied", colors.mul(maskBGR));
	waitKey();
}

