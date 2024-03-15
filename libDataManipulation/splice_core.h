#pragma once

#include "splice_config.h"

namespace vole { namespace cmfd_gt {

enum TRANSFORM_TYPE { ROT, SCALE, ROT_SCALE };

typedef struct {
	int snippet_no;
	TRANSFORM_TYPE type;
	double scaling_factor;
	int angle;
} TRANSFORM_OP;

class SpliceCore {
public:
	SpliceCore(
		cv::Mat_<cv::Vec3b> &orig,
		std::vector<cv::Mat_<cv::Vec3b> > &snippets,
		std::vector<cv::Mat_<unsigned char> > &snippet_alpha,
		std::vector<int> &source_positions,
		std::vector<int> &insert_positions,
		int transparency_threshold
	);
	
	void add_snippet_noise(int snippet_no, double stddev, bool blur = false);
	void add_global_noise(double stddev, bool blur = false);
	void add_rotation(int snippet_no, int angle, bool is_replay = false);
	void add_scaling(int snippet_no, double factor, bool is_replay = false);
	void add_rotation_and_scaling(int snippet_no, int angle, double factor, bool is_replay = false);
	void add_snippet_jpeg(int snippet_no, int quality);
	void add_global_jpeg(int quality);
	void splice();

	cv::Mat_<cv::Vec3b> getTamperedImage();
	cv::Mat_<unsigned char> getAlphaImage();

protected:
	cv::Mat_<cv::Vec3b> img;         					// image to modify 
	std::vector<cv::Mat_<cv::Vec3b> > snippets;          // modified snippets - for target area
	std::vector<cv::Mat_<cv::Vec3b> > orig_snippets;     // unmodified snippets, for source area
	std::vector<cv::Mat_<unsigned char> > snippet_alpha; // modified alpha - for target area
	std::vector<cv::Mat_<unsigned char> > orig_alpha;    // unmodified alpha, for source area
	std::vector<cv::Mat_<unsigned char> > backup_snippet_alpha; // always unchanged, for replay
	std::vector<cv::Point> source_positions;
	std::vector<cv::Point> insert_positions;
	std::vector<cv::Point> source_center_positions;
	std::vector<cv::Point> insert_center_positions;
	cv::Mat_<unsigned char> alpha_mask;
	std::vector<std::vector<TRANSFORM_OP> > transform_log;

	cv::Point lastCropLeft, lastCropRight;
	int transparency_threshold;

	void splice(int snippet_no, cv::Point src_pos, bool is_source);
	cv::Mat affineTransform(cv::Mat snippet, double angle, double scale, bool inverseTransform = false, bool cropTransparentPixels = true, bool repeatCrop = false);

	bool isZero(unsigned char c) { return c == 0; }
	bool isZero(cv::Vec3b c) { return ((c[0] == 0) && (c[1] == 0) && (c[2] == 0)); }
	cv::Mat_<unsigned char> computeDifferenceImage(cv::Mat_<unsigned char> img1, cv::Mat_<unsigned char> img2);
	bool cropBoundaryPixels(cv::Mat_<unsigned char> &alpha, cv::Mat_<cv::Vec3b> &snippet, int xLeft, int yTop, int xRight, int yBottom);
	void replayFromSource(int snippet_no);
	void invertTargetTransform(int snippet_no);
	void twoWayReplay(int snippet_no);
	cv::Mat_<unsigned char> centerCopyImage(cv::Mat_<unsigned char> copySource, cv::Mat_<unsigned char> copyTarget, bool onlyMatting = false); 

	// debug routine
	void show_synthesized_image(std::string what, int snippet_no);

	template <class T>
	cv::Mat_<T> cropTransparentPixels(cv::Mat_<T> snippet, int miny, int maxy, int minx, int maxx)
	{
		return snippet(cv::Range(miny, maxy), cv::Range(minx, maxx));
	}

	template <class T>
	cv::Mat_<T> cropTransparentPixels(cv::Mat_<T> snippet)
	{
		int minx = 0;
		for (int x = 0; x < snippet.cols; ++x)
			for (int y = 0; y < snippet.rows; ++y)
				if (!isZero(snippet[y][x])) { minx = x; goto GotMinX; }
		GotMinX:
			int maxx = 0;
			for (int x = snippet.cols-1; x >= 0; --x)
				for (int y = 0; y < snippet.rows; ++y)
					if (!isZero(snippet[y][x])) { maxx = x; goto GotMaxX; }
		GotMaxX:
			int miny = 0;
			for (int y = 0; y < snippet.rows; ++y)
				for (int x = 0; x < snippet.cols; ++x)
					if (!isZero(snippet[y][x])) { miny = y; goto GotMinY; }
		GotMinY:
			int maxy = 0;
			for (int y = snippet.rows-1; y > 0; --y)
				for (int x = 0; x < snippet.cols; ++x)
					if (!isZero(snippet[y][x])) { maxy = y; goto GotMaxY; }
		GotMaxY:
		lastCropLeft.x = minx; lastCropLeft.y = miny;
		lastCropRight.x = maxx; lastCropRight.y = maxy;
		return snippet(cv::Range(miny, maxy), cv::Range(minx, maxx));
	}
};

}}

