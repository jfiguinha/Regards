#include <header.h>
#include "pfm.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ConvertUtility.h>
#define uchar unsigned char

void skip_space(std::fstream& fileStream)
{
    char c;
    do
    {
        c = fileStream.get();
    } while (c == '\n' || c == ' ' || c == '\t' || c == '\r');
    fileStream.unget();
}

int littleendian()
{
    int intval = 1;
    uchar* uval = (uchar*)&intval;
    return uval[0] == 1;
}

void swapBytes(float* fptr)
{
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
    std::fstream file(CConvertUtility::ConvertToStdString(path), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << path << std::endl;
        return;
    }

    std::string bands;
    file >> bands;
    file >> width;
    file >> height;
    float scalef;
    file >> scalef;
}

cv::Mat CPfm::ReadFilePFM(const wxString& path, const bool& thumbnail)
{
    cv::Mat image;
    
    std::fstream file(CConvertUtility::ConvertToStdString(path), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << path << std::endl;
        return image;
    }

    std::string bands;
    int width, height;
    float scalef, fvalue;
    float vfvalue[3];

    file >> bands;
    file >> width;
    file >> height;
    file >> scalef;

    int littleEndianFile = (scalef < 0);
    int littleEndianMachine = littleendian();
    int needSwap = (littleEndianFile != littleEndianMachine);

    char c = file.get();
    if (c == '\r')
        c = file.get();
    if (c != '\n') {
        if (c == ' ' || c == '\t' || c == '\r') {
            std::cerr << "newline expected";
            return image;
        }
        std::cerr << "whitespace expected";
        return image;
    }

    if (bands == "Pf") {
        image.create(height, width, CV_32FC1);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; ++j) {
                file.read((char*)&fvalue, sizeof(fvalue));
                if (needSwap) {
                    swapBytes(&fvalue);
                }
                image.at<float>(i, j) = fvalue;
            }
        }
    }
    else if (bands == "PF") {
        image.create(height, width, CV_32FC3);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; ++j) {
                file.read((char*)&vfvalue, sizeof(vfvalue));
                if (needSwap) {
                    swapBytes(&vfvalue[0]);
                    swapBytes(&vfvalue[1]);
                    swapBytes(&vfvalue[2]);
                }

                if (thumbnail) {
                    image.at<cv::Vec3f>(i, j) = cv::Vec3f(vfvalue[0], vfvalue[1], vfvalue[2]);
                }
                else {
                    image.at<cv::Vec3f>(i, j) = cv::Vec3f(vfvalue[2], vfvalue[1], vfvalue[0]);
                }
            }
        }
    }
    else {
        std::cerr << "unknown bands description";
        return image;
    }
    return image;
}

int CPfm::WriteFilePFM(const cv::Mat& image, const wxString& path, float scalef)
{
   

    if (image.empty()) {
        std::cerr << "Empty image" << std::endl;
        return -1;
    }

    std::fstream file(CConvertUtility::ConvertToStdString(path), std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing: " << path << std::endl;
        return -1;
    }

    std::string bands = "PF";
    int width = image.size().width, height = image.size().height;

    if (littleendian()) {
        scalef = -scalef;
    }

    file << bands << "\n";
    file << width << "\n";
    file << height << "\n";
    file << scalef << "\n";

    float vfvalue[3];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; ++j) {
            vfvalue[0] = image.at<cv::Vec3f>(i, j)[2];
            vfvalue[1] = image.at<cv::Vec3f>(i, j)[1];
            vfvalue[2] = image.at<cv::Vec3f>(i, j)[0];
            file.write((char*)&vfvalue, sizeof(vfvalue));
        }
    }
    return 0;
}