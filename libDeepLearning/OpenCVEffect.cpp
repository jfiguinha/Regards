#include "header.h"
#include "OpenCVEffect.h"
#include <RegardsBitmap.h>
#include <CvPlot/cvplot.h>

using namespace Regards::OpenCV;
using namespace cv;


class COpenCVEffectPimpl
{
public:
	static void BrightnessAndContrastAuto(cv::Mat &image, float clipHistPercent = 0);
	static void EqualizeHistogram(cv::Mat &src);
	static void BrightnessAndContrastAuto(cv::UMat & image, float clipHistPercent);

};



void COpenCVEffectPimpl::EqualizeHistogram(cv::Mat &src)
{
	Mat chans[4];
	split(src, chans);

#pragma omp parallel for
	for (int i = 0; i < 4; i++)
	{
		chans[i] = Mat::zeros(src.rows, src.cols, CV_8UC1);
		cv::equalizeHist(chans[i], chans[i]);
	}

	merge(chans, 4, src);

}

/**
 *  \brief Automatic brightness and contrast optimization with optional histogram clipping
 *  \param [in]src Input image GRAY or BGR or BGRA
 *  \param [out]dst Destination image
 *  \param clipHistPercent cut wings of histogram at given percent tipical=>1, 0=>Disabled
 *  \note In case of BGRA image, we won't touch the transparency
*/
void COpenCVEffectPimpl::BrightnessAndContrastAuto(cv::Mat &image, float clipHistPercent)
{
	int histSize = 256;
	float alpha, beta;
	double minGray = 0, maxGray = 0;

	bool opencl = true;
	if (opencl)
	{
		cv::UMat gray;
		cvtColor(image, gray, cv::COLOR_BGR2GRAY);

		if (clipHistPercent == 0)
		{
			// keep full available range
			cv::minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			cv::UMat h;
			
			std::vector<int> channels = { 0 }; // Analyze only the channel 0.
			std::vector<int> hsize = { 256 }; //Quantize the intensities in the image using 256 levels even if all the levels are not present.
			std::vector<float> hranges = { 0,256 }; // The range is between 0 - 255 (so less than 256).

			cv::calcHist(std::vector<cv::UMat>(1, gray), channels, cv::noArray(), h, hsize, hranges);

			cv::Mat hist;

			h.copyTo(hist);

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

	}
	else
	{
		CV_Assert(clipHistPercent >= 0);
		CV_Assert((image.type() == CV_8UC1) || (image.type() == CV_8UC3) || (image.type() == CV_8UC4));

		//to calculate grayscale histogram
		cv::Mat gray;
		if (image.type() == CV_8UC1) gray = image;
		else if (image.type() == CV_8UC3) cvtColor(image, gray, cv::COLOR_BGR2GRAY);
		else if (image.type() == CV_8UC4) cvtColor(image, gray, cv::COLOR_BGR2GRAY);
		if (clipHistPercent == 0)
		{
			// keep full available range
			cv::minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			cv::Mat hist; //the grayscale histogram

			float range[] = { 0, 256 };
			const float* histRange = { range };
			bool uniform = true;
			bool accumulate = false;
			calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

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
	}


	// current range
	float inputRange = maxGray - minGray;

	alpha = (histSize - 1) / inputRange;   // alpha expands current range to histsize range
	beta = -minGray * alpha;             // beta shifts current range so that minGray will go to 0

	cv::convertScaleAbs(image, image, alpha, beta);

	return;
}


/**
 *  \brief Automatic brightness and contrast optimization with optional histogram clipping
 *  \param [in]src Input image GRAY or BGR or BGRA
 *  \param [out]dst Destination image
 *  \param clipHistPercent cut wings of histogram at given percent tipical=>1, 0=>Disabled
 *  \note In case of BGRA image, we won't touch the transparency
*/
void COpenCVEffectPimpl::BrightnessAndContrastAuto(cv::UMat & src, float clipHistPercent)
{
	int histSize = 256;
	float alpha, beta;
	double minGray = 0, maxGray = 0;

	bool opencl = true;
	if (opencl)
	{
		cv::UMat gray;
		cvtColor(src, gray, cv::COLOR_BGR2GRAY);

		if (clipHistPercent == 0)
		{
			// keep full available range
			cv::minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			cv::UMat h;

			std::vector<int> channels = { 0 }; // Analyze only the channel 0.
			std::vector<int> hsize = { 256 }; //Quantize the intensities in the image using 256 levels even if all the levels are not present.
			std::vector<float> hranges = { 0,256 }; // The range is between 0 - 255 (so less than 256).

			cv::calcHist(std::vector<cv::UMat>(1, gray), channels, cv::noArray(), h, hsize, hranges);

			cv::Mat hist;

			h.copyTo(hist);

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

	}

	// current range
	float inputRange = maxGray - minGray;

	alpha = (histSize - 1) / inputRange;   // alpha expands current range to histsize range
	beta = -minGray * alpha;             // beta shifts current range so that minGray will go to 0

	cv::convertScaleAbs(src, src, alpha, beta);

	return;
}

void COpenCVEffect::BrightnessAndContrastAuto(cv::UMat & image, float clipHistPercent)
{
	COpenCVEffectPimpl::BrightnessAndContrastAuto(image, 1);
}

void COpenCVEffect::BrightnessAndContrastAuto(cv::Mat & image, float clipHistPercent)
{
	COpenCVEffectPimpl::BrightnessAndContrastAuto(image, 1);
}


void COpenCVEffect::BrightnessAndContrastAuto(CRegardsBitmap * pBitmap, float clipHistPercent)
{
	cv::Mat dest;
	cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
	COpenCVEffectPimpl::BrightnessAndContrastAuto(image, 1);
	pBitmap->SetBitmap(dest.data, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	dest.release();
	image.release();
}

void COpenCVEffect::EqualizeHistogram(CRegardsBitmap * pBitmap)
{
	cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
	COpenCVEffectPimpl::EqualizeHistogram(image);
	pBitmap->SetBitmap(image.data, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	image.release();
}


void COpenCVEffect::LoadAndRotate(const wxString& filePath, const int& rotate)
{
	Mat src = imread(filePath.ToStdString());
	if (rotate == 90) {
		// Rotate clockwise 270 degrees
		cv::transpose(src, src);
		cv::flip(src, src, 0);
	}
	else if (rotate == 180) {
		// Rotate clockwise 180 degrees
		cv::flip(src, src, -1);
	}
	else if (rotate == 270) {
		// Rotate clockwise 90 degrees
		cv::transpose(src, src);
		cv::flip(src, src, 1);
	}
	imwrite(filePath.ToStdString(), src);
	src.release();
}


void COpenCVEffect::CalculateHistogram(CRegardsBitmap * pBitmap, CRegardsBitmap* histogram, const int &colorChoice, const wxColour& colorBgnd, const wxColour& colorFont)
{
	Mat hist;
	cv::Mat src;
	int hist_w = histogram->GetBitmapWidth(), hist_h = histogram->GetBitmapHeight();
	cv::Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	Scalar color = Scalar(255, 255, 255);

	if (colorChoice == 0)
	{
		cvtColor(image, src, cv::COLOR_BGRA2GRAY);
		int histSize = 256;
		float range[] = { 0, 256 }; //the upper boundary is exclusive
		const float* histRange = { range };
		bool uniform = true, accumulate = false;

		calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		int hist_w = histogram->GetBitmapWidth(), hist_h = histogram->GetBitmapHeight();
		int bin_w = cvRound((double)hist_w / histSize);
		normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
				color, 2, 8, 0);
		}
	}
	else
	{
		cvtColor(image, src, cv::COLOR_BGRA2BGR);
		vector<Mat> bgr_planes;
		split(src, bgr_planes);
		int histSize = 256;
		float range[] = { 0, 256 }; //the upper boundary is exclusive
		const float* histRange = { range };
		bool uniform = true, accumulate = false;
		
		if (colorChoice == 1)
			calcHist(&bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		if (colorChoice == 2)
			calcHist(&bgr_planes[1], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		if (colorChoice == 3)
			calcHist(&bgr_planes[2], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		int hist_w = histogram->GetBitmapWidth(), hist_h = histogram->GetBitmapHeight();
		int bin_w = cvRound((double)hist_w / histSize);

		
		if (colorChoice == 1)
			color = Scalar(255, 0, 0);
		if (colorChoice == 2)
			color = Scalar(0, 255, 0);
		if (colorChoice == 3)
			color = Scalar(0, 0, 255);
		normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
					Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
					color, 2, 8, 0);
		}

		bgr_planes[0].release();
		bgr_planes[1].release();
		bgr_planes[2].release();
	}
	

	//auto axes = CvPlot::plot(hist, "-b");
	CvPlot::Axes axes = CvPlot::makePlotAxes();
	axes.SetBackgroundColor(cv::Scalar(colorBgnd.Blue(), colorBgnd.Green(), colorBgnd.Red()));
	axes.SetTextColor(cv::Scalar(colorFont.Blue(), colorFont.Green(), colorFont.Red()));
	axes.create<CvPlot::Series>(hist)
		.setColor(color);
	//axes.Set


	cv::Mat mat = axes.render(hist_h, hist_w);


	cvtColor(mat, histImage, cv::COLOR_BGR2BGRA);
	histogram->SetBitmap(histImage.data, hist_w, hist_h);
	histogram->VertFlipBuf();

	hist.release();
	image.release();
	src.release();
	mat.release();
	histImage.release();
}

void COpenCVEffect::NormalizeHistogram(CRegardsBitmap* pictureData, const int& colorChoice, const int& minValue, const int& maxValue)
{
	int min = 0;
	int max = 255;
	//pixel = (pixel - min) * (nmax - nmin) / (max - min) + nmin

	//uint8_t * data = pictureData->GetPtBitmap();
	for (int y = 0; y < pictureData->GetBitmapHeight(); y++)
	{
		for (int x = 0; x < pictureData->GetBitmapWidth(); x++)
		{
			CRgbaquad color = pictureData->GetColorValue(x, y);
			if (colorChoice == 0)
			{
				color.SetRed((color.GetRed() - min) * (maxValue - minValue) / (max - min) + minValue);
				color.SetGreen((color.GetGreen() - min) * (maxValue - minValue) / (max - min) + minValue);
				color.SetBlue((color.GetBlue() - min) * (maxValue - minValue) / (max - min) + minValue);
			}
			else if (colorChoice == 1)
			{
				color.SetRed((color.GetRed() - min) * (maxValue - minValue) / (max - min) + minValue);
			}
			else if (colorChoice == 2)
			{
				color.SetGreen((color.GetGreen() - min) * (maxValue - minValue) / (max - min) + minValue);
			}
			else if (colorChoice == 3)
			{
				color.SetBlue( (color.GetBlue() - min) * (maxValue - minValue) / (max - min) + minValue);
			}
			pictureData->SetColorValue(x, y, color);
		}
	}
	
}
