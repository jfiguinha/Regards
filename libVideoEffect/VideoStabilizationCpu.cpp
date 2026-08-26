#include "header.h"
#include "VideoStabilizationCpu.h"

using namespace Regards::OpenCV;
using namespace cv;



COpenCVStabilizationCpu::COpenCVStabilizationCpu(const int& nbFrame)
{
	pimpl = std::make_unique<CVideoStabilizationPimpl_<Mat>>();
	this->nbFrame = nbFrame;
	pimpl->nbFrameMax = nbFrame;
}


void COpenCVStabilizationCpu::SetNbFrameBuffer(const int& nbFrame)
{
	pimpl->nbFrameMax = nbFrame;
}

int COpenCVStabilizationCpu::GetNbFrame()
{
	return nbFrame;
}

void COpenCVStabilizationCpu::Init()
{
	nbFrameBuffer = 0;
	pimpl->Init();
}

int COpenCVStabilizationCpu::GetNbFrameBuffer()
{
	return nbFrameBuffer;
}

void COpenCVStabilizationCpu::AddFrame(Regards::Picture::CPictureArray& image)
{
	pimpl->AnalyseFrame(image.getMat());
}

void COpenCVStabilizationCpu::BufferFrame(Regards::Picture::CPictureArray& image)
{
	pimpl->AnalyseFrame(image.getMat());
	nbFrameBuffer++;
}

Regards::Picture::CPictureArray COpenCVStabilizationCpu::CorrectFrame(Regards::Picture::CPictureArray& image)
{
	pimpl->CalculTransformation();
	pimpl->CorrectedFrame(image.getMat());
	return image;
}

