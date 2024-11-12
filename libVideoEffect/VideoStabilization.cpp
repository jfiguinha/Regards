#include "header.h"
#include "VideoStabilization.h"
#include "VideoStabilizationCpu.h"
#include "VideoStabilizationOpenCL.h"
#include "VideoStabilizationCuda.h"
using namespace Regards::OpenCV;
using namespace cv;

const int SMOOTHING_RADIUS = 50; // In frames. The larger the more stable the video, but less reactive to sudden panning

COpenCVStabilization::COpenCVStabilization(const int& nbFrame, const int& type)
{
	this->type = type;
	switch (type)
	{
	case TYPE_CPU:
		openCVCpu = new COpenCVStabilizationCpu(nbFrame);
		break;
	case TYPE_OPENCL:
		openCVopenCL = new COpenCVStabilizationOpenCL(nbFrame);
		break;
	case TYPE_CUDA:
		openCVCuda = new COpenCVStabilizationCuda(nbFrame);
		break;

	}
}

COpenCVStabilization::~COpenCVStabilization()
{
	switch (type)
	{
	case TYPE_CPU:
		delete openCVCpu;
		break;
	case TYPE_OPENCL:
		delete openCVopenCL;
		break;
	case TYPE_CUDA:
		delete openCVCuda;
		break;
	}
}

void COpenCVStabilization::SetNbFrameBuffer(const int& nbFrame)
{
	switch (type)
	{
	case TYPE_CPU:
		openCVCpu->SetNbFrameBuffer(nbFrame);
		break;
	case TYPE_OPENCL:
		openCVopenCL->SetNbFrameBuffer(nbFrame);
		break;
	case TYPE_CUDA:
		openCVCuda->SetNbFrameBuffer(nbFrame);
		break;

	}
}

int COpenCVStabilization::GetNbFrame()
{
	switch (type)
	{
	case TYPE_CPU:
		return openCVCpu->GetNbFrame();
		break;
	case TYPE_OPENCL:
		return openCVopenCL->GetNbFrame();
		break;
	case TYPE_CUDA:
		return openCVCuda->GetNbFrame();
		break;

	}
}

void COpenCVStabilization::Init()
{
	switch (type)
	{
	case TYPE_CPU:
		openCVCpu->Init();
		break;
	case TYPE_OPENCL:
		openCVopenCL->Init();
		break;
	case TYPE_CUDA:
		openCVCuda->Init();
		break;

	}
}

int COpenCVStabilization::GetNbFrameBuffer()
{
	switch (type)
	{

	case TYPE_OPENCL:
		return openCVopenCL->GetNbFrameBuffer();
		break;
	case TYPE_CUDA:
		return openCVCuda->GetNbFrameBuffer();
		break;

	default:
		return openCVCpu->GetNbFrameBuffer();
		break;

	}
}

void COpenCVStabilization::AddFrame(const Mat& image)
{
	openCVCpu->AddFrame(image);
}

void COpenCVStabilization::BufferFrame(const Mat& image)
{
	openCVCpu->BufferFrame(image);
}

void COpenCVStabilization::CorrectFrame(Mat& image)
{
	openCVCpu->CorrectFrame(image);
}

void COpenCVStabilization::AddFrame(const UMat& image)
{
	openCVopenCL->AddFrame(image);
}

void COpenCVStabilization::BufferFrame(const UMat& image)
{
	openCVopenCL->BufferFrame(image);
}

cv::UMat COpenCVStabilization::CorrectFrame(UMat& image)
{
	return openCVopenCL->CorrectFrame(image);
}

void COpenCVStabilization::AddFrame(const cv::cuda::GpuMat& image)
{
	openCVCuda->AddFrame(image);
}

void COpenCVStabilization::BufferFrame(const cv::cuda::GpuMat& image)
{
	openCVCuda->BufferFrame(image);
}

cv::cuda::GpuMat COpenCVStabilization::CorrectFrame(cv::cuda::GpuMat& image)
{
	return openCVCuda->CorrectFrame(image);
}
