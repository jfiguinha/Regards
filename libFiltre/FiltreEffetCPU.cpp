#include <header.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <utility>
#include "FiltreEffetCPU.h"
#include "PerlinNoise.h"
#include "LensFlare.h"
#include "MotionBlur.h"
#include "Filtre.h"
#include <DeepLearning.h>
#include "Wave.h"
#include <ImageLoadingFormat.h>
#include "avir.h"
#include "MeanShift.h"
#include <opencv2/xphoto.hpp>
#include <FaceDetector.h>
#include "VideoStabilization.h"
#include <opencv2/dnn_superres.hpp>
#include <FileUtility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <ConvertUtility.h>
#include <opencv2/xphoto/inpainting.hpp>

#include "InterpolationFilters.h"
#include <wx/filename.h>
#include <effect_id.h>
#include <appcontext.h>
using namespace Regards::OpenCV;
using namespace Regards::OpenGL;
using namespace Regards::DeepLearning;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;
using namespace Regards::FiltreEffet;

#define EDSR 0
#define ESPCN 1
#define FSRCNN 2
#define LapSRN 3

template<typename F>
void CFiltreEffetCPU::ExecuteSafe(F&& func)
{
	try
	{
		Mat& image = preview ? paramOutput : input;
		if (image.empty())
			return;

		func(image);
	}
	catch (const cv::Exception& e)
	{
		LogError(e.what());
	}
}

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
	static cv::Mat BuildContrastLUT(double alpha, double beta);
};

template<typename T, typename... Args>
void CFiltreEffetCPU::MakeAndCompute(cv::Mat& image, Args&&... args)
{
	auto filtre = std::make_unique<T>(std::forward<Args>(args)...);
	filtre->SetParameter(image, backColor);
	filtre->Compute();
}

void CFiltreEffetCPU::ApplyKernel3x3(cv::Mat& image, const cv::Mat& kernel)
{
	filter2D(image, image, image.depth(), kernel);
}

void CFiltreEffetCPU::FusionInternal(cv::Mat& image, const cv::Mat& bitmapSecond, float pourcentage)
{
	if (bitmapSecond.empty())
		return;

	Mat dst;
	if (bitmapSecond.channels() == 4)
		cvtColor(bitmapSecond, dst, COLOR_BGRA2BGR);
	else if (bitmapSecond.channels() == 3)
		dst = bitmapSecond;
	else
		return;

	if (dst.size() != image.size())
		resize(dst, dst, image.size(), 0, 0, INTER_LINEAR);

	const float beta = 1.0f - pourcentage;
	addWeighted(image, pourcentage, dst, beta, 0.0, image);
}

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
	wxString documentPath = CFileUtility::GetDocumentFolderPathWithFilename("model");
	wxFileName file(documentPath, wxString::Format("%s_x%d.pb", modelName, scale));
	return CConvertUtility::ConvertToStdString(file.GetFullPath());
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
		Mat local = bitmap->GetMatrix().getMat();
		filename = bitmap->GetFilename();
		vector<Mat> channels;
		extractChannel(local, alphaChannel, 3);
		cvtColor(local, input, COLOR_BGRA2BGR);
		preview = false;
	}
}

Regards::Picture::CPictureArray CFiltreEffetCPU::GetMatrix()
{
	Regards::Picture::CPictureArray out;
	ExecuteSafe([&](cv::Mat& image)
		{
			out.SetArray(image);
		});

	return out;
}


static const float a(0.073235f);
static const float b(0.176765f);
static const cv::Mat K = (cv::Mat_<float>(3, 3) << a, b, a, b, 0.0f, b, a, b, a);

int CFiltreEffetCPU::Inpaint(const cv::Mat &mask, int algorithm)
{
	ExecuteSafe([&](cv::Mat& image)
		{

			Mat reconstructed;

			{
				// distort image
				Mat im_distorted(image.size(), image.type(), Scalar::all(0));
				image.copyTo(im_distorted, mask); // copy valid pixels only (i.e. non-zero pixels in mask)
				// reconstruct the distorted image
				// choose quality profile fast (xphoto::INPAINT_FSR_FAST) or best (xphoto::INPAINT_FSR_BEST)

				xphoto::inpaint(im_distorted, mask, reconstructed, algorithm);

				reconstructed.convertTo(image, CV_8UC3);
			}
		});

    return 0;
}

bool CFiltreEffetCPU::StabilizeVideo(Regards::OpenCV::COpenCVStabilization* openCVStabilization)
{
	bool frameStabilized = false;
	ExecuteSafe([&](cv::Mat& image)
		{
			
			Regards::Picture::CPictureArray dest = Regards::Picture::CPictureArray(image);

			if (openCVStabilization != nullptr)
			{
				if (openCVStabilization->GetNbFrameBuffer() == 0)
				{
					openCVStabilization->BufferFrame(dest);
				}
				else
				{
					frameStabilized = true;
					openCVStabilization->AddFrame(dest);
				}

				if (frameStabilized)
				{
					Regards::Picture::CPictureArray output = openCVStabilization->CorrectFrame(dest);
					output.copyTo(image);
				}
			}
		});

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
	int width = 0;
	ExecuteSafe([&](cv::Mat& image)
		{
			width = image.size().width;
		});

	return width;
}

int CFiltreEffetCPU::GetHeight()
{
	int height = 0;
	ExecuteSafe([&](cv::Mat& image)
		{
			height = image.size().height;
		});
	return height;
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
    //printf("Use Auto Contrast \n");
    
	int histSize = 256;
	float alpha, beta;
	double minGray = 0, maxGray = 0;

	std::vector<cv::Mat> yuv_planes(3);
	cv::Mat gpuframe_3channel(image.size(), CV_8UC3);
	cv::cvtColor(image, gpuframe_3channel, COLOR_BGR2YUV, 3);
	cv::split(gpuframe_3channel, yuv_planes);


	if (clipHistPercent == 0)
	{
		// keep full available range
		minMaxLoc(yuv_planes[0], &minGray, &maxGray);
	}
	else
	{
		Mat hist; //the grayscale histogram

		float range[] = {0, 256};
		const float* histRange = {range};
		bool uniform = true;
		bool accumulate = false;
		calcHist(&yuv_planes[0], 1, nullptr, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

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
	float inputRange = static_cast<float>(maxGray - minGray);
	if (inputRange <= 0.0f)
		return;

	alpha = (histSize - 1) / inputRange; // alpha expands current range to histsize range
	beta = -minGray * alpha; // beta shifts current range so that minGray will go to 0

	convertScaleAbs(image, image, alpha, beta);
}

int CFiltreEffetCPU::BokehEffect(const int& radius, const int& boxsize, const int& nbFace, const wxRect& listFace)
{
	ExecuteSafe([&](cv::Mat& image)
		{

			if (nbFace > 0)
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
		});

	return 0;
}

int CFiltreEffetCPU::OilPaintingEffect(const int& size, const int& dynRatio)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			xphoto::oilPainting(image, image, size, dynRatio, COLOR_BGR2Lab);
		});
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
	ExecuteSafe([&](cv::Mat& image)
		{
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
		});
	return 0;
}


void CFiltreEffetCPU::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (bitmap != nullptr)
	{
		Mat local = bitmap->GetMatrix().getMat();
		filename = bitmap->GetFilename();
		vector<Mat> channels;
		extractChannel(local, alphaChannel, 3);
		cvtColor(local, input, COLOR_BGRA2BGR);
		preview = false;
	}
}

int CFiltreEffetCPU::RedEye()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			bool fastDetection = true;
			CRegardsConfigParam* param = CParamInit::getInstance();
			if (param != nullptr)
				fastDetection = param->GetFastDetectionFace();

			CDeepLearning::RemoveRedEyes(image, fastDetection);
		});

	return 0;
}

int CFiltreEffetCPU::WaveFilter(int x, int y, short height, int scale, int radius)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			auto waveFilter = std::make_unique<CWaveFilter>();
			waveFilter->ProcessEffect(image, x, y, height, scale, radius);

		});
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
	ExecuteSafe([&](cv::Mat& image)
	{
		const bool sketchMode = mode == 1;
		const bool alienMode = mode == 2;
		const bool evilMode = mode == 3;

		Mat srcGray;
		cvtColor(image, srcGray, COLOR_BGR2GRAY);
		medianBlur(srcGray, srcGray, 7);

		const Size size = srcGray.size();
		Mat mask(size, CV_8U);
		Mat edges(size, CV_8U);

		if (!evilMode)
		{
			Laplacian(srcGray, edges, CV_8U, 5);
			threshold(edges, mask, 80, 255, THRESH_BINARY_INV);
			RemovePepperNoise(mask);
		}
		else
		{
			Mat edges2;
			Scharr(srcGray, edges, CV_8U, 1, 0);
			Scharr(srcGray, edges2, CV_8U, 1, 0, -1);
			edges += edges2;
			threshold(edges, mask, 12, 255, THRESH_BINARY_INV);
			medianBlur(mask, mask, 3);
		}

		if (sketchMode)
		{
			cvtColor(mask, image, COLOR_GRAY2BGR);
			return;
		}

		const Size smallSize(std::max(1, size.width / 2), std::max(1, size.height / 2));
		Mat smallImg(smallSize, CV_8UC3);
		resize(image, smallImg, smallSize, 0, 0, INTER_LINEAR);

		Mat tmp(smallSize, CV_8UC3);
		for (int i = 0; i < 7; ++i)
		{
			bilateralFilter(smallImg, tmp, 9, 9, 7);
			bilateralFilter(tmp, smallImg, 9, 9, 7);
		}

		if (alienMode)
			ChangeFacialSkinColor(smallImg, edges);

		Mat dst;
		resize(smallImg, dst, size, 0, 0, INTER_LINEAR);
		dst.copyTo(image, mask);
	});

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
	Mat dst;
	ExecuteSafe([&](cv::Mat& image)
		{
			cvtColor(image, dst, COLOR_BGR2Lab);
			// Initilize Mean Shift with spatial bandwith and color bandwith
			CMeanShift msProcess(fSpatialRadius, fColorRadius);
			//MSProc(fSpatialRadius, fColorRadius);
			// Filtering Process
			msProcess.MSFiltering(dst);
			cvtColor(dst, image, COLOR_Lab2BGR);
		});
	return 0;
}

int CFiltreEffetCPU::DetailEnhance(const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat output;
			detailEnhance(image, output, sigma_s, sigma_r);
			output.copyTo(image);
		});

	return 0;
}

int CFiltreEffetCPU::EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat output;
			edgePreservingFilter(image, output, flags, sigma_s, sigma_r);
			output.copyTo(image);
		});
	return 0;
}

int CFiltreEffetCPU::PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat img1;
			Mat output;
			pencilSketch(image, img1, output, sigma_s, sigma_r, shade_factor);
			output.copyTo(image);
		});
	return 0;
}

int CFiltreEffetCPU::Stylization(const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat output;
			stylization(image, output, sigma_s, sigma_r);
			output.copyTo(image);
		});
	return 0;
}


int CFiltreEffetCPU::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat output;
			bilateralFilter(image, output, fSize, sigmaX, sigmaP, BORDER_DEFAULT);
			output.copyTo(image);
		});
	return 0;
}

int CFiltreEffetCPU::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
                                   const int& searchWindowSize)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			fastNlMeansDenoisingColored(image, image, h, hColor, templateWindowSize, searchWindowSize);
		});
	return 0;
}


int CFiltreEffetCPU::SharpenMasking(const float& sharpness)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat blurred;
			double sigma = 1, threshold = 5, amount = sharpness;
			cv::GaussianBlur(image, blurred, Size(), sigma, sigma);
			Mat lowConstrastMask = abs(image - blurred) < threshold;
			Mat sharpened = image * (1 + amount) + blurred * (-amount);
			image.copyTo(sharpened, lowConstrastMask);
			sharpened.copyTo(image);
		});
	return 0;
}

wxImage CFiltreEffetCPU::GetwxImage()
{
	wxImage wx;

	ExecuteSafe([&](cv::Mat& image)
	{
		if (image.empty())
			return;

		Mat rgb;
		if (image.channels() == 3)
			cvtColor(image, rgb, COLOR_BGR2RGB);
		else if (image.channels() == 4)
			cvtColor(image, rgb, COLOR_BGRA2RGB);
		else
			return;

		const size_t size = rgb.total() * rgb.elemSize();
		auto* data = static_cast<unsigned char*>(std::malloc(size));
		if (data == nullptr)
			return;

		std::memcpy(data, rgb.data, size);
		wx = wxImage(rgb.cols, rgb.rows, data, false);
	});

	return wx;
}


Mat CFiltreEffetCPU::Interpolation(const Mat& inputData, const int& widthOut, const int& heightOut, const wxRect& rc,
	const int& method, int flipH, int flipV, int angle, int ratio)
{
	Mat cvImage;
	if (inputData.empty() || widthOut <= 0 || heightOut <= 0 || rc.width <= 0 || rc.height <= 0)
		return cvImage;

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

		rectGlobal &= Rect(0, 0, inputData.cols, inputData.rows);
		if (rectGlobal.width <= 0 || rectGlobal.height <= 0)
			return cvImage;

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

		// Application des méthodes d'interpolation
		if (method == 7)
		{
			cv::Mat inBuf, outBuf(Size(widthOut, heightOut), CV_8UC4, Scalar(0, 0, 0));
			cvtColor(cvImage, inBuf, cv::COLOR_BGR2BGRA);

			avir::CImageResizer ImageResizer(8);
			avir::CImageResizerVars Vars;
			Vars.UseSRGBGamma = true;
			ImageResizer.resizeImage(
				reinterpret_cast<uint8_t*>(inBuf.data), inBuf.cols, inBuf.rows, inBuf.step,
				reinterpret_cast<uint8_t*>(outBuf.data), widthOut, heightOut, 4, 0, &Vars
			);

			cvtColor(outBuf, cvImage, cv::COLOR_BGRA2BGR);
		}
		else if (method > 7)
		{
			int local_method = method - 7 + 1000;
			std::unique_ptr<C2PassScale> m_LocalFilter;

			switch (local_method)
			{
			case BOXFILTER: m_LocalFilter = std::make_unique<CBoxFilter>(); break;
			case BILINEARFILTER: m_LocalFilter = std::make_unique<CBilinearFilter>(); break;
			case GAUSSIANFILTER: m_LocalFilter = std::make_unique<CGaussianFilter>(); break;
			case HAMMINGFILTER: m_LocalFilter = std::make_unique<CHammingFilter>(); break;
			case BLACKMANFILTER: m_LocalFilter = std::make_unique<CBlackmanFilter>(); break;
			case QUADRATICFILTER: m_LocalFilter = std::make_unique<CQuadraticFilter>(); break;
			case MITCHELLFILTER: m_LocalFilter = std::make_unique<CMitchellFilter>(); break;
			case TRIANGLEFILTER: m_LocalFilter = std::make_unique<CTriangleFilter>(); break;
			case HERMITEFILTER: m_LocalFilter = std::make_unique<CHermiteFilter>(); break;
			case HANNINGFILTER: m_LocalFilter = std::make_unique<CHanningFilter>(); break;
			case CATROMFILTER: m_LocalFilter = std::make_unique<CCatromFilter>(); break;
			}

			if (m_LocalFilter)
			{
				cv::Mat outBuf(Size(widthOut, heightOut), CV_8UC3, Scalar(0, 0, 0));
				m_LocalFilter->Execute(cvImage, outBuf);
				cvImage = outBuf;
			}
		}
		else
		{
			if (cvImage.cols != widthOut || cvImage.rows != heightOut)
			{
				resize(cvImage, cvImage, Size(widthOut, heightOut), method);
			}
		}




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
                                    int flipH, int flipV, int angle, int ratio, bool bgraOutput)
{
	paramOutput = Interpolation(input, widthOut, heightOut, rc, method, flipH, flipV, angle, ratio);
	preview = true;
}


int CFiltreEffetCPU::HistogramNormalize()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			vector<Mat> bgr_planes;
			split(image, bgr_planes);
			int gridsize = 8;
			Ptr<CLAHE> clahe = createCLAHE(2.0, Size(gridsize, gridsize));
			clahe->apply(bgr_planes[0], bgr_planes[0]);
			clahe->apply(bgr_planes[1], bgr_planes[1]);
			clahe->apply(bgr_planes[2], bgr_planes[2]);
			cv::merge(bgr_planes, image);
		});

	return 0;
	//return CHistogramme::HistogramNormalize(pBitmap);
}

int CFiltreEffetCPU::HistogramEqualize()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			vector<Mat> bgr_planes;
			split(image, bgr_planes);
			equalizeHist(bgr_planes[0], bgr_planes[0]);
			equalizeHist(bgr_planes[1], bgr_planes[1]);
			equalizeHist(bgr_planes[2], bgr_planes[2]);

			cv::merge(bgr_planes, image);
		});

	return 0;
}

int CFiltreEffetCPU::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			if (hq3d == nullptr)
			{
				hq3d = std::make_unique<Chqdn3d>(image.size().width, image.size().height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
			}
			else if (oldLevelDenoise != LumSpac || image.size().width != oldwidthDenoise || image.size().height
				!= oldheightDenoise)
			{
				hq3d.reset(new Chqdn3d(image.size().width, image.size().height, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault));
			}

			hq3d->ApplyDenoise3D(image);
		});

	return 0;
}

int CFiltreEffetCPU::LensFlare(const int& iPosX, const int& iPosY, const int& iPuissance, const int& iType,
                               const int& iIntensity, const int& iColor, const int& iColorIntensity)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat output;
			cvtColor(image, output, COLOR_BGR2BGRA);
			auto filtre = std::make_unique<CLensFlare>();
			filtre->LensFlare(&output, iPosX, iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
			cvtColor(output, image, COLOR_BGRA2BGR);
		});
	return 0;
}

//---------------------------------------------------------------------
//Effet CRgbaquad 
//---------------------------------------------------------------------
int CFiltreEffetCPU::RGBFilter(const int& red, const int& green, const int& blue)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			auto color = Scalar(red, green, blue);
			image = image + color;
		});
	return 0;
}

int CFiltreEffetCPU::Solarize(const long& threshold)
{
	ExecuteSafe([&](cv::Mat& image)
	{
		MakeAndCompute<CSolarize>(image, static_cast<int>(threshold));
	});

	return 0;
}

int CFiltreEffetCPU::Posterize(const float& level, const float& gamma)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			MakeAndCompute<CPosterize>(image, level);
		});

	return 0;
}


//---------------------------------------------------------------------
//Effet Clouds 
//---------------------------------------------------------------------
int CFiltreEffetCPU::CloudsFilter(const CRgbaquad& color1, const CRgbaquad& color2, const float& amplitude,
                                  const float& frequence, const int& octave, const int& intensity)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			auto m_perlinNoise = std::make_unique<CPerlinNoise>();
			auto localBitmap = Mat(250, 250, CV_8UC4);
			m_perlinNoise->Clouds(localBitmap, color1, color2, amplitude / 100.0f, frequence / 100.0f, octave);
			resize(localBitmap, localBitmap, Size(image.size().width, image.size().height), INTER_CUBIC);
			FusionInternal(image, localBitmap, intensity / 100.0f);
		});
	return 0;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffetCPU::Swirl(const float& radius, const float& angle)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			MakeAndCompute<CSwirl>(image, angle, radius);

		});

	return 0;
}

//---------------------------------------------------------------------
//Lumière et Contraste 
//---------------------------------------------------------------------
int CFiltreEffetCPU::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			float alpha = contrast / 100.0f; //# Contrast control(1.0 - 3.0)
			float beta = brightness; //# Brightness control(0 - 100)
			convertScaleAbs(image, image, alpha, beta);
		});
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NiveauDeGris()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat dest;
			cvtColor(image, dest, COLOR_BGR2GRAY);
			cvtColor(dest, image, COLOR_GRAY2BGR);
		});

	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NoirEtBlanc()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat dest;
			cvtColor(image, dest, COLOR_BGR2GRAY);
			threshold(dest, dest, 127, 255, THRESH_BINARY);
			cvtColor(dest, image, COLOR_GRAY2BGR);
		});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sepia()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat kernel =
				(cv::Mat_<float>(3, 3)
					<<
					0.272, 0.534, 0.131,
					0.349, 0.686, 0.168,
					0.393, 0.769, 0.189);

			cv::transform(image, image, kernel);
		});
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Soften()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			static short kernel[] = { 1, 1, 1, 1, 8, 1, 1, 1, 1 };
			MakeAndCompute<CMatrixConvolution>(image, kernel, 3, 16, 0);
		});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Blur(const int& radius)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			blur(image, image, Size(radius, radius));
		});
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::GaussianBlur(const int& radius, const int& boxSize)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			cv::GaussianBlur(image, image, Size(boxSize, boxSize), radius);
		});
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Emboss()
{
	ExecuteSafe([&](cv::Mat& image)
	{
		static const cv::Mat kernel = (cv::Mat_<float>(3, 3) << -1, 0, 0, 0, 0, 0, 0, 0, 1);
		ApplyKernel3x3(image, kernel);
	});

	return 0;
}
int CFiltreEffetCPU::SharpenStrong()
{
	ExecuteSafe([&](cv::Mat& image)
	{
		static const cv::Mat kernel = (cv::Mat_<float>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
		ApplyKernel3x3(image, kernel);
	});

	return 0;
}
int CFiltreEffetCPU::Sharpen()
{
	ExecuteSafe([&](cv::Mat& image)
	{
		static const cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		ApplyKernel3x3(image, kernel);
	});

	return 0;
}
int CFiltreEffetCPU::Erode()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			erode(image, image, Mat());
		});
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Median()
{
	ExecuteSafe([&](cv::Mat& image)
		{

			medianBlur(image, image, 3);
		});
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Noise()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			MakeAndCompute<CNoise>(image);

			});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Dilate()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			dilate(image, image, Mat());
		});
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Negatif()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			bitwise_not(image, image);
		});
	return 0;
}

int CFiltreEffetCPU::LensDistortionFilter(const int& size)
{
	if (size <= 0)
		return 0;

	ExecuteSafe([&](cv::Mat& image)
	{
		if (image.empty() || image.channels() != 3)
			return;

		const Mat source = image.clone();
		const int centerX = source.cols / 2;
		const int centerY = source.rows / 2;
		const double strength = static_cast<double>(size) / 100.0;
		const double correctionRadius = std::hypot(static_cast<double>(source.rows), static_cast<double>(source.cols)) / strength;

		for (int y = 0; y < image.rows; ++y)
		{
			for (int x = 0; x < image.cols; ++x)
			{
				const double dx = static_cast<double>(x - centerX);
				const double dy = static_cast<double>(y - centerY);
				const double distance = std::hypot(dx, dy);
				const double r = distance / correctionRadius;
				const double theta = r == 0.0 ? 1.0 : std::atan(r) / r;
				const int sourceX = std::clamp(static_cast<int>(std::lround(centerX + theta * dx)), 0, source.cols - 1);
				const int sourceY = std::clamp(static_cast<int>(std::lround(centerY + theta * dy)), 0, source.rows - 1);
				image.at<Vec3b>(y, x) = source.at<Vec3b>(sourceY, sourceX);
			}
		}
	});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreEdge()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			Mat dest;
			cvtColor(image, dest, COLOR_BGR2GRAY);

			Mat img_blur;
			cv::GaussianBlur(dest, img_blur, Size(3, 3), 0, 0);
			Mat edges;
			Canny(img_blur, edges, 100, 200, 3, false);

			cvtColor(edges, image, COLOR_GRAY2BGR);
		});
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreMosaic(const int& size)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			MakeAndCompute<CMosaic>(image, size);
		});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipVertical()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			flip(image, image, 0);
		});
	return 0;
}


int CFiltreEffetCPU::SuperResolutionNCNN()
{

	ExecuteSafe([&](cv::Mat& image)
		{
			image = CFaceDetector::SuperResolution(image);
		});
	
	return 0;
}

int CFiltreEffetCPU::Colorization()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			image = CFaceDetector::Colorisation(image);
		});



	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipHorizontal()
{
	ExecuteSafe([&](cv::Mat& image)
		{

			flip(image, image, 1);
		});

	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	ExecuteSafe([&](cv::Mat& image)
		{
			auto filtre = std::make_unique<CMotionBlur>();
			filtre->MotionBlur(image, radius, sigma, angle);
		});

	return 0;
}

int CFiltreEffetCPU::GroundGlassEffect(const double& radius)
{
	if (radius <= 0.0)
		return 0;

	ExecuteSafe([&](cv::Mat& image)
	{
		const int number = static_cast<int>(radius);
		if (number <= 0 || image.empty() || image.channels() != 3)
			return;

		Mat imageResult = image.clone();
		RNG rng;

		for (int y = 0; y < image.rows; ++y)
		{
			for (int x = 0; x < image.cols; ++x)
			{
				const int maxOffsetY = std::min(number - 1, image.rows - 1 - y);
				const int maxOffsetX = std::min(number - 1, image.cols - 1 - x);
				const int maxOffset = std::min(maxOffsetX, maxOffsetY);
				if (maxOffset <= 0)
					continue;

				const int randomNum = rng.uniform(0, maxOffset + 1);
				imageResult.at<Vec3b>(y, x) = image.at<Vec3b>(y + randomNum, x + randomNum);
			}
		}

		imageResult.copyTo(image);
	});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	ExecuteSafe([&](cv::Mat& image)
		{

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

			out.copyTo(image);
		});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	ExecuteSafe([&](cv::Mat& image)
		{

			float coeff = static_cast<float>(intensity) / 100.0f;
			float diff = 1.0f - coeff;
			Mat out;
			Mat out_one;
			out_one = image.mul(diff);

			auto color = Scalar(clValue.GetBlue(), clValue.GetGreen(), clValue.GetRed());
			Scalar out_two = color * coeff;

			add(out_one, out_two, image);
		});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate90()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			cv::rotate(image, image, ROTATE_90_COUNTERCLOCKWISE);
		});

	return 0;
}

int CFiltreEffetCPU::Rotate180()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			cv::rotate(image, image, ROTATE_180);
		});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate270()
{
	ExecuteSafe([&](cv::Mat& image)
		{
			cv::rotate(image, image, ROTATE_90_CLOCKWISE);
		});

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Resize(const int& imageWidth, const int& imageHeight, const int& interpolation)
{
	if (imageWidth <= 0 || imageHeight <= 0)
		return 0;

	try
	{
		if (input.empty())
			return 0;

		resize(input, paramOutput, Size(imageWidth, imageHeight), 0, 0, interpolation);
		preview = true;
	}
	catch (const cv::Exception& e)
	{
		LogError(e.what());
	}

	return 0;
}

int CFiltreEffetCPU::Fusion(Mat& bitmapSecond, const float& pourcentage)
{
	ExecuteSafe([&](cv::Mat& image)
	{
		FusionInternal(image, bitmapSecond, pourcentage);
	});
	return 0;
}

Mat CFiltreEffetCPU::GetBitmap(const bool& source)
{
	Mat output;
	const Mat* selected = source ? &input : ((preview && !paramOutput.empty()) ? &paramOutput : &input);
	if (selected->empty())
		return output;

	selected->copyTo(output);
	if (output.channels() == 3)
		cvtColor(output, output, COLOR_BGR2BGRA);

	return output;
}

int CFiltreEffetCPU::BrightnessAndContrastAuto(float clipHistPercent)
{
	
	ExecuteSafe([&](cv::Mat& image)
		{
			BrightnessAndContrastAuto(image, clipHistPercent);
		});

	return 0;
}
