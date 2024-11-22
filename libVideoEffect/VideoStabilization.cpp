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
#ifdef USE_CUDA
	case TYPE_CUDA:
		openCVCuda = new COpenCVStabilizationCuda(nbFrame);
		break;
#endif

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
#ifdef USE_CUDA
	case TYPE_CUDA:
		delete openCVCuda;
		break;
#endif
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
#ifdef USE_CUDA
	case TYPE_CUDA:
		openCVCuda->SetNbFrameBuffer(nbFrame);
		break;
#endif
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
#ifdef USE_CUDA
	case TYPE_CUDA:
		return openCVCuda->GetNbFrame();
		break;
#endif
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
#ifdef USE_CUDA
	case TYPE_CUDA:
		openCVCuda->Init();
		break;
#endif
	}
}

int COpenCVStabilization::GetNbFrameBuffer()
{
	switch (type)
	{

	case TYPE_OPENCL:
		return openCVopenCL->GetNbFrameBuffer();
		break;
#ifdef USE_CUDA
	case TYPE_CUDA:
		return openCVCuda->GetNbFrameBuffer();
		break;
#endif
	default:
		return openCVCpu->GetNbFrameBuffer();
		break;

	}
}

void COpenCVStabilization::AddFrame(Regards::Picture::CPictureArray& image)
{
	switch (type)
	{

	case TYPE_OPENCL:
		return openCVopenCL->AddFrame(image);
		break;
#ifdef USE_CUDA
	case TYPE_CUDA:
		return openCVCuda->AddFrame(image);
		break;
#endif
	default:
		return openCVCpu->AddFrame(image);
		break;

	}

}

void COpenCVStabilization::BufferFrame(Regards::Picture::CPictureArray& image)
{
	switch (type)
	{

	case TYPE_OPENCL:
		return openCVopenCL->BufferFrame(image);
		break;
#ifdef USE_CUDA
	case TYPE_CUDA:
		return openCVCuda->BufferFrame(image);
		break;
#endif
	default:
		return openCVCpu->BufferFrame(image);
		break;

	}
}

Regards::Picture::CPictureArray COpenCVStabilization::CorrectFrame(Regards::Picture::CPictureArray& image)
{
	switch (type)
	{
	case TYPE_OPENCL:
		return openCVopenCL->CorrectFrame(image);
		break;
#ifdef USE_CUDA
	case TYPE_CUDA:
		return openCVCuda->CorrectFrame(image);
		break;
#endif
	}
    return openCVCpu->CorrectFrame(image);
}
