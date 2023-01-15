#include <header.h>
#include "pfm.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <ConvertUtility.h>
#define uchar unsigned char

void skip_space(fstream& fileStream)
{
	// skip white space in the headers or pnm files

	char c;
	do
	{
		c = fileStream.get();
	}
	while (c == '\n' || c == ' ' || c == '\t' || c == '\r');
	fileStream.unget();
}

// check whether machine is little endian
int littleendian()
{
	int intval = 1;
	uchar* uval = (uchar*)&intval;
	return uval[0] == 1;
}

void swapBytes(float* fptr)
{
	// if endianness doesn't agree, swap bytes
	uchar* ptr = (uchar*)fptr;
	uchar tmp = ptr[0];
	ptr[0] = ptr[3];
	ptr[3] = tmp;
	tmp = ptr[1];
	ptr[1] = ptr[2];
	ptr[2] = tmp;
}

void CPfm::GetDimensions(const wxString& path, int& width, int& height)
{
	// create fstream object to read in pfm file
	// open the file in binary
	fstream file(CConvertUtility::ConvertToUTF8(path), ios::in | ios::binary);

	// init variables
	string bands; // what type is the image   "Pf" = grayscale    (1-band)
	//                          "PF" = color        (3-band)
	float scalef; // , fvalue;   // scale factor and temp value to hold pixel value
	//float vfvalue[3];
	// extract header information, skips whitespace
	file >> bands;
	file >> width;
	file >> height;
	file >> scalef;
}

cv::Mat CPfm::ReadFilePFM(const wxString& path, const bool& thumbnail)
{
	cv::Mat image;

	// create fstream object to read in pfm file
	// open the file in binary
	fstream file(CConvertUtility::ConvertToStdString(path).c_str(), ios::in | ios::binary);

	// init variables
	string bands; // what type is the image   "Pf" = grayscale    (1-band)
	//                          "PF" = color        (3-band)
	int width, height; // width and height of the image
	float scalef, fvalue; // scale factor and temp value to hold pixel value
	float vfvalue[3];
	// extract header information, skips whitespace
	file >> bands;
	file >> width;
	file >> height;
	file >> scalef;

	// determine endianness
	int littleEndianFile = (scalef < 0);
	int littleEndianMachine = littleendian();
	int needSwap = (littleEndianFile != littleEndianMachine);

	//cout << setfill('=') << setw(19) << "=" << endl;
	//cout << "Reading image to pfm file: " << path << endl;
	//cout << "Little Endian?: " << ((needSwap) ? "false" : "true") << endl;
	//cout << "width: " << width << endl;
	//cout << "height: " << height << endl;
	//cout << "scale: " << scalef << endl;

	// skip SINGLE newline character after reading third arg
	char c = file.get();
	if (c == '\r') // <cr> in some files before newline
		c = file.get();
	if (c != '\n')
	{
		if (c == ' ' || c == '\t' || c == '\r')
		{
			cout << "newline expected";
			return image;
		}
		cout << "whitespace expected";
		return image;
	}

	if (bands == "Pf")
	{
		// handle 1-band image
		//cout << "Reading grayscale image (1-band)" << endl;
		//cout << "Reading into CV_32FC1 image" << endl;
		image.create(height, width, CV_32FC4);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; ++j)
			{
				int pos = j * 4 + i * width * 4;
				file.read((char*)&fvalue, sizeof(fvalue));
				if (needSwap)
				{
					swapBytes(&fvalue);
				}
				image.at<float>(pos + 0) = fvalue;
				image.at<float>(pos + 1) = fvalue;
				image.at<float>(pos + 2) = fvalue;
			}
		}
	}
	else if (bands == "PF")
	{
		// handle 3-band image
		//cout << "Reading color image (3-band)" << endl;
		//cout << "Reading into CV_32FC3 image" << endl;
		image.create(height, width, CV_32FC4);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; ++j)
			{
				file.read((char*)&vfvalue, sizeof(vfvalue));
				if (needSwap)
				{
					swapBytes(&vfvalue[0]);
					swapBytes(&vfvalue[1]);
					swapBytes(&vfvalue[2]);
				}

				int pos = j * 4 + i * width * 4;

				if (thumbnail)
				{
					image.at<float>(pos + 0) = vfvalue[0];
					image.at<float>(pos + 1) = vfvalue[1];
					image.at<float>(pos + 2) = vfvalue[2];
				}
				else
				{
					image.at<float>(pos + 2) = vfvalue[0];
					image.at<float>(pos + 1) = vfvalue[1];
					image.at<float>(pos + 0) = vfvalue[2];
				}
			}
		}
	}
	else
	{
		cout << "unknown bands description";
		return image;
	}
	cout << setfill('=') << setw(19) << "=" << endl << endl;
	return image;
}

int CPfm::WriteFilePFM(const cv::Mat& image, const wxString& path, float scalef)
{
	if (!image.empty())
	{
		// create fstream object to write out pfm file
		// open the file in binary
		fstream file(CConvertUtility::ConvertToUTF8(path), ios::out | ios::binary);


		string bands = "PF";
		int width = image.size().width, height = image.size().height; // width and height of the image
		//float fvalue;       // scale factor and temp value to hold pixel value
		float vfvalue[3]; // temp value to hold 3-band pixel value

		// sign of scalefact indicates endianness, see pfms specs
		if (littleendian())
			scalef = -scalef;

		// insert header information
		file << bands << "\n";
		file << width << "\n";
		file << height << "\n";
		file << scalef << "\n";

		//cout << setfill('=') << setw(19) << "=" << endl;
		//cout << "Writing image to pfm file: " << path << endl;
		//cout << "Little Endian?: " << ((littleendian()) ? "true" : "false") << endl;
		//cout << "width: " << width << endl;
		//cout << "height: " << height << endl;
		//cout << "scale: " << scalef << endl;

		//cout << "writing color image (3-band)" << endl;
		//cout << "writing into CV_32FC3 image" << endl;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; ++j)
			{
				int pos = j * 4 + i * width * 4;
				vfvalue[0] = image.at<float>(pos + 2);
				vfvalue[1] = image.at<float>(pos + 1);
				vfvalue[2] = image.at<float>(pos + 0);
				file.write((char*)&vfvalue, sizeof(vfvalue));
			}
		}

		//cout << setfill('=') << setw(19) << "=" << endl << endl;
	}
	return 0;
}
