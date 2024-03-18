#include <header.h>
#include "FiltreEffetCPU.h"
#include "PerlinNoise.h"
#include "LensFlare.h"
#include "MotionBlur.h"
#include "Filtre.h"
#include <DeepLearning.h>
#include "Wave.h"
#include <ImageLoadingFormat.h>
#include <hqdn3d.h>
#include "MeanShift.h"
#include <fstream>
#include <opencv2/xphoto.hpp>
#include <FaceDetector.h>
#include "VideoStabilization.h"
#include <opencv2/dnn_superres.hpp>
#include <FileUtility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <ConvertUtility.h>

using namespace Regards::OpenCV;
using namespace Regards::OpenGL;
using namespace Regards::DeepLearning;
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
	static double getMaxDisFromCorners(const Size& imgSize, const Point& center);
	static double dist(Point a, Point b);
	static Mat upscaleImage(Mat img, int method, int scale);
	static string GenerateModelPath(string modelName, int scale);
	static bool TestIfMethodIsValid(int method, int scale);
	static Rect CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle,
	                       float ratioX, float ratioY, int x, int y, float left, float top);
};


Rect CFiltreEffetCPUImpl::CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV,
                                     int angle, float ratioX, float ratioY, int x, int y, float left, float top)
{
	Rect rect;
	float posX = static_cast<float>(x) * ratioX + left * ratioX;
	float posY = static_cast<float>(y) * ratioY + top * ratioY;

	if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
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

		posX = widthIn - posX - 1;
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
    wxString documentPath = CFileUtility::GetDocumentFolderPath();

	wxString path = "";
#ifdef WIN32
	path = documentPath + "\\model\\" + modelName + "_x" + to_string(scale) + ".pb";
#else
	path = documentPath + "/model/"  + modelName + "_x" + to_string(scale) + ".pb";
#endif
	return CConvertUtility::ConvertToStdString(path);
}

bool CFiltreEffetCPUImpl::TestIfMethodIsValid(int method, int scale)
{
	if (method == EDSR && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	if (method == ESPCN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	if (method == FSRCNN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	if (method == LapSRN && (scale == 2 || scale == 4 || scale == 8))
	{
		return true;
	}
	return false;
}

Mat CFiltreEffetCPUImpl::upscaleImage(Mat img, int method, int scale)
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
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return outputImage;
}


CFiltreEffetCPU::CFiltreEffetCPU(CRgbaquad back_color, CImageLoadingFormat* bitmap)
	: IFiltreEffet(back_color)
{
	this->backColor = back_color;

	if (bitmap != nullptr && bitmap->IsOk())
	{
		Mat local = bitmap->GetOpenCVPicture();
		filename = bitmap->GetFilename();
		vector<Mat> channels;
		extractChannel(local, alphaChannel, 3);
		cvtColor(local, input, COLOR_BGRA2BGR);
		preview = false;
	}
}

Mat CFiltreEffetCPU::GetMat()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	return image;
}

UMat CFiltreEffetCPU::GetUMat()
{
	UMat image;
	if (preview)
		paramOutput.copyTo(image);
	else
		input.copyTo(image);

	return image;
}


bool CFiltreEffetCPU::StabilizeVideo(Regards::OpenCV::COpenCVStabilization* openCVStabilization)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	bool frameStabilized = false;

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
	if (!wxFileExists(filePath))
	{
		return;
	}

	Mat src = imread(CConvertUtility::ConvertToStdString(filePath));
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
	imwrite(CConvertUtility::ConvertToStdString(filePath), src);
	src.release();
}


int CFiltreEffetCPU::GetWidth()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	return image.size().width;
}

int CFiltreEffetCPU::GetHeight()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	return image.size().height;
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

		float range[] = {0, 256};
		const float* histRange = {range};
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

int CFiltreEffetCPU::BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect& listFace)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	if (nbFace > 0)
	{
		//cv::flip(image, image, 0);

		try
		{
			//Resize the rectangle
			wxRect rectCopy = listFace;
			Rect rect;
			int width = listFace.width;
			rect.width = listFace.width * 2;
			rect.x = listFace.x - (width / 2);
			rect.y = 0;
			rect.height = image.size().height;

			Mat blur;
			cv::GaussianBlur(image, blur, Size(boxsize, boxsize), radius);


			int maxWidth = image.cols;
			int maxHeight = image.rows;
			if ((rect.width + rect.x) > maxWidth)
				rect.width = maxWidth - rect.x;

			if ((rect.height + rect.y) > maxHeight)
				rect.height = maxHeight - rect.y;

			Mat croppedImage = image(rect);
			Mat src_gray;
			Mat detected_edges;
			Mat output;

			Mat blur_crop;
			cv::GaussianBlur(croppedImage, blur_crop, Size(3, 3), 0);
			//medianBlur(croppedImage, blur_crop, 3);

			cvtColor(blur_crop, src_gray, COLOR_BGR2GRAY);

			// apply your filter
			Canny(src_gray, src_gray, 200, 100);

			// find the contours
			vector<vector<Point>> contours;
			findContours(src_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

			// you could also reuse img1 here
			Mat mask = Mat::zeros(src_gray.rows, src_gray.cols, CV_8UC1);
			//threshold(mask, mask, 0, 255, THRESH_BINARY_INV);
			// CV_FILLED fills the connected components found
			drawContours(mask, contours, -1, Scalar(255), FILLED);


			Point center(rect.width / 2, (rectCopy.y + rectCopy.height) / 2); //Declaring the center point
			Size xy((rectCopy.width / 2) * 1.5, (rectCopy.height / 2) * 1.5);
			//Declaring the major and minor axis of the ellipse//
			int angle = 0; //angle of rotation//
			int starting_point = 0; //Starting point of the ellipse//
			int ending_point = 360; //Ending point of the ellipse//
			Scalar line_Color(255, 255, 255); //Color of the Ellipse//

			//namedWindow("whiteMatrix");//Declaring a window to show the ellipse//
			ellipse(mask, center, xy, angle, starting_point, ending_point, line_Color, -1, LINE_AA);
			//Drawing the ellipse

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


			cv::GaussianBlur(croppedImage, blur_crop, Size(boxsize, boxsize), radius);


			// normalize so imwrite(...)/imshow(...) shows the mask correctly!
			normalize(mask.clone(), mask, 0.0, 255.0, NORM_MINMAX, CV_8UC1);


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


			Rect _rect(0, rect.height / 2, rect.width, rect.height / 2);
			rectangle(mask, _rect, Scalar(255, 255, 255), -1);

			// and copy the magic apple
			croppedImage.copyTo(blur_crop, mask);

			Rect copy(rect.x, rect.y, croppedImage.cols, croppedImage.rows);
			blur_crop.copyTo(blur(copy));

			blur.copyTo(image);
		}
		catch (...)
		{
		}

		//cv::flip(image, image, 0);
	}
	//
	return 0;
}

int CFiltreEffetCPU::OilPaintingEffect(const int& size, const int& dynRatio)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	xphoto::oilPainting(image, image, size, dynRatio, COLOR_BGR2Lab);

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
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	auto firstPt = Point(image.size().width / 2, image.size().height / 2);
	double maxImageRad = static_cast<float>(radius / 100.0) * CFiltreEffetCPUImpl::getMaxDisFromCorners(
		image.size(), firstPt);
	double maxImageRadPower = maxImageRad * static_cast<float>(power / 100.0);

	Mat labImg(image.size(), CV_8UC3);

	cvtColor(image, labImg, COLOR_BGR2Lab);

	for (int row = 0; row < image.size().height; row++)
	{
		for (int col = 0; col < image.size().width; col++)
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

	cvtColor(labImg, image, COLOR_Lab2BGR);
	labImg.release();
	return 0;
}


void CFiltreEffetCPU::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (bitmap != nullptr)
	{
		Mat local = bitmap->GetOpenCVPicture();
		filename = bitmap->GetFilename();
		vector<Mat> channels;
		extractChannel(local, alphaChannel, 3);
		cvtColor(local, input, COLOR_BGRA2BGR);
		preview = false;
	}
}

int CFiltreEffetCPU::RedEye()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	bool fastDetection = true;
	CRegardsConfigParam* param = CParamInit::getInstance();
	if (param != nullptr)
		fastDetection = param->GetFastDetectionFace();

	CDeepLearning::DetectEyes(image, fastDetection);

	return 0;
}

int CFiltreEffetCPU::WaveFilter(int x, int y, short height, int scale, int radius)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	auto waveFilter = new CWaveFilter();
	waveFilter->ProcessEffect(image, x, y, height, scale, radius);
	delete waveFilter;
	return 0;
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

	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat dst;

	// Convert from BGR color to Grayscale
	Mat srcGray;
	cvtColor(image, srcGray, COLOR_BGRA2GRAY);

	// Remove the pixel noise with a good Median filter, before we start detecting edges.
	medianBlur(srcGray, srcGray, 7);

	Size size = srcGray.size();
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
		cvtColor(mask, image, COLOR_GRAY2BGR);
		dst.release();
		mask.release();
		edges.release();
		return -1;
	}

	// Do the bilateral filtering at a shrunken scale, since it
	// runs so slowly but doesn't need full resolution for a good effect.
	Size smallSize;
	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;

	auto smallImg = Mat(smallSize, CV_8UC3);
	resize(image, smallImg, smallSize, 0, 0, INTER_LINEAR);

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
	//cvtColor(smallImg, srcBgr, COLOR_BGR2BGRA);
	resize(smallImg, dst, size, 0, 0, INTER_LINEAR);

	// Use the blurry cartoon image, except for the strong edges that we will leave black.
	dst.copyTo(image, mask);

	dst.release();
	mask.release();
	tmp.release();
	//srcBgr.release();
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
	Mat image, dst;
	if (preview)
		image = paramOutput;
	else
		image = input;

	cvtColor(image, dst, COLOR_BGR2Lab);
	// Initilize Mean Shift with spatial bandwith and color bandwith
	CMeanShift msProcess(fSpatialRadius, fColorRadius);
	//MSProc(fSpatialRadius, fColorRadius);
	// Filtering Process
	msProcess.MSFiltering(dst);
	cvtColor(dst, image, COLOR_Lab2RGB);

	return 0;
}

int CFiltreEffetCPU::DetailEnhance(const double& sigma_s, const double& sigma_r)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	try
	{
		Mat output;
		detailEnhance(image, output, sigma_s, sigma_r);
		output.copyTo(image);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return 0;
}

int CFiltreEffetCPU::EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	try
	{
		Mat output;
		edgePreservingFilter(image, output, flags, sigma_s, sigma_r);
		output.copyTo(image);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return 0;
}

int CFiltreEffetCPU::PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	try
	{
		Mat img1;
		Mat output;
		pencilSketch(image, img1, output, sigma_s, sigma_r, shade_factor);
		output.copyTo(image);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return 0;
}

int CFiltreEffetCPU::Stylization(const double& sigma_s, const double& sigma_r)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	try
	{
		Mat output;
		stylization(image, output, sigma_s, sigma_r);
		output.copyTo(image);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return 0;
}


int CFiltreEffetCPU::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	try
	{
		Mat output;
		bilateralFilter(image, output, fSize, sigmaX, sigmaP, BORDER_DEFAULT);
		output.copyTo(image);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	return 0;
}

int CFiltreEffetCPU::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
                                   const int& searchWindowSize)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	try
	{
		fastNlMeansDenoisingColored(image, image, h, hColor, templateWindowSize, searchWindowSize);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	return 0;
}


int CFiltreEffetCPU::SharpenMasking(const float& sharpness)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	try
	{
		Mat blurred;
		double sigma = 1, threshold = 5, amount = sharpness;
		cv::GaussianBlur(image, blurred, Size(), sigma, sigma);
		Mat lowConstrastMask = abs(image - blurred) < threshold;
		Mat sharpened = image * (1 + amount) + blurred * (-amount);
		image.copyTo(sharpened, lowConstrastMask);
		sharpened.copyTo(image);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	return 0;
}

CFiltreEffetCPU::~CFiltreEffetCPU()
{
	if (hq3d != nullptr)
		delete hq3d;
}

wxImage CFiltreEffetCPU::GetwxImage()
{
	/*
	CImageLoadingFormat picture;

	if (preview)
		picture.SetPicture(paramOutput);
	else
		picture.SetPicture(input);


	
	return picture.GetwxImage();
	*/


	Mat im2;

	if (preview)
		im2 = paramOutput;
	else
		im2 = input;

	//if (im2.channels() == 1) { cvtColor(im2, im2, cv::COLOR_GRAY2RGB); }
	//else if (im2.channels() == 4) { cvtColor(im2, im2, cv::COLOR_BGRA2RGB); }
	//else { cvtColor(im2, im2, cv::COLOR_BGR2RGB); }

	long imsize = im2.rows * im2.cols * im2.channels();
	wxImage wx(im2.cols, im2.rows, static_cast<unsigned char*>(malloc(imsize)), false);
	unsigned char* s = im2.data;
	unsigned char* d = wx.GetData();
	memcpy(d, s, imsize);
	return wx;
}

Mat CFiltreEffetCPU::Interpolation(const Mat& inputData, const int& widthOut, const int& heightOut, const wxRect& rc,
                                   const int& method, int flipH, int flipV, int angle, int ratio)
{
	Mat cvImage;
	//cv::Mat cvImage;
	//inputData.copyTo(cvImage);
	try
	{
		float ratioX = static_cast<float>(inputData.cols) / rc.width;
		float ratioY = static_cast<float>(inputData.rows) / rc.height;
		if (angle == 90 || angle == 270)
		{
			ratioX = static_cast<float>(inputData.cols) / static_cast<float>(rc.height);
			ratioY = static_cast<float>(inputData.rows) / static_cast<float>(rc.width);
		}

		Rect rectGlobal;
		Rect rect_begin = CFiltreEffetCPUImpl::CalculRect(inputData.cols, inputData.rows, widthOut, heightOut, flipH,
		                                                  flipV, angle, ratioX, ratioY, 0, 0, rc.x, rc.y);
		Rect rect_end = CFiltreEffetCPUImpl::CalculRect(inputData.cols, inputData.rows, widthOut, heightOut, flipH,
		                                                flipV, angle, ratioX, ratioY, widthOut, heightOut, rc.x, rc.y);
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

		if ((rectGlobal.height + rectGlobal.y) > inputData.rows)
		{
			rectGlobal.height = inputData.rows - rectGlobal.y;
		}
		if ((rectGlobal.width + rectGlobal.x) > inputData.cols)
		{
			rectGlobal.width = inputData.cols - rectGlobal.x;
		}

		//cv::Mat crop;
		inputData(rectGlobal).copyTo(cvImage);
		//crop.copyTo(cvImage);

		if (angle == 270)
		{
			if (flipV && flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_CLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_COUNTERCLOCKWISE);
			else
				cv::rotate(cvImage, cvImage, ROTATE_90_CLOCKWISE);
		}
		else if (angle == 90)
		{
			if (flipV && flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_COUNTERCLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_CLOCKWISE);
			else
				cv::rotate(cvImage, cvImage, ROTATE_90_COUNTERCLOCKWISE);
		}
		else if (angle == 180)
		{
			cv::rotate(cvImage, cvImage, ROTATE_180);
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
				resize(cvImage, cvImage, Size(widthOut, heightOut), method);
			}
		}

		if (cvImage.cols != widthOut || cvImage.rows != heightOut)
			resize(cvImage, cvImage, Size(widthOut, heightOut), INTER_NEAREST_EXACT);

		//Apply Transformation

		if (flipH)
		{
			if (angle == 90 || angle == 270)
				flip(cvImage, cvImage, 0);
			else
				flip(cvImage, cvImage, 1);
		}
		if (flipV)
		{
			if (angle == 90 || angle == 270)
				flip(cvImage, cvImage, 1);
			else
				flip(cvImage, cvImage, 0);
		}

		//cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2BGRA);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	return cvImage;
}

void CFiltreEffetCPU::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
                                    int flipH, int flipV, int angle, int ratio)
{
	paramOutput = Interpolation(input, widthOut, heightOut, rc, method, flipH, flipV, angle, ratio);
	preview = true;
}


int CFiltreEffetCPU::HistogramNormalize()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	vector<Mat> bgr_planes;
	split(image, bgr_planes);
	int gridsize = 8;
	Ptr<CLAHE> clahe = createCLAHE(2.0, Size(gridsize, gridsize));
	clahe->apply(bgr_planes[0], bgr_planes[0]);
	clahe->apply(bgr_planes[1], bgr_planes[1]);
	clahe->apply(bgr_planes[2], bgr_planes[2]);
	cv::merge(bgr_planes, image);

	bgr_planes[0].release();
	bgr_planes[1].release();
	bgr_planes[2].release();

	return 0;
	//return CHistogramme::HistogramNormalize(pBitmap);
}

int CFiltreEffetCPU::HistogramEqualize()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	vector<Mat> bgr_planes;
	split(image, bgr_planes);
	equalizeHist(bgr_planes[0], bgr_planes[0]);
	equalizeHist(bgr_planes[1], bgr_planes[1]);
	equalizeHist(bgr_planes[2], bgr_planes[2]);

	cv::merge(bgr_planes, image);

	bgr_planes[0].release();
	bgr_planes[1].release();
	bgr_planes[2].release();

	image.release();

	return 0;
}

int CFiltreEffetCPU::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	if (hq3d == nullptr)
	{
		hq3d = new Chqdn3d(image.size().width, image.size().height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	}
	else if (oldLevelDenoise != LumSpac || image.size().width != oldwidthDenoise || image.size().height
		!= oldheightDenoise)
	{
		delete hq3d;
		hq3d = new Chqdn3d(image.size().width, image.size().height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	}

	hq3d->ApplyDenoise3D(image);


	return 0;
}

int CFiltreEffetCPU::LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType,
                               const int& iIntensity, const int& iColor, const int& iColorIntensity)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	{
	cv:Mat output;
		cvtColor(image, output, COLOR_BGR2BGRA);
		auto filtre = new CLensFlare();
		filtre->LensFlare(&output, iPosX, iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
		delete filtre;
		cvtColor(output, image, COLOR_BGRA2BGR);
	}
	return 0;
}

//---------------------------------------------------------------------
//Effet CRgbaquad 
//---------------------------------------------------------------------
int CFiltreEffetCPU::RGBFilter(const int& red, const int& green, const int& blue)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	auto color = Scalar(red, green, blue);
	image = image + color;
	return 0;
}

int CFiltreEffetCPU::Solarize(const long& threshold)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	auto filtre = new CSolarize(static_cast<int>(threshold));
	filtre->SetParameter(image, backColor);
	filtre->Compute();
	delete filtre;

	return 0;
}

int CFiltreEffetCPU::Posterize(const float& level, const float& gamma)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	auto filtre = new CPosterize(level, gamma);
	filtre->SetParameter(image, backColor);
	filtre->Compute();
	delete filtre;

	return 0;
}


//---------------------------------------------------------------------
//Effet Clouds 
//---------------------------------------------------------------------
int CFiltreEffetCPU::CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude,
                                  const float& frequence, const int& octave, const int& intensity)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	auto m_perlinNoise = new CPerlinNoise();
	auto localBitmap = Mat(250, 250, CV_8UC4);
	m_perlinNoise->Clouds(localBitmap, color1, color2, amplitude / 100.0f, frequence / 100.0f, octave);
	delete m_perlinNoise;
	resize(localBitmap, localBitmap, Size(image.size().width, image.size().height), INTER_CUBIC);
	Fusion(localBitmap, intensity / 100.0f);
	return 0;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffetCPU::Swirl(const float& radius, const float& angle)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	auto filtre = new CSwirl(angle, radius);
	filtre->SetParameter(image, backColor);
	filtre->Compute();
	delete filtre;

	return 0;
}

//---------------------------------------------------------------------
//Lumière et Contraste 
//---------------------------------------------------------------------
int CFiltreEffetCPU::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	float alpha = contrast / 100.0f; //# Contrast control(1.0 - 3.0)
	float beta = brightness; //# Brightness control(0 - 100)
	convertScaleAbs(image, image, alpha, beta);
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NiveauDeGris()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat dest;
	cvtColor(image, dest, COLOR_BGR2GRAY);
	cvtColor(dest, image, COLOR_GRAY2BGR);

	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NoirEtBlanc()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat dest;
	cvtColor(image, dest, COLOR_BGR2GRAY);
	threshold(dest, dest, 127, 255, THRESH_BINARY);
	cvtColor(dest, image, COLOR_GRAY2BGR);
	dest.release();

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sepia()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	Mat kernel =
	(cv::Mat_<float>(3, 3)
		<<
		0.272, 0.534, 0.131,
		0.349, 0.686, 0.168,
		0.393, 0.769, 0.189);

	cv::transform(image, image, kernel);

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Soften()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	short kernel[] = {1, 1, 1, 1, 8, 1, 1, 1, 1};
	auto filtre = new CMatrixConvolution(kernel, 3, 16, 0);
	filtre->SetParameter(image, backColor);
	filtre->Compute();
	delete filtre;

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Blur(const int& radius)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	blur(image, image, Size(radius, radius));
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::GaussianBlur(const int& radius, const int& boxSize)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	cv::GaussianBlur(image, image, Size(boxSize, boxSize), radius);
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Emboss()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	// Construct kernel (all entries initialized to 0)
	Mat kernel(3, 3, CV_32F, Scalar(0));
	kernel.at<float>(0, 0) = -1.0;
	kernel.at<float>(2, 2) = 1.0;

	filter2D(image, image, image.depth(), kernel);

	kernel.release();


	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::SharpenStrong()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

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

	filter2D(image, image, image.depth(), kernel);
	kernel.release();


	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sharpen()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat kernel(3, 3, CV_32F, Scalar(0));
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;

	filter2D(image, image, image.depth(), kernel);

	kernel.release();

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Erode()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	erode(image, image, Mat());

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Median()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	medianBlur(image, image, 3);

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Noise()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	auto filtre = new CNoise();
	filtre->SetParameter(image, backColor);
	filtre->Compute();
	delete filtre;

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Dilate()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	dilate(image, image, Mat());

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Negatif()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	bitwise_not(image, image);

	return 0;
}

int CFiltreEffetCPU::LensDistortionFilter(const int& size)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat eiffel;
	image.copyTo(eiffel);

	int halfWidth = eiffel.rows / 2;
	int halfHeight = eiffel.cols / 2;
	double strength = static_cast<double>(size) / 100;
	double correctionRadius = sqrt(pow(eiffel.rows, 2) + pow(eiffel.cols, 2)) / strength;

	int newX, newY;
	double distance;
	double theta;
	int sourceX;
	int sourceY;
	double r;
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; j++)
		{
			newX = i - halfWidth;
			newY = j - halfHeight;
			distance = sqrt(pow(newX, 2) + pow(newY, 2));
			r = distance / correctionRadius;
			if (r == 0.0)
				theta = 1;
			else
				theta = atan(r) / r;

			sourceX = round(halfWidth + theta * newX);
			sourceY = round(halfHeight + theta * newY);


			image.at<Vec3b>(i, j)[0] = eiffel.at<Vec3b>(sourceX, sourceY)[0];
			image.at<Vec3b>(i, j)[1] = eiffel.at<Vec3b>(sourceX, sourceY)[1];
			image.at<Vec3b>(i, j)[2] = eiffel.at<Vec3b>(sourceX, sourceY)[2];
		}
	}
	return 0;
	//imwrite("d:\\test.jpg", image);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreEdge()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat dest;
	cvtColor(image, dest, COLOR_BGR2GRAY);

	Mat img_blur;
	cv::GaussianBlur(dest, img_blur, Size(3, 3), 0, 0);
	Mat edges;
	Canny(img_blur, edges, 100, 200, 3, false);

	cvtColor(edges, image, COLOR_GRAY2BGR);

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreMosaic(const int& size)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	auto filtre = new CMosaic(size);
	filtre->SetParameter(image, backColor);
	filtre->Compute();
	delete filtre;

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipVertical()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	flip(image, image, 0);
	return 0;
}


int CFiltreEffetCPU::SuperResolutionNCNN()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	cv::Mat img_up = CFaceDetector::SuperResolution(image);
	
	if (preview)
		paramOutput = img_up;
	else
		input = img_up;

	return 0;
}

int CFiltreEffetCPU::Colorization()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	if (preview)
		paramOutput = CFaceDetector::Colorisation(image);
	else
		input = CFaceDetector::Colorisation(image);

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipHorizontal()
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	flip(image, image, 1);

	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	auto filtre = new CMotionBlur();
	filtre->MotionBlur(image, radius, sigma, angle);
	delete filtre;

	return 0;
}

int CFiltreEffetCPU::GroundGlassEffect(const double& radius)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat imageResult = image.clone();
	RNG rng;
	int randomNum;
	int Number = radius;

	for (int i = 0; i < image.rows - Number; i++)
		for (int j = 0; j < image.cols - Number; j++)
		{
			randomNum = rng.uniform(0, Number);
			imageResult.at<Vec3b>(i, j)[0] = image.at<Vec3b>(i + randomNum, j + randomNum)[0];
			imageResult.at<Vec3b>(i, j)[1] = image.at<Vec3b>(i + randomNum, j + randomNum)[1];
			imageResult.at<Vec3b>(i, j)[2] = image.at<Vec3b>(i + randomNum, j + randomNum)[2];
		}

	imageResult.copyTo(image);

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat out;
	// get rotation matrix for rotating the image around its center in pixel coordinates
	const Point2f center((image.cols - 1) / 2.0, (image.rows - 1) / 2.0);
	Mat rot = getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle, center not relevant
	Rect2f bbox = RotatedRect(Point2f(), image.size(), angle).boundingRect2f();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - image.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - image.rows / 2.0;

	warpAffine(image, out, rot, bbox.size());


	if (preview)
		out.copyTo(paramOutput);
	else
		out.copyTo(image);

	rot.release();

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;


	float coeff = static_cast<float>(intensity) / 100.0f;
	float diff = 1.0f - coeff;
	Mat out;
	Mat out_one;
	out_one = image.mul(diff);

	auto color = Scalar(clValue.GetBlue(), clValue.GetGreen(), clValue.GetRed());
	Scalar out_two = color * coeff;

	add(out_one, out_two, image);

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
	if (preview)
		cv::rotate(paramOutput, paramOutput, ROTATE_90_COUNTERCLOCKWISE);
	else
		cv::rotate(input, input, ROTATE_90_COUNTERCLOCKWISE);

	return 0;
}

int CFiltreEffetCPU::Rotate180()
{
	if (preview)
		cv::rotate(paramOutput, paramOutput, ROTATE_180);
	else
		cv::rotate(input, input, ROTATE_180);

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate270()
{
	if (preview)
		cv::rotate(paramOutput, paramOutput, ROTATE_90_CLOCKWISE);
	else
		cv::rotate(input, input, ROTATE_90_CLOCKWISE);

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Resize(const int& imageWidth, const int& imageHeight, const int& interpolation)
{
	resize(input, paramOutput, Size(imageHeight, imageWidth), 0, 0, INTER_CUBIC);
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Fusion(Mat& bitmapSecond, const float& pourcentage)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	Mat dst;
	cvtColor(bitmapSecond, dst, COLOR_BGRA2BGR);
	float beta = (1.0 - pourcentage);
	addWeighted(image, pourcentage, dst, beta, 0.0, dst);
	dst.copyTo(image);
	return 0;
}

Mat CFiltreEffetCPU::GetBitmap(const bool& source)
{
	Mat output;

	if (source)
	{
		input.copyTo(output);
		cvtColor(output, output, COLOR_BGR2BGRA);
		//cv::insertChannel(alphaChannel, output, 3);
	}
	else if (preview && !paramOutput.empty())
	{
		paramOutput.copyTo(output);
		cvtColor(output, output, COLOR_BGR2BGRA);
	}
	else
	{
		input.copyTo(output);
		cvtColor(output, output, COLOR_BGR2BGRA);
		//cv::insertChannel(alphaChannel, output, 3);
	}

	return output;
}

int CFiltreEffetCPU::BrightnessAndContrastAuto(float clipHistPercent)
{
	Mat image;
	if (preview)
		image = paramOutput;
	else
		image = input;

	BrightnessAndContrastAuto(image, clipHistPercent);

	return 0;
}
