#pragma once

namespace vole {
	namespace cmfdgt {
		// computes a bounding box around all not fully transparent pixels,
		// from the image boundary to inside of the image
		cv::Rect computeBoundingBox(cv::Mat_<unsigned char> full_alpha_image);
	}
}

