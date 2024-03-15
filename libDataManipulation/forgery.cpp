#include <header.h>
#include "forgery.h"


#include <iostream>
#include <vector>
#include <cstdlib>

using namespace cv;

Forgery::Forgery(const cv::Mat_<cv::Vec3b> original, const cv::Mat_<cv::Vec3b> tampered, double prune_factor, int opaque_intensity, bool use_l2, int verbosity)
	: original(original), tampered(tampered), prune_factor(prune_factor), opaque_intensity(opaque_intensity), l2dist(use_l2), verbosity(verbosity)
{
	if (prune_factor <= std::numeric_limits<double>::epsilon()) {
		std::cerr << "WARNING: prune factor must be positive; autonomously changed prune factor from "
			<< prune_factor << " to 0.1" << std::endl;
		prune_factor = 0.1;
	}
	if (opaque_intensity < 0) {
		std::cerr << "WARNING: opaque intensity must be between 0 and 255 (given: " << opaque_intensity << ", changed to 0." << std::endl;
		opaque_intensity = 0;
	}
	if (opaque_intensity > 255) {
		std::cerr << "WARNING: opaque intensity must be between 0 and 255 (given: " << opaque_intensity << ", changed to 255." << std::endl;
		opaque_intensity = 255;
	}
}

cv::Mat_<unsigned char> Forgery::annotateMatchesAlphaOnly(const SubImage &p, cv::Mat_<cv::Vec3b> copy, GoodMatch insertPos, GoodMatch origPos) {
	int xoff1 = origPos.bestPositions()[0].x;
	int yoff1 = origPos.bestPositions()[0].y;
	int xoff2 = insertPos.bestPositions()[0].x;
	int yoff2 = insertPos.bestPositions()[0].y;
	
	cv::Mat_<unsigned char> ground_truth(copy.rows, copy.cols, (unsigned char)0);
	for (int y = 0; y < p.thrmask.rows; ++y) {
		for (int x = 0; x < p.thrmask.cols; ++x) {
			if ((p.thrmask[y][x] >= opaque_intensity) &&
				(fabs((double)(copy[yoff1+y][xoff1+x][0] - copy[yoff2+y][xoff2+x][0])) < 6) &&
				(fabs((double)(copy[yoff1+y][xoff1+x][1] - copy[yoff2+y][xoff2+x][1])) < 6) &&
				(fabs((double)(copy[yoff1+y][xoff1+x][2] - copy[yoff2+y][xoff2+x][2])) < 6))
			{
				ground_truth[yoff1+y][xoff1+x] = 255;
				ground_truth[yoff2+y][xoff2+x] = 255;
			}
		}
	}
	return ground_truth;
}

cv::Mat_<unsigned char> Forgery::annotateMatches(const SubImage &p, cv::Mat_<cv::Vec3b> copy, GoodMatch insertPos, GoodMatch origPos) {
	int xoff1 = origPos.bestPositions()[0].x;
	int yoff1 = origPos.bestPositions()[0].y;
	int xoff2 = insertPos.bestPositions()[0].x;
	int yoff2 = insertPos.bestPositions()[0].y;
	
	cv::Mat_<unsigned char> ground_truth(copy.rows, copy.cols, (unsigned char)0);
	for (int y = 0; y < p.thrmask.rows; ++y) {
		for (int x = 0; x < p.thrmask.cols; ++x) {
			if ((fabs((double)(copy[yoff1+y][xoff1+x][0] - copy[yoff2+y][xoff2+x][0])) < 6) &&
				(fabs((double)(copy[yoff1+y][xoff1+x][1] - copy[yoff2+y][xoff2+x][1])) < 6) &&
				(fabs((double)(copy[yoff1+y][xoff1+x][2] - copy[yoff2+y][xoff2+x][2])) < 6))
			{
				ground_truth[yoff1+y][xoff1+x] = 255;
				ground_truth[yoff2+y][xoff2+x] = 255;
			}
		}
	}
	return ground_truth;
}

// altered pixels
cv::Mat_<unsigned char> Forgery::annotateOtherPixels(const SubImage &p, GoodMatch insertPos,
	cv::Mat_<cv::Vec3b> orig, cv::Mat_<cv::Vec3b> copy, cv::Mat_<unsigned char> ground_truth)
{
	int xoff = insertPos.bestPositions()[0].x;
	int yoff = insertPos.bestPositions()[0].y;

	for (int y = 0; y < p.thrmask.rows; ++y) {
		for (int x = 0; x < p.thrmask.cols; ++x) {
			if ((p.mask[y][x] > 1) && (ground_truth[y+yoff][x+xoff] < opaque_intensity) &&
				((fabs((double)(orig[yoff+y][xoff+x][0] - copy[yoff+y][xoff+x][0])) >= 6) ||
				 (fabs((double)(orig[yoff+y][xoff+x][1] - copy[yoff+y][xoff+x][1])) >= 6) ||
				 (fabs((double)(orig[yoff+y][xoff+x][2] - copy[yoff+y][xoff+x][2])) >= 6)))
			{
				ground_truth[y+yoff][x+xoff] = 128;
			}
		}
	}
	return ground_truth;
}

// make sure that this location is never found again
cv::Mat_<cv::Vec3b> Forgery::neutralize_best_match(const SubImage &p, cv::Mat_<cv::Vec3b> copy, GoodMatch gm) {
	cv::Mat_<cv::Vec3b> neutralized;
	copy.copyTo(neutralized);
	int xoff = gm.bestPositions()[0].x;
	int yoff = gm.bestPositions()[0].y;
	for (int y = 0; y < p.thrmask.rows; ++y) {
		for (int x = 0; x < p.thrmask.cols; ++x) {
			if (p.thrmask[y][x] >= opaque_intensity) { // invert pixel
				neutralized[yoff+y][xoff+x][0] = (neutralized[yoff+y][xoff+x][0] + 128) % 256;
				neutralized[yoff+y][xoff+x][1] = (neutralized[yoff+y][xoff+x][1] + 128) % 256;
				neutralized[yoff+y][xoff+x][2] = (neutralized[yoff+y][xoff+x][2] + 128) % 256;
			}
		}
	}
	return neutralized;
}

GoodMatch Forgery::hierarchic_find(const SubImage &p, cv::Rect bbox, int stepsize, int tolerance) const {
	GoodMatch gm = find(p, bbox, stepsize, tolerance, true);
	while (stepsize > 1 && !gm.valid()) {
		stepsize = (int)stepsize / 2;
		if (verbosity > 1) std::cout << "nothing found, trying stepsize " << stepsize << std::endl;
		gm = find(p, bbox, stepsize, tolerance, true);
	}
	if (stepsize == 1) return gm;
	if (verbosity > 0)
		std::cout << "good match: " << std::endl << gm << ", verifying it..." << std::endl;
	return find(p, bbox, 1, gm, tolerance); // search exactly the six best matches
}

GoodMatch Forgery::find(const SubImage& p, cv::Rect bbox, int stepsize, GoodMatch gm, int tolerance) const {
	// set up experiment where exactly the good matches are searched
	std::vector<cv::Point> sixBestPositions = gm.bestPositions();
	if (!gm.valid()) {
		std::cout << "Forgery::find(): no valid matches, aborted." << std::endl;
		return gm;
	}

	std::vector<cv::Point> new_six_best_positions;
	std::vector<double> new_six_best_scores;
	GoodMatch tmp;
	for (int i = 0; i < 6; ++i) {
		if (sixBestPositions[i].x != -1) {
			cv::Rect one_pos(sixBestPositions[i].x, sixBestPositions[i].y, p.thrmask.cols, p.thrmask.rows);
			GoodMatch tmp = find(p, one_pos, 1, tolerance, true);
			if (tmp.valid()) {
				new_six_best_scores.push_back(tmp.bestScores()[0]);
				new_six_best_positions.push_back(tmp.bestPositions()[0]);
			}
		}
	}
	// get the worst case score from the last computation, copy the rest
	GoodMatch result(tmp.getWorstPossibleScore());
	for (unsigned int i = 0; i < new_six_best_scores.size(); ++i) {
		result.insertScore(new_six_best_scores[i], new_six_best_positions[i]);
	}
	return result;
}


/* Note: This method is not clever. It is O(N*M) in worst case. This could
         be decreased to almost O(N), as typically almost all pixels are 0
         in diffmap. Use linear filtering trick to prune! */
// another extension will be to find multiple regions like in the 'House of Hell'
GoodMatch Forgery::find(const SubImage& p, cv::Rect bbox, int stepsize, int tolerance, bool verbose) const {
	/* upper bound of badness we allow. set this conservatively! */
	// count # pixels that can be verified with given stepsize
	int n_opaque_pixels = 0;
	for (int y = 0; y < p.thrmask.rows; y += stepsize) {
		for (int x = 0; x < p.thrmask.cols; x += stepsize) {
			if (p.thrmask[y][x] >= opaque_intensity) n_opaque_pixels++;
		}
	}
	double worst_possible_score = n_opaque_pixels/ prune_factor /*(stepsize*stepsize)*/;
	if (verbose && (verbosity > 0)) std::cout << "Threshold mask contains " << n_opaque_pixels
		<< " opaque pixels, we drop positions with badness score " << worst_possible_score << std::endl;
	GoodMatch matches(worst_possible_score);
	double worst_score = matches.getWorstScore(); // the one to beat.

	// run through all possible fits w/o out-of-bounds
//	minx = 0; maxx = tampered.cols - p.thrmask.cols;
//	miny = 0; maxy = tampered.rows - p.thrmask.rows;
	
	/* lets see what diffmap fits our needs best */
	
	int maxy = bbox.y+bbox.height-p.thrmask.rows+1;
	int maxx = bbox.x+bbox.width-p.thrmask.cols+1;

	for (int y = bbox.y; y < maxy; ++y) {
		if (!(y % 2)) {
			if (verbosity > 1)
				std::cout << "\rScanned " << (y*100)/tampered.rows << " %"; std::cout.flush();
		}
		for (int x = bbox.x; x < maxx; ++x) {
			double score = 0;

			for (int winy = 0; winy < p.thrmask.rows; winy+=stepsize) {
				const cv::Vec3b *tampered_row = tampered[y+winy];
				const cv::Vec3b *snippet_row = p.colors[winy];
				const unsigned char *snippet_mask_row = p.thrmask[winy];
				for (int winx = 0; winx < p.thrmask.cols; winx+=stepsize) {
					if (snippet_mask_row[winx] < opaque_intensity) { continue; }
					if (l2dist) { // less pruning here, because the overall distance is smaller
						// scale every distance to a range between 0 and 1.2;
						// maximum distance becomes then sqrt(3*1.2) = 1.7,
						// average distance on true template should be much smaller
						
						double d1 = (tampered_row[x+winx][0] - snippet_row[winx][0]) * 1.2 / 255.;
						double d2 = (tampered_row[x+winx][1] - snippet_row[winx][1]) * 1.2 / 255.;
						double d3 = (tampered_row[x+winx][2] - snippet_row[winx][2]) * 1.2 / 255.;
						
						score += std::sqrt(d1*d1 + d2*d2 + d3*d3);
					} else {
						// l1 distance to this pixel
						if ((fabs((double)(tampered_row[x+winx][0] - snippet_row[winx][0])) > tolerance) ||
							(fabs((double)(tampered_row[x+winx][1] - snippet_row[winx][1])) > tolerance) ||
							(fabs((double)(tampered_row[x+winx][2] - snippet_row[winx][2])) > tolerance))
							score++;
					}
					if (score > worst_score)
						goto hell;
				}
			}
hell:
			if (score < worst_score) {
				matches.insertScore(score, Point(x, y));
				worst_score = matches.getWorstScore();
			}
//			if (score == 0) { // perfect match
//				goto perfect_match_found;
//			}
		}
	}
//perfect_match_found:
	if (verbosity > 1) std::cout << std::endl;
	return matches;
}

void Forgery::markTampered(Mat_<float>& target) {
	//TODO make the following work instead of that pure code wasteage
	target.setTo(Scalar(0.5), absmap);
/*	MatIterator_<float> itt = target.begin(), itm = absmap.begin();
	for (; itt != target.end(); itt++, itm++)
		if ((*itm > EPSILON) && *itt < 0.5)
			*itt = 0.5;*/
}

/** mark copy-moved regions in target; both regions are delimited by their
 * starting coordinates region1 and region2, and by the clipping mask.
 * the examination is done on the tampered image
 */
void Forgery::markCMFD(Mat_<unsigned char>& target, Point region1, Point region2,
                       const Mat_<unsigned char>& mask) const {
	/* here we take the time to do a real test of which pixels are indeed still
	   found 1:1 in the tampered image. So we do not just trust the information
	   in the subimage mask. Smearing etc. may have destroyed a big deal.
	   Additionally we take care that we do not include pixels that are
	   the same by chance by applying the snippet mask */

	for (int y = 0; y < mask.rows; ++y) {
		const unsigned char *rm = mask[y];
		const cv::Vec3b *r1 = tampered[region1.y+y]; unsigned char *rt1 = target[region1.y+y];
		const cv::Vec3b *r2 = tampered[region2.y+y]; unsigned char *rt2 = target[region2.y+y];
		for (int x = 0; x < mask.cols; ++x) {
			if (rm[x] < 128) // pixel not part of snippet
				continue;
			if (norm(r1[region1.x+x] - r2[region2.x+x]) == 0) {
				rt1[region1.x+x] = rt2[region2.x+x] = 255;
			}
		}	
	}
}

void Forgery::show() const {
	imshow("Difference", diff);
	imshow("Diff. map", diffmap);
	imshow("In absolute", absmap);
	waitKey();
}

