#include "libraw_opencl.h"
#include <OpenCLRawDev.h>
using namespace Regards::OpenCL;

CLibRawOpenCL::CLibRawOpenCL(void * openclContext)
{
	 openclRawDev = new COpenCLRawDev((COpenCLContext *)openclContext);
}

CLibRawOpenCL::~CLibRawOpenCL()
{
	delete openclRawDev;
}

void CLibRawOpenCL::InitData(void * dataInput, int sizeInput, int sizeOutput, int _width, int _height, int scale)
{
	openclRawDev->InitData(dataInput, sizeInput, sizeOutput, _width, _height, scale);
}

void * CLibRawOpenCL::GetOutputData(void * dataOutput, int sizeOutput, int lpass)
{
	return openclRawDev->GetOutputData(dataOutput, sizeOutput, lpass);
}

void * CLibRawOpenCL::GetAlphaChannel(void * dataOutput, int sizeOutput)
{
	return openclRawDev->GetAlphaChannel(dataOutput, sizeOutput);
}

void CLibRawOpenCL::WaveletDenoiseChannel(int size, float _threshold, float * noise)
{
	 openclRawDev->WaveletDenoiseChannel(size, _threshold, noise);
}

void * CLibRawOpenCL::GetTempData()
{
	return openclRawDev->GetTempData();
}

void CLibRawOpenCL::WaveletDenoiseCol(int hpass, int lpass, int sc)
{
	openclRawDev->WaveletDenoiseCol(hpass, lpass, sc);
}

void CLibRawOpenCL::WaveletDenoiseRow(int lpass, int sc)
{
	openclRawDev->WaveletDenoiseRow(lpass, sc);
}

void CLibRawOpenCL::WaveletDenoiseNormalize(int size, int hpass, int lpass, float thold)
{
	openclRawDev->WaveletDenoiseNormalize(size, hpass, lpass, thold);
}