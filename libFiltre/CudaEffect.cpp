#include <header.h>
#include "CudaEffect.h"

#ifndef __APPLE__

#include <CudaFilter.h>
#include <MotionBlur.h>
#include <ImageLoadingFormat.h>
#define minmax
#include <DeepLearning.h>
using namespace Regards::FiltreEffet;
using namespace Regards::Cuda;
using namespace Regards::DeepLearning;

extern string platformName;
#define NONE_FILTER 12

#ifndef __APPLE__
#include "opencv2/cudaimgproc.hpp"
#include <opencv2/cudaarithm.hpp>
#include "opencv2/cudawarping.hpp"
#endif

CCudaEffect::CCudaEffect(const CRgbaquad& backColor, CImageLoadingFormat* bitmap)
	: IFiltreEffet(backColor)
{
	cudaFilter = new CCudaFilter();
	this->backColor = backColor;
	SetBitmap(bitmap);
}

bool CCudaEffect::StabilizeVideo(OpenCV::COpenCVStabilization* stabilization)
{
	return true;
}

Regards::Picture::CPictureArray CCudaEffect::GetMatrix()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput;
	}
	return input;

}


int CCudaEffect::GetWidth()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.cols;
	}
	return input.cols;
}

int CCudaEffect::GetHeight()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.rows;
	}
	return input.rows;
}

int CCudaEffect::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->HQDn3D(LumSpac, temporalLumaDefault, temporalSpatialLumaDefault, paramOutput);
		}
		else
		{
			cudaFilter->HQDn3D(LumSpac, temporalLumaDefault, temporalSpatialLumaDefault, input);
		}
	}
	return 0;
}


int CCudaEffect::WaveFilter(int x, int y, short height, int scale, int radius)
{
	return -1;
}


void CCudaEffect::SetBitmap(CImageLoadingFormat* bitmap)
{

	cv::cuda::GpuMat source;

	if (bitmap != nullptr && bitmap->IsOk())
	{
		source = bitmap->GetCudaPicture();
		//input.upload(local);

		if (source.channels() == 3)
			cv::cuda::cvtColor(source, input, cv::COLOR_BGR2BGRA);
		else if (source.channels() == 1)
			cv::cuda::cvtColor(source, input, cv::COLOR_GRAY2BGRA);
		else
			source.copyTo(input);

		filename = bitmap->GetFilename();
		//vector<cv::Mat> channels;
		//cv::extractChannel(local, alphaChannel, 3);
		//cv::cvtColor(local, input, cv::COLOR_BGRA2BGR);
		//local.copyTo(input);
		//delete _bitmap;
		preview = false;
	}
}

CCudaEffect::~CCudaEffect()
{
	delete cudaFilter;
	input.release();
	paramOutput.release();
}


cv::Mat CCudaEffect::GetBitmap(const bool& source)
{
	cv::Mat bitmapOut;
	if (source)
	{
		input.download(bitmapOut);
	}
	else if (preview && !paramOutput.empty())
	{
		paramOutput.download(bitmapOut);
	}
	else
	{
		input.download(bitmapOut);
	}

	return bitmapOut;
}

//-----------------------------------------------------------------------------------------------
//Get Output
//-----------------------------------------------------------------------------------------------

wxImage CCudaEffect::GetwxImage(cv::cuda::GpuMat& input)
{
	cv::Mat cvDest;
	cv::Mat source;

	input.download(source);

	cv::cvtColor(source, cvDest, cv::COLOR_BGRA2RGB);

	long imsize = cvDest.rows * cvDest.cols * cvDest.channels();
	wxImage wx(cvDest.cols, cvDest.rows, static_cast<unsigned char*>(malloc(imsize)), false);
	unsigned char* s = cvDest.data;
	unsigned char* d = wx.GetData();
	memcpy(d, s, imsize);


	return wx;
}


//-----------------------------------------------------------------------------------------------
//Get Output
//-----------------------------------------------------------------------------------------------

wxImage CCudaEffect::GetwxImage()
{
	if (preview && !paramOutput.empty())
	{
		return GetwxImage(paramOutput);
	}
	return GetwxImage(input);
}


int CCudaEffect::LensDistortionFilter(const int& size)
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->LensDistortion(size, paramOutput);
	}
	else
	{
		cudaFilter->LensDistortion(size, input);
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

int CCudaEffect::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->BilateralEffect(paramOutput, fSize, sigmaX, sigmaP);
	}
	else
	{
		cudaFilter->BilateralEffect(input, fSize, sigmaX, sigmaP);
	}
	return 0;
}

int CCudaEffect::DetailEnhance(const double& sigma_s, const double& sigma_r)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->DetailEnhance(paramOutput, sigma_s, sigma_r);
		}
		else
		{
			cudaFilter->DetailEnhance(input, sigma_s, sigma_r);
		}
	}
	return 0;
}

int CCudaEffect::EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->EdgePreservingFilter(paramOutput, flags, sigma_s, sigma_r);
		}
		else
		{
			cudaFilter->EdgePreservingFilter(input, flags, sigma_s, sigma_r);
		}
	}
	return 0;
}

int CCudaEffect::PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->PencilSketch(paramOutput, sigma_s, sigma_r, shade_factor);
		}
		else
		{
			cudaFilter->PencilSketch(input, sigma_s, sigma_r, shade_factor);
		}
	}
	return 0;
}

int CCudaEffect::Stylization(const double& sigma_s, const double& sigma_r)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->Stylization(paramOutput, sigma_s, sigma_r);
		}
		else
		{
			cudaFilter->Stylization(input, sigma_s, sigma_r);
		}
	}
	return 0;
}

int CCudaEffect::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->BrightnessAndContrast(brightness, contrast, paramOutput);
		}
		else
		{
			cudaFilter->BrightnessAndContrast(brightness, contrast, input);
		}
	}
	return 0;
}

int CCudaEffect::Swirl(const float& radius, const float& angle)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->Swirl(radius, angle, paramOutput);
		}
		else
		{
			cudaFilter->Swirl(radius, angle, input);
		}
	}
	return 0;
}

int CCudaEffect::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
	const int& searchWindowSize)
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->NlMeans(paramOutput, h, hColor, templateWindowSize, searchWindowSize);
	}
	else
	{
		cudaFilter->NlMeans(input, h, hColor, templateWindowSize, searchWindowSize);
	}

	return 0;
}

int CCudaEffect::Posterize(const float& level, const float& gamma)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->Posterize(level, gamma, paramOutput);
		}
		else
		{
			cudaFilter->Posterize(level, gamma, input);
		}
	}
	return 0;
}

int CCudaEffect::MotionBlur(const double& radius, const double& sigma, const double& angle)
{
	//CRegardsBitmap * bitmapOut = new CRegardsBitmap(width, height);

	{
		vector<double> kernel;
		vector<wxPoint> offsets;

		if (sigma == 0.0)
			return 0;

		kernel = CMotionBlur::GetMotionBlurKernel(radius, sigma);

		if (kernel.size() < 3)
			return false;


		{
			offsets = CMotionBlur::GetOffsetKernel(kernel.size(), angle);

			if (preview && !paramOutput.empty())
			{
				cudaFilter->MotionBlurCompute(kernel, offsets, kernel.size(), paramOutput);
			}
			else
			{
				cudaFilter->MotionBlurCompute(kernel, offsets, kernel.size(), input);
			}
		}
	}
	//delete bitmapOut;
	return 0;
}


int CCudaEffect::Median()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->Median(paramOutput);
		}
		else
		{
			cudaFilter->Median(input);
		}
	}
	return 0;
}

int CCudaEffect::Negatif()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->ColorEffect("Negatif", paramOutput);
		}
		else
		{
			cudaFilter->ColorEffect("Negatif", input);
		}
	}

	return 0;
}

int CCudaEffect::BrightnessAndContrastAuto(float clipHistPercent)
{
	try
	{
		{
			if (preview && !paramOutput.empty())
			{
				cudaFilter->BrightnessAndContrastAuto(paramOutput, clipHistPercent);
			}
			else
			{
				cudaFilter->BrightnessAndContrastAuto(input, clipHistPercent);
			}
		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	return 0;
}


int CCudaEffect::Sepia()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->ColorEffect("Sepia", paramOutput);
		}
		else
		{
			cudaFilter->ColorEffect("Sepia", input);
		}
	}

	return 0;
}

int CCudaEffect::NoirEtBlanc()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->ColorEffect("NoirEtBlanc", paramOutput);
		}
		else
		{
			cudaFilter->ColorEffect("NoirEtBlanc", input);
		}
	}

	return 0;
}

int CCudaEffect::NiveauDeGris()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->ColorEffect("GrayLevel", paramOutput);
		}
		else
		{
			cudaFilter->ColorEffect("GrayLevel", input);
		}
	}

	return 0;
}

int CCudaEffect::FlipVertical()
{
	{
		if (preview && !paramOutput.empty())
		{
			cv::flip(paramOutput, paramOutput, 0);
		}
		else
		{
			cv::flip(input, input, 0);
		}
	}

	return 0;
}

int CCudaEffect::FlipHorizontal()
{
	if (preview && !paramOutput.empty())
	{
		cv::flip(paramOutput, paramOutput, 1);
	}
	else
	{
		cv::flip(input, input, 1);
	}

	return 0;
}

int CCudaEffect::Rotate90()
{
	if (preview && !paramOutput.empty())
	{
		cv::rotate(paramOutput, paramOutput, cv::ROTATE_90_CLOCKWISE);
	}
	else
	{
		cv::rotate(input, input, cv::ROTATE_90_CLOCKWISE);
	}
	return 0;
}

int CCudaEffect::Rotate180()
{
	if (preview && !paramOutput.empty())
	{
		cv::rotate(paramOutput, paramOutput, cv::ROTATE_180);
	}
	else
	{
		cv::rotate(input, input, cv::ROTATE_180);
	}
	return 0;
}

int CCudaEffect::Rotate270()
{
	if (preview && !paramOutput.empty())
	{
		cv::rotate(paramOutput, paramOutput, cv::ROTATE_90_COUNTERCLOCKWISE);
	}
	else
	{
		cv::rotate(input, input, cv::ROTATE_90_COUNTERCLOCKWISE);
	}
	return 0;
}


int CCudaEffect::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->Rotate("RotateFree", widthOut, heightOut, angle, paramOutput);
	}
	else
	{
		cudaFilter->Rotate("RotateFree", widthOut, heightOut, angle, input);
	}
	return 0;
}

int CCudaEffect::SharpenMasking(const float& sharpness)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->SharpenMasking(sharpness, paramOutput);
		}
		else
		{
			cudaFilter->SharpenMasking(sharpness, input);
		}
	}

	return 0;
}

int CCudaEffect::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->PhotoFiltre(clValue, intensity, paramOutput);
		}
		else
		{
			cudaFilter->PhotoFiltre(clValue, intensity, input);
		}
	}

	return 0;
}

int CCudaEffect::Solarize(const long& threshold)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->Solarize(threshold, paramOutput);
		}
		else
		{
			cudaFilter->Solarize(threshold, input);
		}
	}

	return 0;
}

int CCudaEffect::RGBFilter(const int& red, const int& green, const int& blue)
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->RGBFilter(red, green, blue, paramOutput);
	}
	else
	{
		cudaFilter->RGBFilter(red, green, blue, input);
	}
	return 0;
}

int CCudaEffect::FiltreMosaic(const int& size)
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->FiltreMosaic(paramOutput, size);
	}
	else
	{
		cudaFilter->FiltreMosaic(input, size);
	}
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CCudaEffect::Fusion(cv::Mat& bitmapSecond, const float& pourcentage)
{
	cv::cuda::GpuMat second;
	bitmapSecond.copyTo(second);

	if (preview && !paramOutput.empty())
	{
		cudaFilter->Fusion(paramOutput, second, pourcentage);
	}
	else
	{
		cudaFilter->Fusion(input, second, pourcentage);
	}
	return 0;
}

int CCudaEffect::Soften()
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", paramOutput);
	}
	else
	{
		cudaFilter->FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", input);
	}
	return 0;
}

int CCudaEffect::Noise()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->Noise(paramOutput);
		}
		else
		{
			cudaFilter->Noise(input);
		}
	}

	return 0;
}

int CCudaEffect::Blur(const int& radius)
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->Blur(radius, paramOutput);
		}
		else
		{
			cudaFilter->Blur(radius, input);
		}
	}

	return 0;
}

int CCudaEffect::Emboss()
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->Emboss(paramOutput);
	}
	else
	{
		cudaFilter->Emboss(input);
	}

	return 0;
}


int CCudaEffect::SharpenStrong()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->SharpenStrong(paramOutput);
		}
		else
		{
			cudaFilter->SharpenStrong(input);
		}
	}
	return 0;
}

int CCudaEffect::Sharpen()
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->Sharpen(paramOutput);
	}
	else
	{
		cudaFilter->Sharpen(input);
	}
	return 0;
}

int CCudaEffect::FiltreEdge()
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->Edge(paramOutput);
	}
	else
	{
		cudaFilter->Edge(input);
	}
	return 0;
}

int CCudaEffect::Erode()
{
	{
		if (preview && !paramOutput.empty())
		{
			cudaFilter->ErodeDilate("Erode", paramOutput);
		}
		else
		{
			cudaFilter->ErodeDilate("Erode", input);
		}
	}
	return 0;
}

int CCudaEffect::Dilate()
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->ErodeDilate("Dilate", paramOutput);
	}
	else
	{
		cudaFilter->ErodeDilate("Dilate", input);
	}
	return 0;
}

int CCudaEffect::GaussianBlur(const int& radius, const int& boxSize)
{
	if (preview && !paramOutput.empty())
	{
		cudaFilter->GaussianBlur(radius, boxSize, paramOutput);
	}
	else
	{
		cudaFilter->GaussianBlur(radius, boxSize, input);
	}
	return 0;
}

void CCudaEffect::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
	int flipH, int flipV, int angle, int ratio)
{
	{
		paramOutput = cudaFilter->Interpolation(widthOut, heightOut, rc, method, input, flipH, flipV, angle, ratio);
		preview = true;
	}
}


#endif