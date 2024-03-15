#include <header.h>
#include <iostream>
#include "command_reverse_engineer_snippet.h"

namespace vole {
namespace cmfdgt {

CommandReverseEngineerSnippet::CommandReverseEngineerSnippet()
	: Command(
		"gt_get_snippet",
		config,
		"Christian Riess",
		"christian.riess@informatik.uni-erlangen.de")
{
}

int CommandReverseEngineerSnippet::execute()
{
	if (config.orig_file.length() < 1) { std::cout << "Add original image!" << std::endl; printHelp(); return 1; }
	cv::Mat_<cv::Vec3b> orig = cv::imread(config.orig_file, cv::IMREAD_COLOR);
    if (orig.empty()) { std::cout << "error loading image " << config.orig_file << std::endl; return 1; }

	if (config.copy_file.length() < 1) { std::cout << "Add copied image (--copy)!" << std::endl; printHelp(); return 1; }
	cv::Mat_<cv::Vec3b> copy = cv::imread(config.copy_file, cv::IMREAD_COLOR);
    if (orig.empty()) { std::cout << "error loading image " << config.orig_file << std::endl; return 1; }

	if (config.snippet_file.length() < 1) { std::cout << "Add copy snippet (--snippet)!" << std::endl; printHelp(); return 1; }
	if (config.snippet_alpha_file.length() < 1) { std::cout << "Add snippet alpha channel file (--snippet_alpha)!" << std::endl; printHelp(); return 1; }
	if (config.full_snippet_alpha_file.length() < 1) { std::cout << "Add full snippet alpha channel file (--full_snippet_alpha)!" << std::endl; printHelp(); return 1; }

	if ((orig.cols != copy.cols) || (orig.rows != copy.rows)) {
		std::cout << "ERROR: original and tampered image have different size, aborted." << std::endl;
		return 1;
	}

	cv::Mat_<unsigned char> mask(copy.rows, copy.cols, (unsigned char)0);
	int miny = -1; int minx = -1;
	int maxy = -1; int maxx = -1;
	for (int y = 0; y < copy.rows; ++y) {
		for (int x = 0; x < copy.cols; ++x) {
			if ((orig[y][x][0] != copy[y][x][0]) || (orig[y][x][1] != copy[y][x][1]) || (orig[y][x][2] != copy[y][x][2])) {
				mask[y][x] = 255;
				if (miny == -1) {
					minx = maxx = x;
					miny = maxy = y;
				} else {
					if (y > maxy) maxy = y;
					if (x > maxx) maxx = x;
				}
			}
		}
	}
	if (miny == -1) {
		std::cout << "ERROR: original and tampered image are identical, aborted." << std::endl;
		return 1;
	}
	cv::imwrite(config.full_snippet_alpha_file, mask);
	std::cout << "min = " << minx << ", " << miny << ", max = " << maxx << ", " << maxy << std::endl;
	std::cout << "dim orig: " << orig.cols << "x" << orig.rows << "... minx/y: " << minx << ", " << miny << " width: " << maxx-minx+1 << ", " << maxy-miny+1 << std::endl;
	cv::Mat_<unsigned char> snippet_small(mask, cv::Rect(minx, miny, maxx-minx+1, maxy-miny+1));
	cv::imwrite(config.snippet_alpha_file, snippet_small);
	cv::Mat_<cv::Vec3b> image_cutout(copy, cv::Rect(minx, miny, maxx-minx+1, maxy-miny+1));
	cv::imwrite(config.snippet_file, image_cutout);
	std::cout << "full alpha mask:\t" << config.full_snippet_alpha_file << std::endl;
	std::cout << "cropped alpha mask:\t" << config.snippet_alpha_file << std::endl;
	std::cout << "image part:\t" << config.snippet_file << std::endl;
}

void CommandReverseEngineerSnippet::printHelp() const {
	std::cout << "computes the changed regions between original and tampered image" << std::endl;
	std::cout << std::endl;
	std::cout << "As cue for the inserted regions, the command gt requires an alpha" << std::endl;
	std::cout << "channel containing only the changed regions. Note, however, that" << std::endl;
	std::cout << "the present command can handle multiple inserted regions only" << std::endl;
	std::cout << "poorly, as only one global difference image is printed." << std::endl;
	std::cout << std::endl;
	std::cout << "Thus, the preferred way of creating ground truth is to export the" << std::endl;
	std::cout << "copy layer in the graphics program separately, and not to resort to" << std::endl;
	std::cout << "this command." << std::endl;
}

void CommandReverseEngineerSnippet::printShortHelp() const {
	std::cout << "computes the changed regions between original and tampered image" << std::endl;
}



}
}

