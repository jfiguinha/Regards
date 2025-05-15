#pragma once
#include <LibResource.h>
#include "OpenCLParameter.h"
#include "OpenCLContext.h"

extern cv::ocl::OpenCLExecutionContext clExecCtx;
using namespace Regards::OpenCL;
using namespace cv;

class CAvirFilterOpenCL
{
public:
	static cv::UMat GetDataOpenCLHtoV2D(cv::UMat& src);
	static UMat ConvertToFloat(cv::UMat& src, const int& width, const int& height);
	static UMat UpSample2D(cv::UMat& src, const int& width, const int& height, int widthSrc, int start, int outLen, int ResampleFactor);
	static UMat doFilterOpenCL2D(cv::UMat& src, const int& width, const int& height, const float* f, int flen, int step);
	static UMat GetDataOpenCLHtoVDither2D(cv::UMat& src, float gm, float PkOut, float TrMul0);
	static UMat doResize2OpenCL2D(cv::UMat& src, const int& width, const int& height,
		int* PositionTab, int posTabSize, float* ftp, int ftpTabSize, int IntFltLen);
	static UMat doResizeOpenCL2D(cv::UMat& src, const int& width, const int& height, int* PositionTab, int posTabSize, float* ftp, int ftpTabSize, int IntFltLen);

	static UMat doFilterOpenCL2DLastStep(cv::UMat& src, const float* f, int flen, int step, float gm, float PkOut, float TrMul0);
	static UMat doFilterOpenCL2DV(cv::UMat& src, const float* f, int flen, int step);

};