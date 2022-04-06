#pragma once
#include "header.h"
class CFace
{
public:
    float confidence;
    cv::Mat croppedImage;
    cv::Rect myROI;
    int angle = 0;
};

struct FaceBox
{
    int x, y, w, h;
    float angle, scale, score;

    cv::Rect getBox() { return cv::Rect(x, y, w, h); }

    FaceBox(int x_, int y_, int w_, int h_, float a_, float s_, float c_)
        : x(x_), y(y_), w(w_), h(h_), angle(a_), scale(s_), score(c_)
    {}
};

inline void VerifRectSize(cv::Rect & rect, cv::Mat & matSrc)
{
    rect.x = max(rect.x, 0);
    rect.y = max(rect.y, 0);

    bool is_inside = (rect & cv::Rect(0, 0, matSrc.cols, matSrc.rows)) == rect;
    if (!is_inside)
    {
        if ((rect.width + rect.x) > matSrc.cols)
        {
            rect.width = matSrc.cols - rect.x - 1;
        }

        if ((rect.height + rect.y) > matSrc.rows)
        {
            rect.height = matSrc.rows - rect.y - 1;
        }
    }
}