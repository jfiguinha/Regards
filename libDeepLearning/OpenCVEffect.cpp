// ReSharper disable All
// ReSharper disable All
#include <header.h>
#include "OpenCVEffect.h"
#include <RegardsBitmap.h>
#include <CvPlot/cvplot.h>
#include <tbb/parallel_for.h>
using namespace Regards::OpenCV;
using namespace cv;




void COpenCVEffectPimpl::EqualizeHistogram(Mat& src)
{
	Mat chans[4];
	split(src, chans);

//	tbb::parallel_for(tbb::blocked_range<int>(0, 4),
//		[&](tbb::blocked_range<int> r)
		tbb::parallel_for(0, 4, 1, [=](int i)
		{
			//chans[i] = Mat::zeros(src.rows, src.cols, CV_8UC1);
			equalizeHist(chans[i], chans[i]);
		});

	merge(chans, 4, src);
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
auto COpenCVEffectPimpl::BrightnessAndContrastAuto(Mat& image, float clipHistPercent, bool opencl) -> void
{
	int histSize = 256;
	float alpha, beta;
	double minGray = 0, maxGray = 0;

	
	if (opencl)
	{
		UMat gray;
		cvtColor(image, gray, COLOR_BGRA2GRAY);

		if (clipHistPercent == 0)
		{
			// keep full available range
			minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			UMat h;

			std::vector<int> channels = {0}; // Analyze only the channel 0.
			std::vector<int> hsize = {256};
			//Quantize the intensities in the image using 256 levels even if all the levels are not present.
			std::vector<float> hranges = {0, 256}; // The range is between 0 - 255 (so less than 256).

			calcHist(std::vector<UMat>(1, gray), channels, noArray(), h, hsize, hranges);

			Mat hist;

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
		Mat gray;
		if (image.type() == CV_8UC1) gray = image;
		else if (image.type() == CV_8UC3) cvtColor(image, gray, COLOR_BGR2GRAY);
		else if (image.type() == CV_8UC4) cvtColor(image, gray, COLOR_BGR2GRAY);
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
	}


	// current range
	float inputRange = maxGray - minGray;

	alpha = (histSize - 1) / inputRange; // alpha expands current range to histsize range
	beta = -minGray * alpha; // beta shifts current range so that minGray will go to 0

	convertScaleAbs(image, image, alpha, beta);
}


// ReSharper disable once CppDoxygenUnresolvedReference
/**
 *  \brief Automatic brightness and contrast optimization with optional histogram clipping
 *  \param [in]src Input image GRAY or BGR or BGRA
 *  \param [out]dst Destination image
 *  \param clipHistPercent cut wings of histogram at given percent tipical=>1, 0=>Disabled
 *  \note In case of BGRA image, we won't touch the transparency
*/
void COpenCVEffectPimpl::BrightnessAndContrastAuto(UMat& src, float clipHistPercent)
{
	int histSize = 256;
	float alpha, beta;
	double minGray = 0, maxGray = 0;

	bool opencl = true;
	if (opencl)
	{
		UMat gray;
		cvtColor(src, gray, COLOR_BGR2GRAY);

		if (clipHistPercent == 0)
		{
			// keep full available range
			minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			UMat h;

			std::vector<int> channels = {0}; // Analyze only the channel 0.
			std::vector<int> hsize = {256};
			//Quantize the intensities in the image using 256 levels even if all the levels are not present.
			std::vector<float> hranges = {0, 256}; // The range is between 0 - 255 (so less than 256).

			calcHist(std::vector<UMat>(1, gray), channels, noArray(), h, hsize, hranges);

			Mat hist;

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

	alpha = (histSize - 1) / inputRange; // alpha expands current range to histsize range
	beta = -minGray * alpha; // beta shifts current range so that minGray will go to 0

	convertScaleAbs(src, src, alpha, beta);
}

void COpenCVEffect::BrightnessAndContrastAuto(UMat& image, float clipHistPercent)
{
	COpenCVEffectPimpl::BrightnessAndContrastAuto(image, 1);
}

void COpenCVEffect::BrightnessAndContrastAuto(Mat& image, float clipHistPercent)
{
	COpenCVEffectPimpl::BrightnessAndContrastAuto(image, 1);
}


void COpenCVEffect::BrightnessAndContrastAuto(CRegardsBitmap* pBitmap, float clipHistPercent)
{
	//Mat dest;
	//Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
	COpenCVEffectPimpl::BrightnessAndContrastAuto(pBitmap->GetMatrix(), 1);
	//pBitmap->SetBitmap(dest.data, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	//dest.release();
//	image.release();
}

void COpenCVEffect::EqualizeHistogram(CRegardsBitmap* pBitmap)
{
	//Mat image(pBitmap->GetBitmapHeight(), pBitmap->GetBitmapWidth(), CV_8UC4, pBitmap->GetPtBitmap());
	COpenCVEffectPimpl::EqualizeHistogram(pBitmap->GetMatrix());
	//pBitmap->SetBitmap(image.data, pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	//image.release();
}


void COpenCVEffect::LoadAndRotate(const wxString& filePath, const int& rotate)
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


void COpenCVEffect::CalculateHistogram(CRegardsBitmap* pBitmap, CRegardsBitmap* histogram, const int& colorChoice,
                                       const wxColour& colorBgnd, const wxColour& colorFont)
{
	Mat hist;
	Mat src;
	int hist_w = histogram->GetBitmapWidth(), hist_h = histogram->GetBitmapHeight();
	Mat image = pBitmap->GetMatrix();
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	auto color = Scalar(255, 255, 255);

	if (colorChoice == 0)
	{
		cvtColor(image, src, COLOR_BGRA2GRAY);
		int histSize = 256;
		float range[] = {0, 256}; //the upper boundary is exclusive
		const float* histRange = {range};
		bool uniform = true, accumulate = false;

		calcHist(&src, 1, nullptr, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		int bin_w = cvRound(static_cast<double>(hist_w) / histSize);
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
		cvtColor(image, src, COLOR_BGRA2BGR);
		vector<Mat> bgr_planes;
		split(src, bgr_planes);
		int histSize = 256;
		float range[] = {0, 256}; //the upper boundary is exclusive
		const float* histRange = {range};
		bool uniform = true, accumulate = false;

		if (colorChoice == 1)
			calcHist(&bgr_planes[0], 1, nullptr, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		if (colorChoice == 2)
			calcHist(&bgr_planes[1], 1, nullptr, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		if (colorChoice == 3)
			calcHist(&bgr_planes[2], 1, nullptr, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		int bin_w = cvRound(static_cast<double>(hist_w) / histSize);

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
	axes.SetBackgroundColor(Scalar(colorBgnd.Blue(), colorBgnd.Green(), colorBgnd.Red()));
	axes.SetTextColor(Scalar(colorFont.Blue(), colorFont.Green(), colorFont.Red()));
	axes.create<CvPlot::Series>(hist)
	    .setColor(color);
	//axes.Set


	Mat mat = axes.render(hist_h, hist_w);


	cvtColor(mat, histImage, COLOR_BGR2BGRA);
	histogram->SetMatrix(histImage);
	histogram->VertFlipBuf();

	hist.release();
	image.release();
	src.release();
	mat.release();
	histImage.release();
}

void COpenCVEffect::NormalizeHistogram(CRegardsBitmap* pictureData, const int& colorChoice, const int& minValue,
                                       const int& maxValue)
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
				color.SetBlue((color.GetBlue() - min) * (maxValue - minValue) / (max - min) + minValue);
			}
			pictureData->SetColorValue(x, y, color);
		}
	}
}
