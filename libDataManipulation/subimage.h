#pragma once

#include "ground_truth_common.h"

/** Encapsulates an image or a snippet as a combination of the color
 * information and the transparency information. Note that the transparency
 * mask is processed with morphologic operators that are thresholding at
 * opaque_intensity. Values higher than opaque_intensity are considered opaque
 * pixels, the others as transparent pixels.
 */
class SubImage {
public:

	SubImage(const cv::Mat_<cv::Vec3b> colors, const cv::Mat_<unsigned char> mask, int opaque_intensity);
	void show() const;


	cv::Mat_<cv::Vec3b> colors;
	cv::Mat_<unsigned char> mask, thrmask;
	cv::Mat_<cv::Vec3b> maskBGR;
	int opaque_intensity;
};

