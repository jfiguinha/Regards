#include "header.h"
#include "VideoStabilization.h"
#include "VideoStabilizationCpu.h"
#include "VideoStabilizationOpenCL.h"

using namespace Regards::OpenCV;
using namespace cv;





COpenCVStabilization::COpenCVStabilization(const int& nbFrame, const int& type)
{
	this->type = type;
	switch (type)
	{
	case TYPE_CPU:
		opencvStabilization = std::make_unique<COpenCVStabilizationCpu>(nbFrame);
		break;
	case TYPE_OPENCL:
		opencvStabilization = std::make_unique<COpenCVStabilizationOpenCL>(nbFrame);
		break;

	}
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
