#include "header.h"
#include "VideoStabilization.h"
#include "VideoStabilizationCpu.h"
#include "VideoStabilizationOpenCL.h"

using namespace Regards::OpenCV;
using namespace cv;

const int SMOOTHING_RADIUS = 50; // In frames. The larger the more stable the video, but less reactive to sudden panning




COpenCVStabilization::COpenCVStabilization(const int& nbFrame, const int& type)
{
	this->type = type;
	switch (type)
	{
	case TYPE_CPU:
		opencvStabilization = new COpenCVStabilizationCpu(nbFrame);
		break;
	case TYPE_OPENCL:
		opencvStabilization = new COpenCVStabilizationOpenCL(nbFrame);
		break;
	}
}

COpenCVStabilization::~COpenCVStabilization()
{
    delete opencvStabilization;
}

void COpenCVStabilization::SetNbFrameBuffer(const int& nbFrame)
{
    opencvStabilization->SetNbFrameBuffer(nbFrame);
}

int COpenCVStabilization::GetNbFrame()
{
    return opencvStabilization->GetNbFrame();
}

void COpenCVStabilization::Init()
{
    opencvStabilization->Init();
}

int COpenCVStabilization::GetNbFrameBuffer()
{
    return opencvStabilization->GetNbFrameBuffer();
}

void COpenCVStabilization::AddFrame(Regards::Picture::CPictureArray& image)
{
    return opencvStabilization->AddFrame(image);
}

void COpenCVStabilization::BufferFrame(Regards::Picture::CPictureArray& image)
{
    return opencvStabilization->BufferFrame(image);
}

Regards::Picture::CPictureArray COpenCVStabilization::CorrectFrame(Regards::Picture::CPictureArray& image)
{
    return opencvStabilization->CorrectFrame(image);
}
