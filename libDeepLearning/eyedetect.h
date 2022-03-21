#pragma once

class CEyeDetect
{
public:
	CEyeDetect();
	~CEyeDetect();
	vector<cv::Rect> EyesPosition(cv::Mat & faceROI);
	int findEyesSource(cv::Mat & faceROI);
	int findEyes(cv::Mat & faceROI);
	int GetAngleEyes(cv::Mat & faceROI);

private:
	bool DetectCircle(cv::Mat src);
	void testPossibleCentersFormula(int x, int y, const cv::Mat& weight, double gx, double gy, cv::Mat& out);
	cv::Point2f findEyeCorner(cv::Mat region, bool left, bool left2);
	cv::Point2f findSubpixelEyeCorner(cv::Mat region, cv::Point maxP);
	cv::Mat eyeCornerMap(const cv::Mat& region, bool left, bool left2);
	cv::Point findEyeCenter(cv::Mat face, cv::Rect eye, std::string debugWindow);
	cv::Point unscalePoint(cv::Point p, cv::Rect origSize);
	cv::Mat floodKillEdges(cv::Mat& mat);
	bool inMat(cv::Point p, int rows, int cols);
	bool floodShouldPushPoint(const cv::Point& np, const cv::Mat& mat);
	double computeDynamicThreshold(const cv::Mat& mat, double stdDevFactor);
	cv::Mat computeMatXGradient(const cv::Mat& mat);
	cv::Mat matrixMagnitude(const cv::Mat& matX, const cv::Mat& matY);
	void scaleToFastSize(const cv::Mat& src, cv::Mat& dst);
	vector<cv::Rect> ContourDetection(cv::Mat img);
	// Debugging
	const bool kPlotVectorField = false;

	// Size constants
	const int kEyePercentTop = 25;
	const int kEyePercentSide = 13;
	const int kEyePercentHeight = 30;
	const int kEyePercentWidth = 35;

	// Preprocessing
	const bool kSmoothFaceImage = false;
	const float kSmoothFaceFactor = 0.005;

	// Algorithm Parameters
	const int kFastEyeWidth = 40;
	const int kWeightBlurSize = 5;
	const bool kEnableWeight = true;
	const float kWeightDivisor = 1.0;
	const double kGradientThreshold = 80.0;

	// Postprocessing
	const bool kEnablePostProcess = true;
	const float kPostProcessThreshold = 0.90;

	// Eye Corner
	const bool kEnableEyeCorner = false;

	// not constant because stupid opencv type signatures
	float kEyeCornerKernel[4][6] = {
	  {-1,-1,-1, 1, 1, 1},
	  {-1,-1,-1,-1, 1, 1},
	  {-1,-1,-1,-1, 0, 3},
	  { 1, 1, 1, 1, 1, 1},
	};

	cv::Mat* leftCornerKernel;
	cv::Mat* rightCornerKernel;

	cv::Rect _rect;
	vector<cv::Point> _contour;
};