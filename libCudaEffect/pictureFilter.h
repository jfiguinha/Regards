#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/mat.hpp>

#include <vector>
using namespace std;

void mosaicFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int fTileSize);
void solarizationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int threshold);
void posterisationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int level);
void distorsionFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, float correctionRadius);
void sharpenMasking(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const cv::cuda::GpuMat& gaussian, float sharpness);
void motionBlur(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const vector<double>& kernelMotion, const vector<wxPoint>& offsets, int kernelSize);
void softenFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output);
void sepiaFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output);
void cuda_filter2d(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const vector<float>& kernelMotion, int kernelSize);
void noiseFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output);
void swirlFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, float radius, float angleDegree);