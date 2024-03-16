#pragma once
//ncnn
#include <ncnn/net.h>
#include <ncnn/cpu.h>
#include <ncnn/layer.h>

class RealESRGAN
{
public:
    RealESRGAN();
    ~RealESRGAN();

    int load(const std::string& parampath, const std::string& modelpath);
    int tile_process(const cv::Mat& inimage, cv::Mat& outimage);
    bool IsLoaded();

public:
    int inference(const cv::Mat& in, ncnn::Mat& out, int w, int h);
    int preprocess(const cv::Mat& img, cv::Mat& pad_img, int& img_pad_h, int& img_pad_w);
   
    static cv::Mat to_ocv(const cv::Mat& source, const ncnn::Mat& result);
    // realesrgan parameters
    int scale;
    int tile_size;
    int tile_pad;
    const float norm_vals[3] = { 1 / 255.0f, 1 / 255.0f, 1 / 255.0f };
private:
    ncnn::Net net;
    bool isLoad = false;
};


class CColorisationNCNN
{
public:
    CColorisationNCNN();
    ~CColorisationNCNN();

    int load(const std::string& parampath, const std::string& modelpath);
    bool IsLoaded();
    cv::Mat Execute(const cv::Mat& source);

private:
    ncnn::Net net;
    bool isLoad = false;
};

