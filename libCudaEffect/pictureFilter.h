#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/mat.hpp>

void mosaicFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int fTileSize);
void solarizationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int threshold);
void posterisationFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, int level);
void distorsionFilter(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output, float correctionRadius);