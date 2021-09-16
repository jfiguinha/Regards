
#include <header.h>
#include "FiltreEffetCPU.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/core.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/imgproc.hpp>
#include "InterpolationBicubic.h"
#include "InterpolationFilters.h"
#include "PerlinNoise.h"
#include "LensFlare.h"
#include "MotionBlur.h"
#include "Filtre.h"
#include "RedEye.h"
#include "Wave.h"
#include <ImageLoadingFormat.h>
#include <OpenCVEffect.h>
#include <hqdn3d.h>
#include "MeanShift.h"
#include <RegardsBitmap.h>
#include <fstream>
#include "bm3dfilter.h"
#include <FileUtility.h>
#include <FaceDetector.h>


using namespace Regards::OpenCV;

extern float value[256];
using namespace Regards::FiltreEffet;

class CFiltreEffetCPUImpl
{
public:
	//Vignette effect
	static void generateGradient(Mat& mask, const double& radius, const double& power);
	static double getMaxDisFromCorners(const Size& imgSize, const Point& center);
	static double dist(Point a, Point b);
};


CFiltreEffetCPU::CFiltreEffetCPU(const CRgbaquad& back_color, CImageLoadingFormat* bitmap)
	: IFiltreEffet(back_color)
{
	this->backColor = back_color;
	bitmapOut = nullptr;
	pBitmap = bitmap->GetRegardsBitmap();
}

int CFiltreEffetCPU::GetWidth()
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			return bitmapOut->GetBitmapWidth();
	}
	else
	{
		if (pBitmap != nullptr)
			return pBitmap->GetBitmapWidth();
	}
	return 0;
}

int CFiltreEffetCPU::GetHeight()
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			return bitmapOut->GetBitmapHeight();
	}
	else
	{
		if (pBitmap != nullptr)
			return pBitmap->GetBitmapHeight();
	}
	return 0;
}

CRegardsBitmap* CFiltreEffetCPU::GetPtBitmap()
{
	return pBitmap;
}


int CFiltreEffetCPU::BokehEffect(const int& radius, const int& boxsize, const int & nbFace, const wxRect & listFace)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;
	Mat dst;

	pBitmap->VertFlipBuf();
	
	Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
	cvtColor(image, dst, COLOR_BGRA2BGR);

	if(nbFace > 0)
	{
		try
		{
			//Resize the rectangle
			wxRect rectCopy = listFace;
			cv::Rect rect;
			int width = listFace.width;
			rect.width = listFace.width * 2;
			rect.x = listFace.x - (width / 2);
			rect.y = 0;
			rect.height = dst.rows;

			Mat blur;
			cv::GaussianBlur(dst, blur, cv::Size(radius, boxsize), 0);


			int maxWidth = dst.cols;
			int maxHeight = dst.rows;
			if ((rect.width + rect.x) > maxWidth)
				rect.width = maxWidth - rect.x;

			if ((rect.height + rect.y) > maxHeight)
				rect.height = maxHeight - rect.y;
			
			cv::Mat croppedImage = dst(rect);
			Mat src_gray;
			Mat detected_edges;
			Mat output;

			Mat blur_crop;
			cv::GaussianBlur(croppedImage, blur_crop, cv::Size(3, 3), 0);
			//medianBlur(croppedImage, blur_crop, 3);

			cvtColor(blur_crop, src_gray, COLOR_BGR2GRAY);

			// apply your filter
			Canny(src_gray, src_gray, 200, 100);

			// find the contours
			vector< vector<Point> > contours;
			findContours(src_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

			// you could also reuse img1 here
			Mat mask = Mat::zeros(src_gray.rows, src_gray.cols, CV_8UC1);
			//threshold(mask, mask, 0, 255, THRESH_BINARY_INV);
			// CV_FILLED fills the connected components found
			drawContours(mask, contours, -1, Scalar(255), FILLED);



			/*
			 Before drawing all contours you could also decide
			 to only draw the contour of the largest connected component
			 found. Here's some commented out code how to do that:
			*/

			//    vector<double> areas(contours.size());
			//    for(int i = 0; i < contours.size(); i++)
			//        areas[i] = contourArea(Mat(contours[i]));
			//    double max;
			//    Point maxPosition;
			//    minMaxLoc(Mat(areas),0,&max,0,&maxPosition);
			//    drawContours(mask, contours, maxPosition.y, Scalar(1), CV_FILLED);

				// let's create a new image now
			//Mat crop(src_gray.rows, src_gray.cols, CV_8UC3);


			cv::GaussianBlur(croppedImage, blur_crop, cv::Size(radius, boxsize), 0);


			// normalize so imwrite(...)/imshow(...) shows the mask correctly!
			normalize(mask.clone(), mask, 0.0, 255.0, cv::NORM_MINMAX, CV_8UC1);


			int oldx = 0;
			//bitwise_not(mask, mask);
			//std::map<int, int> listOfPoint;

			for (int y = 0; y < rect.height; y++)
			{
				for (int x = 0; x < rect.width; x++)
				{
					uchar color = mask.at<uchar>(y, x);
					if (color == 255)
					{
						if (oldx != 0)
						{
							if (x < (oldx * 0.98))
								x = oldx * 0.98;
						}

						if (x > (rectCopy.width * 0.8))
							x = (rectCopy.width * 0.8);
						//stop searching
						for (int _x = 0; _x < x; _x++)
						{
							mask.at<uchar>(y, _x) = 0;
						}

						for (int _x = x; _x < rect.width - x; _x++)
						{
							mask.at<uchar>(y, _x) = 255;
						}

						for (int _x = rect.width - x; _x < rect.width; _x++)
						{
							mask.at<uchar>(y, _x) = 0;
						}
						oldx = x;

						break;
					}
				}
			}


			for (int y = rect.height - 1; y >= 0; y--)
			{
				for (int x = 0; x < rect.width; x++)
				{
					uchar color = mask.at<uchar>(y, x);
					if (color == 255)
					{
						if (oldx != 0)
						{
							if (x < (oldx * 0.98))
								x = oldx * 0.98;
						}
						//stop searching
						for (int _x = 0; _x < x; _x++)
						{
							mask.at<uchar>(y, _x) = 0;
						}

						for (int _x = x; _x < rect.width - x; _x++)
						{
							mask.at<uchar>(y, _x) = 255;
						}

						for (int _x = rect.width - x; _x < rect.width; _x++)
						{
							mask.at<uchar>(y, _x) = 0;
						}
						oldx = x;

						break;
					}
				}
			}

			// show the images
			//imshow("original", croppedImage);
			//imshow("mask", mask);
			//imshow("canny", src_gray);
			//imshow("cropped", blur_crop);

			//waitKey();
			//destroyAllWindows();

			// and copy the magic apple
			croppedImage.copyTo(blur_crop, mask);

			Rect copy(rect.x, rect.y, croppedImage.cols, croppedImage.rows);

			blur_crop.copyTo(blur(copy));

			cvtColor(blur, dst, COLOR_BGR2BGRA);
			bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		}
		catch(...)
		{
			
		}


	}
	
	dst.release();
	image.release();
	bitmap->VertFlipBuf();
	return 0;
}

int CFiltreEffetCPU::OilPaintingEffect(const int& size, const int& dynRatio)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;
	Mat dst;
	Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
	cvtColor(image, dst, COLOR_BGRA2BGR);
	xphoto::oilPainting(dst, image, size, dynRatio, COLOR_BGR2Lab);
	cvtColor(image, dst, COLOR_BGR2BGRA);
	bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
	dst.release();
	image.release();
	return 0;
}

// Helper function to calculate the distance between 2 points.
double CFiltreEffetCPUImpl::dist(Point a, Point b)
{
	return sqrt(pow(static_cast<double>(a.x - b.x), 2) + pow(static_cast<double>(a.y - b.y), 2));
}

// Helper function that computes the longest distance from the edge to the center point.
double CFiltreEffetCPUImpl::getMaxDisFromCorners(const Size& imgSize, const Point& center)
{
	// given a rect and a line
	// get which corner of rect is farthest from the line

	std::vector<Point> corners(4);
	corners[0] = Point(0, 0);
	corners[1] = Point(imgSize.width, 0);
	corners[2] = Point(0, imgSize.height);
	corners[3] = Point(imgSize.width, imgSize.height);

	double maxDis = 0;
	for (int i = 0; i < 4; ++i)
	{
		double dis = dist(corners[i], center);
		if (maxDis < dis)
			maxDis = dis;
	}

	return maxDis;
}

double fastCos(double x)
{
	x += 1.57079632;
	if (x > 3.14159265)
		x -= 6.28318531;
	if (x < 0)
		return 1.27323954 * x + 0.405284735 * x * x;
	return 1.27323954 * x - 0.405284735 * x * x;
}

// Helper function that creates a gradient image.   
// firstPt, radius and power, are variables that control the artistic effect of the filter.
void CFiltreEffetCPUImpl::generateGradient(Mat& mask, const double& radius, const double& power)
{
	auto firstPt = Point(mask.size().width / 2, mask.size().height / 2);
	double maxImageRad = (radius * getMaxDisFromCorners(mask.size(), firstPt));
	double maxImageRadPower = maxImageRad * power;
	mask.setTo(Scalar(1));
	for (int i = 0; i < mask.rows; i++)
	{
		for (int j = 0; j < mask.cols; j++)
		{
			double temp = dist(firstPt, Point(j, i));

			if (temp > maxImageRad)
			{
				mask.at<double>(i, j) = 0;
			}
			else
			{
				if (temp > maxImageRadPower)
				{
					double max = maxImageRad - maxImageRadPower;
					const double value = temp - maxImageRadPower;
					mask.at<double>(i, j) = (max - value) / max;
				}
				else
					mask.at<double>(i, j) = 1;
			}
		}
	}
}


double dist(double ax, double ay, double bx, double by)
{
	return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}


int CFiltreEffetCPU::VignetteEffect(const double& radius, const double& power)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(src, src, COLOR_BGRA2BGR);
		//cv::Mat dst = Mat::zeros(src.size(), src.type());
		/*
			Mat kernel_X = getGaussianKernel(src.cols, (float)(radius / 100.0) * (float)src.cols);
			Mat kernel_Y = getGaussianKernel(src.rows, (float)(radius / 100.0) * (float)src.rows);
			Mat kernel_X_transpose;
			transpose(kernel_X, kernel_X_transpose);
			Mat kernel = kernel_Y * kernel_X_transpose;
	
			vector<Mat> bgr_planes;
			split(src, bgr_planes);	
	
			Mat mask_v;
			normalize(kernel, mask_v, 0, 1, NORM_MINMAX);
	
			for (int i = 0; i < 3; i++)
			{
				Mat proc_img;
				bgr_planes[i].convertTo(proc_img, CV_64F);
				multiply(mask_v, proc_img, proc_img);
				convertScaleAbs(proc_img, proc_img);
				proc_img.convertTo(bgr_planes[i], CV_8U);
			}
	
			cv::merge(bgr_planes, dst);
			*/


		auto firstPt = Point(src.size().width / 2, src.size().height / 2);
		double maxImageRad = static_cast<float>(radius / 100.0) * CFiltreEffetCPUImpl::getMaxDisFromCorners(
			src.size(), firstPt);
		double maxImageRadPower = maxImageRad * static_cast<float>(power / 100.0);

		Mat labImg(src.size(), CV_8UC3);

		cvtColor(src, labImg, COLOR_BGR2Lab);

		for (int row = 0; row < src.size().height; row++)
		{
			for (int col = 0; col < src.size().width; col++)
			{

				double temp = CFiltreEffetCPUImpl::dist(firstPt, Point(col, row));
				if (temp > maxImageRad)
					labImg.at<Vec3b>(row, col)[0] = 0;
				else
				{
					if (temp > maxImageRadPower)
					{
						double max = maxImageRad - maxImageRadPower;
						double _value = temp - maxImageRadPower;
						labImg.at<Vec3b>(row, col)[0] *= (max - _value) / max;
					}
				}
			}
		}

		cvtColor(labImg, src, COLOR_Lab2BGR);
		cvtColor(src, src, COLOR_BGR2BGRA);
		bitmap->SetBitmap(src.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());

		labImg.release();
		src.release();
	}

	return 0;
}


void CFiltreEffetCPU::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			delete bitmapOut;
		bitmapOut = bitmap->GetRegardsBitmap();
	}
	else
	{
		if (pBitmap != nullptr)
			delete pBitmap;
		pBitmap = bitmap->GetRegardsBitmap();
	}
}

int CFiltreEffetCPU::RedEye()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRedEye redeye;
		return redeye.RemoveRedEye(bitmap);
	}
	return 0;
}

CRegardsFloatBitmap* CFiltreEffetCPU::GetFloatBitmap(const bool& source)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	CImageLoadingFormat imageLoading(false);
	imageLoading.SetPicture(bitmap);
	return imageLoading.GetFloatBitmap(true);
}

int CFiltreEffetCPU::WaveFilter(int x, int y, short height, int scale, int radius)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto waveFilter = new CWaveFilter();
		waveFilter->ProcessEffect(bitmap, x, y, height, scale, radius);
		delete waveFilter;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::Bm3d(const int& fSigma)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		const int nbProcess = thread::hardware_concurrency();
		auto bm3dFilter = new CBm3DFilter(bitmap, value[fSigma]);
		bm3dFilter->DetermineData(nbProcess - 1);
		bm3dFilter->ExecuteFilter();
		CRegardsBitmap* copyBitmap = bm3dFilter->GetResult();
		memcpy(bitmap->GetPtBitmap(), copyBitmap->GetPtBitmap(), copyBitmap->GetBitmapSize());
		delete bm3dFilter;
		return 0;
	}
	return -1;
}

// Remove black dots (upto 4x4 in size) of noise from a pure black & white image.
// ie: The input image should be mostly white (255) and just contains some black (0) noise
// in addition to the black (0) edges.
void CFiltreEffetCPU::RemovePepperNoise(Mat& mask)
{
	// For simplicity, ignore the top & bottom row border.
	for (int y = 2; y < mask.rows - 2; y++)
	{
		// Get access to each of the 5 rows near this pixel.
		uchar* pThis = mask.ptr(y);
		uchar* pUp1 = mask.ptr(y - 1);
		uchar* pUp2 = mask.ptr(y - 2);
		uchar* pDown1 = mask.ptr(y + 1);
		uchar* pDown2 = mask.ptr(y + 2);

		// For simplicity, ignore the left & right row border.
		pThis += 2;
		pUp1 += 2;
		pUp2 += 2;
		pDown1 += 2;
		pDown2 += 2;
		for (int x = 2; x < mask.cols - 2; x++)
		{
			uchar v = *pThis; // Get the current pixel value (either 0 or 255).
			// If the current pixel is black, but all the pixels on the 2-pixel-radius-border are white
			// (ie: it is a small island of black pixels, surrounded by white), then delete that island.
			if (v == 0)
			{
				bool allAbove = *(pUp2 - 2) && *(pUp2 - 1) && *(pUp2) && *(pUp2 + 1) && *(pUp2 + 2);
				bool allLeft = *(pUp1 - 2) && *(pThis - 2) && *(pDown1 - 2);
				bool allBelow = *(pDown2 - 2) && *(pDown2 - 1) && *(pDown2) && *(pDown2 + 1) && *(pDown2 + 2);
				bool allRight = *(pUp1 + 2) && *(pThis + 2) && *(pDown1 + 2);
				bool surroundings = allAbove && allLeft && allBelow && allRight;
				if (surroundings == true)
				{
					// Fill the whole 5x5 block as white. Since we know the 5x5 borders
					// are already white, just need to fill the 3x3 inner region.
					*(pUp1 - 1) = 255;
					*(pUp1 + 0) = 255;
					*(pUp1 + 1) = 255;
					*(pThis - 1) = 255;
					*(pThis + 0) = 255;
					*(pThis + 1) = 255;
					*(pDown1 - 1) = 255;
					*(pDown1 + 0) = 255;
					*(pDown1 + 1) = 255;
				}
				// Since we just covered the whole 5x5 block with white, we know the next 2 pixels
				// won't be black, so skip the next 2 pixels on the right.
				pThis += 2;
				pUp1 += 2;
				pUp2 += 2;
				pDown1 += 2;
				pDown2 += 2;
			}
			// Move to the next pixel.
			pThis++;
			pUp1++;
			pUp2++;
			pDown1++;
			pDown2++;
		}
	}
}

int CFiltreEffetCPU::CartoonifyImage(const int& mode)
{
	bool sketchMode = false;
	bool alienMode = false;
	bool evilMode = false;

	switch (mode)
	{
	case 1:
		sketchMode = true;
		break;
	case 2:
		alienMode = true;
		break;
	case 3:
		evilMode = true;
		break;
	default:
		break;
	}

	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	Mat dst;
	Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());

	// Convert from BGR color to Grayscale
	Mat srcGray;
	cvtColor(src, srcGray, COLOR_BGRA2GRAY);

	// Remove the pixel noise with a good Median filter, before we start detecting edges.
	medianBlur(srcGray, srcGray, 7);

	Size size = src.size();
	auto mask = Mat(size, CV_8U);
	auto edges = Mat(size, CV_8U);
	if (!evilMode)
	{
		// Generate a nice edge mask, similar to a pencil line drawing.
		Laplacian(srcGray, edges, CV_8U, 5);
		threshold(edges, mask, 80, 255, THRESH_BINARY_INV);
		// Mobile cameras usually have lots of noise, so remove small
		// dots of black noise from the black & white edge mask.
		RemovePepperNoise(mask);
	}
	else
	{
		// Evil mode, making everything look like a scary bad guy.
		// (Where "srcGray" is the original grayscale image plus a medianBlur of size 7x7).
		Mat edges2;
		Scharr(srcGray, edges, CV_8U, 1, 0);
		Scharr(srcGray, edges2, CV_8U, 1, 0, -1);
		edges += edges2;
		threshold(edges, mask, 12, 255, THRESH_BINARY_INV);
		medianBlur(mask, mask, 3);
		edges2.release();
	}
	//imshow("edges", edges);
	//imshow("mask", mask);

	// For sketch mode, we just need the mask!
	if (sketchMode)
	{
		// The output image has 3 channels, not a single channel.
		cvtColor(mask, dst, COLOR_GRAY2BGRA);
		bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());

		dst.release();
		mask.release();
		src.release();
		edges.release();
		return -1;
	}

	// Do the bilateral filtering at a shrunken scale, since it
	// runs so slowly but doesn't need full resolution for a good effect.
	Size smallSize;
	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;

	Mat srcBgr;
	cvtColor(src, srcBgr, COLOR_BGRA2BGR);
	auto smallImg = Mat(smallSize, CV_8UC3);
	resize(srcBgr, smallImg, smallSize, 0, 0, INTER_LINEAR);

	// Perform many iterations of weak bilateral filtering, to enhance the edges
	// while blurring the flat regions, like a cartoon.
	auto tmp = Mat(smallSize, CV_8UC3);
	int repetitions = 7; // Repetitions for strong cartoon effect.
	for (int i = 0; i < repetitions; i++)
	{
		int d = 9; // Filter size. Has a large effect on speed.
		double sigmaColor = 9; // Filter color strength.
		double sigmaSpace = 7; // Positional strength. Effects speed.
		bilateralFilter(smallImg, tmp, d, sigmaColor, sigmaSpace);
		bilateralFilter(tmp, smallImg, d, sigmaColor, sigmaSpace);
	}

	if (alienMode)
	{
		// Apply an "alien" filter, when given a shrunken image and the full-res edge mask.
		// Detects the color of the pixels in the middle of the image, then changes the color of that region to green.
		ChangeFacialSkinColor(smallImg, edges);
	}

	// Go back to the original scale.
	cvtColor(smallImg, srcBgr, COLOR_BGR2BGRA);
	resize(srcBgr, src, size, 0, 0, INTER_LINEAR);

	// Use the blurry cartoon image, except for the strong edges that we will leave black.
	src.copyTo(dst, mask);
	bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());

	dst.release();
	mask.release();
	src.release();
	tmp.release();
	srcBgr.release();
	smallImg.release();
	edges.release();
	return 0;
}

// Apply an "alien" filter, when given a shrunken BGR image and the full-res edge mask.
// Detects the color of the pixels in the middle of the image, then changes the color of that region to green.
void CFiltreEffetCPU::ChangeFacialSkinColor(Mat smallImgBGR, Mat bigEdges)
{
	// Convert to Y'CrCb color-space, since it is better for skin detection and color adjustment.
	auto yuv = Mat(smallImgBGR.size(), CV_8UC3);
	cvtColor(smallImgBGR, yuv, COLOR_BGR2YCrCb);

	// The floodFill mask has to be 2 pixels wider and 2 pixels taller than the small image.
	// The edge mask is the full src image size, so we will shrink it to the small size,
	// storing into the floodFill mask data.
	int sw = smallImgBGR.cols;
	int sh = smallImgBGR.rows;
	Mat maskPlusBorder = Mat::zeros(sh + 2, sw + 2, CV_8U);
	Mat mask = maskPlusBorder(Rect(1, 1, sw, sh)); // mask is a ROI in maskPlusBorder.
	resize(bigEdges, mask, smallImgBGR.size());

	// Make the mask values just 0 or 255, to remove weak edges.
	threshold(mask, mask, 80, 255, THRESH_BINARY);
	// Connect the edges together, if there was a pixel gap between them.
	dilate(mask, mask, Mat());
	erode(mask, mask, Mat());
	//imshow("constraints for floodFill", mask);

	// YCrCb Skin detector and color changer using multiple flood fills into a mask.
	// Apply flood fill on many points around the face, to cover different shades & colors of the face.
	// Note that these values are dependent on the face outline, drawn in drawFaceStickFigure().
	const int NUM_SKIN_POINTS = 6;
	Point skinPts[NUM_SKIN_POINTS];
	skinPts[0] = Point(sw / 2, sh / 2 - sh / 6);
	skinPts[1] = Point(sw / 2 - sw / 11, sh / 2 - sh / 6);
	skinPts[2] = Point(sw / 2 + sw / 11, sh / 2 - sh / 6);
	skinPts[3] = Point(sw / 2, sh / 2 + sh / 16);
	skinPts[4] = Point(sw / 2 - sw / 9, sh / 2 + sh / 16);
	skinPts[5] = Point(sw / 2 + sw / 9, sh / 2 + sh / 16);
	// Skin might be fairly dark, or slightly less colorful.
	// Skin might be very bright, or slightly more colorful but not much more blue.
	const int LOWER_Y = 60;
	const int UPPER_Y = 80;
	const int LOWER_Cr = 25;
	const int UPPER_Cr = 15;
	const int LOWER_Cb = 20;
	const int UPPER_Cb = 15;
	auto lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
	auto upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);
	// Instead of drawing into the "yuv" image, just draw 1's into the "maskPlusBorder" image, so we can apply it later.
	// The "maskPlusBorder" is initialized with the edges, because floodFill() will not go across non-zero mask pixels.
	Mat edgeMask = mask.clone(); // Keep an duplicate copy of the edge mask.
	for (int i = 0; i < NUM_SKIN_POINTS; i++)
	{
		// Use the floodFill() mode that stores to an external mask, instead of the input image.
		const int flags = 4 | FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY;
		floodFill(yuv, maskPlusBorder, skinPts[i], Scalar(), nullptr, lowerDiff, upperDiff, flags);
	}

	// After the flood fill, "mask" contains both edges and skin pixels, whereas
	// "edgeMask" just contains edges. So to get just the skin pixels, we can remove the edges from it.
	mask -= edgeMask;
	// "mask" now just contains 1's in the skin pixels and 0's for non-skin pixels.

	// Change the color of the skin pixels in the given BGR image.
	int Red = 0;
	int Green = 70;
	int Blue = 0;
	add(smallImgBGR, Scalar(Blue, Green, Red), smallImgBGR, mask);

	edgeMask.release();
	yuv.release();
	maskPlusBorder.release();
	mask.release();
}


int CFiltreEffetCPU::MeanShift(const float& fSpatialRadius, const float& fColorRadius)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat dst;
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(src, dst, COLOR_BGRA2BGR);
		cvtColor(dst, dst, COLOR_BGR2Lab);
		// Initilize Mean Shift with spatial bandwith and color bandwith
		CMeanShift msProcess(fSpatialRadius, fColorRadius);
		//MSProc(fSpatialRadius, fColorRadius);
		// Filtering Process
		msProcess.MSFiltering(dst);
		cvtColor(dst, dst, COLOR_Lab2RGB);
		cvtColor(dst, dst, COLOR_BGR2BGRA);
		bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());

		dst.release();
		src.release();
	}
	return 0;
}

int CFiltreEffetCPU::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat dst;
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(src, dst, COLOR_BGRA2BGR);
		bilateralFilter(dst, src, fSize, sigmaX, sigmaP, BORDER_DEFAULT);
		cvtColor(src, dst, COLOR_BGR2BGRA);
		bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dst.release();
		src.release();
	}
	return -1;
}

int CFiltreEffetCPU::NlmeansFilter(const int& h, const int& templateWindowSize, const int& searchWindowSize)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		//auto nlmeans = new CNlmeans(fsize, bsize, sigma);
		//nlmeans->SetParameter(bitmap, backColor);
		//nlmeans->Compute();
		//delete nlmeans;
		Mat dst;
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(src, dst, COLOR_BGRA2BGR);
		fastNlMeansDenoisingColored(dst, src, h, h, templateWindowSize, searchWindowSize);
		cvtColor(src, dst, COLOR_BGR2BGRA);
		bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dst.release();
		src.release();
		return 0;
	}
	return -1;
}


int CFiltreEffetCPU::SharpenMasking(const float& sharpness)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto sharpenMasking = new CSharpenMasking(sharpness);
		sharpenMasking->SetParameter(bitmap, backColor);
		sharpenMasking->Compute();
		delete sharpenMasking;
		return 0;
	}
	return -1;
}

CFiltreEffetCPU::~CFiltreEffetCPU()
{
	if (bitmapOut != nullptr)
		delete bitmapOut;
	bitmapOut = nullptr;

	if (pBitmap != nullptr)
		delete pBitmap;
	pBitmap = nullptr;

	if (hq3d != nullptr)
		delete hq3d;
}

wxImage CFiltreEffetCPU::GetwxImage()
{
	int width;
	int height;
	unsigned char* data;
	if (bitmapOut != nullptr)
	{
		width = bitmapOut->GetBitmapWidth();
		height = bitmapOut->GetBitmapHeight();
		data = bitmapOut->GetPtBitmap();
	}
	else
	{
		width = pBitmap->GetBitmapWidth();
		height = pBitmap->GetBitmapHeight();
		data = pBitmap->GetPtBitmap();
	}
	wxImage anImage(width, height, false);
	unsigned char* dataOut = anImage.GetData();
	int size = width * height;
	//pBitmap->VertFlipBuf();

	if (data != nullptr)
	{
		tbb::parallel_for(tbb::blocked_range<int>(0, size),
			[&](tbb::blocked_range<int> r)
			{
				for (int i = 0; i < size; ++i)
				{
					const int y = i / width;
					const int x = i - (y * width);
					const int calcul = (height - y - 1) * width + x;
					dataOut[i * 3] = data[(calcul << 2) + 2]; // R
					dataOut[i * 3 + 1] = data[(calcul << 2) + 1]; // G
					dataOut[i * 3 + 2] = data[(calcul << 2)]; // B
				}
			});
	}
	return anImage;
}

wxImage CFiltreEffetCPU::GetwxImage(CRegardsBitmap* bitmap)
{
	const int width = bitmap->GetBitmapWidth();
	const int height = bitmap->GetBitmapHeight();
	unsigned char* data = bitmap->GetPtBitmap();
	wxImage anImage(width, height, false);
	unsigned char* dataOut = anImage.GetData();
	const int size = width * height;
	//pBitmap->VertFlipBuf();

	if (data != nullptr)
	{
		tbb::parallel_for(tbb::blocked_range<int>(0, size),
			[&](tbb::blocked_range<int> r)
			{
				for (int i = 0; i < size; ++i)
				{
					const int y = i / width;
					const int x = i - (y * width);
					const int pos = i * 3;
					const int calcul = ((height - y - 1) * width + x) << 2;
					//int calcul = y * width + x;
					dataOut[pos] = data[calcul + 2]; // R
					dataOut[pos + 1] = data[calcul + 1]; // G
					dataOut[pos + 2] = data[calcul]; // B
				}
			});
	}

	return anImage;
}


void CFiltreEffetCPU::Interpolation(const int& widthOut, const int& heightOut, const int& method, int flipH, int flipV,
                                    int angle)
{
	if (bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	ApplyInterpolationFilters(pBitmap, bitmapOut, flipH, flipV, angle, method);

/*
 *
 * static void ApplyInterpolationFilters(CRegardsBitmap* bitmap, CRegardsBitmap* & bitmapOut, const wxRect& rc,
                                      const int& flipH, const int& flipV, const int& angle,
                                      const int& filterInterpolation)
 */
}

void CFiltreEffetCPU::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
                                    int flipH, int flipV, int angle)
{
	if (bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	//CInterpolationBicubic interpolation;
	ApplyInterpolationFilters(pBitmap, bitmapOut, rc, flipH, flipV, angle, method);
}

int CFiltreEffetCPU::HistogramNormalize()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;


	if (bitmap != nullptr)
	{
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(image, image, COLOR_BGRA2BGR);
		vector<Mat> bgr_planes;
		split(image, bgr_planes);
		int gridsize = 8;
		Ptr<CLAHE> clahe = createCLAHE(2.0, Size(gridsize, gridsize));
		clahe->apply(bgr_planes[0], bgr_planes[0]);
		clahe->apply(bgr_planes[1], bgr_planes[1]);
		clahe->apply(bgr_planes[2], bgr_planes[2]);
		cv::merge(bgr_planes, dest);

		cvtColor(dest, dest, COLOR_BGR2BGRA);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());

		bgr_planes[0].release();
		bgr_planes[1].release();
		bgr_planes[2].release();
		dest.release();
		image.release();
	}
	return 0;
	//return CHistogramme::HistogramNormalize(pBitmap);
}

int CFiltreEffetCPU::HistogramEqualize()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;


	if (bitmap != nullptr)
	{
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());

		cvtColor(image, image, COLOR_BGRA2BGR);

		vector<Mat> bgr_planes;
		split(image, bgr_planes);
		equalizeHist(bgr_planes[0], bgr_planes[0]);
		equalizeHist(bgr_planes[1], bgr_planes[1]);
		equalizeHist(bgr_planes[2], bgr_planes[2]);

		cv::merge(bgr_planes, dest);
		cvtColor(dest, dest, COLOR_BGR2BGRA);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());

		bgr_planes[0].release();
		bgr_planes[1].release();
		bgr_planes[2].release();
		dest.release();
		image.release();
	}
	return 0;
	//return CHistogramme::HistogramEqualize(pBitmap);
}

int CFiltreEffetCPU::HQDn3D(const double& LumSpac, const double& ChromSpac, const double& LumTmp,
                            const double& ChromTmp)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;


	if (bitmap != nullptr)
	{
		if (hq3d == nullptr)
		{
			hq3d = new Chqdn3d(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), LumSpac, LumTmp);
		}
		else if (oldLevelDenoise != LumSpac || bitmap->GetBitmapWidth() != oldwidthDenoise || bitmap->GetBitmapHeight()
			!= oldheightDenoise)
		{
			delete hq3d;
			hq3d = new Chqdn3d(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), LumSpac, LumTmp);
		}

		hq3d->ApplyDenoise3D(bitmap);
	}

	return 0;
}

int CFiltreEffetCPU::LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType,
                               const int& iIntensity, const int& iColor, const int& iColorIntensity)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CLensFlare();
		filtre->LensFlare(bitmap, iPosX, iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
		delete filtre;
	}
	return 0;
}

//---------------------------------------------------------------------
//Effet CRgbaquad 
//---------------------------------------------------------------------
int CFiltreEffetCPU::RGBFilter(const int& red, const int& green, const int& blue)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CRgbFiltre(red, green, blue);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

int CFiltreEffetCPU::Solarize(const long& threshold)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CSolarize(static_cast<int>(threshold));
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

int CFiltreEffetCPU::Posterize(const float& level, const float& gamma)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CPosterize(level, gamma);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}


//---------------------------------------------------------------------
//Effet Clouds 
//---------------------------------------------------------------------
int CFiltreEffetCPU::CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude,
                                  const float& frequence, const int& octave, const int& intensity)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto m_perlinNoise = new CPerlinNoise();
		CRegardsBitmap localBitmap(250, 250);
		auto _local = new CRegardsBitmap(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		m_perlinNoise->Clouds(&localBitmap, color1, color2, amplitude / 100.0f, frequence / 100.0f, octave);
		delete m_perlinNoise;
		CInterpolationBicubic interpolation;
		interpolation.Execute(&localBitmap, _local);
		Fusion(_local, intensity / 100.0f);
		delete _local;
	}
	return 0;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffetCPU::Swirl(const float& radius, const float& angle)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CSwirl(angle, radius);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//---------------------------------------------------------------------
//Lumière et Contraste 
//---------------------------------------------------------------------
int CFiltreEffetCPU::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	double offset;

	if (contrast == 0)
		Contrast(1, 128);
	else if (contrast > 0)
	{
		offset = contrast;
		offset /= 100;
		offset = 1 + offset;
		Contrast(offset, 128);
	}
	else if (contrast < 0)
	{
		offset = -contrast;
		offset /= 100;
		offset = 1 - offset;
		//lValue = 128 + lContrast;
		Contrast(offset, 128);
	}

	Lightness(brightness);

	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NiveauDeGris()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(image, dest, COLOR_BGRA2GRAY);
		cvtColor(dest, dest, COLOR_GRAY2BGRA);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
		/*
		CGrayScale * filtre = new CGrayScale();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
	}
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NoirEtBlanc()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		CBlackAndWhite * filtre = new CBlackAndWhite();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(image, dest, COLOR_BGRA2GRAY);
		threshold(dest, dest, 127, 255, THRESH_BINARY);
		cvtColor(dest, dest, COLOR_GRAY2BGRA);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sepia()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CSepiaFiltre();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;

		/*
		cv::Mat dest;
		cv::Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cv::cvtColor(image, dest, COLOR_BGRA2GRAY);
		cv::cvtColor(dest, dest, COLOR_GRAY2BGR);
		cv::Mat kernel =
			(cv::Mat_<float>(3, 3)
				<<
				0.272, 0.534, 0.131,
				0.349, 0.686, 0.168,
				0.393, 0.769, 0.189);
		
		cv::transform(dest, dest, kernel);
		cv::cvtColor(dest, dest, COLOR_BGR2BGRA);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		*/
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Soften()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = {1, 1, 1, 1, 8, 1, 1, 1, 1};
		auto filtre = new CMatrixConvolution(kernel, 3, 16, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Blur(const int& radius)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		int sizeKernel = radius * 3;
		short * kernel = new short[sizeKernel];
		for(int i = 0;i < sizeKernel;i++)
			kernel[i] = 1;
		//{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, radius, sizeKernel, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		blur(image, dest, Size(radius, radius));
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
	}
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::GaussianBlur(const int& radius, const int& boxSize)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cv::GaussianBlur(image, dest, Size(boxSize, boxSize), radius);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
		/*
		CGaussianBlur gaussianBlur;
		gaussianBlur.GaussianBlur(bitmap, radius);
		*/
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Emboss()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		short kernel[] = { -1, 0, 0, 0, 0, 0, 0, 0, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 127);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/

		// Construct kernel (all entries initialized to 0)
		Mat kernel(3, 3, CV_32F, Scalar(0));
		kernel.at<float>(0, 0) = -1.0;
		kernel.at<float>(2, 2) = 1.0;
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		filter2D(image, dest, image.depth(), kernel, Point(-1, -1), 127);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
		kernel.release();
		//filter the image
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::SharpenStrong()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		short kernel[] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
		Mat kernel(3, 3, CV_32F, Scalar(0));
		kernel.at<float>(0, 0) = -1.0;
		kernel.at<float>(0, 1) = -1.0;
		kernel.at<float>(0, 2) = -1.0;
		kernel.at<float>(1, 0) = -1.0;
		kernel.at<float>(1, 1) = 9.0;
		kernel.at<float>(1, 2) = -1.0;
		kernel.at<float>(2, 0) = -1.0;
		kernel.at<float>(2, 1) = -1.0;
		kernel.at<float>(2, 2) = -1.0;
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		filter2D(image, dest, image.depth(), kernel);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
		kernel.release();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sharpen()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		short kernel[] = { -1, -1, -1, -1, 16, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 8, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
		Mat kernel(3, 3, CV_32F, Scalar(0));
		kernel.at<float>(1, 1) = 5.0;
		kernel.at<float>(0, 1) = -1.0;
		kernel.at<float>(2, 1) = -1.0;
		kernel.at<float>(1, 0) = -1.0;
		kernel.at<float>(1, 2) = -1.0;
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		filter2D(image, dest, image.depth(), kernel);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
		kernel.release();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Erode()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		CErode * filtre = new CErode();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/

		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		erode(image, dest, Mat());
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Median()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		CMedian * filtre = new CMedian();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		medianBlur(image, dest, 3);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Noise()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CNoise();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Dilate()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		dilate(image, dest, Mat());
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
		/*
		CDilate * filtre = new CDilate();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Negatif()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		CNegatif * filtre = new CNegatif();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/
		Mat dest;
		Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		bitwise_not(image, dest);
		bitmap->SetBitmap(dest.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		dest.release();
		image.release();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Contrast(const double& contrast, const uint8_t& offset)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CContrast(contrast, offset);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

int CFiltreEffetCPU::Lightness(const double& factor)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CLightness(factor);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreEdge()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
		auto filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreMosaic()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CMosaic(5);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}


	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipVertical()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->VertFlipBuf();
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipHorizontal()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->HorzFlipBuf();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CMotionBlur();
		filtre->MotionBlur(bitmap, radius, sigma, angle);
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat dst;
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());

		// get rotation matrix for rotating the image around its center in pixel coordinates
		const Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
		Mat rot = getRotationMatrix2D(center, angle, 1.0);
		// determine bounding rectangle, center not relevant
		Rect2f bbox = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();
		// adjust transformation matrix
		rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
		rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;

		warpAffine(src, dst, rot, bbox.size());
		bitmap->SetBitmap(dst.data, dst.cols, dst.rows);

		dst.release();
		src.release();
		rot.release();
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		auto filtre = new CPhotoFiltre(clValue, intensity);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

void CFiltreEffetCPU::RotateMatrix(const int& angle, Mat& src)
{
	Mat dst;
	if (angle == 90)
	{
		// Rotate clockwise 270 degrees
		transpose(src, dst);
		flip(dst, src, 0);
	}
	else if (angle == 180)
	{
		// Rotate clockwise 180 degrees
		flip(src, src, -1);
	}
	else if (angle == 270)
	{
		// Rotate clockwise 90 degrees
		transpose(src, dst);
		flip(dst, src, 1);
	}
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate90()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		RotateMatrix(270, src);
		bitmap->SetBitmap(src.data, bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth());
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate270()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		RotateMatrix(90, src);
		bitmap->SetBitmap(src.data, bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth());
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Resize(const int& imageWidth, const int& imageHeight, const int& interpolation)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		Mat dst(imageHeight, imageWidth, CV_8UC4);
		Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		resize(src, dst, dst.size(), 0, 0, INTER_CUBIC);
		bitmap->SetBitmap(dst.data, imageWidth, imageHeight);
		dst.release();
		src.release();
	}


	/*
	CRegardsBitmap * scaleBitmap = new CRegardsBitmap(imageWidth, imageHeight);

	CInterpolationBicubic imageScale;
	imageScale.Execute(pBitmap, scaleBitmap);
	pBitmap->SetBitmap(scaleBitmap->GetPtBitmap(), imageWidth, imageHeight);

	delete scaleBitmap;
	*/
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Fusion(CRegardsBitmap* bitmapSecond, const float& pourcentage)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->FusionBitmap(bitmapSecond, pourcentage);
	}
	return 0;
}

void CFiltreEffetCPU::GetBitmap(CRegardsBitmap* & bitmap, const bool& source)
{
	if (bitmap != nullptr)
	{
		if (bitmapOut != nullptr && !source)
		{
			bitmap->SetFilename(bitmapOut->GetFilename());
			bitmap->SetBitmap(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
		}
		else
		{
			bitmap->SetFilename(pBitmap->GetFilename());
			bitmap->SetBitmap(pBitmap->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
		}
	}
}

#define SCALEBITS            8
#define ONE_HALF             (1 << (SCALEBITS - 1))
#define FIX(x)               ((int) ((x) * (1L<<SCALEBITS) + 0.5))

void CFiltreEffetCPU::GetYUV420P(uint8_t* & lum, uint8_t* & cb, uint8_t* & cr, const int& widthOut,
                                 const int& heightOut)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	const int wrap = bitmap->GetBitmapWidth();
	const int wrap3 = bitmap->GetBitmapWidth() * 4;
	uint8_t* p = bitmap->GetPtBitmap();
	for (int y = 0; y < bitmap->GetBitmapHeight(); y += 2)
	{
		for (int x = 0; x < bitmap->GetBitmapWidth(); x += 2)
		{
			int r = p[2];
			int g = p[1];
			int b = p[0];
			int r1 = r;
			int g1 = g;
			int b1 = b;
			lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
			r = p[6];
			g = p[5];
			b = p[4];
			r1 += r;
			g1 += g;
			b1 += b;
			lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
			p += wrap3;
			lum += wrap;
			r = p[2];
			g = p[1];
			b = p[0];
			r1 += r;
			g1 += g;
			b1 += b;
			lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
			r = p[6];
			g = p[5];
			b = p[4];
			r1 += r;
			g1 += g;
			b1 += b;
			lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;

			cb[0] = (((-FIX(0.16874) * r1 - FIX(0.33126) * g1 +
				FIX(0.50000) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
			cr[0] = (((FIX(0.50000) * r1 - FIX(0.41869) * g1 -
				FIX(0.08131) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
			cb++;
			cr++;
			p += -wrap3 + 2 * 4;
			lum += -wrap + 2;
		}
		p += wrap3;
		lum += wrap;
	}
}

CRegardsBitmap* CFiltreEffetCPU::GetBitmap(const bool& source)
{
	auto copy = new CRegardsBitmap();
	if (bitmapOut != nullptr && !source)
	{
		copy->SetFilename(bitmapOut->GetFilename());
		copy->SetBitmap(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
	}
	else
	{
		copy->SetFilename(pBitmap->GetFilename());
		copy->SetBitmap(pBitmap->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	}
	return copy;
}

int CFiltreEffetCPU::BrightnessAndContrastAuto(float clipHistPercent)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	COpenCVEffect::BrightnessAndContrastAuto(bitmap, clipHistPercent);

	return 0;
}
