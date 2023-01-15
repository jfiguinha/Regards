#include <header.h>
#include "ascii.h"
#include <algorithm>    // std::max
#include <fstream>
#include "ImageLoadingFormat.h"
using namespace std;

const string DENSITY = "@QB#NgWM8RDHdOKq9$6khEPXwmeZaoS2yjufF]}{tx1zv7lciL/\\|?*>r^;:_\"~,'.-`";

char CBitmapToAscii::getDensity(int value)
{
	// Since we don't have 255 characters, we have to use percentages
	int charValue = static_cast<int>(round(DENSITY.size() / 255.0 * value));
	charValue = max(charValue, 0);
	charValue = min(charValue, static_cast<int>(DENSITY.size()) - 1);
	return DENSITY[charValue];
}

void CBitmapToAscii::SaveToAscii(CImageLoadingFormat* source, const string& filenameOut)
{
	// source->Flip();

	float new_ratio = 1;
	int pictureWidth = source->GetWidth();
	int pictureHeight = source->GetHeight();
	int width = 256;
	int height = 256;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		new_ratio = static_cast<float>(width) / static_cast<float>(pictureWidth);
	else
		new_ratio = static_cast<float>(height) / static_cast<float>(pictureHeight);

	if ((pictureHeight * new_ratio) > height)
	{
		new_ratio = static_cast<float>(height) / static_cast<float>(pictureHeight);
	}
	if ((pictureWidth * new_ratio) > width)
	{
		new_ratio = static_cast<float>(width) / static_cast<float>(pictureWidth);
	}

	cv::Mat out;
	resize(source->GetOpenCVPicture(), out, cv::Size(source->GetWidth() * new_ratio, source->GetHeight() * new_ratio),
	       cv::INTER_CUBIC);
	std::ofstream outfile(filenameOut);
	if (outfile.fail())
	{
		cerr << "!Error opening " << filenameOut << endl;
		return;
	}
	cvtColor(out, out, cv::COLOR_BGRA2GRAY);
	for (int y = 0; y < out.rows; y++)
	{
		for (int x = 0; x < out.cols; x++)
		{
			int data = out.at<uchar>(cv::Point(x, y));
			outfile << getDensity(data);
		}
		outfile << endl;
	}
	outfile.close();
}
