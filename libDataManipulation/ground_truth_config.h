#pragma once

#include "config.h"

/**
 * Configuration parameters for the ground truth generation.
 */
namespace vole {

class GroundTruthConfig : public Config {
public:	
	GroundTruthConfig(const std::string &prefix = std::string());

	//! input file name
	std::string orig_file;

	//! file name of full image including the tampered region
	std::string copy_file;
	//! output file name
	std::string ground_truth_file;
	//! file name of manipulated region
	std::string snippet_file;
	//! alpha threshold for the overlay
	std::string snippet_alpha_file;

	//! alpha channel of the snippet in the dimensions of the full image
	std::string full_snippet_alpha_file;

	/** Intensity limit for considering a snippet and an image pixel equal
	 * w.r.t. L1-distance. Thus, if between two pixels all color channels
	 * differ only up to l1_tolerance, the pixels are assumed to be equal.
	 * Reasonable values empirically turned out to be between 10 and 20. Values
	 * higher than 20 can lead to rather arbitrary matches, while lower values
	 * overly harshly reject pixels that were subject to some amount of
	 * blending in the image.
	 */
	int l1_tolerance;

	/** speed-up factor to abort the matching of the snippet with a particular
	 * image position when the score becomes too bad. The comparison is aborted
	 * when the matching error exceeds #SnippetPixels/prune_factor, i.e. the
	 * higher prune_factor, the earlier is a snippet rejected.
	 */
	double prune_factor;

	//! minimum intensity in the alpha channel to consider a pixel as copied
	int opaque_intensity;

	/** use L2-distance instead of L1-distance. Empirically, L2 distance
	 * appeared to be more robust than L1 distance. */
	bool use_l2;

	virtual std::string getString() const;

	protected:
	#ifdef WITH_BOOST
		virtual void initBoostOptions();
	#endif // WITH_BOOST

};

}
