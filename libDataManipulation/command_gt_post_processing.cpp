#include <header.h>
#include "command_gt_post_processing.h"

namespace vole { namespace cmfdgt {

CommandGtPostProcessing::CommandGtPostProcessing()
	: Command(
		"gt_postproc",
		config,
		"Christian Riess",
		"christian.riess@informatik.uni-erlangen.de")
{
}

int CommandGtPostProcessing::execute()
{
	if (config.ground_truth_file.length() < 1) { std::cout << "no input ground truth file (--ground_truth), aborted." << std::endl; return 1; }
	if (config.pp_ground_truth_file.length() < 1) { std::cout << "no output file (--pp_ground_truth), aborted." << std::endl; return 1; }

	cv::Mat_<unsigned char> ground_truth = cv::imread(config.ground_truth_file, cv::IMREAD_GRAYSCALE);
	if (ground_truth.rows < 1) {
		std::cout << "unable to read input file " << config.ground_truth_file << ", aborted." << std::endl;
		return 1;
	}
	cv::Mat_<unsigned char> postprocessed(ground_truth.rows, ground_truth.cols, (unsigned char)0);

	// rules:
	// if a directly copied pixel (white,255) has sufficient support, it remains white
	// If a not directly copied pixel (128) has sufficient support, it becomes white
	// If a directly copied pixel (white,255) has insufficient support, it becomes light gray (170)
	// If a not directly copied pixel (128) has insufficient support, it becomes dark gray (85)

	// erode/dilate the ground truth image according to the parameters.

	// compute integral image on ground truth labels
	cv::Mat_<int> integral(ground_truth.rows+1, ground_truth.cols+1, (int)0);
	for (int y = 0; y < integral.rows; ++y) integral[y][0] = 0;
	for (int x = 0; x < integral.cols; ++x) integral[0][x] = 0;

	for (int y = 1; y < integral.rows; ++y) {
		for (int x = 1; x < integral.cols; ++x) {
			integral[y][x] =
				integral[y][x-1] + integral[y-1][x] - integral[y-1][x-1]
				+ ((ground_truth[y-1][x-1] == 255) ? 1 : 0);
		}
	}

	int to_left = (int)(config.support_size / 2);
	int to_right = std::max<int>(0, config.support_size - to_left - 1);

	int left, right, up, down;
	for (int y = 0; y < ground_truth.rows; ++y) {
		if (y-to_left < 0) up = y; else up = to_left;
		if (y+to_right > ground_truth.rows-1) down = ground_truth.rows-1-y; else down = to_right;
		for (int x = 0; x < ground_truth.cols; ++x) {
			if (x-to_left < 0) left = x; else left = to_left;
			if (x+to_right > ground_truth.cols-1) right = ground_truth.cols-1-x; else right = to_right;

			// FIXME: unroll loop: not necessary to compute volume, left,
			// right, up, down every iteration
			int volume = (down + up + 1) * (right + left + 1);
			
			int nCopied = integral[y+down][x+right] - integral[y-up][x+right]
				- integral[y+down][x-left] + integral[y-up][x-left];
			
			int ratio = (int)((nCopied*100) / volume);
			if (ratio >= config.support_ratio) {
				postprocessed[y][x] = 255;
			} else {
				if (ground_truth[y][x] == 255) {
					postprocessed[y][x] = 170;
				} else if (ground_truth[y][x] > 0) {
					postprocessed[y][x] = 85;
				}
			}
		}
	}

	cv::imwrite(config.pp_ground_truth_file, postprocessed);

	return 0;
}


void CommandGtPostProcessing::printHelp() const {
	std::cout << "smooths the ground truth from pixel-wise gt to blockwise gt" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "The method discards ground truth pixels with too little support in the neighborhood." << std::endl;
	std::cout << "The reason is to alter the ground truth information to a map that guarantees minimum support" << std::endl;
	std::cout << "for a copied pixel, which should in principle support blockwise CMFD algorithms." << std::endl;
}

void CommandGtPostProcessing::printShortHelp() const {
	std::cout << "post-processing for the ground truth data" << std::endl;
}



}}
