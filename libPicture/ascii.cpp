#include <header.h>
#include "ascii.h"
#include <algorithm>    // std::max, std::min
#include <fstream>
#include "ImageLoadingFormat.h"
using namespace std;

const string DENSITY = "@QB#NgWM8RDHdOKq9$6khEPXwmeZaoS2yjufF]}{tx1zv7lciL/\\|?*>r^;:_\"~,'.-`";

char CBitmapToAscii::getDensity(int value) {
    int charValue = static_cast<int>(round(DENSITY.size() / 255.0 * value));
    charValue = max(charValue, 0);
    charValue = min(charValue, static_cast<int>(DENSITY.size()) - 1);
    return DENSITY[charValue];
}

void CBitmapToAscii::SaveToAscii(CImageLoadingFormat* source, const string& filenameOut) {
    constexpr int MAX_WIDTH = 256;
    constexpr int MAX_HEIGHT = 256;

    int pictureWidth = source->GetWidth();
    int pictureHeight = source->GetHeight();
    float new_ratio = 1;

    if (pictureWidth > pictureHeight) {
        new_ratio = static_cast<float>(MAX_WIDTH) / static_cast<float>(pictureWidth);
    }
    else {
        new_ratio = static_cast<float>(MAX_HEIGHT) / static_cast<float>(pictureHeight);
    }

    cv::Mat resizedImg;
    auto srcMat = source->GetMatrix().getMat();
    resize(srcMat, resizedImg, cv::Size(), new_ratio, new_ratio, cv::INTER_CUBIC);

    cv::Mat grayImg;
    cvtColor(resizedImg, grayImg, cv::COLOR_BGRA2GRAY);

    // Ensure the new dimensions are within the MAX_WIDTH and MAX_HEIGHT
    if (grayImg.cols > MAX_WIDTH || grayImg.rows > MAX_HEIGHT) {
        resize(grayImg, grayImg, cv::Size(MAX_WIDTH, MAX_HEIGHT), 0, 0, cv::INTER_CUBIC);
    }

    ofstream outfile(filenameOut);
    if (outfile.fail()) {
        cerr << "!Error opening " << filenameOut << endl;
        return;
    }

    try {
        for (int y = 0; y < grayImg.rows; y++) {
            for (int x = 0; x < grayImg.cols; x++) {
                int data = grayImg.at<uchar>(y, x);  // Access the pixel value
                outfile << getDensity(data);
            }
            outfile << endl;
        }
    }
    catch (const std::exception& e) {
        cerr << "Exception occurred while writing to file: " << e.what() << endl;
        outfile.close();
        throw;
    }

    outfile.close();
}