#include <header.h>
#include <OpenCLFilter.h>
#include "OpenCLEffect.h"
#include <MotionBlur.h>
#include <ImageLoadingFormat.h>
#include <appcontext.h>
#define minmax

using namespace Regards::OpenCL;
using namespace Regards::FiltreEffet;

#define NONE_FILTER 12

template<typename F>
void COpenCLEffect::ExecuteSafe(F&& func)
{
	try
	{
		cv::UMat convert;

		if (preview && !paramOutput.empty())
		{
			convert = paramOutput;
		}
		else
		{
			convert = input;
		}
		func(convert);
	}
	catch (const cv::Exception& e)
	{
		LogError(e.what());
	}
}


COpenCLEffect::COpenCLEffect(const CRgbaquad& backColor, CImageLoadingFormat* bitmap, COpenCLContext* openCLContext)
	: IFiltreEffet(backColor)
{
	this->backColor = backColor;
	flag = CL_MEM_COPY_HOST_PTR;
	SetBitmap(bitmap);
	openclFilter = std::make_unique<COpenCLFilter>(openCLContext);
}

bool COpenCLEffect::StabilizeVideo(OpenCV::COpenCVStabilization* stabilization)
{
	return true;
}


Regards::Picture::CPictureArray COpenCLEffect::GetMatrix()
{
	Regards::Picture::CPictureArray pictureArray;
	ExecuteSafe([&](cv::UMat& image)
		{
			pictureArray.SetArray(image);
		});

	return pictureArray;
}

int COpenCLEffect::GetWidth()
{
	int width = 0;
	ExecuteSafe([&](cv::UMat& image)
		{
			width = image.size().width;
		});

	return width;
}

int COpenCLEffect::GetHeight()
{
	int height = 0;
	ExecuteSafe([&](cv::UMat& image)
		{
			height = image.size().height;
		});

	return height;
}

int COpenCLEffect::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->HQDn3D(LumSpac, temporalLumaDefault, temporalSpatialLumaDefault, image);
		});

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
		cv::UMat local = bitmap->GetMatrix().getUMat();

		if (local.channels() == 4)
			cvtColor(local, input, cv::COLOR_BGRA2BGR);
		else if (local.channels() == 1)
			cvtColor(local, input, cv::COLOR_GRAY2BGR);
		else
			local.copyTo(input);

		filename = bitmap->GetFilename();

		preview = false;
	}
}

COpenCLEffect::~COpenCLEffect()
{
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
	wxImage img;
	ExecuteSafe([&](cv::UMat& image)
		{
			img = GetwxImage(image);
		});

	return img;
}


int COpenCLEffect::LensDistortionFilter(const int& size)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->LensDistortion(size, image);
		});
	return 0;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

int COpenCLEffect::BilateralFilter(const int& fSize, const int& sigmaX, const int& sigmaP)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->BilateralEffect(image, fSize, sigmaX, sigmaP);
		});
	return 0;
}

int COpenCLEffect::DetailEnhance(const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->DetailEnhance(image, sigma_s, sigma_r);
		});
	return 0;
}

int COpenCLEffect::EdgePreservingFilter(const int& flags, const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->EdgePreservingFilter(image, flags, sigma_s, sigma_r);
		});
	return 0;
}

int COpenCLEffect::PencilSketch(const double& sigma_s, const double& sigma_r, const double& shade_factor)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->PencilSketch(image, sigma_s, sigma_r, shade_factor);
		});
	return 0;
}

int COpenCLEffect::Stylization(const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Stylization(image, sigma_s, sigma_r);
		});
	return 0;
}

int COpenCLEffect::BrightnessAndContrast(const double& brightness, const double& contrast)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->BrightnessAndContrast(brightness, contrast, image);
		});
	return 0;
}

int COpenCLEffect::Swirl(const float& radius, const float& angle)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Swirl(radius, angle, image);
		});
	return 0;
}

int COpenCLEffect::NlmeansFilter(const int& h, const int& hColor, const int& templateWindowSize,
	const int& searchWindowSize)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->NlMeans(image, h, hColor, templateWindowSize, searchWindowSize);
		});
	return 0;
}

int COpenCLEffect::Posterize(const float& level, const float& gamma)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Posterize(level, gamma, image);
		});
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

			ExecuteSafe([&](cv::UMat& image)
				{
					openclFilter->MotionBlurCompute(kernel, offsets, kernel.size(), image);
				});
		}
	}
	//delete bitmapOut;
	return 0;
}


int COpenCLEffect::Median()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Median(paramOutput);
		});
	return 0;
}

int COpenCLEffect::Negatif()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->ColorEffect("Negatif", image);
		});

	return 0;
}

int COpenCLEffect::BrightnessAndContrastAuto(float clipHistPercent)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->BrightnessAndContrastAuto(image, clipHistPercent);
		});

	return 0;
}


int COpenCLEffect::Sepia()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->ColorEffect("Sepia", image);
		});

	return 0;
}

int COpenCLEffect::NoirEtBlanc()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->ColorEffect("NoirEtBlanc", image);
		});

	return 0;
}

int COpenCLEffect::NiveauDeGris()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->ColorEffect("GrayLevel", image);
		});

	return 0;
}

int COpenCLEffect::FlipVertical()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			flip(image, image, 0);
		});

	return 0;
}

int COpenCLEffect::FlipHorizontal()
{
	ExecuteSafe([&](cv::UMat& image)
		{
		flip(image, image, 1);
		});

	return 0;
}

int COpenCLEffect::Rotate90()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
		});
	return 0;
}

int COpenCLEffect::Rotate180()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			cv::rotate(image, image, cv::ROTATE_180);
		});

	return 0;
}

int COpenCLEffect::Rotate270()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			cv::rotate(image, image, cv::ROTATE_90_COUNTERCLOCKWISE);
		});

	return 0;
}


int COpenCLEffect::RotateFree(const double& angle, const int& widthOut, const int& heightOut)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Rotate("RotateFree", widthOut, heightOut, angle, image);
		});
	return 0;
}

int COpenCLEffect::SharpenMasking(const float& sharpness)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->SharpenMasking(sharpness, image);
		});
	return 0;
}

int COpenCLEffect::PhotoFiltre(const CRgbaquad& clValue, const int& intensity)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->PhotoFiltre(clValue, intensity, image);
		});
	return 0;
}

int COpenCLEffect::Solarize(const long& threshold)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Solarize(threshold, image);
		});
	return 0;
}

int COpenCLEffect::RGBFilter(const int& red, const int& green, const int& blue)
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->RGBFilter(red, green, blue, image);
		});
	return 0;
}

int COpenCLEffect::FiltreMosaic(const int& size)
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->FiltreMosaic(image, size);
		});
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int COpenCLEffect::Fusion(cv::Mat& bitmapSecond, const float& pourcentage)
{
	cv::UMat second;
	bitmapSecond.copyTo(second);

	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->Fusion(image, second, pourcentage);
		});
	return 0;
}

int COpenCLEffect::Soften()
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->FiltreConvolution("IDR_OPENCL_SOFTEN", "Soften", image);
		});
	return 0;
}

int COpenCLEffect::Noise()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Noise(image);
		});
	return 0;
}

int COpenCLEffect::Blur(const int& radius)
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->Blur(radius, image);
		});
	return 0;
}

int COpenCLEffect::Emboss()
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->Emboss(image);
		});
	return 0;
}


int COpenCLEffect::SharpenStrong()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->SharpenStrong(image);
		});
	return 0;
}

int COpenCLEffect::Sharpen()
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->Sharpen(image);
		});
	return 0;
}

int COpenCLEffect::FiltreEdge()
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->Edge(image);
		});
	return 0;
}

int COpenCLEffect::Erode()
{
	ExecuteSafe([&](cv::UMat& image)
		{
			openclFilter->ErodeDilate("Erode", image);
		});
	return 0;
}

int COpenCLEffect::Dilate()
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->ErodeDilate("Dilate", image);
		});
	return 0;
}

int COpenCLEffect::GaussianBlur(const int& radius, const int& boxSize)
{
	ExecuteSafe([&](cv::UMat& image)
		{
		openclFilter->GaussianBlur(radius, boxSize, image);
		});
	return 0;
}

void COpenCLEffect::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
                                  int flipH, int flipV, int angle, int ratio, bool bgraOutput)
{
	{
		paramOutput = openclFilter->Interpolation(widthOut, heightOut, rc, method, input, flipH, flipV, angle, ratio, bgraOutput);
		preview = true;
	}
}
