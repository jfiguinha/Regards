#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/mat.hpp>
#include <RGBAQuad.h>
#include <vector>
using namespace std;


void motionBlur(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const vector<double>& kernelMotion, const vector<wxPoint>& offsets, int kernelSize);
void softenFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output);
void cuda_filter2d(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, const vector<float>& kernelMotion, int kernelSize);
void noiseFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output);
void swirlFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, float radius, float angleDegree);


void convert_to_gray(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output);