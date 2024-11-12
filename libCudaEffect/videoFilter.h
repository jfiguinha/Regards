#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core/mat.hpp>

void convertNV12toRGB32(cv::cuda::GpuMat& output, uint8_t* bufferY, int sizeY, uint8_t* bufferUV, int sizeUV, const int& width,
	const int& height, const int& lineSize, const int& widthOut, const int& heightOut,
	const int& colorRange, const int& colorSpace);
void convertYUVtoRGB32(cv::cuda::GpuMat& output, uint8_t* bufferY, int sizeY, uint8_t* bufferU, int sizeU, uint8_t* bufferV, int sizeV,
	const int& width, const int& height, const int& lineSize, const int& widthOut,
	const int& heightOut, const int& colorRange, const int& colorSpace);
