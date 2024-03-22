#ifndef COLORNET_H_INCLUDED
#define COLORNET_H_INCLUDED
#include <ncnn/net.h>
#include <ncnn/cpu.h>
#include <ncnn/layer.h>

class CColorization
{
public:
    CColorization();
    ~CColorization();
    int load(const std::string& param_path, const std::string& model_path);
    cv::Mat colorization(const cv::Mat& bgr);
    bool IsLoaded();
private:
    ncnn::Net net;
    bool isLoad = false;
};




#endif // COLORNET_H_INCLUDED
