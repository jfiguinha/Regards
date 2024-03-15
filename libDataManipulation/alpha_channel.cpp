#include <header.h>
#include <iostream>

#include "alpha_channel.h"

namespace vole {
	namespace cmfdgt {

		// computes a bounding box around all not fully transparent pixels,
		// from the image boundary to inside of the image
		cv::Rect computeBoundingBox(cv::Mat_<unsigned char> full_alpha_image) {
			cv::Mat_<int> horiz(1, full_alpha_image.cols, 0);
			cv::Mat_<int> vert(full_alpha_image.rows, 1, 0);

			for (int y = 0; y < full_alpha_image.rows; ++y) {
				for (int x = 0; x < full_alpha_image.cols; ++x) {
					horiz[0][x] += full_alpha_image[y][x];
					vert[y][0] += full_alpha_image[y][x];
				}
			}

			cv::Rect bounding_box(0, 0, full_alpha_image.cols, full_alpha_image.rows);

			// figure out left boundary
			for (int x = 0; (x < full_alpha_image.cols) && (horiz[0][x] == 0); ++x) bounding_box.x = x;
			// figure out left boundary
			for (int y = 0; (y < full_alpha_image.rows) && (vert[y][0] == 0); ++y) bounding_box.y = y;

			bounding_box.width = full_alpha_image.cols - bounding_box.x;
			bounding_box.height = full_alpha_image.rows - bounding_box.y;

			// figure out right boundary
			for (int x = full_alpha_image.cols-1; (x > 0) && (horiz[0][x] == 0); --x) { bounding_box.width--; }

			// figure out right boundary
			for (int y = full_alpha_image.rows-1; (y > 0) && (vert[y][0] == 0); --y) { bounding_box.height--; }
			
			return bounding_box;
		}
	}
}
