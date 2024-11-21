#include "header.h"
#include "PictureArray.h"
#include <opencv2/core/opengl.hpp>
using namespace Regards::Picture;

CPictureArray::CPictureArray(const cv::_InputArray::KindFlag& type)
{
	kind = type;
}

CPictureArray::CPictureArray(cv::Mat& m)
{
	m = mat;
	kind = cv::_InputArray::KindFlag::MAT;
}

CPictureArray::CPictureArray(cv::cuda::GpuMat& d_mat)
{
	d_mat = gpuMat;
	kind = cv::_InputArray::KindFlag::CUDA_GPU_MAT;
}

CPictureArray::CPictureArray(cv::UMat& m)
{
	umat = m;
	kind = cv::_InputArray::KindFlag::UMAT;
}

int CPictureArray::getWidth()
{
	if (kind == cv::_InputArray::KindFlag::CUDA_GPU_MAT)
	{
		return gpuMat.size().width;
	}
	else if (kind == cv::_InputArray::KindFlag::MAT)
	{
		return mat.size().width;
	}
	return umat.size().width;
}

int CPictureArray::getHeight()
{
	if (kind == cv::_InputArray::KindFlag::CUDA_GPU_MAT)
	{
		return gpuMat.size().height;
	}
	else if (kind == cv::_InputArray::KindFlag::MAT)
	{
		return mat.size().height;
	}
	return umat.size().height;
}

void CPictureArray::CopyFrom(cv::ogl::Texture2D* tex)
{
	if (kind == cv::_InputArray::KindFlag::CUDA_GPU_MAT)
	{
		tex->copyFrom(gpuMat, true);
	}
	else if (kind == cv::_InputArray::KindFlag::MAT)
	{
		tex->copyFrom(mat, true);
	}
	else
	{
		tex->copyFrom(umat, true);
	}
}

cv::_InputArray::KindFlag CPictureArray::Kind()
{
	return kind;
}

cv::UMat& CPictureArray::getUMat()
{
	if(kind == cv::_InputArray::KindFlag::CUDA_GPU_MAT)
	{
		cv::Mat local;
		gpuMat.download(local);
		cv::UMat u;
		local.copyTo(u);
		return u;
	}
	else if(kind == cv::_InputArray::KindFlag::MAT)
	{
		cv::UMat u;
		mat.copyTo(u);
		return u;
	}
	return umat;
}

cv::Mat& CPictureArray::getMat()
{
	if (kind == cv::_InputArray::KindFlag::CUDA_GPU_MAT)
	{
		cv::Mat local;
		gpuMat.download(local);
		return local;
	}
	else if (kind == cv::_InputArray::KindFlag::UMAT)
	{
		cv::Mat local;
		umat.copyTo(local);
		return local;
	}
	return mat;
}

cv::cuda::GpuMat& CPictureArray::getGpuMat()
{
	if (kind == cv::_InputArray::KindFlag::MAT)
	{
		cv::cuda::GpuMat cudaMat;
		cudaMat.upload(mat);
		return cudaMat;
	}
	else if (kind == cv::_InputArray::KindFlag::UMAT)
	{
		cv::Mat local;
		umat.copyTo(local);
		cv::cuda::GpuMat cudaMat;
		cudaMat.upload(local);
		return cudaMat;
	}
	return gpuMat;
}


void CPictureArray::copyTo(cv::Mat& m)
{
	cv::Mat mat = getMat();
	mat.copyTo(m);
}

void CPictureArray::copyTo(cv::cuda::GpuMat& d_mat)
{
	cv::cuda::GpuMat mat = getGpuMat();
	mat.copyTo(d_mat);
}

void CPictureArray::copyTo(cv::UMat& m)
{
	cv::UMat mat = getUMat();
	mat.copyTo(m);
}