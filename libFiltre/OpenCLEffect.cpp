#include <header.h>
#include <OpenCLFilter.h>
#include "OpenCLEffect.h"
#include <MotionBlur.h>
#include <ImageLoadingFormat.h>
#define minmax
#include <DeepLearning.h>
using namespace Regards::OpenCL;
using namespace Regards::FiltreEffet;
using namespace Regards::DeepLearning;
extern cv::ocl::OpenCLExecutionContext clExecCtx;
extern string platformName;
#define NONE_FILTER 12

COpenCLEffect::COpenCLEffect(const CRgbaquad& backColor, CImageLoadingFormat* bitmap)
	: IFiltreEffet(backColor)
{
	this->backColor = backColor;
	flag = CL_MEM_COPY_HOST_PTR;
	SetBitmap(bitmap);
	openclFilter = new COpenCLFilter();
}

bool COpenCLEffect::StabilizeVideo(OpenCV::COpenCVStabilization* stabilizationt)
{
	return true;
}

cv::Mat COpenCLEffect::GetMat()
{
	/*
	cv::Mat output;

	if (preview && !paramOutput.empty())
	{
		paramOutput.copyTo(output);
	}
	else
	{
		input.copyTo(output);
	}

	cvtColor(output, output, cv::COLOR_BGR2BGRA);

	return output;
	*/
	cv::UMat convert;
	cv::Mat output;

	if (preview && !paramOutput.empty())
	{
		cv::cvtColor(paramOutput, convert, cv::COLOR_BGR2BGRA);
		//paramSrc.copyTo(output);
	}
	else
	{
		cv::cvtColor(input, convert, cv::COLOR_BGR2BGRA);

	}

	convert.copyTo(output);

	return output;
}

cv::UMat COpenCLEffect::GetUMat()
{
	cv::UMat output;

#ifdef WIN32
	if (platformName.find("Intel") == 0)
	{
		if (preview && !paramOutput.empty())
		{
			cv::cvtColor(paramOutput, output, cv::COLOR_BGR2BGRA);
		}
		else
		{
			cv::cvtColor(input, output, cv::COLOR_BGR2BGRA);
		}
	}
	else
	{
		if (preview && !paramOutput.empty())
		{
			cv::cvtColor(paramOutput, output, cv::COLOR_BGR2RGBA);
		}
		else
		{
			cv::cvtColor(input, output, cv::COLOR_BGR2RGBA);
		}
	}

#else

	if (preview && !paramOutput.empty())
	{
		cv::cvtColor(paramOutput, output, cv::COLOR_BGR2RGBA);
	}
	else
	{
		cv::cvtColor(input, output, cv::COLOR_BGR2RGBA);
	}

#endif




	return output;

	/*
	cv::UMat output;

	if (preview && !paramOutput.empty())
	{
		paramOutput.copyTo(output);
	}
	else
	{
		input.copyTo(output);
	}

	cvtColor(output, output, cv::COLOR_BGR2BGRA);

	return output;
	*/
}

int COpenCLEffect::GetSizeData() const
{
	return sizeof(cl_uint) * 4;
}

int COpenCLEffect::GetWidth()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.cols;
	}
	return input.cols;
}

int COpenCLEffect::GetHeight()
{
	if (preview && !paramOutput.empty())
	{
		return paramOutput.rows;
	}
	return input.rows;
}

int COpenCLEffect::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->HQDn3D(LumSpac, temporalLumaDefault, temporalSpatialLumaDefault, paramOutput);
		}
		else
		{
			openclFilter->HQDn3D(LumSpac, temporalLumaDefault, temporalSpatialLumaDefault, input);
		}
	}
	return 0;
}


int COpenCLEffect::WaveFilter(int x, int y, short height, int scale, int radius)
{
	return -1;
}


void COpenCLEffect::SetBitmap(CImageLoadingFormat* bitmap)
{

	if (bitmap != nullptr && bitmap->IsOk())
	{
		cv::Mat local = bitmap->GetOpenCVPicture();

		if (local.channels() == 4)
			cvtColor(local, input, cv::COLOR_BGRA2BGR);
		else if (local.channels() == 1)
			cvtColor(local, input, cv::COLOR_GRAY2BGR);
		else
			local.copyTo(input);

		filename = bitmap->GetFilename();
		//vector<cv::Mat> channels;
		//cv::extractChannel(local, alphaChannel, 3);
		//cv::cvtColor(local, input, cv::COLOR_BGRA2BGR);
		//local.copyTo(input);
		//delete _bitmap;
		preview = false;
	}
}

COpenCLEffect::~COpenCLEffect()
{
	delete openclFilter;
	input.release();
	paramOutput.release();
}


cv::Mat COpenCLEffect::GetBitmap(const bool& source)
{
	cv::Mat bitmapOut;
	if (source)
	{
		input.copyTo(bitmapOut);
	}
	else if (preview && !paramOutput.empty())
	{
		paramOutput.copyTo(bitmapOut);
	}
	else
	{
		input.copyTo(bitmapOut);
	}

	return bitmapOut;
}

//-----------------------------------------------------------------------------------------------
//Get Output
//-----------------------------------------------------------------------------------------------

wxImage COpenCLEffect::GetwxImage(cv::UMat& input)
{
	cv::Mat cvDest;
	cvtColor(input, cvDest, cv::COLOR_BGR2RGB);

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

wxImage COpenCLEffect::GetwxImage()
{
	if (preview && !paramOutput.empty())
	{
		return GetwxImage(paramOutput);
	}
	return GetwxImage(input);
}


int COpenCLEffect::LensDistortionFilter(const int& size)
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->LensDistortion(size, paramOutput);
	}
	else
	{
		openclFilter->LensDistortion(size, input);
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

int COpenCLEffect::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->BilateralEffect(paramOutput, fSize, sigmaX, sigmaP);
	}
	else
	{
		openclFilter->BilateralEffect(input, fSize, sigmaX, sigmaP);
	}
	return 0;
}

int COpenCLEffect::DetailEnhance(const double& sigma_s, const double& sigma_r)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->DetailEnhance(paramOutput, sigma_s, sigma_r);
		}
		else
		{
			openclFilter->DetailEnhance(input, sigma_s, sigma_r);
		}
	}
	return 0;
}

int COpenCLEffect::EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->EdgePreservingFilter(paramOutput, flags, sigma_s, sigma_r);
		}
		else
		{
			openclFilter->EdgePreservingFilter(input, flags, sigma_s, sigma_r);
		}
	}
	return 0;
}

int COpenCLEffect::PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->PencilSketch(paramOutput, sigma_s, sigma_r, shade_factor);
		}
		else
		{
			openclFilter->PencilSketch(input, sigma_s, sigma_r, shade_factor);
		}
	}
	return 0;
}

int COpenCLEffect::Stylization(const double& sigma_s, const double& sigma_r)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->Stylization(paramOutput, sigma_s, sigma_r);
		}
		else
		{
			openclFilter->Stylization(input, sigma_s, sigma_r);
		}
	}
	return 0;
}

int COpenCLEffect::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->BrightnessAndContrast(brightness, contrast, paramOutput);
		}
		else
		{
			openclFilter->BrightnessAndContrast(brightness, contrast, input);
		}
	}
	return 0;
}

int COpenCLEffect::Swirl(const float& radius, const float& angle)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->Swirl(radius, angle, paramOutput);
		}
		else
		{
			openclFilter->Swirl(radius, angle, input);
		}
	}
	return 0;
}

int COpenCLEffect::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
                                 const int& searchWindowSize)
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->NlMeans(paramOutput, h, hColor, templateWindowSize, searchWindowSize);
	}
	else
	{
		openclFilter->NlMeans(input, h, hColor, templateWindowSize, searchWindowSize);
	}

	return 0;
}

int COpenCLEffect::Posterize(const float& level, const float& gamma)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->Posterize(level, gamma, paramOutput);
		}
		else
		{
			openclFilter->Posterize(level, gamma, input);
		}
	}
	return 0;
}

int COpenCLEffect::MotionBlur(const double& radius, const double& sigma, const double& angle)
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
				openclFilter->MotionBlurCompute(kernel, offsets, kernel.size(), paramOutput);
			}
			else
			{
				openclFilter->MotionBlurCompute(kernel, offsets, kernel.size(), input);
			}
		}
	}
	//delete bitmapOut;
	return 0;
}


int COpenCLEffect::Median()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->Median(paramOutput);
		}
		else
		{
			openclFilter->Median(input);
		}
	}
	return 0;
}

int COpenCLEffect::Negatif()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->ColorEffect("Negatif", paramOutput);
		}
		else
		{
			openclFilter->ColorEffect("Negatif", input);
		}
	}

	return 0;
}

int COpenCLEffect::BrightnessAndContrastAuto(float clipHistPercent)
{
	try
	{
		{
			if (preview && !paramOutput.empty())
			{
				openclFilter->BrightnessAndContrastAuto(paramOutput, clipHistPercent);
			}
			else
			{
				openclFilter->BrightnessAndContrastAuto(input, clipHistPercent);
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


int COpenCLEffect::Sepia()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->ColorEffect("Sepia", paramOutput);
		}
		else
		{
			openclFilter->ColorEffect("Sepia", input);
		}
	}

	return 0;
}

int COpenCLEffect::NoirEtBlanc()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->ColorEffect("NoirEtBlanc", paramOutput);
		}
		else
		{
			openclFilter->ColorEffect("NoirEtBlanc", input);
		}
	}

	return 0;
}

int COpenCLEffect::NiveauDeGris()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->ColorEffect("GrayLevel", paramOutput);
		}
		else
		{
			openclFilter->ColorEffect("GrayLevel", input);
		}
	}

	return 0;
}

int COpenCLEffect::FlipVertical()
{
	{
		if (preview && !paramOutput.empty())
		{
			flip(paramOutput, paramOutput, 0);
		}
		else
		{
			flip(input, input, 0);
		}
	}

	return 0;
}

int COpenCLEffect::FlipHorizontal()
{
	if (preview && !paramOutput.empty())
	{
		flip(paramOutput, paramOutput, 1);
	}
	else
	{
		flip(input, input, 1);
	}

	return 0;
}

int COpenCLEffect::Rotate90()
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

int COpenCLEffect::Rotate180()
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

int COpenCLEffect::Rotate270()
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


int COpenCLEffect::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->Rotate("RotateFree", widthOut, heightOut, angle, paramOutput);
	}
	else
	{
		openclFilter->Rotate("RotateFree", widthOut, heightOut, angle, input);
	}
	return 0;
}

int COpenCLEffect::SharpenMasking(const float& sharpness)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->SharpenMasking(sharpness, paramOutput);
		}
		else
		{
			openclFilter->SharpenMasking(sharpness, input);
		}
	}

	return 0;
}

int COpenCLEffect::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->PhotoFiltre(clValue, intensity, paramOutput);
		}
		else
		{
			openclFilter->PhotoFiltre(clValue, intensity, input);
		}
	}

	return 0;
}

int COpenCLEffect::Solarize(const long& threshold)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->Solarize(threshold, paramOutput);
		}
		else
		{
			openclFilter->Solarize(threshold, input);
		}
	}

	return 0;
}

int COpenCLEffect::RGBFilter(const int& red, const int& green, const int& blue)
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->RGBFilter(red, green, blue, paramOutput);
	}
	else
	{
		openclFilter->RGBFilter(red, green, blue, input);
	}
	return 0;
}

int COpenCLEffect::FiltreMosaic(const int& size)
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->FiltreMosaic(paramOutput, size);
	}
	else
	{
		openclFilter->FiltreMosaic(input, size);
	}
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int COpenCLEffect::Fusion(cv::Mat& bitmapSecond, const float& pourcentage)
{
	cv::UMat second;
	bitmapSecond.copyTo(second);

	if (preview && !paramOutput.empty())
	{
		openclFilter->Fusion(paramOutput, second, pourcentage);
	}
	else
	{
		openclFilter->Fusion(input, second, pourcentage);
	}
	return 0;
}

int COpenCLEffect::Soften()
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", paramOutput);
	}
	else
	{
		openclFilter->FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", input);
	}
	return 0;
}

int COpenCLEffect::Noise()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->Noise(paramOutput);
		}
		else
		{
			openclFilter->Noise(input);
		}
	}

	return 0;
}

int COpenCLEffect::Blur(const int& radius)
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->Blur(radius, paramOutput);
		}
		else
		{
			openclFilter->Blur(radius, input);
		}
	}

	return 0;
}

int COpenCLEffect::Emboss()
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->Emboss(paramOutput);
	}
	else
	{
		openclFilter->Emboss(input);
	}

	return 0;
}


int COpenCLEffect::SharpenStrong()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->SharpenStrong(paramOutput);
		}
		else
		{
			openclFilter->SharpenStrong(input);
		}
	}
	return 0;
}

int COpenCLEffect::Sharpen()
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->Sharpen(paramOutput);
	}
	else
	{
		openclFilter->Sharpen(input);
	}
	return 0;
}

int COpenCLEffect::FiltreEdge()
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->Edge(paramOutput);
	}
	else
	{
		openclFilter->Edge(input);
	}
	return 0;
}

int COpenCLEffect::Erode()
{
	{
		if (preview && !paramOutput.empty())
		{
			openclFilter->ErodeDilate("Erode", paramOutput);
		}
		else
		{
			openclFilter->ErodeDilate("Erode", input);
		}
	}
	return 0;
}

int COpenCLEffect::Dilate()
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->ErodeDilate("Dilate", paramOutput);
	}
	else
	{
		openclFilter->ErodeDilate("Dilate", input);
	}
	return 0;
}

int COpenCLEffect::GaussianBlur(const int& radius, const int& boxSize)
{
	if (preview && !paramOutput.empty())
	{
		openclFilter->GaussianBlur(radius, boxSize, paramOutput);
	}
	else
	{
		openclFilter->GaussianBlur(radius, boxSize, input);
	}
	return 0;
}

void COpenCLEffect::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
                                  int flipH, int flipV, int angle, int ratio)
{
	{
		paramOutput = openclFilter->Interpolation(widthOut, heightOut, rc, method, input, flipH, flipV, angle, ratio);
		preview = true;
	}
}
