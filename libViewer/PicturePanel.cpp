#include <header.h>
#include "PicturePanel.h"
#include <RGBAQuad.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <opencv2/core.hpp>
#include <wx/spinctrl.h>
#include <CvPlot/cvplot.h>
using namespace Regards::Window;
using namespace Regards::Picture;

CPicturePanel::CPicturePanel(wxWindow* parent, wxWindowID id, const CThemeThumbnail& theme)
	: CWindowMain("CPicturePanel", parent, id)
{
	w = 0;
	h = 0;
	colorBack = theme.colorBack;
	CThemeFont themeFont = theme.themeIcone.font;
	colorFont = themeFont.GetColorFont();
	Connect(wxEVT_PAINT, wxPaintEventHandler(CPicturePanel::on_paint));
	//image = nullptr;
	choice_control = new wxComboBox(this, -1, "", wxDefaultPosition, wxSize(100, -1));
	choice_control->AppendString("All channel");
	choice_control->AppendString("Blue");
	choice_control->AppendString("Green");
	choice_control->AppendString("Red");
	choice_control->Select(0);
	Connect(wxEVT_COMBOBOX, reinterpret_cast<wxObjectEventFunction>(&CPicturePanel::OnChannelSelect));
	processEnd = true;
}


void CPicturePanel::CalculateHistogram(cv::Mat & pBitmap, cv::Mat& histogram, const int& colorChoice, const wxColour& colorBgnd, const wxColour& colorFont)
{
	cv::Mat hist;
	cv::Mat src;
	int hist_w = histogram.size().width, hist_h = histogram.size().height;
	cv::Mat image = pBitmap;
	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	auto color = cv::Scalar(255, 255, 255);

	if (colorChoice == 0)
	{
		cv::cvtColor(image, src, cv::COLOR_BGRA2GRAY);
		int histSize = 256;
		float range[] = { 0, 256 }; //the upper boundary is exclusive
		const float* histRange = { range };
		bool uniform = true, accumulate = false;

		calcHist(&src, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		int bin_w = cvRound(static_cast<double>(hist_w) / histSize);
		normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		for (int i = 1; i < histSize; i++)
		{
			line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
				cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
				color, 2, 8, 0);
		}
	}
	else
	{
		cvtColor(image, src, cv::COLOR_BGRA2BGR);
		vector<cv::Mat> bgr_planes;
		split(src, bgr_planes);
		int histSize = 256;
		float range[] = { 0, 256 }; //the upper boundary is exclusive
		const float* histRange = { range };
		bool uniform = true, accumulate = false;

		if (colorChoice == 1)
			calcHist(&bgr_planes[0], 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		if (colorChoice == 2)
			calcHist(&bgr_planes[1], 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
		if (colorChoice == 3)
			calcHist(&bgr_planes[2], 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		int bin_w = cvRound(static_cast<double>(hist_w) / histSize);

		if (colorChoice == 1)
			color = cv::Scalar(255, 0, 0);
		if (colorChoice == 2)
			color = cv::Scalar(0, 255, 0);
		if (colorChoice == 3)
			color = cv::Scalar(0, 0, 255);
		normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		for (int i = 1; i < histSize; i++)
		{
			line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
				cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
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
	cv::flip(histImage, histogram, 0);

	hist.release();
	image.release();
	src.release();
	mat.release();
	histImage.release();
}

void CPicturePanel::NormalizeHistogram(cv::Mat&  pictureData, const int& colorChoice, const int& minValue, const int& maxValue)
{
	int min = 0;
	int max = 255;
	//pixel = (pixel - min) * (nmax - nmin) / (max - min) + nmin
	cv::Mat mat = pictureData;
	//uint8_t * data = pictureData->GetPtBitmap();
	for (int y = 0; y < pictureData.size().height; y++)
	{
		for (int x = 0; x < pictureData.size().width; x++)
		{
			CRgbaquad color = CRgbaquad::GetColorValue(&mat,x,y); // pictureData->GetColorValue(x, y);
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
			//pictureData->SetColorValue(x, y, color);

			int i = (x << 2) + (y * (pictureData.cols << 2));
			memcpy(pictureData.data + i, &color, sizeof(CRgbaquad));
		}
	}
}


void CPicturePanel::CreateHistogram()
{
	wxSize size = this->GetSize();
	const int neww = size.GetWidth();
	const int newh = size.GetHeight();

	int ch_w = 0;
	int ch_h = 0;
	choice_control->GetSize(&ch_w, &ch_h);

	const int width = neww - (marged * 2);
	const int height = newh - ch_h - (marged * 4);

	if (histogram.empty())
		histogram = cv::Mat(height,width,CV_8UC4);

	if (width != histogram.size().width || height != histogram.size().height)
	{
		histogram = cv::Mat(height, width, CV_8UC4);
	}

	if (refreshPicture)
	{
		CImageLoadingFormat picture;
		CalculateHistogram(pictureOriginal, histogram, channelSelect, colorBack, colorFont);
		picture.SetPicture(histogram);
		this->image = picture.GetwxImage();
	}
}

void CPicturePanel::OnChannelSelect(wxCommandEvent& event)
{
	channelSelect = choice_control->GetSelection();
	refreshPicture = true;


	needToRefresh = true;
}

CPicturePanel::~CPicturePanel()
{
	delete choice_control;
}

void CPicturePanel::SetPictureToDisplay(cv::Mat& picture)
{
	picture.copyTo(pictureOriginal);

	channelSelect = 0;
	choice_control->Select(0);

	refreshPicture = true;
	needToRefresh = true;
}

void CPicturePanel::UpdateScreenRatio()
{
	needToRefresh = true;
}

void CPicturePanel::on_paint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	const wxRect rc = GetWindowRect();
	FillRect(&dc, rc, colorBack);

	int neww, newh;
	dc.GetSize(&neww, &newh);

	int ch_w = 0;
	int ch_h = 0;
	choice_control->GetSize(&ch_w, &ch_h);

	int xPos = (neww - ch_w) / 2;
	choice_control->SetPosition(wxPoint(xPos, marged));

	CreateHistogram();

	dc.DrawBitmap(image, marged, marged * 2 + ch_h, false);

}
