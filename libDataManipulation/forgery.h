#pragma once

#include "ground_truth_common.h"
#include "subimage.h"
#include "good_match.h"

class Forgery {

public:

	Forgery(const cv::Mat_<cv::Vec3b> original, const cv::Mat_<cv::Vec3b> tampered, double prune_factor, int opaque_intensity, bool use_l2, int verbosity = 0);

	/* Note: This method is not clever. It is O(N*M) in worst case. This could
			 be decreased to almost O(N), as typically almost all pixels are 0
			 in diffmap. Use linear filtering trick to prune! */
	// another extension will be to find multiple regions like in the House of Hell
	GoodMatch hierarchic_find(const SubImage&, cv::Rect bbox, int stepsize, int tolerance) const;
	GoodMatch find(const SubImage& p, cv::Rect bbox, int stepsize, GoodMatch gm, int tolerance) const;
	GoodMatch find(const SubImage&, cv::Rect bbox, int stepsize, int tolerance, bool verbose = false) const;
	void markTampered(cv::Mat_<float> & target);
	cv::Mat_<cv::Vec3b> neutralize_best_match(const SubImage &p, cv::Mat_<cv::Vec3b> copy, GoodMatch gm);

	cv::Mat_<unsigned char> annotateMatches(const SubImage &, cv::Mat_<cv::Vec3b> copy, GoodMatch insertPos, GoodMatch origPos);
	cv::Mat_<unsigned char> annotateMatchesAlphaOnly(const SubImage &, cv::Mat_<cv::Vec3b> copy, GoodMatch insertPos, GoodMatch origPos);
	cv::Mat_<unsigned char> annotateOtherPixels(const SubImage &, GoodMatch insertPos,
		cv::Mat_<cv::Vec3b> orig, cv::Mat_<cv::Vec3b> copy, cv::Mat_<unsigned char> ground_truth);

	/** mark copy-moved regions in target; both regions are delimited by their
	* starting coordinates region1 and region2, and by the clipping mask.
	* the examination is done on the tampered image
	*/
	void markCMFD(cv::Mat_<unsigned char> & target, cv::Point region1, cv::Point region2,
	              const cv::Mat_<unsigned char>& mask) const;

	void show() const;


protected:

	cv::Mat_<cv::Vec3b> original, tampered;
	cv::Mat_<cv::Vec3b> diff;
	cv::Mat_<float> diffmap, diffmap_e, diffmap_d;
	cv::Mat_<char> absmap;
	cv::Rect bbox;
	GoodMatch matches;

	double prune_factor;
	int opaque_intensity;
	bool l2dist;
	int verbosity;
};


