#include "header.h"
#include "PictureArray.h"
using namespace Regards::Picture;

CPictureArray::CPictureArray(const cv::_InputArray::KindFlag& type)
{
	kind = type;
}

CPictureArray::CPictureArray(cv::Mat& m)
{
	mat = m;
	kind = cv::_InputArray::KindFlag::MAT;
}

void CPictureArray::SetArray(cv::Mat& m)
{
  	mat = m;
	kind = cv::_InputArray::KindFlag::MAT;  
}

void CPictureArray::SetArray(cv::UMat& m)
{
  	umat = m;
	kind = cv::_InputArray::KindFlag::UMAT;  
}


CPictureArray::CPictureArray(cv::UMat& m)
{
	umat = m;
	kind = cv::_InputArray::KindFlag::UMAT;
}

int CPictureArray::getWidth()
{
    if (kind == cv::_InputArray::KindFlag::MAT)
	{
		return mat.size().width;
	}
	return umat.size().width;
}

int CPictureArray::getHeight()
{
    if (kind == cv::_InputArray::KindFlag::MAT)
	{
		return mat.size().height;
	}
	return umat.size().height;
}

cv::_InputArray::KindFlag CPictureArray::Kind()
{
	return kind;
}

cv::UMat& CPictureArray::getUMat()
{
    if(kind == cv::_InputArray::KindFlag::MAT)
	{
		mat.copyTo(umat);
		return umat;
	}
	return umat;
}

cv::Mat& CPictureArray::getMat()
{
    if (kind == cv::_InputArray::KindFlag::UMAT)
	{
		umat.copyTo(mat);
		return mat;
	}
	return mat;
}

void CPictureArray::copyTo(cv::Mat& m)
{
	cv::Mat mat = getMat();
	mat.copyTo(m);
}

void CPictureArray::copyTo(cv::UMat& m)
{
	cv::UMat mat = getUMat();
	mat.copyTo(m);
}

bool CPictureArray::empty()
{
    if (kind == cv::_InputArray::KindFlag::UMAT)
	{
		return umat.empty();
	}
	return mat.empty();
}


void CPictureArray::Release()
{
    if (kind == cv::_InputArray::KindFlag::UMAT)
	{
		umat.release();
	}
	mat.release();
}