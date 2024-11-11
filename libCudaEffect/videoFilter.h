#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/mat.hpp>

void convertNV12toRGB32(cv::cuda::GpuMat& output, const cv::Mat& inputY,const cv::Mat& inputUV, int pitch, int colorRange, int colorSpace);
void convertYUVtoRGB32(cv::cuda::GpuMat& output, const cv::Mat& inputY,const cv::Mat& inputUV, int pitch, int colorRange, int colorSpace);
