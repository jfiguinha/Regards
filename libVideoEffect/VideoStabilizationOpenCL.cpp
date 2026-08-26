#include "header.h"
#include "VideoStabilizationOpenCL.h"

using namespace Regards::OpenCV;
using namespace cv;


COpenCVStabilizationOpenCL::COpenCVStabilizationOpenCL(const int& nbFrame)
{
	pimpl = std::make_unique<CVideoStabilizationPimpl_<UMat>>();
	this->nbFrame = nbFrame;
	pimpl->nbFrameMax = nbFrame;
}

void COpenCVStabilizationOpenCL::SetNbFrameBuffer(const int& nbFrame)
{
	pimpl->nbFrameMax = nbFrame;
}

int COpenCVStabilizationOpenCL::GetNbFrame()
{
	return nbFrame;
}

void COpenCVStabilizationOpenCL::Init()
{
	nbFrameBuffer = 0;
	pimpl->Init();
}

int COpenCVStabilizationOpenCL::GetNbFrameBuffer()
{
	return nbFrameBuffer;
}


void COpenCVStabilizationOpenCL::AddFrame(Regards::Picture::CPictureArray& image)
{
	pimpl->AnalyseFrame(image.getUMat());
}

void COpenCVStabilizationOpenCL::BufferFrame(Regards::Picture::CPictureArray& image)
{
	pimpl->AnalyseFrame(image.getUMat());
	nbFrameBuffer++;
}


Regards::Picture::CPictureArray COpenCVStabilizationOpenCL::CorrectFrame(Regards::Picture::CPictureArray& image)
{
	pimpl->CalculTransformation();
	cv::UMat mat = pimpl->CorrectedFrame(image.getUMat());
	return Regards::Picture::CPictureArray(mat);
	//frame.copyTo(frame);
}
