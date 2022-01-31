#include <header.h>
#include "FiltreEffetCPU.h"
#include "PerlinNoise.h"
#include "LensFlare.h"
#include "MotionBlur.h"
#include "Filtre.h"
#include "RedEye.h"
#include "Wave.h"
#include <ImageLoadingFormat.h>
#include <hqdn3d.h>
#include "MeanShift.h"
#include <RegardsBitmap.h>
#include <fstream>
#include <GLTexture.h>
#include <opencv2/xphoto.hpp>
#include "VideoStabilization.h"
#include <opencv2/dnn_superres.hpp>
#include <FileUtility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
using namespace Regards::OpenCV;
using namespace Regards::OpenGL;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;
extern float value[256];
using namespace Regards::FiltreEffet;

#define EDSR 0
#define ESPCN 1
#define FSRCNN 2
#define LapSRN 3

class CFiltreEffetCPUImpl
{
public:
	//Vignette effect
	static void generateGradient(Mat& mask, const double& radius, const double& power);
	static double getMaxDisFromCorners(const cv::Size& imgSize, const cv::Point& center);
	static double dist(cv::Point a, cv::Point b);
	static cv::Mat upscaleImage(cv::Mat img, int method, int scale);
	static string GenerateModelPath(string modelName, int scale);
	static bool TestIfMethodIsValid(int method, int scale);
	static cv::Rect CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, float ratioX, float ratioY, int x, int y, float left, float top);

};


cv::Rect CFiltreEffetCPUImpl::CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, float ratioX, float ratioY, int x, int y, float left, float top)
{
	cv::Rect rect;
	float posX = (float)x * ratioX + left * ratioX;
	float posY = (float)y * ratioY + top * ratioY;

	if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
	}
	else if (angle == 180)
	{
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
	}
	else if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
	}

	if (angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}

	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}

	rect.x = abs(posX);
	rect.y = abs(posY);
	return rect;
}


string CFiltreEffetCPUImpl::GenerateModelPath(string modelName, int scale)
{
	wxString path = "";
#ifdef WIN32
	path = CFileUtility::GetResourcesFolderPath() + "\\model\\" + modelName + "_x" + to_string(scale) + ".pb";
#else
	path = CFileUtility::GetResourcesFolderPath() + "/model/"  + modelName + "_x" + to_string(scale) + ".pb";
#endif
	return path.ToStdString();
}

bool CFiltreEffetCPUImpl::TestIfMethodIsValid(int method, int scale)
{
	if (method == EDSR && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	else if (method == ESPCN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	else if (method == FSRCNN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	else if (method == LapSRN && (scale == 2 || scale == 4 || scale == 8))
	{
		return true;
	}
	return false;
}

cv::Mat CFiltreEffetCPUImpl::upscaleImage(cv::Mat img, int method, int scale)
{
	Mat outputImage;
	try
	{
		//muDnnSuperResImpl.lock();

		DnnSuperResImpl sr;


		switch (method)
		{
		case EDSR:
		{
			string algorithm = "edsr";
			sr.readModel(GenerateModelPath("EDSR", scale));
			sr.setModel(algorithm, scale);
		}
		break;

		case ESPCN:
		{
			string algorithm = "espcn";
			sr.readModel(GenerateModelPath("ESPCN", scale));
			sr.setModel(algorithm, scale);
		}
		break;
		case FSRCNN:
		{
			string algorithm = "fsrcnn";
			sr.readModel(GenerateModelPath("FSRCNN", scale));
			sr.setModel(algorithm, scale);
		}
		break;
		case LapSRN:
		{
			string algorithm = "lapsrn";
			sr.readModel(GenerateModelPath("LapSRN", scale));
			sr.setModel(algorithm, scale);
		}
		break;
		}

		sr.setPreferableTarget(DNN_TARGET_CPU);
		sr.upsample(img, outputImage);

		//muDnnSuperResImpl.unlock();

	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return outputImage;
}



CFiltreEffetCPU::CFiltreEffetCPU(const CRgbaquad& back_color, CImageLoadingFormat* bitmap)
	: IFiltreEffet(back_color)
{
	this->backColor = back_color;
	bitmapOut = nullptr;
	pBitmap = bitmap->GetRegardsBitmap();
	if(pBitmap == nullptr)
	{
		printf("toto");
	}
}

bool CFiltreEffetCPU::StabilizeVideo(Regards::OpenCV::COpenCVStabilization * openCVStabilization)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	bool frameStabilized = false;
	cv::Mat image = bitmap->GetMatrix();
	if (openCVStabilization != nullptr)
	{
		if (openCVStabilization->GetNbFrameBuffer() == 0)
		{
			openCVStabilization->BufferFrame(image);
		}
		else
		{
			frameStabilized = true;
			openCVStabilization->AddFrame(image);
		}

		if (frameStabilized)
			openCVStabilization->CorrectFrame(image);
	}

	return frameStabilized;
}

void CFiltreEffetCPU::LoadAndRotate(const wxString& filePath, const int& rotate)
{
	if (!wxFileExists(filePath.ToStdString()))
	{
		return;
	}

	Mat src = imread(filePath.ToStdString());
	if (src.empty())
		return;

	if (rotate == 90)
	{
		// Rotate clockwise 270 degrees
		transpose(src, src);
		flip(src, src, 0);
	}
	else if (rotate == 180)
	{
		// Rotate clockwise 180 degrees
		flip(src, src, -1);
	}
	else if (rotate == 270)
	{
		// Rotate clockwise 90 degrees
		transpose(src, src);
		flip(src, src, 1);
	}
	imwrite(filePath.ToStdString(), src);
	src.release();
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

// ReSharper disable once CppDoxygenUnresolvedReference
// ReSharper disable once CppDoxygenUnresolvedReference
/**
 *  \brief Automatic brightness and contrast optimization with optional histogram clipping
 *  \param [in]src Input image GRAY or BGR or BGRA
 *  \param [out]dst Destination image
 *  \param clipHistPercent cut wings of histogram at given percent tipical=>1, 0=>Disabled
 *  \note In case of BGRA image, we won't touch the transparency
*/
void CFiltreEffetCPU::BrightnessAndContrastAuto(Mat& image, float clipHistPercent)
{
	int histSize = 256;
	float alpha, beta;
	double minGray = 0, maxGray = 0;

	CV_Assert(clipHistPercent >= 0);
	CV_Assert((image.type() == CV_8UC1) || (image.type() == CV_8UC3) || (image.type() == CV_8UC4));

	//to calculate grayscale histogram
	Mat gray;
	if (image.type() == CV_8UC1) gray = image;
	else if (image.type() == CV_8UC3) cvtColor(image, gray, COLOR_BGR2GRAY);
	else if (image.type() == CV_8UC4) cvtColor(image, gray, COLOR_BGRA2GRAY);
	if (clipHistPercent == 0)
	{
		// keep full available range
		minMaxLoc(gray, &minGray, &maxGray);
	}
	else
	{
		Mat hist; //the grayscale histogram

		float range[] = { 0, 256 };
		const float* histRange = { range };
		bool uniform = true;
		bool accumulate = false;
		calcHist(&gray, 1, nullptr, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		// calculate cumulative distribution from the histogram
		std::vector<float> accumulator(histSize);
		accumulator[0] = hist.at<float>(0);
		for (int i = 1; i < histSize; i++)
		{
			accumulator[i] = accumulator[i - 1] + hist.at<float>(i);
		}

		// locate points that cuts at required value
		float max = accumulator.back();
		clipHistPercent *= (max / 100.0); //make percent as absolute
		clipHistPercent /= 2.0; // left and right wings
		// locate left cut
		minGray = 0;
		while (accumulator[minGray] < clipHistPercent)
			minGray++;

		// locate right cut
		maxGray = histSize - 1;
		while (accumulator[maxGray] >= (max - clipHistPercent))
		{
			maxGray--;
			if (maxGray == 0)
				break;
		}
	}


	// current range
	float inputRange = maxGray - minGray;

	alpha = (histSize - 1) / inputRange; // alpha expands current range to histsize range
	beta = -minGray * alpha; // beta shifts current range so that minGray will go to 0

	convertScaleAbs(image, image, alpha, beta);
}
/*
void CFiltreEffetCPU::CopyPictureToTexture2D(GLTexture* texture, const bool& source, int rgba)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (texture != nullptr)
	{
		try
		{
			texture->SetData(bitmap);
		}
		catch (...)
		{

		}
	}
}*/

int CFiltreEffetCPU::BokehEffect(const int& radius, const int& boxsize, const int & nbFace, const wxRect & listFace)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;
	Mat dst;

	pBitmap->VertFlipBuf();
	
	cvtColor(bitmap->GetMatrix(), dst, COLOR_BGRA2BGR);

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
			cv::GaussianBlur(dst, blur, cv::Size(boxsize, boxsize), radius);


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
			vector< vector<cv::Point> > contours;
			findContours(src_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

			// you could also reuse img1 here
			Mat mask = Mat::zeros(src_gray.rows, src_gray.cols, CV_8UC1);
			//threshold(mask, mask, 0, 255, THRESH_BINARY_INV);
			// CV_FILLED fills the connected components found
			drawContours(mask, contours, -1, Scalar(255), FILLED);

		
			cv::Point center(rect.width / 2, (rectCopy.y + rectCopy.height) / 2);//Declaring the center point
			cv::Size xy((rectCopy.width / 2) * 1.5, (rectCopy.height / 2) * 1.5);//Declaring the major and minor axis of the ellipse//
			int angle = 0;//angle of rotation//
			int starting_point = 0;//Starting point of the ellipse//
			int ending_point = 360;//Ending point of the ellipse//
			Scalar line_Color(255, 255, 255);//Color of the Ellipse//

			//namedWindow("whiteMatrix");//Declaring a window to show the ellipse//
			ellipse(mask, center, xy, angle, starting_point, ending_point, line_Color, -1, LINE_AA);//Drawing the ellipse

			//waitKey(0);//Waiting for Keystroke

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


			cv::GaussianBlur(croppedImage, blur_crop, cv::Size(boxsize, boxsize), radius);


			// normalize so imwrite(...)/imshow(...) shows the mask correctly!
			normalize(mask.clone(), mask, 0.0, 255.0, cv::NORM_MINMAX, CV_8UC1);


			int oldx = 0;
			for (int y = 0; y < rect.height; y++)
			{
				for (int x = 0; x < rect.width; x++)
				{
					uchar color = mask.at<uchar>(y, x);
					if (color == 255)
					{
						int _x = x;
						if (oldx != 0)
						{
							if (x < (oldx * 0.98))
								x = oldx * 0.98;
						}

						if (y > rect.height / 2)
						{
							if (x > _x)
								x = _x;
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

						if (y > rect.height / 2)
						{
							if (x < oldx)
								x = oldx;
						}
						
						oldx = x;

						break;
					}

					
				}
			}


			cv::Rect _rect(0, rect.height / 2, rect.width, rect.height / 2);
			cv::rectangle(mask, _rect, cv::Scalar(255, 255, 255), -1);
			
			// and copy the magic apple
			croppedImage.copyTo(blur_crop, mask);

			cv::Rect copy(rect.x, rect.y, croppedImage.cols, croppedImage.rows);
			blur_crop.copyTo(blur(copy));

			cvtColor(blur, bitmap->GetMatrix(), COLOR_BGR2BGRA);
		}
		catch(...)
		{
			
		}


	}
	
	dst.release();
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
	Mat image;
	cvtColor(bitmap->GetMatrix(), dst, COLOR_BGRA2BGR);
	xphoto::oilPainting(dst, image, size, dynRatio, COLOR_BGR2Lab);
	cvtColor(image, bitmap->GetMatrix(), COLOR_BGR2BGRA);
	dst.release();
	image.release();
	return 0;
}

// Helper function to calculate the distance between 2 points.
double CFiltreEffetCPUImpl::dist(cv::Point a, cv::Point b)
{
	return sqrt(pow(static_cast<double>(a.x - b.x), 2) + pow(static_cast<double>(a.y - b.y), 2));
}

// Helper function that computes the longest distance from the edge to the center point.
double CFiltreEffetCPUImpl::getMaxDisFromCorners(const cv::Size& imgSize, const cv::Point& center)
{
	// given a rect and a line
	// get which corner of rect is farthest from the line

	std::vector<cv::Point> corners(4);
	corners[0] = cv::Point(0, 0);
	corners[1] = cv::Point(imgSize.width, 0);
	corners[2] = cv::Point(0, imgSize.height);
	corners[3] = cv::Point(imgSize.width, imgSize.height);

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
	auto firstPt = cv::Point(mask.size().width / 2, mask.size().height / 2);
	double maxImageRad = (radius * getMaxDisFromCorners(mask.size(), firstPt));
	double maxImageRadPower = maxImageRad * power;
	mask.setTo(Scalar(1));
	for (int i = 0; i < mask.rows; i++)
	{
		for (int j = 0; j < mask.cols; j++)
		{
			double temp = dist(firstPt, cv::Point(j, i));

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
		Mat src;
		cvtColor(bitmap->GetMatrix(), src, COLOR_BGRA2BGR);

		auto firstPt = cv::Point(src.size().width / 2, src.size().height / 2);
		double maxImageRad = static_cast<float>(radius / 100.0) * CFiltreEffetCPUImpl::getMaxDisFromCorners(
			src.size(), firstPt);
		double maxImageRadPower = maxImageRad * static_cast<float>(power / 100.0);

		Mat labImg(src.size(), CV_8UC3);

		cvtColor(src, labImg, COLOR_BGR2Lab);

		for (int row = 0; row < src.size().height; row++)
		{
			for (int col = 0; col < src.size().width; col++)
			{

				double temp = CFiltreEffetCPUImpl::dist(firstPt, cv::Point(col, row));
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
		cvtColor(src, bitmap->GetMatrix(), COLOR_BGR2BGRA);

		labImg.release();
		src.release();
	}

	return 0;
}


void CFiltreEffetCPU::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (pBitmap != nullptr)
		delete pBitmap;
	pBitmap = bitmap->GetRegardsBitmap();
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
	//Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());

	// Convert from BGR color to Grayscale
	Mat srcGray;
	cvtColor(bitmap->GetMatrix(), srcGray, COLOR_BGRA2GRAY);

	// Remove the pixel noise with a good Median filter, before we start detecting edges.
	medianBlur(srcGray, srcGray, 7);

	cv::Size size = srcGray.size();
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
		cvtColor(mask, bitmap->GetMatrix(), COLOR_GRAY2BGRA);
		dst.release();
		mask.release();
		edges.release();
		return -1;
	}

	// Do the bilateral filtering at a shrunken scale, since it
	// runs so slowly but doesn't need full resolution for a good effect.
	cv::Size smallSize;
	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;

	Mat srcBgr;
	cvtColor(bitmap->GetMatrix(), srcBgr, COLOR_BGRA2BGR);
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
	resize(srcBgr, dst, size, 0, 0, INTER_LINEAR);

	// Use the blurry cartoon image, except for the strong edges that we will leave black.
	dst.copyTo(bitmap->GetMatrix(), mask);

	dst.release();
	mask.release();
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
	Mat mask = maskPlusBorder(cv::Rect(1, 1, sw, sh)); // mask is a ROI in maskPlusBorder.
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
	cv::Point skinPts[NUM_SKIN_POINTS];
	skinPts[0] = cv::Point(sw / 2, sh / 2 - sh / 6);
	skinPts[1] = cv::Point(sw / 2 - sw / 11, sh / 2 - sh / 6);
	skinPts[2] = cv::Point(sw / 2 + sw / 11, sh / 2 - sh / 6);
	skinPts[3] = cv::Point(sw / 2, sh / 2 + sh / 16);
	skinPts[4] = cv::Point(sw / 2 - sw / 9, sh / 2 + sh / 16);
	skinPts[5] = cv::Point(sw / 2 + sw / 9, sh / 2 + sh / 16);
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
		cvtColor(bitmap->GetMatrix(), dst, COLOR_BGRA2BGR);
		cvtColor(dst, dst, COLOR_BGR2Lab);
		// Initilize Mean Shift with spatial bandwith and color bandwith
		CMeanShift msProcess(fSpatialRadius, fColorRadius);
		//MSProc(fSpatialRadius, fColorRadius);
		// Filtering Process
		msProcess.MSFiltering(dst);
		cvtColor(dst, dst, COLOR_Lab2RGB);
		cvtColor(dst, bitmap->GetMatrix(), COLOR_BGR2BGRA);
		dst.release();
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
		Mat out;
		cvtColor(bitmap->GetMatrix(), dst, COLOR_BGRA2BGR);
		bilateralFilter(dst, out, fSize, sigmaX, sigmaP, BORDER_DEFAULT);
		cvtColor(out, bitmap->GetMatrix(), COLOR_BGR2BGRA);
		dst.release();
		out.release();
	}
	return -1;
}

int CFiltreEffetCPU::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize)
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
		Mat out;
		cvtColor(bitmap->GetMatrix(), dst, COLOR_BGRA2BGR);
		fastNlMeansDenoisingColored(dst, out, h, hColor, templateWindowSize, searchWindowSize);
		cvtColor(out, bitmap->GetMatrix(), COLOR_BGR2BGRA);
		dst.release();
		out.release();
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
		Mat blurred; 
		double sigma = 1, threshold = 5, amount = sharpness;
		cv::GaussianBlur(bitmap->GetMatrix(), blurred, cv::Size(), sigma, sigma);
		Mat lowConstrastMask = abs(bitmap->GetMatrix() - blurred) < threshold;
		Mat sharpened = bitmap->GetMatrix() * (1 + amount) + blurred * (-amount);
		bitmap->GetMatrix().copyTo(sharpened, lowConstrastMask);
		bitmap->SetMatrix(sharpened);
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
	if (bitmapOut != nullptr)
	{
		return GetwxImage(bitmapOut);
	}
	return GetwxImage(pBitmap);
}

wxImage CFiltreEffetCPU::GetwxImage(CRegardsBitmap* bitmap)
{
	cv::Mat cvDest;
	cv::cvtColor(bitmap->GetMatrix(), cvDest, cv::COLOR_BGRA2RGB);
	cv::flip(cvDest, cvDest, 0);
	wxImage anImage(cvDest.cols, cvDest.rows, cvDest.data, TRUE);
	return anImage;
}

CRegardsBitmap* CFiltreEffetCPU::Interpolation(CRegardsBitmap* pBitmap, const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
	int flipH, int flipV, int angle, int ratio)
{
	cv::Mat cvImage;
	cv::cvtColor(pBitmap->GetMatrix(), cvImage, cv::COLOR_BGRA2BGR);

	try
	{
		float ratioX = (float)cvImage.cols / rc.width;
		float ratioY = (float)cvImage.rows / rc.height;
		if (angle == 90 || angle == 270)
		{
			ratioX = (float)cvImage.cols / (float)rc.height;
			ratioY = (float)cvImage.rows / (float)rc.width;
		}

		cv::Rect rectGlobal;
		cv::Rect rect_begin = CFiltreEffetCPUImpl::CalculRect(cvImage.cols, cvImage.rows, widthOut, heightOut, flipH, flipV, angle, ratioX, ratioY, 0, 0, rc.x, rc.y);
		cv::Rect rect_end = CFiltreEffetCPUImpl::CalculRect(cvImage.cols, cvImage.rows, widthOut, heightOut, flipH, flipV, angle, ratioX, ratioY, widthOut, heightOut, rc.x, rc.y);
		rectGlobal.x = rect_begin.x;
		rectGlobal.y = rect_begin.y;
		rectGlobal.width = rect_end.x;
		rectGlobal.height = rect_end.y;
		if (rectGlobal.x > rectGlobal.width)
		{
			int x_end = rectGlobal.x;
			int x = rectGlobal.width;
			rectGlobal.x = x;
			rectGlobal.width = x_end - x;
		}
		else
		{
			rectGlobal.width -= rectGlobal.x;
		}

		if (rectGlobal.y > rectGlobal.height)
		{
			int y_end = rectGlobal.y;
			int y = rectGlobal.height;
			rectGlobal.y = y;
			rectGlobal.height = y_end - y;
		}
		else
		{
			rectGlobal.height -= rectGlobal.y;
		}

		if ((rectGlobal.height + rectGlobal.y) > cvImage.rows)
		{
			rectGlobal.height = cvImage.rows - rectGlobal.y;
		}
		if ((rectGlobal.width + rectGlobal.x) > cvImage.cols)
		{
			rectGlobal.width = cvImage.cols - rectGlobal.x;
		}

		cvImage = cvImage(rectGlobal);

		if (angle == 90)
		{
			if (flipV && flipH)
				cv::rotate(cvImage, cvImage, cv::ROTATE_90_CLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvImage, cvImage, cv::ROTATE_90_COUNTERCLOCKWISE);
			else
				cv::rotate(cvImage, cvImage, cv::ROTATE_90_CLOCKWISE);

		}
		else if (angle == 270)
		{
			if (flipV && flipH)
				cv::rotate(cvImage, cvImage, cv::ROTATE_90_COUNTERCLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvImage, cvImage, cv::ROTATE_90_CLOCKWISE);
			else
				cv::rotate(cvImage, cvImage, cv::ROTATE_90_COUNTERCLOCKWISE);

		}
		else if (angle == 180)
		{
			cv::rotate(cvImage, cvImage, cv::ROTATE_180);
		}


		/*
		nearest neighbor interpolation
		INTER_NEAREST = 0,
		bilinear interpolation
		INTER_LINEAR = 1,
		bicubic interpolation
		INTER_CUBIC = 2,
		resampling using pixel area relation. It may be a preferred method for image decimation, as
		it gives moire'-free results. But when the image is zoomed, it is similar to the INTER_NEAREST
		method.
		INTER_AREA = 3,
		Lanczos interpolation over 8x8 neighborhood
		INTER_LANCZOS4 = 4,
		Bit exact bilinear interpolation
		INTER_LINEAR_EXACT = 5,
		Bit exact nearest neighbor interpolation. This will produce same results as
		the nearest neighbor method in PIL, scikit-image or Matlab.
		INTER_NEAREST_EXACT = 6,
		*/
		if (ratio != 100)
		{
			CRegardsConfigParam* regardsParam = CParamInit::getInstance();
			int superDnn = regardsParam->GetSuperResolutionType();
			int useSuperResolution = regardsParam->GetUseSuperResolution();
			if (useSuperResolution && CFiltreEffetCPUImpl::TestIfMethodIsValid(superDnn, (ratio / 100)))
			{
				cvImage = CFiltreEffetCPUImpl::upscaleImage(cvImage, superDnn, (ratio / 100));
			}
			else
			{
				cv::resize(cvImage, cvImage, cv::Size(widthOut, heightOut), method);
			}
		}

		if (cvImage.cols != widthOut || cvImage.rows != heightOut)
			cv::resize(cvImage, cvImage, cv::Size(widthOut, heightOut), cv::INTER_NEAREST_EXACT);

		//Apply Transformation

		if (flipH)
		{
			if (angle == 90 || angle == 270)
				cv::flip(cvImage, cvImage, 0);
			else
				cv::flip(cvImage, cvImage, 1);
		}
		if (flipV)
		{
			if (angle == 90 || angle == 270)
				cv::flip(cvImage, cvImage, 1);
			else
				cv::flip(cvImage, cvImage, 0);
		}

		//cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2BGRA);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;

	}

	cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2BGRA);

	CRegardsBitmap* bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	bitmapOut->SetMatrix(cvImage);
	return bitmapOut;
}

void CFiltreEffetCPU::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
	int flipH, int flipV, int angle, int ratio)
{
	if (bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = Interpolation(pBitmap, widthOut, heightOut, rc, method, flipH, flipV, angle, ratio);
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
		Mat image;// (bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		cvtColor(bitmap->GetMatrix(), image, COLOR_BGRA2BGR);
		vector<Mat> bgr_planes;
		split(image, bgr_planes);
		int gridsize = 8;
		cv::Ptr<CLAHE> clahe = createCLAHE(2.0, cv::Size(gridsize, gridsize));
		clahe->apply(bgr_planes[0], bgr_planes[0]);
		clahe->apply(bgr_planes[1], bgr_planes[1]);
		clahe->apply(bgr_planes[2], bgr_planes[2]);
		cv::merge(bgr_planes, dest);

		cvtColor(dest, bitmap->GetMatrix(), COLOR_BGR2BGRA);

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
		Mat image;// (bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());

		cvtColor(bitmap->GetMatrix(), image, COLOR_BGRA2BGR);

		vector<Mat> bgr_planes;
		split(image, bgr_planes);
		equalizeHist(bgr_planes[0], bgr_planes[0]);
		equalizeHist(bgr_planes[1], bgr_planes[1]);
		equalizeHist(bgr_planes[2], bgr_planes[2]);

		cv::merge(bgr_planes, dest);
		cvtColor(dest, bitmap->GetMatrix(), COLOR_BGR2BGRA);

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
		cv::Scalar color = cv::Scalar(red, green, blue);
		cv::Mat out = bitmap->GetMatrix() + color;
		bitmap->SetMatrix(out);
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
		cv::resize(localBitmap.GetMatrix(), _local->GetMatrix(), cv::Size(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight()), INTER_CUBIC);
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
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		float alpha = contrast / 100.0f; //# Contrast control(1.0 - 3.0)
		float beta = brightness; //# Brightness control(0 - 100)
		cv::Mat image;
		cv::convertScaleAbs(bitmap->GetMatrix(), image, alpha, beta);
		bitmap->SetMatrix(image);
		//adjusted = cv2.convertScaleAbs(image, alpha = alpha, beta = beta)
	}

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
		cvtColor(bitmap->GetMatrix(), dest, COLOR_BGRA2GRAY);
		cvtColor(dest, bitmap->GetMatrix(), COLOR_GRAY2BGRA);
		dest.release();
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
		Mat dest;
		cvtColor(bitmap->GetMatrix(), dest, COLOR_BGRA2GRAY);
		threshold(dest, dest, 127, 255, THRESH_BINARY);
		cvtColor(dest, bitmap->GetMatrix(), COLOR_GRAY2BGRA);
		dest.release();
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
		/*
		auto filtre = new CSepiaFiltre();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
		*/

		Mat dest;
		cvtColor(bitmap->GetMatrix(), dest, COLOR_BGRA2BGR);

		cv::Mat kernel =
			(cv::Mat_<float>(3, 3)
				<<
				0.272, 0.534, 0.131,
				0.349, 0.686, 0.168,
				0.393, 0.769, 0.189);

		cv::transform(dest, dest, kernel);
		cvtColor(dest, dest, COLOR_BGR2BGRA);
		bitmap->SetMatrix(dest);

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
		Mat dest;
		blur(bitmap->GetMatrix(), dest, cv::Size(radius, radius));
		bitmap->SetMatrix(dest);
		dest.release();
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
		cv::GaussianBlur(bitmap->GetMatrix(), dest, cv::Size(boxSize, boxSize), radius);
		bitmap->SetMatrix(dest);
		dest.release();
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
		// Construct kernel (all entries initialized to 0)
		Mat kernel(3, 3, CV_32F, Scalar(0));
		kernel.at<float>(0, 0) = -1.0;
		kernel.at<float>(2, 2) = 1.0;
		Mat dest;
		filter2D(bitmap->GetMatrix(), dest, bitmap->GetMatrix().depth(), kernel);
		bitmap->SetMatrix(dest);
		dest.release();
		kernel.release();
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
		filter2D(bitmap->GetMatrix(), dest, bitmap->GetMatrix().depth(), kernel);
		bitmap->SetMatrix(dest);
		dest.release();
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
		Mat kernel(3, 3, CV_32F, Scalar(0));
		kernel.at<float>(1, 1) = 5.0;
		kernel.at<float>(0, 1) = -1.0;
		kernel.at<float>(2, 1) = -1.0;
		kernel.at<float>(1, 0) = -1.0;
		kernel.at<float>(1, 2) = -1.0;
		Mat dest;
		filter2D(bitmap->GetMatrix(), dest, bitmap->GetMatrix().depth(), kernel);
		bitmap->SetMatrix(dest);
		dest.release();
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
		Mat dest;
		erode(bitmap->GetMatrix(), dest, Mat());
		bitmap->SetMatrix(dest);
		dest.release();
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
		Mat dest;
		medianBlur(bitmap->GetMatrix(), dest, 3);
		bitmap->SetMatrix(dest);
		dest.release();
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

		/*
		vector<Mat> bgr_planes;
		split(bitmap->GetMatrix(), bgr_planes);

		for (int i = 0; i < 3; i++)
		{
			cv::Mat noise = Mat(bgr_planes[i].size(), CV_64F);
			Mat result;
			normalize(bgr_planes[i], result, 0.0, 1.0, NORM_MINMAX, CV_64F);
			cv::randn(noise, 0, 0.1);
			result = result + noise;
			normalize(result, result, 0.0, 1.0, NORM_MINMAX, CV_64F);
			result.convertTo(bgr_planes[i], CV_32F, 255, 0);
		}

		cv::merge(bgr_planes, bitmap->GetMatrix());
		*/
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
		dilate(bitmap->GetMatrix(), dest, Mat());
		bitmap->SetMatrix(dest);
		dest.release();
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
		Mat dest;
		bitwise_not(bitmap->GetMatrix(), dest);
		bitmap->SetMatrix(dest);
		dest.release();
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
		/*
		short kernel[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
		auto filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	*/
		
		Mat dest;
		cvtColor(bitmap->GetMatrix(), dest, COLOR_BGRA2GRAY);

		Mat img_blur;
		cv::GaussianBlur(dest, img_blur, cv::Size(3, 3), 0, 0);
		Mat edges;
		Canny(img_blur, edges, 100, 200, 3, false);

		cvtColor(edges, dest, COLOR_GRAY2BGRA);
		bitmap->SetMatrix(dest);
		

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

int CFiltreEffetCPU::GroundGlassEffect(const double& radius)
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{

		Mat imageSource = bitmap->GetMatrix();
		cvtColor(imageSource, imageSource, COLOR_BGRA2BGR);
		Mat imageResult = imageSource.clone();
		RNG rng;
		int randomNum;
		int Number = radius;

		for (int i = 0; i < imageSource.rows - Number; i++)
			for (int j = 0; j < imageSource.cols - Number; j++)
			{
				randomNum = rng.uniform(0, Number);
				imageResult.at<Vec3b>(i, j)[0] = imageSource.at<Vec3b>(i + randomNum, j + randomNum)[0];
				imageResult.at<Vec3b>(i, j)[1] = imageSource.at<Vec3b>(i + randomNum, j + randomNum)[1];
				imageResult.at<Vec3b>(i, j)[2] = imageSource.at<Vec3b>(i + randomNum, j + randomNum)[2];
			}

		cvtColor(imageResult, imageResult, COLOR_BGR2BGRA);
		bitmap->SetMatrix(imageResult);
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
		//Mat dst;
		Mat src = bitmap->GetMatrix();

		// get rotation matrix for rotating the image around its center in pixel coordinates
		const Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
		Mat rot = getRotationMatrix2D(center, angle, 1.0);
		// determine bounding rectangle, center not relevant
		Rect2f bbox = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();
		// adjust transformation matrix
		rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
		rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;

		warpAffine(src, bitmap->GetMatrix(), rot, bbox.size());

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
		float coeff = (float)intensity / 100.0f;
		float diff = 1.0f - coeff;
		cv::Mat out;
		cv::Mat out_one;
		out_one = bitmap->GetMatrix().mul(diff);

		cv::Scalar color = cv::Scalar(clValue.GetBlue(), clValue.GetGreen(), clValue.GetRed());
		cv::Scalar out_two = color * coeff;

		cv::add(out_one, out_two, out);
		bitmap->SetMatrix(out);
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
		bitmap->Rotate90();
	}
	return 0;
}

int CFiltreEffetCPU::Rotate180()
{
	CRegardsBitmap* bitmap;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->Rotate180();
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
		bitmap->Rotate270();
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
		resize(bitmap->GetMatrix(), dst, dst.size(), 0, 0, INTER_CUBIC);
		bitmap->SetMatrix(dst);
		dst.release();
	}

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
			bitmap->SetMatrix(bitmapOut->GetMatrix());
		}
		else
		{
			bitmap->SetFilename(pBitmap->GetFilename());
			bitmap->SetMatrix(pBitmap->GetMatrix());
			//bitmap->SetBitmap(pBitmap->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
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
		copy->SetMatrix(bitmapOut->GetMatrix());
	}
	else
	{
		copy->SetFilename(pBitmap->GetFilename());
		copy->SetMatrix(pBitmap->GetMatrix());
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

	BrightnessAndContrastAuto(bitmap->GetMatrix(), clipHistPercent);

	return 0;
}
