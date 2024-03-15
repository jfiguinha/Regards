#include <header.h>
#include "command_ground_truth.h"

// #include "core_ground_truth.h"
#include "ground_truth_common.h"
#include "forgery.h"
#include "good_match.h"
#include "alpha_channel.h"

#include <limits>

using namespace boost::program_options;

namespace vole {

GroundTruth::GroundTruth()
	: Command(
		"gt",
		config,
		"Johannes Jordan, Christian Riess",
		"{johannes.jordan,christian.riess}@informatik.uni-erlangen.de")
{
}

int GroundTruth::execute()
{
	if (config.orig_file.length() < 1) { std::cerr << "No original image given!" << std::endl; printHelp(); return 1; }
	if (config.ground_truth_file.length() < 1) { std::cerr << "No ground truth output file given! (--ground_truth)" << std::endl; return 1; } 
	if (config.copy_file.length() < 1) { std::cerr << "No copied image given (--copy)!" << std::endl; printHelp(); return 1; }
	if (config.snippet_file.length() < 1) { std::cerr << "No copy snippet given (--snippet)!" << std::endl; printHelp(); return 1; }
	if (config.snippet_alpha_file.length() < 1) { std::cerr << "No snippet alpha channel file given (--snippet_alpha)!" << std::endl; printHelp(); return 1; }
	if (config.full_snippet_alpha_file.length() < 1) { std::cerr << "No full-size snippet alpha file given (--full_snippet_alpha)!" << std::endl; printHelp(); return 1; }

	cv::Mat_<cv::Vec3b> original = cv::imread(config.orig_file, cv::IMREAD_COLOR);
    if (original.empty()) { std::cerr << "error loading original image " << config.orig_file << std::endl; return 1; }


	cv::Mat_<cv::Vec3b> tampered = cv::imread(config.copy_file, cv::IMREAD_COLOR);
	if (tampered.empty()) { std::cerr << "error loading image " << config.copy_file << std::endl; return 1; }

	cv::Mat_<cv::Vec3b> snippet =  cv::imread(config.snippet_file, cv::IMREAD_COLOR);
	if (snippet.empty()) { std::cerr << "error loading image " << config.snippet_file << std::endl; return 1; }

	cv::Mat_<unsigned char> snippet_alpha = cv::imread(config.snippet_alpha_file, cv::IMREAD_GRAYSCALE);
	if (snippet_alpha.empty()) { std::cerr << "error loading image " << config.snippet_alpha_file << std::endl; return 1; }

	cv::Mat_<unsigned char> full_snippet_alpha = cv::imread(config.full_snippet_alpha_file, cv::IMREAD_GRAYSCALE); // grayscale
	cv::Rect bounding_box = vole::cmfdgt::computeBoundingBox(full_snippet_alpha);

	if (config.verbosity > 1) 
		std::cout << "bounding box: (" << bounding_box.x << ", " << bounding_box.y << ")-("
			<< bounding_box.x+bounding_box.width << ", " << bounding_box.y+bounding_box.height << ")" << std::endl;

	// determine position of the snippet in the bounding box first;
	
	// invert these pixels for the subsequent search.

	// go through whole image and find all good l1-matches with snippet
	SubImage s(snippet, snippet_alpha, config.opaque_intensity);
	Forgery f(original, tampered, config.prune_factor, config.opaque_intensity, config.use_l2);
	cv::Rect leanbb(std::max<int>(0, bounding_box.x - 20), std::max<int>(0, bounding_box.y - 20), 1, 1);
	leanbb.width = std::min(bounding_box.width+40, tampered.cols-leanbb.x);
	leanbb.height = std::min(bounding_box.height+40, tampered.rows-leanbb.y);

	if (config.verbosity > 1) 
		std::cout << "lean bounding box: (" << leanbb.x << ", " << leanbb.y << ")-("
			<< leanbb.x+leanbb.width << ", " << leanbb.y+leanbb.height << ")" << std::endl;

	GoodMatch gm = f.hierarchic_find(s, leanbb, 14, config.l1_tolerance);
	if (!gm.valid()) {
		std::cout << "oops, nothing found! maybe the boundaries are too strict :/" << std::endl;
		return 1;
	}
	std::cout << "INSERTED POSITION: good match at: ( " << gm.bestPositions()[0].x << "," << gm.bestPositions()[0].y << " )" << std::endl;

	// ideal solution:
	// look through the matches: how many are reasonable? force the number of reasonable matches with a parameter
	// decide for orig region and copies.
	//
	// current solution: search a second match (the original region)
	cv::Mat_<cv::Vec3b> neutralized = f.neutralize_best_match(s, tampered, gm);
	Forgery f2(original, neutralized, config.prune_factor, config.opaque_intensity, config.use_l2);
	GoodMatch gm2 = f2.hierarchic_find(s, cv::Rect(0, 0, neutralized.cols, neutralized.rows), 14, config.l1_tolerance);
	if (!gm2.valid()) {
		std::cout << "oops, no second match found! Maybe the boundaries are too strict :/" << std::endl;
		return 1;
	}
	std::cout << "SOURCE POSITION: good match at: ( " << gm2.bestPositions()[0].x << "," << gm2.bestPositions()[0].y << " )" << std::endl;

	// decide which matches fit pairwise best together
	// [skipped for the moment]

	// create ground truth map for the union of these matches
	cv::Mat_<unsigned char> ground_truth = f.annotateMatchesAlphaOnly(s, tampered, gm, gm2);

	// enhance ground truth map with all differences of orig and tampered (if available)
	ground_truth = f.annotateOtherPixels(s, gm, original, tampered, ground_truth);

	// do region support post-processing (e.g. "do I have in an 8x8 region a chance?")
	// (in CommandGtPostProcessing)
	

	cv::imwrite(config.ground_truth_file, ground_truth);
	return 0;
}

void GroundTruth::printHelp() const {
	std::cout << "Find the source and insertion coordinates of a snippet" << std::endl;
	std::cout << std::endl;
	std::cout << "FIXME write more information here" << std::endl;
}

void GroundTruth::printShortHelp() const {
	std::cout << "Find the source and insertion coordinates of a snippet" << std::endl;
}

}
