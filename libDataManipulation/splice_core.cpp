#include <header.h>
#include "splice_core.h"
#include <cassert>

namespace vole { namespace cmfd_gt {

SpliceCore::SpliceCore(cv::Mat_<cv::Vec3b> &orig,
	std::vector<cv::Mat_<cv::Vec3b> > &snippets, std::vector<cv::Mat_<unsigned char> > &snippet_alpha,
	std::vector<int> &source_positions,
	std::vector<int> &insert_positions,
	int transparency_threshold)
: img(orig), orig_snippets(snippets), backup_snippet_alpha(snippet_alpha),
  transform_log(snippets.size()), transparency_threshold(transparency_threshold)
{
	alpha_mask = cv::Mat_<unsigned char>(img.rows, img.cols, (unsigned char)0);

	// create copies of the snippets to work on
	assert(backup_snippet_alpha.size() == orig_snippets.size());
	for (unsigned int i = 0; i < backup_snippet_alpha.size(); ++i) {
		this->snippet_alpha.push_back(cv::Mat_<unsigned char>());
		backup_snippet_alpha[i].copyTo(this->snippet_alpha[i]);

		this->orig_alpha.push_back(cv::Mat_<unsigned char>());
		backup_snippet_alpha[i].copyTo(this->orig_alpha[i]);

		this->snippets.push_back(cv::Mat_<cv::Vec3b>());
		orig_snippets[i].copyTo(this->snippets[i]);

	}
//	img.copyTo(img);

	for (unsigned int i = 0; i < orig_snippets.size(); ++i) {
		this->source_positions.push_back(cv::Point(source_positions[i*2], source_positions[i*2+1]));
		this->insert_positions.push_back(cv::Point(insert_positions[i*2], insert_positions[i*2+1]));
		source_center_positions.push_back(cv::Point(source_positions[i*2]+(orig_alpha[i].cols/2), source_positions[i*2+1]+(orig_alpha[i].rows/2)));
		insert_center_positions.push_back(cv::Point(insert_positions[i*2]+(orig_alpha[i].cols/2), insert_positions[i*2+1]+(orig_alpha[i].rows/2)));
	}
}

void SpliceCore::add_rotation(int snippet_no, int angle, bool is_replay) {
	snippet_alpha[snippet_no] = affineTransform(snippet_alpha[snippet_no], angle, 1, false, false, false);

	if (!is_replay) {
		snippets[snippet_no] = affineTransform(snippets[snippet_no], angle, 1, false, false, true);
		assert(snippet_alpha[snippet_no].rows == snippets[snippet_no].rows);
		assert(snippet_alpha[snippet_no].cols == snippets[snippet_no].cols);

		// remember operation for overlap handling (if necessary, during performing
		// the actual splicing)
		TRANSFORM_OP op;
		op.snippet_no = snippet_no;
		op.type = ROT;
		op.angle = angle;
		transform_log[snippet_no].push_back(op);
	}
}

void SpliceCore::add_scaling(int snippet_no, double factor, bool is_replay) {
	snippet_alpha[snippet_no] = affineTransform(snippet_alpha[snippet_no], 0, factor, false, false, false);

	if (!is_replay) {
		snippets[snippet_no] = affineTransform(snippets[snippet_no], 0, factor, false, false, true);
		assert(snippet_alpha[snippet_no].rows == snippets[snippet_no].rows);
		assert(snippet_alpha[snippet_no].cols == snippets[snippet_no].cols);

		// remember operation for overlap handling (if necessary, during performing
		// the actual splicing)
		TRANSFORM_OP op;
		op.snippet_no = snippet_no;
		op.type = SCALE;
		op.scaling_factor = factor;
		transform_log[snippet_no].push_back(op);
	}
}

void SpliceCore::add_rotation_and_scaling(int snippet_no, int angle, double factor, bool is_replay) {
	snippet_alpha[snippet_no] = affineTransform(snippet_alpha[snippet_no], angle, factor, false, false, false);

	if (!is_replay) {
		snippets[snippet_no] = affineTransform(snippets[snippet_no], angle, factor, false, false, true);
		assert(snippet_alpha[snippet_no].rows == snippets[snippet_no].rows);
		assert(snippet_alpha[snippet_no].cols == snippets[snippet_no].cols);

		// remember operation for overlap handling (if necessary, during performing
		// the actual splicing)
		TRANSFORM_OP op;
		op.snippet_no = snippet_no;
		op.type = ROT_SCALE;
		op.angle = angle;
		op.scaling_factor = factor;
		transform_log[snippet_no].push_back(op);
	}
}

void SpliceCore::splice() {

	std::cout << "habe " << transform_log.size() << " snippet logs" << std::endl
		<< "mit folgenden eintraegen: " << std::endl;
	for (unsigned int i = 0; i < transform_log.size(); ++i) {
		std::cout << "snippet " << i << ": ";
		for (unsigned int j = 0; j < transform_log[i].size(); ++j) {
			TRANSFORM_OP op = transform_log[i][j];
			std::cout << "(type="<<op.type;
			if (op.type != ROT) std::cout << ",scaling="<<op.scaling_factor;
			if (op.type != SCALE) std::cout << ",rot="<<op.angle;
			std::cout << ")  ";
		}
		std::cout << std::endl;
	}

	// try to embed the original positions in the ground truth as well.

	for (int i = 0; i < (int) orig_snippets.size(); ++i) {
		splice(i, source_center_positions[i], true );
		splice(i, insert_center_positions[i], false );
	}

	// create final ground truth
	for (int i = 0; i < (int) orig_snippets.size(); ++i) {
		int sx = source_center_positions[i].x - (orig_alpha[i].cols/2);
		int sy = source_center_positions[i].y - (orig_alpha[i].rows/2);
		int minx = std::max<int>(-sx, 0);
		int miny = std::max<int>(-sy, 0);
		int maxx = std::min<int>(alpha_mask.cols-sx, orig_alpha[i].cols);
		int maxy = std::min<int>(alpha_mask.rows-sy, orig_alpha[i].rows);
		std::cout << "inserting orig from (" << minx << "," << miny << ") to (" << maxx << "," << maxy << "), offset: " << sx << "," << sy << ")" << std::endl;
		for (int y = miny; y < maxy; ++y) {
			for (int x = minx; x < maxx; ++x) {
				alpha_mask[y+sy][x+sx] = std::max<int>(alpha_mask[y+sy][x+sx], orig_alpha[i][y][x]);
			}
		}

		sx = insert_center_positions[i].x - (snippet_alpha[i].cols/2);
		sy = insert_center_positions[i].y - (snippet_alpha[i].rows/2);
		minx = std::max<int>(-sx, 0);
		miny = std::max<int>(-sy, 0);
		maxx = std::min<int>(alpha_mask.cols-sx, snippet_alpha[i].cols);
		maxy = std::min<int>(alpha_mask.rows-sy, snippet_alpha[i].rows);
		std::cout << "inserting snippet from (" << minx << "," << miny << ") to (" << maxx << "," << maxy << "), offset: " << sx << "," << sy << ")" << std::endl;
		for (int y = miny; y < maxy; ++y) {
			for (int x = minx; x < maxx; ++x) {
				alpha_mask[y+sy][x+sx] = std::max<int>(alpha_mask[y+sy][x+sx], snippet_alpha[i][y][x]);
			}
		}
	}
}

bool SpliceCore::cropBoundaryPixels(cv::Mat_<unsigned char> &alpha, cv::Mat_<cv::Vec3b> &snippet, int xLeft, int yTop, int xRight, int yBottom) {
	if ((xLeft == 0) && (yTop == 0) && (xRight == 0) && (yBottom == 0)) return false;

	for (int y = 0; y < yTop; ++y) for (int x = 0; x < alpha.cols; ++x) alpha[y][x] = 0;
	for (int y = alpha.rows-yBottom+1; y < alpha.rows; ++y) for (int x = 0; x < alpha.cols; ++x) alpha[y][x] = 0;
	for (int y = 0; y < alpha.rows; ++y) for (int x = 0; x < xLeft; ++x) alpha[y][x] = 0;
	for (int y = 0; y < alpha.rows; ++y) for (int x = alpha.cols-xRight+1; x < alpha.cols; ++x) alpha[y][x] = 0;

	return true;
}

// recreate target
void SpliceCore::replayFromSource(int snippet_no) {
	std::cout << "replay " << snippet_no << std::endl;
	orig_alpha[snippet_no].copyTo(snippet_alpha[snippet_no]);
	for (unsigned int i = 0; i < transform_log[snippet_no].size(); ++i) {
		TRANSFORM_OP op = transform_log[snippet_no][i];
		if (op.type == SCALE) { add_scaling(snippet_no, op.scaling_factor, true); }
		if (op.type == ROT) { add_rotation(snippet_no, op.angle, true); }
		if (op.type == ROT_SCALE) { add_rotation_and_scaling(snippet_no, op.angle, op.scaling_factor, true); }
	}
}

cv::Mat_<unsigned char> SpliceCore::centerCopyImage(cv::Mat_<unsigned char> copySource, cv::Mat_<unsigned char> copyTarget, bool onlyMatting) {
	// Fit the result in the original snippet. This is a little bit cumbersome,
	// since the dimensions of the original alpha channel and the inversely
	// transformed alpha channel might not fit anymore

	cv::Mat_<unsigned char> target(copyTarget.rows, copyTarget.cols, (unsigned char)0);
	if (onlyMatting) copyTarget.copyTo(target);
	int yTop = 0; int yBottom = target.rows;
	int xLeft = 0; int xRight = target.cols;
	if (copySource.rows <= target.rows) {
		int delta = target.rows-copySource.rows;
		yTop = (int)((delta) / 2);
		yBottom -= (delta - yTop);
	}
	if (copySource.cols <= target.cols) {
		int delta = target.cols-copySource.cols;
		xLeft = (int)((delta) / 2);
		xRight -= (delta - xLeft);
	}
	int offsety = -yTop; int offsetx = -xLeft;
	if (copySource.rows > target.rows) {
		offsety += (int)((copySource.rows - target.rows)/2);
	}
	if (copySource.cols > target.cols) {
		offsetx += (int)((copySource.cols - target.cols)/2);
	}
	if (onlyMatting) {
		for (int y = yTop; y < yBottom; ++y) {
			for (int x = xLeft; x < xRight; ++x) {
				target[y][x] = std::min<int>(copySource[y+offsety][x+offsetx], target[y][x]);
			}
		}
	} else {
		for (int y = yTop; y < yBottom; ++y) {
			for (int x = xLeft; x < xRight; ++x) {
				target[y][x] = copySource[y+offsety][x+offsetx];
			}
		}
	}
	return target;
}

void SpliceCore::invertTargetTransform(int snippet_no) {
	cv::Mat_<unsigned char> result;
	snippet_alpha[snippet_no].copyTo(result);

	for (int i = ((int)transform_log[snippet_no].size())-1; i >= 0; --i) {
		TRANSFORM_OP op = transform_log[snippet_no][i];
		if (op.type == SCALE)     { result = affineTransform(result, 0, op.scaling_factor, true, false, false); }
		if (op.type == ROT)       { result = affineTransform(result, op.angle, 1, true, false, false); }
		if (op.type == ROT_SCALE) { result = affineTransform(result, op.angle, op.scaling_factor, true, false, false); }
	}
	orig_alpha[snippet_no] = centerCopyImage(result, orig_alpha[snippet_no]);
}

// relevant if both source and target location are partially occluded by new insert
void SpliceCore::twoWayReplay(int snippet_no) {
	cv::Mat_<unsigned char> result;
	snippet_alpha[snippet_no].copyTo(result);

	for (int i = ((int)transform_log[snippet_no].size())-1; i >= 0; --i) {
		TRANSFORM_OP op = transform_log[snippet_no][i];
		if (op.type == SCALE)     { result = affineTransform(result, 0, op.scaling_factor, true, false, false); }
		if (op.type == ROT)       { result = affineTransform(result, op.angle, 1, true, false, false); }
		if (op.type == ROT_SCALE) { result = affineTransform(result, op.angle, op.scaling_factor, true, false, false); }
	}
	orig_alpha[snippet_no] = centerCopyImage(result, orig_alpha[snippet_no], true);

	orig_alpha[snippet_no].copyTo(snippet_alpha[snippet_no]);
	for (unsigned int i = 0; i < transform_log[snippet_no].size(); ++i) {
		TRANSFORM_OP op = transform_log[snippet_no][i];
		if (op.type == SCALE) { add_scaling(snippet_no, op.scaling_factor, true); }
		if (op.type == ROT) { add_rotation(snippet_no, op.angle, true); }
		if (op.type == ROT_SCALE) { add_rotation_and_scaling(snippet_no, op.angle, op.scaling_factor, true); }
	}
}

// TODO: src_pos ist jetzt center, bestimme aus center cropping coordinaten.
// bei ueberschreib-test: auch center-coordinaten der anderen snippets
// benutzen, und in relative snippet-coordinaten umrechnen
void SpliceCore::splice(int snippet_no, cv::Point src_pos, bool is_source) {
	cv::Mat_<unsigned char> alpha;
	cv::Mat_<cv::Vec3b> snippet;
	if (is_source) {
		alpha = orig_alpha[snippet_no];
		snippet = orig_snippets[snippet_no];
	} else {
		alpha = snippet_alpha[snippet_no];
		snippet = snippets[snippet_no];
	}

	//  (left/top): how many rows/columns are out-of-bounds when the snippet is copied in the image?
	cv::Point snippetLeftOffset(std::max<int>(0, alpha.cols/2 - src_pos.x), std::max<int>(0, (alpha.rows/2)-src_pos.y));
	//  (right/bottom): how many rows/columns are out-of-bounds when the snippet is copied in the image?
	cv::Point snippetRightOffset(
		std::max<int>(0, src_pos.x + (alpha.cols-(alpha.cols/2)) - img.cols),
		std::max<int>(0, src_pos.y + (alpha.rows-(alpha.rows/2)) - img.rows));
	cv::Point snippetRightBound(alpha.cols - snippetRightOffset.x, alpha.rows - snippetRightOffset.y);

	std::cout << "snippetLeftOffset = " << snippetLeftOffset.x << "," << snippetLeftOffset.y << ", "
		<< "snippetRightOffset = " << snippetRightOffset.x << "," << snippetRightOffset.y << std::endl;
	// pre-insert: crop everything that exceeds image dimensions
	// out of image bounds?
	bool alpha_changed_must_replay =
		cropBoundaryPixels( alpha, snippet, snippetLeftOffset.x, snippetLeftOffset.y,
			snippetRightOffset.x, snippetRightOffset.y);
			
	if (alpha_changed_must_replay) {
		// copyTo is wrong, as it changes the image dimensions; copy manually
		//alpha.copyTo(orig_alpha[snippet_no]);
//		orig_alpha[snippet_no] = centerCopyImage(alpha, orig_alpha[snippet_no]);

	//	if (src_pos.x < 0) {
	//		src_pos.x = 0; 
	//		if (is_source) { source_positions[snippet_no].x = 0; } else { insert_positions[snippet_no].x = 0; } // remember cropping
	//	}
	//	if (src_pos.y < 0) {
	//		src_pos.y = 0;
	//		if (is_source) { source_positions[snippet_no].y = 0; } else { insert_positions[snippet_no].y = 0; } // remember croping
	//	}
		if (is_source)
			replayFromSource(snippet_no);
		else
			invertTargetTransform(snippet_no);
	}
	
	bool *lower_src_layers_to_replay, *lower_target_layers_to_replay;

	lower_src_layers_to_replay = new bool[snippet_no+1];
	lower_target_layers_to_replay = new bool[snippet_no+1];
	for (int i = 0; i <= snippet_no; ++i) {
		lower_src_layers_to_replay[i] = false;
		lower_target_layers_to_replay[i] = false;
	}

	// go through the alpha channel, and check every pixel if it collides with lower layers
//	int yUpper = std::min<int>(alpha.rows, img.rows-src_pos.y);
//	int xUpper = std::min<int>(alpha.cols, img.cols-src_pos.x);
	cv::Point imgOffset(src_pos.x - (alpha.cols/2), src_pos.y - (alpha.rows/2) );
	for (int y = snippetLeftOffset.y; y < snippetRightBound.y; ++y) {
		for (int x = snippetLeftOffset.x; x < snippetRightBound.x; ++x) {
			float alpha_factor = alpha[y][x] / 255.0;
			for (int c = 0; c < 3; c++) {
				img[y+imgOffset.y][x+imgOffset.x][c] = (snippet[y][x][c]*alpha_factor + img[y+imgOffset.y][x+imgOffset.x][c]*(1.0-alpha_factor));
			}

//			if (alpha[y][x] < transparency_threshold) continue; // not sufficiently opaque? skip...
			if (alpha[y][x] == 0) continue; // less than 10% opaque? skip...

			if (!is_source) { // multiple copies from one source are ok and don't need to be validated
			// note that self-occlusion is possible, i.e. that a snippet shadows itself
				for (int i = snippet_no; i >= 0; --i) { // we check only if target regions killed source regions
					// compute snippet offset from center coordinates
					cv::Point otherSnippetOffset(
						/* upper-left corner topmost-img */    imgOffset.x
						/* upper-left corner other snippet */  - (source_center_positions[i].x - (orig_alpha[i].cols/2))
						/* current position */                 + x,
						/* upper-left corner topmost-img */    imgOffset.y
						/* upper-left corner other snippet */  - (source_center_positions[i].y - (orig_alpha[i].rows/2))
						/* current position */                 + y
					);
					if ((otherSnippetOffset.x < 0) || (otherSnippetOffset.x > orig_alpha[i].cols-1)) continue;
					if ((otherSnippetOffset.y < 0) || (otherSnippetOffset.y > orig_alpha[i].rows-1)) continue;
					if ((alpha[y][x] > transparency_threshold) && (orig_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] > 0)) {
						orig_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] = 0;
						lower_src_layers_to_replay[i] = true;
					} else {
						// TODO: embarrasing situation: how to mix the alpha channels?
						// best solution: calculate color distance of the pixel?
						// For the moment: weaken overwritten alpha channel by
						// the amount of the overwriting alpha channel. Note
						// that this poorly generalizes in the case of multiple
						// layers
						if ((orig_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] > 255-alpha[y][x])) {
							orig_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] = 255-alpha[y][x];
							lower_src_layers_to_replay[i] = true;
						}
					}
				}
			}

			 // check if source or target overwrote target
			for (int i = snippet_no-1; i >= 0; --i) {
				cv::Point otherSnippetOffset(
					/* upper-left corner topmost-img */    imgOffset.x
					/* upper-left corner other snippet */  - (insert_center_positions[i].x - (snippet_alpha[i].cols/2))
					/* current position */                 + x,
					/* upper-left corner topmost-img */    imgOffset.y
					/* upper-left corner other snippet */  - (insert_center_positions[i].y - (snippet_alpha[i].rows/2))
					/* current position */                 + y
				);
				if ((otherSnippetOffset.x < 0) || (otherSnippetOffset.x > snippet_alpha[i].cols-1)) continue;
				if ((otherSnippetOffset.y < 0) || (otherSnippetOffset.y > snippet_alpha[i].rows-1)) continue;
				if ((alpha[y][x] > transparency_threshold) && (snippet_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] > 0)) {
					snippet_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] = 0;
					lower_target_layers_to_replay[i] = true;
				} else {
					// TODO: embarrasing situation: how to mix the alpha channels?
					// best solution: calculate color distance of the pixel?
					// For the moment: weaken overwritten alpha channel by
					// the amount of the overwriting alpha channel. Note
					// that this poorly generalizes in the case of multiple
					// layers
					if ((snippet_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] > 255-alpha[y][x])) {
						snippet_alpha[i][otherSnippetOffset.y][otherSnippetOffset.x] = 255-alpha[y][x];
						lower_target_layers_to_replay[i] = true;
					}
				}
			}
		}
	}

	// judgement day: which recomputation bits are set?
	for (int i = 0; i <= snippet_no; i++) {
		if (lower_src_layers_to_replay[i] && lower_target_layers_to_replay[i]) {
			twoWayReplay(i);
		} else {
			if (lower_src_layers_to_replay[i]) {
				replayFromSource(i);
			} else if (lower_target_layers_to_replay[i]) {
				invertTargetTransform(i);
			}
		}
	}

	delete[] lower_src_layers_to_replay;
	delete[] lower_target_layers_to_replay;
}


cv::Mat SpliceCore::affineTransform(cv::Mat snippet, double angle, double scale, bool inverseTransform, bool cropTransparentPixels, bool repeatCrop) {
	double my_scale = std::max<double>(1, scale);
	double diag = (1 + std::sqrt(my_scale*my_scale*snippet.rows*snippet.rows + my_scale*my_scale*snippet.cols*snippet.cols));
	cv::Mat bigEnough = cv::Mat::zeros(static_cast<int>(diag), static_cast<int>(diag), snippet.type());

	assert((bigEnough.rows >= snippet.rows) && (bigEnough.cols >= snippet.cols));

	int x0 = (diag - snippet.cols) / 2;
	int x1 = x0 + snippet.cols;
	int y0 = (diag - snippet.rows) / 2;
	int y1 = y0 + snippet.rows;
	
	cv::Mat roi = bigEnough( cv::Range(y0,y1), cv::Range(x0,x1) );

	snippet.copyTo(roi);

	// compute rotation matrix
	cv::Point2f center(diag/2, diag/2);
	cv::Mat_<float> rot = cv::getRotationMatrix2D(center, angle, scale);

	// apply rotation matrix
	cv::Mat rotated;
	cv::Size2f size(diag, diag);
	int flags = cv::INTER_CUBIC;
	if (inverseTransform) flags |= cv::WARP_INVERSE_MAP;
	cv::warpAffine(bigEnough, rotated, rot, size, flags);

	if (!cropTransparentPixels) return rotated;

	assert((snippet.type() == CV_8UC1) || (snippet.type() == CV_8UC3));
	if (repeatCrop) {
		if (snippet.type() == CV_8UC1)
			return this->cropTransparentPixels<unsigned char>(rotated, lastCropLeft.y, lastCropRight.y, lastCropLeft.x, lastCropRight.x);
		if (snippet.type() == CV_8UC3)
			return this->cropTransparentPixels<cv::Vec3b>(rotated, lastCropLeft.y, lastCropRight.y, lastCropLeft.x, lastCropRight.x);
	} else {
		if (snippet.type() == CV_8UC1) return this->cropTransparentPixels<unsigned char>(rotated);
		if (snippet.type() == CV_8UC3) return this->cropTransparentPixels<cv::Vec3b>(rotated);
	}

	std::cout << "ERROR: affineTransform() should not reach this point. Something internal is messed up." << std::endl;
	return cv::Mat();
}

void SpliceCore::add_snippet_jpeg(int snippet_no, int quality) {
	std::vector<unsigned char> buf;
	std::vector<int> params;
	params.push_back(cv::IMWRITE_JPEG_QUALITY);
	params.push_back(quality);
	bool result = cv::imencode(".jpg", snippets[snippet_no], buf, params);
	if (!result) {
		std::cerr << "SpliceCore::add_global_jpeg(): jpeg encoding failed, skipped." << std::endl;
		return;
	}
	cv::Mat_<unsigned char> wrap(buf);
	snippets[snippet_no] = cv::imdecode(wrap, cv::IMREAD_COLOR);
}

void SpliceCore::add_global_jpeg(int quality) {
	std::vector<unsigned char> buf;
	std::vector<int> params;
	params.push_back(cv::IMWRITE_JPEG_QUALITY);
	params.push_back(quality);
	bool result = cv::imencode(".jpg", img, buf, params);
	if (!result) {
		std::cerr << "SpliceCore::add_global_jpeg(): jpeg encoding failed, skipped." << std::endl;
		return;
	}
	cv::Mat_<unsigned char> wrap(buf);
	img = cv::imdecode(wrap, cv::IMREAD_COLOR);
}

cv::Mat_<cv::Vec3b> SpliceCore::getTamperedImage() {
	return img;
}

cv::Mat_<unsigned char> SpliceCore::getAlphaImage() {
	return alpha_mask;
}

void SpliceCore::add_snippet_noise(int snippet_no, double stddev, bool blur) {
	
	// fills the matrix with normally distributed random values;
	cv::Mat_<cv::Vec3f> noise(snippets[snippet_no].size());
	randn(noise, cv::Scalar::all(0.), cv::Scalar::all(stddev));
	// blur the noise a bit, kernel size is 3x3 and both sigma’s are set to 0.5
	if (blur) cv::GaussianBlur(noise, noise, cv::Size(3, 3), 0.5, 0.5);
	for (int y = 0; y < noise.rows; ++y) 
		for (int x = 0; x < noise.cols; ++x) 
			for (int c = 0; c < 3; ++c) 
				snippets[snippet_no][y][x][c] += noise[y][x][c]*255;
}

void SpliceCore::add_global_noise(double stddev, bool blur) {
	
	// fills the matrix with normally distributed random values;
	cv::Mat_<cv::Vec3f> noise(img.size());
	randn(noise, cv::Scalar::all(0.), cv::Scalar::all(stddev));
	// blur the noise a bit, kernel size is 3x3 and both sigma’s are set to 0.5
	if (blur) cv::GaussianBlur(noise, noise, cv::Size(3, 3), 0.5, 0.5);
	for (int y = 0; y < noise.rows; ++y) 
		for (int x = 0; x < noise.cols; ++x) 
			for (int c = 0; c < 3; ++c) 
				img[y][x][c] += noise[y][x][c]*255;
}
// for debugging purposes: how well does the inverse transformation work?
cv::Mat_<unsigned char> SpliceCore::computeDifferenceImage(cv::Mat_<unsigned char> img1, cv::Mat_<unsigned char> img2) {
	if ((img1.rows != img2.rows) || (img1.cols != img2.cols)) {
		std::cerr << "WARNING: computeDifferenceImage(): input images have different dimensions (img1: "
			<< img1.rows << "x" << img1.cols << ", img2: " << img2.rows << "x" << img2.cols << ")"
			<< std::endl;
	}
	int uppery = std::min<int>(img1.rows, img2.rows);
	int upperx = std::min<int>(img1.cols, img2.cols);

	cv::Mat_<unsigned char> diff = (128 + img1(cv::Range(0, uppery), cv::Range(0, upperx)) - img2(cv::Range(0, uppery), cv::Range(0, upperx)));
	return diff;	
}

// debug routine
void SpliceCore::show_synthesized_image(std::string what, int snippet_no) {
	cv::Mat_<cv::Vec3b> tmp(std::max<int>(orig_snippets[snippet_no].rows, snippets[snippet_no].rows), orig_snippets[snippet_no].cols + snippets[snippet_no].cols, cv::Vec3b(0, 0, 0));
	
	for (int y = 0; y < std::min<int>(orig_snippets[snippet_no].rows, orig_alpha[snippet_no].rows); ++y) {
		for (int x = 0; x < std::min<int>(orig_snippets[snippet_no].cols, orig_alpha[snippet_no].cols); ++x) {
			tmp[y][x][0] = orig_snippets[snippet_no][y][x][0];
			tmp[y][x][1] = orig_snippets[snippet_no][y][x][1];
			tmp[y][x][2] = orig_alpha[snippet_no][y][x];
		}
	}
	
	for (int y = 0; y < std::min<int>(snippets[snippet_no].rows, snippet_alpha[snippet_no].rows); ++y) {
		for (int x = 0; x < std::min<int>(snippets[snippet_no].cols, snippet_alpha[snippet_no].cols); ++x) {
			tmp[y][x+orig_snippets[snippet_no].cols][0] = snippets[snippet_no][y][x][0];
			tmp[y][x+orig_snippets[snippet_no].cols][1] = snippets[snippet_no][y][x][1];
			tmp[y][x+orig_snippets[snippet_no].cols][2] = snippet_alpha[snippet_no][y][x];
		}
	}
	cv::imshow(what, tmp);
	char u = 'b';
	while (u != 'a') u = cv::waitKey();
}

}}
