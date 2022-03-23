#include "header.h"
#include "eyedetect.h"
using namespace cv;

void CEyeDetect::testPossibleCentersFormula(int x, int y, const cv::Mat& weight, double gx, double gy, cv::Mat& out) {
	// for all possible centers
	for (int cy = 0; cy < out.rows; ++cy) {
		double* Or = out.ptr<double>(cy);
		const unsigned char* Wr = weight.ptr<unsigned char>(cy);
		for (int cx = 0; cx < out.cols; ++cx) {
			if (x == cx && y == cy) {
				continue;
			}
			// create a vector from the possible center to the gradient origin
			double dx = x - cx;
			double dy = y - cy;
			// normalize d
			double magnitude = sqrt((dx * dx) + (dy * dy));
			dx = dx / magnitude;
			dy = dy / magnitude;
			double dotProduct = dx * gx + dy * gy;
			dotProduct = std::max(0.0, dotProduct);
			// square and multiply by the weight
			if (kEnableWeight) {
				Or[cx] += dotProduct * dotProduct * (Wr[cx] / kWeightDivisor);
			}
			else {
				Or[cx] += dotProduct * dotProduct;
			}
		}
	}
}


void CEyeDetect::scaleToFastSize(const cv::Mat& src, cv::Mat& dst) {
	cv::resize(src, dst, cv::Size(kFastEyeWidth, (((float)kFastEyeWidth) / src.cols) * src.rows));
}

cv::Mat CEyeDetect::matrixMagnitude(const cv::Mat& matX, const cv::Mat& matY) {
	cv::Mat mags(matX.rows, matX.cols, CV_64F);
	for (int y = 0; y < matX.rows; ++y) {
		const double* Xr = matX.ptr<double>(y), * Yr = matY.ptr<double>(y);
		double* Mr = mags.ptr<double>(y);
		for (int x = 0; x < matX.cols; ++x) {
			double gX = Xr[x], gY = Yr[x];
			double magnitude = sqrt((gX * gX) + (gY * gY));
			Mr[x] = magnitude;
		}
	}
	return mags;
}


cv::Mat CEyeDetect::computeMatXGradient(const cv::Mat& mat) {
	cv::Mat out(mat.rows, mat.cols, CV_64F);

	for (int y = 0; y < mat.rows; ++y) {
		const uchar* Mr = mat.ptr<uchar>(y);
		double* Or = out.ptr<double>(y);

		Or[0] = Mr[1] - Mr[0];
		for (int x = 1; x < mat.cols - 1; ++x) {
			Or[x] = (Mr[x + 1] - Mr[x - 1]) / 2.0;
		}
		Or[mat.cols - 1] = Mr[mat.cols - 1] - Mr[mat.cols - 2];
	}

	return out;
}


bool CEyeDetect::inMat(cv::Point p, int rows, int cols) {
	return p.x >= 0 && p.x < cols&& p.y >= 0 && p.y < rows;
}

bool CEyeDetect::floodShouldPushPoint(const cv::Point& np, const cv::Mat& mat) {
	return inMat(np, mat.rows, mat.cols);
}


double CEyeDetect::computeDynamicThreshold(const cv::Mat& mat, double stdDevFactor) {
	cv::Scalar stdMagnGrad, meanMagnGrad;
	cv::meanStdDev(mat, meanMagnGrad, stdMagnGrad);
	double stdDev = stdMagnGrad[0] / sqrt(mat.rows * mat.cols);
	return stdDevFactor * stdDev + meanMagnGrad[0];
}


// returns a mask
cv::Mat CEyeDetect::floodKillEdges(cv::Mat& mat) {
	rectangle(mat, cv::Rect(0, 0, mat.cols, mat.rows), 255);

	cv::Mat mask(mat.rows, mat.cols, CV_8U, 255);
	std::queue<cv::Point> toDo;
	toDo.push(cv::Point(0, 0));
	while (!toDo.empty()) {
		cv::Point p = toDo.front();
		toDo.pop();
		if (mat.at<float>(p) == 0.0f) {
			continue;
		}
		// add in every direction
		cv::Point np(p.x + 1, p.y); // right
		if (floodShouldPushPoint(np, mat)) toDo.push(np);
		np.x = p.x - 1; np.y = p.y; // left
		if (floodShouldPushPoint(np, mat)) toDo.push(np);
		np.x = p.x; np.y = p.y + 1; // down
		if (floodShouldPushPoint(np, mat)) toDo.push(np);
		np.x = p.x; np.y = p.y - 1; // up
		if (floodShouldPushPoint(np, mat)) toDo.push(np);
		// kill it
		mat.at<float>(p) = 0.0f;
		mask.at<uchar>(p) = 0;
	}
	return mask;
}

cv::Point CEyeDetect::unscalePoint(cv::Point p, cv::Rect origSize) {
	float ratio = (((float)kFastEyeWidth) / origSize.width);
	int x = round(p.x / ratio);
	int y = round(p.y / ratio);
	return cv::Point(x, y);
}


cv::Point CEyeDetect::findEyeCenter(cv::Mat face, cv::Rect eye, std::string debugWindow) {
	cv::Mat eyeROIUnscaled = face(eye);
	cv::Mat eyeROI;
	scaleToFastSize(eyeROIUnscaled, eyeROI);
	// draw eye region
	//rectangle(face, eye, 1234);
	//-- Find the gradient
	cv::Mat gradientX = computeMatXGradient(eyeROI);
	cv::Mat gradientY = computeMatXGradient(eyeROI.t()).t();
	//-- Normalize and threshold the gradient
	// compute all the magnitudes
	cv::Mat mags = matrixMagnitude(gradientX, gradientY);
	//compute the threshold
	double gradientThresh = computeDynamicThreshold(mags, kGradientThreshold);
	//double gradientThresh = kGradientThreshold;
	//double gradientThresh = 0;
	//normalize
	for (int y = 0; y < eyeROI.rows; ++y) {
		double* Xr = gradientX.ptr<double>(y), * Yr = gradientY.ptr<double>(y);
		const double* Mr = mags.ptr<double>(y);
		for (int x = 0; x < eyeROI.cols; ++x) {
			double gX = Xr[x], gY = Yr[x];
			double magnitude = Mr[x];
			if (magnitude > gradientThresh) {
				Xr[x] = gX / magnitude;
				Yr[x] = gY / magnitude;
			}
			else {
				Xr[x] = 0.0;
				Yr[x] = 0.0;
			}
		}
	}
	//imshow(debugWindow, gradientX);
	//-- Create a blurred and inverted image for weighting
	cv::Mat weight;
	GaussianBlur(eyeROI, weight, cv::Size(kWeightBlurSize, kWeightBlurSize), 0, 0);
	for (int y = 0; y < weight.rows; ++y) {
		unsigned char* row = weight.ptr<unsigned char>(y);
		for (int x = 0; x < weight.cols; ++x) {
			row[x] = (255 - row[x]);
		}
	}
	//imshow(debugWindow,weight);
	//-- Run the algorithm!
	cv::Mat outSum = cv::Mat::zeros(eyeROI.rows, eyeROI.cols, CV_64F);
	// for each possible gradient location
	// Note: these loops are reversed from the way the paper does them
	// it evaluates every possible center for each gradient location instead of
	// every possible gradient location for every center.
	//printf("Eye Size: %ix%i\n", outSum.cols, outSum.rows);
	for (int y = 0; y < weight.rows; ++y) {
		const double* Xr = gradientX.ptr<double>(y), * Yr = gradientY.ptr<double>(y);
		for (int x = 0; x < weight.cols; ++x) {
			double gX = Xr[x], gY = Yr[x];
			if (gX == 0.0 && gY == 0.0) {
				continue;
			}
			testPossibleCentersFormula(x, y, weight, gX, gY, outSum);
		}
	}
	// scale all the values down, basically averaging them
	double numGradients = (weight.rows * weight.cols);
	cv::Mat out;
	outSum.convertTo(out, CV_32F, 1.0 / numGradients);
	//imshow(debugWindow,out);
	//-- Find the maximum point
	cv::Point maxP;
	double maxVal;
	cv::minMaxLoc(out, NULL, &maxVal, NULL, &maxP);
	//-- Flood fill the edges
	if (kEnablePostProcess) {
		cv::Mat floodClone;
		//double floodThresh = computeDynamicThreshold(out, 1.5);
		double floodThresh = maxVal * kPostProcessThreshold;
		cv::threshold(out, floodClone, floodThresh, 0.0f, cv::THRESH_TOZERO);
		if (kPlotVectorField) {
			//plotVecField(gradientX, gradientY, floodClone);
			imwrite("eyeFrame.png", eyeROIUnscaled);
		}
		cv::Mat mask = floodKillEdges(floodClone);
		//imshow(debugWindow + " Mask",mask);
		//imshow(debugWindow,out);
		// redo max
		cv::minMaxLoc(out, NULL, &maxVal, NULL, &maxP, mask);
	}
	return unscalePoint(maxP, eye);
}

CEyeDetect::CEyeDetect()
{
	rightCornerKernel = new cv::Mat(4, 6, CV_32F, kEyeCornerKernel);
	leftCornerKernel = new cv::Mat(4, 6, CV_32F);
	// flip horizontally
	cv::flip(*rightCornerKernel, *leftCornerKernel, 1);
}

CEyeDetect::~CEyeDetect()
{
	delete leftCornerKernel;
	delete rightCornerKernel;
}

// TODO implement these
cv::Mat CEyeDetect::eyeCornerMap(const cv::Mat& region, bool left, bool left2) {
	cv::Mat cornerMap;

	cv::Size sizeRegion = region.size();
	cv::Range colRange(sizeRegion.width / 4, sizeRegion.width * 3 / 4);
	cv::Range rowRange(sizeRegion.height / 4, sizeRegion.height * 3 / 4);

	cv::Mat miRegion(region, rowRange, colRange);

	cv::filter2D(miRegion, cornerMap, CV_32F,
		(left && !left2) || (!left && !left2) ? *leftCornerKernel : *rightCornerKernel);

	return cornerMap;
}


cv::Point2f CEyeDetect::findSubpixelEyeCorner(cv::Mat region, cv::Point maxP) {

	cv::Size sizeRegion = region.size();

	cv::Mat cornerMap(sizeRegion.height * 10, sizeRegion.width * 10, CV_32F);

	cv::resize(region, cornerMap, cornerMap.size(), 0, 0, cv::INTER_CUBIC);

	cv::Point maxP2;
	cv::minMaxLoc(cornerMap, NULL, NULL, NULL, &maxP2);

	return cv::Point2f(sizeRegion.width / 2 + maxP2.x / 10,
		sizeRegion.height / 2 + maxP2.y / 10);
}

cv::Point2f CEyeDetect::findEyeCorner(cv::Mat region, bool left, bool left2) {
	cv::Mat cornerMap = eyeCornerMap(region, left, left2);

	cv::Point maxP;
	cv::minMaxLoc(cornerMap, NULL, NULL, NULL, &maxP);

	cv::Point2f maxP2;
	maxP2 = findSubpixelEyeCorner(cornerMap, maxP);
	// GFTT
  //  std::vector<cv::Point2f> corners;
  //  cv::goodFeaturesToTrack(region, corners, 500, 0.005, 20);
  //  for (int i = 0; i < corners.size(); ++i) {
  //    cv::circle(region, corners[i], 2, 200);
  //  }
  //  imshow("Corners",region);

	return maxP2;
}

bool CEyeDetect::DetectCircle(Mat gray)
{
	cv::Mat img;
	resize(gray, img, Size(100, 100));
	cv::Rect face = cv::Rect(0, 0,100,100);
	cv::Point leftPupil = findEyeCenter(img, face, "Left Eye");
	//circle(img, leftPupil, 3, 200);
	//imshow("DetectCircle", img);
	//waitKey();

	return false;
 }

vector<cv::Rect> CEyeDetect::ContourDetection(cv::Mat img)
{
	cv::Mat gray;
	cv::Mat candidate;
	vector<cv::Rect> result;
	//img.copyTo(gray);
	resize(img, gray, Size(100, 100));
	//_rect = cv::Rect(0, 0, img.size().width, img.size().height);
	// Invert the source image and convert to grayscale
	//cv::cvtColor(img, gray, COLOR_BGR2GRAY);

	// Other filters
	//cv:equalizeHist(gray, gray);
	//cv::blur(gray, gray, Size(30, 30), Point(-1,-1), BORDER_DEFAULT);

	for (int threshold = 0; threshold <= 255; threshold++) {
		// Convert to binary image by thresholding it
		cv::threshold(gray, candidate, threshold, 255, cv::THRESH_BINARY_INV);

		// Find all contours
		vector<vector<cv::Point> > contours;
		cv::findContours(candidate, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

		// Fill holes in each contour
		cv::drawContours(candidate, contours, -1, CV_RGB(255, 255, 255), -1);

		for (int i = 0; i < contours.size(); i++) {
			double area = cv::contourArea(contours[i]);
			cv::Rect rect = cv::boundingRect(contours[i]);
			int radius = rect.width / 2;

			double sizeRate = (double)rect.width / (double)img.cols;

			// If contour: has round shape and has a specific size relation
			// Then it is the pupil
			if (sizeRate >= 0.25 && sizeRate <= 0.41 &&
				abs(1 - ((double)rect.width / (double)rect.height)) <= 0.2 &&
				abs(1 - (area / (CV_PI * pow(radius, 2)))) <= 0.2)
			{
				_contour = contours[i];
				_rect = cv::boundingRect(contours[i]);
				//_srcImage = img;

				result.push_back(_rect);
			}
		}
	}
	//imshow("DetectCircle", img);
	//waitKey();

	return result;
}

vector<cv::Rect> CEyeDetect::EyesPosition(cv::Mat & faceROI)
{
	vector<cv::Rect> listEye;
	cv::Mat debugFace = faceROI;
	cv::Rect face = cv::Rect(0, 0, faceROI.size().width / 2, faceROI.size().height / 2);

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(face.width * (kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);
	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width * (kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);

	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");
	// get corner regions
	cv::Rect leftCornerRegion;
	leftCornerRegion.x = max(leftPupil.x - (eye_region_width / 2),0);
	leftCornerRegion.y = max(leftPupil.y - (eye_region_height / 2), 0);
	leftCornerRegion.width = eye_region_width;
	leftCornerRegion.height = eye_region_height;

	cv::Rect rightCornerRegion;
	rightCornerRegion.x = max(rightPupil.x - (eye_region_width / 2), 0);
	rightCornerRegion.y = max(rightPupil.y - (eye_region_height / 2), 0);
	rightCornerRegion.width = eye_region_width;
	rightCornerRegion.height = eye_region_height;

	listEye.push_back(leftCornerRegion);
	listEye.push_back(rightCornerRegion);

	return listEye;
}

int CEyeDetect::findEyesSource(cv::Mat & faceROI)
{
	cv::Mat debugFace = faceROI;
	cv::Rect face = cv::Rect(0, 0, faceROI.size().width, faceROI.size().height);

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(face.width * (kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);
	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width * (kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);

	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");
	// get corner regions
	cv::Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	cv::Rect leftLeftCornerRegion(leftEyeRegion);
	leftLeftCornerRegion.width = leftPupil.x;
	leftLeftCornerRegion.height /= 2;
	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
	cv::Rect rightLeftCornerRegion(rightEyeRegion);
	rightLeftCornerRegion.width = rightPupil.x;
	rightLeftCornerRegion.height /= 2;
	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	cv::Rect rightRightCornerRegion(rightEyeRegion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;

	// change eye centers to face coordinates
	rightPupil.x += rightEyeRegion.x;
	rightPupil.y += rightEyeRegion.y;
	leftPupil.x += leftEyeRegion.x;
	leftPupil.y += leftEyeRegion.y;
	// draw eye centers

	//-- Find Eye Corners
	if (kEnableEyeCorner) {
		cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
		leftRightCorner.x += leftRightCornerRegion.x;
		leftRightCorner.y += leftRightCornerRegion.y;
		cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
		leftLeftCorner.x += leftLeftCornerRegion.x;
		leftLeftCorner.y += leftLeftCornerRegion.y;
		cv::Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
		rightLeftCorner.x += rightLeftCornerRegion.x;
		rightLeftCorner.y += rightLeftCornerRegion.y;
		cv::Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
		rightRightCorner.x += rightRightCornerRegion.x;
		rightRightCorner.y += rightRightCornerRegion.y;

	}

	/*
	cv::cvtColor(faceROI, faceROI, COLOR_GRAY2BGR);
	circle(faceROI, leftPupil, 20, Scalar(255, 0, 0), 4);
	circle(faceROI, rightPupil, 20, Scalar(255, 0, 0), 4);
	imshow("Face", faceROI);
	waitKey();
	*/
	double angle = atan2(rightPupil.y - leftPupil.y, rightPupil.x - leftPupil.x) / M_PI * 180;
	return angle;

}

cv::Point CEyeDetect::findEye(const cv::Mat & faceROI)
{
	cv::Mat debugFace = faceROI;
	cv::Rect face = cv::Rect(0, 0, faceROI.size().width, faceROI.size().height);

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(face.width * (kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);


	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");

	/*
	// get corner regions
	cv::Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	cv::Rect leftLeftCornerRegion(leftEyeRegion);
	leftLeftCornerRegion.width = leftPupil.x;
	leftLeftCornerRegion.height /= 2;
	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;

	leftPupil.x += leftEyeRegion.x;
	leftPupil.y += leftEyeRegion.y;
	// draw eye centers

	//-- Find Eye Corners
	if (kEnableEyeCorner) {
		cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
		leftRightCorner.x += leftRightCornerRegion.x;
		leftRightCorner.y += leftRightCornerRegion.y;
		cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
		leftLeftCorner.x += leftLeftCornerRegion.x;
		leftLeftCorner.y += leftLeftCornerRegion.y;

	}
	*/
	return leftPupil;
}

int CEyeDetect::GetAngleEyes(cv::Mat & faceROI)
{
	Mat result;
	Mat source;
	bool isEyeCorrect = true;
	source = faceROI(cv::Rect(20, 20, faceROI.size().width - 40, faceROI.size().height - 40));
	//faceROI.copyTo(source);
	cv::Mat debugFace = faceROI;
	cv::Rect face = cv::Rect(0, 0, source.size().width / 2, source.size().height / 2);

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(source, source, cv::Size(0, 0), sigma);
	}

	int sizeRect = 5;

	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(source, face, "Left Eye");

	cv::Rect faceRight;
	cv::Point rightPupil;

	faceRight.x = source.size().width / 2;
	faceRight.y = 0;
	faceRight.height = source.size().height / 2;
	faceRight.width = source.size().width / 2;
	rightPupil = findEyeCenter(source, faceRight, "Right Eye");
	rightPupil.x += faceRight.x;

	/*
	cv::cvtColor(faceROI, faceROI, COLOR_GRAY2BGR);
	circle(faceROI, leftPupil, 20, Scalar(255, 0, 0), 4);
	circle(faceROI, rightPupil, 20, Scalar(255, 0, 0), 4);
	imshow("Face", faceROI);
	waitKey();
	*/

	double rot_eye_left_right_top = abs(atan2(abs(rightPupil.y - leftPupil.y), abs(rightPupil.x - leftPupil.x)) / M_PI * 180);

	return rot_eye_left_right_top;
}