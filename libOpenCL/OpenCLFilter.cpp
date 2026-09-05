#include <header.h>
#include "OpenCLFilter.h"
#include "utility.h"
#include <opencv2/xphoto.hpp>
#include <FileUtility.h>
#include <mutex>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <opencv2/core/ocl.hpp>
#include <opencv2/dnn_superres.hpp>

#include <wx/filename.h>
#include "OpenCLKernelBuilder.h"
#include <appcontext.h>
extern AppContext application_context;

using namespace Regards::OpenCL;
using namespace cv;

using namespace dnn;
using namespace dnn_superres;

#define OPENCV_METHOD


#define EDSR 0
#define ESPCN 1
#define FSRCNN 2
#define LapSRN 3

std::atomic<bool> isDnnUsed{ false };
std::mutex muDnnSuperResImpl;
int numTexture = -1;


int COpenCLFilter::numTexture = -1;

class ParameterReleaseGuard
{
public:

	explicit ParameterReleaseGuard(std::vector<COpenCLParameter*>& params)
		: params_(params)
	{}

	~ParameterReleaseGuard()
	{
		for (auto* p : params_)
		{
			if (!p->GetNoDelete())
				p->Release();
		}
	}

private:

	std::vector<COpenCLParameter*>& params_;
};

template<typename F>
void ExecuteSafe(F&& func)
{
	try
	{
		func();
	}
	catch (const cv::Exception& e)
	{
		LogError(e.what());
	}
}

template<typename F>
void ExecuteSafeOpenCL(cv::UMat& inputData, F&& func)
{
	try
	{
		cv::UMat source;

		const bool convert = inputData.channels() == 3;

		if (convert)
			cv::cvtColor(inputData, source, cv::COLOR_BGR2BGRA);
		else
			source = inputData;

		cv::UMat result = func(source);

		if (convert)
			cv::cvtColor(result, inputData, cv::COLOR_BGRA2BGR);
		else
			inputData = std::move(result);
	}
	catch (const cv::Exception& e)
	{
		LogError(e.what());
	}
}

template<typename F>
cv::UMat ExecuteSafeOpenCLWithUMatOutput(cv::UMat& inputData, bool bgraOutput, F&& func)
{
	cv::UMat dest;
	try
	{
		cv::UMat source;

		const bool convert = inputData.channels() == 3;

		if (convert)
			cv::cvtColor(inputData, source, cv::COLOR_BGR2BGRA);
		else
			source = inputData;

		cv::UMat result = func(source);


		if (!bgraOutput)
			cv::cvtColor(result, dest, COLOR_BGRA2BGR);
		else
			dest = std::move(result);

	}
	catch (const cv::Exception& e)
	{
		LogError(e.what());
	}
	return dest;
}

class CSuperSampling
{
public:
	CSuperSampling()
	{
	};

	~CSuperSampling()
	{
	};
	string GenerateModelPath(string modelName, int scale);
	bool TestIfMethodIsValid(int method, int scale);
	UMat upscaleImage(UMat img, int method, int scale);

private:
	DnnSuperResImpl sr;
	int oldscale = -1;
	int oldmethod = -1;
};

string CSuperSampling::GenerateModelPath(string modelName, int scale)
{

	wxFileName path = wxFileName(CFileUtility::GetResourcesFolderPath());
	path.AppendDir("model");
	path.SetFullName(modelName + "_x" + to_string(scale) + ".pb");

	return path.GetFullPath().utf8_string();
}

bool CSuperSampling::TestIfMethodIsValid(int method, int scale)
{
	if (method == EDSR && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	if (method == ESPCN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	if (method == FSRCNN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	if (method == LapSRN && (scale == 2 || scale == 4 || scale == 8))
	{
		return true;
	}
	return false;
}

UMat CSuperSampling::upscaleImage(UMat img, int method, int scale)
{
	
	isDnnUsed = true;
	UMat outputImage;

	if (oldscale != scale || oldmethod != method)
	{
		try
		{
			switch (method)
			{
			case EDSR:
				{
					string algorithm = "edsr";
					sr.readModel(GenerateModelPath("EDSR", scale));
					sr.setModel(algorithm, scale);
				}
				break;

			case ESPCN:
				{
					string algorithm = "espcn";
					sr.readModel(GenerateModelPath("ESPCN", scale));
					sr.setModel(algorithm, scale);
				}
				break;
			case FSRCNN:
				{
					string algorithm = "fsrcnn";
					sr.readModel(GenerateModelPath("FSRCNN", scale));
					sr.setModel(algorithm, scale);
				}
				break;
			case LapSRN:
				{
					string algorithm = "lapsrn";
					sr.readModel(GenerateModelPath("LapSRN", scale));
					sr.setModel(algorithm, scale);
				}
				break;
			}

			sr.setPreferableTarget(DNN_TARGET_OPENCL);
			sr.upsample(img, outputImage);

			//muDnnSuperResImpl.unlock();
		}
		catch (Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}
	}
	else
	{
		try
		{
			sr.upsample(img, outputImage);
		}
		catch (Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "CSuperSampling::exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}
	}

	oldscale = scale;
	oldmethod = method;
	isDnnUsed = false;
	return outputImage;
}


COpenCLFilter::COpenCLFilter(COpenCLContext* openCLContext)
	: openCLContext(openCLContext)
{
	bool useMemory = (ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	hq3d = nullptr;
    superSampling = std::make_unique<CSuperSampling>();
	resizer = std::make_unique<COpenCLAvirResizer>(openCLContext);
	
}

COpenCLFilter::~COpenCLFilter()
{
    openclMemTempMap.clear();
}

void COpenCLFilter::DetailEnhance(UMat& inputData, const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&]
	{
		UMat dest;

		if (inputData.channels() == 4)
		{
			cvtColor(inputData, dest, COLOR_BGRA2BGR);
		}

		cv::detailEnhance(inputData, dest, sigma_s, sigma_r);

		if (inputData.channels() == 4)
		{
			cvtColor(dest, inputData, COLOR_BGR2BGRA);
		}
		else
			dest.copyTo(inputData);
	});
}

void COpenCLFilter::EdgePreservingFilter(UMat& inputData, const int& flags, const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&]
		{
			UMat dest;

			if (inputData.channels() == 4)
			{
				cvtColor(inputData, dest, COLOR_BGRA2BGR);
			}

			edgePreservingFilter(inputData, dest, flags, sigma_s, sigma_r);

			if (inputData.channels() == 4)
			{
				cvtColor(dest, inputData, COLOR_BGR2BGRA);
			}
			else
				dest.copyTo(inputData);

		});
}

void COpenCLFilter::PencilSketch(UMat& inputData, const double& sigma_s, const double& sigma_r, const double& shade_factor)
{
	ExecuteSafe([&]
		{

		UMat img1;
		UMat dest;

		if (inputData.channels() == 4)
			cvtColor(inputData, dest, COLOR_BGRA2BGR);

		pencilSketch(inputData, img1, dest, sigma_s, sigma_r, shade_factor);

		if (inputData.channels() == 4)
			cvtColor(dest, inputData, COLOR_BGR2BGRA);
		else
			dest.copyTo(inputData);

		});
}

void COpenCLFilter::Stylization(UMat& inputData, const double& sigma_s, const double& sigma_r)
{
	ExecuteSafe([&]
	{
		UMat dest;
		if (inputData.channels() == 4)
			cvtColor(inputData, dest, COLOR_BGRA2BGR);

		stylization(inputData, dest, sigma_s, sigma_r);

		if (inputData.channels() == 4)
			cvtColor(dest, inputData, COLOR_BGR2BGRA);
		else
			dest.copyTo(inputData);
	});
}

void COpenCLFilter::BilateralEffect(UMat& inputData, const int& fSize, const int& sigmaX, const int& sigmaP)
{
	ExecuteSafe([&]
		{
		UMat dest;
		if (inputData.channels() == 4)
			cvtColor(inputData, dest, COLOR_BGRA2BGR);

		bilateralFilter(inputData, dest, fSize, sigmaX, sigmaP, BORDER_DEFAULT);

		if (inputData.channels() == 4)
			cvtColor(dest, inputData, COLOR_BGR2BGRA);
		else
			dest.copyTo(inputData);

		});
}


void COpenCLFilter::NlMeans(UMat& inputData, const int& h, const int& hColor, const int& templateWindowSize,
                            const int& searchWindowSize)
{
	ExecuteSafe([&]
		{
			UMat ycbcr;
			UMat yChannel;
			UMat yChannelOut;

			cvtColor(inputData, ycbcr, COLOR_BGR2YCrCb);

			// Extract the Y channel (UNE SEULE FOIS)
			extractChannel(ycbcr, yChannel, 0);

			fastNlMeansDenoising(yChannel, yChannelOut, h, templateWindowSize, searchWindowSize);

			// Merge back
			insertChannel(yChannelOut, ycbcr, 0);

			// convert back to RGB
			cvtColor(ycbcr, inputData, COLOR_YCrCb2BGR);
		});
}


void COpenCLFilter::Bm3d(UMat& inputData, const float& fSigma)
{
	ExecuteSafe([&]
		{
			UMat ycbcr;
			UMat yChannel;
			UMat yChannelOut;

			cvtColor(inputData, ycbcr, COLOR_BGR2YUV);

			// Extract the Y channel
			extractChannel(ycbcr, yChannel, 0);

			xphoto::bm3dDenoising(yChannel, yChannelOut, fSigma);

			// Merge the the color planes back into an Lab image
			insertChannel(yChannelOut, ycbcr, 0);

			// convert back to RGB
			cvtColor(ycbcr, inputData, COLOR_YUV2BGR);

			// Temporary Mat not reused, so release from memory.
			yChannel.release();
			ycbcr.release();
			yChannelOut.release();
		});
}




void COpenCLFilter::BrightnessAndContrastAuto(UMat& inputData, float clipHistPercent)
{
	ExecuteSafe([&]
		{
        int histSize = 256;
		float alpha, beta;
		double minGray = 0, maxGray = 0;
		
		std::vector<cv::UMat> yuv_planes(3);
		cv::UMat gpuframe_3channel(inputData.size(), CV_8UC3);
		cv::cvtColor(inputData, gpuframe_3channel, COLOR_BGR2YUV, 3);
		cv::split(gpuframe_3channel, yuv_planes);

		if (clipHistPercent == 0)
		{
			// keep full available range
			minMaxLoc(yuv_planes[0], &minGray, &maxGray);
		}
		else
		{
			UMat h;

			std::vector<int> channels = {0}; // Analyze only the channel 0.
			std::vector<int> hsize = {256};
			//Quantize the intensities in the image using 256 levels even if all the levels are not present.
			std::vector<float> hranges = {0, 256}; // The range is between 0 - 255 (so less than 256).

			calcHist(std::vector<UMat>(1, yuv_planes[0]), channels, noArray(), h, hsize, hranges);

			Mat hist;

			h.copyTo(hist);

			// calculate cumulative distribution from the histogram
			std::vector<float> accumulator(histSize);
			accumulator[0] = hist.at<float>(0);
			for (int i = 1; i < histSize; i++)
			{
				accumulator[i] = accumulator[i - 1] + hist.at<float>(i);
			}

			// locate points that cuts at required value
			float max = accumulator.back();
			clipHistPercent *= (max / 100.0); //make percent as absolute
			clipHistPercent /= 2.0; // left and right wings
			// locate left cut
			minGray = 0;
			while (accumulator[minGray] < clipHistPercent)
				minGray++;

			// locate right cut
			maxGray = histSize - 1;
			while (accumulator[maxGray] >= (max - clipHistPercent))
			{
				maxGray--;
				if (maxGray == 0)
					break;
			}
		}

		// current range
		float inputRange = maxGray - minGray;

		alpha = (histSize - 1) / inputRange; // alpha expands current range to histsize range
		beta = -minGray * alpha; // beta shifts current range so that minGray will go to 0

		convertScaleAbs(inputData, inputData, alpha, beta);

		});
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void COpenCLFilter::Fusion(UMat& inputData, const UMat& secondPictureData, const float& pourcentage)
{
	ExecuteSafe([&]
		{
			UMat dst;
			float beta = (1.0 - pourcentage);
			addWeighted(inputData, pourcentage, secondPictureData, beta, 0.0, dst);
			dst.copyTo(inputData);
		});
}


void COpenCLFilter::SharpenMasking(const float& sharpness, UMat& inputData)
{

	UMat cvDestBgra;
	double sigma = 1;
	cv::GaussianBlur(inputData, cvDestBgra, Size(), sigma, sigma);

	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& image)
		{
			OpenCLKernelBuilder builder;

			auto clBuffer = static_cast<cl_mem>(image.handle(ACCESS_READ));
			auto clBuffer_out = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			builder
				.Image("input", clBuffer)
				.Image("gaussian", clBuffer_out)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Float("sharpness", sharpness);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				"IDR_OPENCL_SHARPENMASKING",
				"SharpenMasking",
				params,
				inputData.cols,
				inputData.rows);
		});

}

void COpenCLFilter::PhotoFiltre(const CRgbaquad& clValue, const int& intensity, UMat& inputData)
{
	ExecuteSafe([&]
		{
			float coeff = static_cast<float>(intensity) / 100.0f;
			float diff = 1.0f - coeff;
			UMat out;
			UMat out_one;
			out_one = inputData.mul(diff);

			auto color = Scalar(clValue.GetBlue(), clValue.GetGreen(), clValue.GetRed());
			Scalar out_two = color * coeff;

			add(out_one, out_two, out);
			out.copyTo(inputData);
		});
}

void COpenCLFilter::RGBFilter(const int& red, const int& green, const int& blue, UMat& inputData)
{
	ExecuteSafe([&]
		{
		UMat out;
		auto color = Scalar(blue, green, red);
		add(inputData, color, out);
		out.copyTo(inputData);
		});
}

void COpenCLFilter::FiltreMosaic(UMat& inputData, const int& size)
{

	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& image)
		{
			OpenCLKernelBuilder builder;

			auto clBuffer = static_cast<cl_mem>(image.handle(ACCESS_READ));

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Int("fTileSize", size);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				"IDR_OPENCL_MOSAIC",
				"Mosaic",
				params,
				inputData.cols,
				inputData.rows);
		});
}

void COpenCLFilter::Blur(const int& radius, UMat& inputData)
{
	ExecuteSafe([&]
		{
			blur(inputData, inputData, Size(radius, radius));
		});
}

void COpenCLFilter::GaussianBlur(const int& radius, const int& boxSize, UMat& inputData)
{
	ExecuteSafe([&]
		{
		cv::GaussianBlur(inputData, inputData, Size(boxSize, boxSize), radius);
		});
}

void COpenCLFilter::MotionBlurCompute(const vector<double>& kernelMotion, const vector<wxPoint>& offsets,
                                      const int& size,
                                      UMat& inputData)
{
	
	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& image)
		{
			auto clBuffer = static_cast<cl_mem>(image.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			vector<float> kernel;
			for (auto i = 0; i < kernelMotion.size(); i++)
				kernel.push_back(kernelMotion[i]);

			vector<int> offsetsMotion;
			for (auto i = 0, j = 0; i < offsets.size(); i++, j += 2)
			{
				offsetsMotion.push_back(offsets[i].x);
				offsetsMotion.push_back(offsets[i].y);
			}

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.FloatArray("kernelMotion", openCLContext->GetContext(), kernel.data(), size, flag)
				.IntArray("offsets", openCLContext->GetContext(), offsetsMotion.data(), size * 2, flag)
				.Int("kernelSize", size);


			auto params = builder.GetParameters();

			cv::UMat dest = ExecuteOpenCLCode(
				"IDR_OPENCL_MOTIONBLUR",
				"MotionBlur",
				params,
				inputData.cols,
				inputData.rows);

			return dest;
		});

}

void COpenCLFilter::Emboss(UMat& inputData)
{
	ExecuteSafe([&]
		{
			// Construct kernel (all entries initialized to 0)
			Mat kernel(3, 3, CV_32F, Scalar(0));
			kernel.at<float>(0, 0) = -1.0;
			kernel.at<float>(2, 2) = 1.0;
			UMat dest;
			filter2D(inputData, dest, inputData.depth(), kernel);
			dest.copyTo(inputData);
		});
}

void COpenCLFilter::Sharpen(UMat& inputData)
{
	ExecuteSafe([&]
		{
			// Construct kernel (all entries initialized to 0)
			// Construct kernel (all entries initialized to 0)
			Mat kernel(3, 3, CV_32F, Scalar(0));
			// assigns kernel values
			kernel.at<float>(1, 1) = 5.0;
			kernel.at<float>(0, 1) = -1.0;
			kernel.at<float>(2, 1) = -1.0;
			kernel.at<float>(1, 0) = -1.0;
			kernel.at<float>(1, 2) = -1.0;

			UMat dest;
			filter2D(inputData, dest, inputData.depth(), kernel);
			dest.copyTo(inputData);
		});
}

void COpenCLFilter::SharpenStrong(UMat& inputData)
{
	ExecuteSafe([&]
		{

			Mat kernel(3, 3, CV_32F, Scalar(0));
			kernel.at<float>(0, 0) = -1.0;
			kernel.at<float>(0, 1) = -1.0;
			kernel.at<float>(0, 2) = -1.0;
			kernel.at<float>(1, 0) = -1.0;
			kernel.at<float>(1, 1) = 9.0;
			kernel.at<float>(1, 2) = -1.0;
			kernel.at<float>(2, 0) = -1.0;
			kernel.at<float>(2, 1) = -1.0;
			kernel.at<float>(2, 2) = -1.0;
			UMat dest;
			filter2D(inputData, dest, inputData.depth(), kernel);
			dest.copyTo(inputData);
		});
}

void COpenCLFilter::Edge(UMat& inputData)
{
	ExecuteSafe([&]
		{
			UMat dest;
			cvtColor(inputData, dest, COLOR_BGR2GRAY);

			Mat img_blur;
			cv::GaussianBlur(dest, img_blur, Size(3, 3), 0, 0);
			UMat edges;
			Canny(img_blur, edges, 100, 200, 3, false);

			cvtColor(edges, inputData, COLOR_GRAY2BGR);
		});
}

void COpenCLFilter::FiltreConvolution(const wxString& programName, const wxString& functionName, UMat& inputData)
{

	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				programName, functionName,
				params,
				inputData.cols,
				inputData.rows);

			});

}

void COpenCLFilter::ErodeDilate(const wxString& functionName, UMat& inputData)
{
	ExecuteSafe([&]
		{
			if (functionName == "Erode")
				erode(inputData, inputData, Mat());
			else if (functionName == "Dilate")
				dilate(inputData, inputData, Mat());
		});
}

void COpenCLFilter::Posterize(const float& level, const float& gamma, UMat& inputData)
{
	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Int("level", level);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				"IDR_OPENCL_COLOR", "Posterisation",
				params,
				inputData.cols,
				inputData.rows);

		});
}


void COpenCLFilter::LensDistortion(const float& strength, UMat& inputData)
{
	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& cvDestBgra)
		{

			double _strength = static_cast<double>(strength) / 100;
			double correctionRadius = sqrt(pow(inputData.rows, 2) + pow(inputData.cols, 2)) / _strength;

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Float("correctionRadius", correctionRadius);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				"IDR_OPENCL_DISTORTION", "Distortion",
				params,
				inputData.cols,
				inputData.rows);

		});
}


void COpenCLFilter::Solarize(const long& threshold, UMat& inputData)
{
	
	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Int("threshold", static_cast<int>(threshold));

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				"IDR_OPENCL_COLOR", "Solarization",
				params,
				inputData.cols,
				inputData.rows);

		});
}

void COpenCLFilter::Median(UMat& inputData)
{
	ExecuteSafe([&]
		{
			medianBlur(inputData, inputData, 3);
		});
}

void COpenCLFilter::Noise(UMat& inputData)
{
	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				"IDR_OPENCL_NOISE", "Noise",
				params,
				inputData.cols,
				inputData.rows);

		});
}


void COpenCLFilter::Flip(const wxString& functionName, UMat& inputData)
{
	
	if (functionName == "FlipVertical")
	{
		flip(inputData, inputData, 0);
	}
	else
	{
		flip(inputData, inputData, 1);
	}
}

void COpenCLFilter::Swirl(const float& radius, const float& angle, UMat& inputData)
{
	ExecuteSafeOpenCL(inputData,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Float("radius", radius)
				.Float("angle", angle);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode(
				"IDR_OPENCL_SWIRL", "Swirl",
				params,
				inputData.cols,
				inputData.rows);

		});
}


cv::UMat COpenCLFilter::Interpolation(const int& widthOut, const int& heightOut, cv::UMat& inputData, const int& method, int width, int height, int flipH, int flipV, int angle, bool bgraOutput)
{

	return ExecuteSafeOpenCLWithUMatOutput(inputData, bgraOutput,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Int("widthOut", widthOut)
				.Int("heightOut", heightOut)
				.Int("flipH", flipH)
				.Int("flipV", flipV)
				.Int("angle", angle)
				.Int("type", method);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode("IDR_OPENCL_INTERPOLATION", "Interpolation", params, widthOut, heightOut);

		});
}


cv::UMat COpenCLFilter::Interpolation(const int& widthOut, const int& heightOut, cv::UMat& inputData, int width, int height, const int& method, bool bgraOutput)
{
	return ExecuteSafeOpenCLWithUMatOutput(inputData, bgraOutput,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Int("widthOut", widthOut)
				.Int("heightOut", heightOut)
				.Int("type", method);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode("IDR_OPENCL_INTERPOLATION", "InterpolationDirect", params, widthOut, heightOut);

		});
}

cv::UMat COpenCLFilter::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method, cv::UMat& inputData, int width, int height, int flipH, int flipV, int angle, bool bgraOutput)
{
	return ExecuteSafeOpenCLWithUMatOutput(inputData, bgraOutput,
		[&](cv::UMat& cvDestBgra)
		{

			auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

			OpenCLKernelBuilder builder;

			builder
				.Image("input", clBuffer)
				.Int("width", inputData.cols)
				.Int("height", inputData.rows)
				.Int("widthOut", widthOut)
				.Int("heightOut", heightOut)
				.Float("left", rc.x)
				.Float("top", rc.y)
				.Float("bitmapWidth", rc.width)
				.Float("bitmapHeight", rc.height)
				.Int("flipH", flipH)
				.Int("flipV", flipV)
				.Int("angle", angle)
				.Int("type", method);

			auto params = builder.GetParameters();

			return ExecuteOpenCLCode("IDR_OPENCL_INTERPOLATION", "InterpolationZone", params, widthOut, heightOut);

		});
}

void COpenCLFilter::BrightnessAndContrast(const double& brightness, const double& contrast, UMat& inputData)
{
	ExecuteSafe([&]
		{
			UMat cvDest;
			convertScaleAbs(inputData, cvDest, contrast / 100.0f, brightness);
			cvDest.copyTo(inputData);
		});
}


void COpenCLFilter::ColorEffect(const wxString& functionName, UMat& inputData)
{
	
	UMat cvDest;
	ExecuteSafe([&]
		{
			if (functionName == "Sepia")
			{
				Mat kernel =
					(cv::Mat_<float>(3, 3)
						<<
						0.272, 0.534, 0.131,
						0.349, 0.686, 0.168,
						0.393, 0.769, 0.189);

				cv::transform(inputData, inputData, kernel);
			}
			else if (functionName == "Negatif")
			{
				bitwise_not(inputData, inputData);
			}
			else if (functionName == "NoirEtBlanc")
			{
				cvtColor(inputData, cvDest, COLOR_BGR2GRAY);
				threshold(cvDest, cvDest, 127, 255, THRESH_BINARY);
				cvtColor(cvDest, inputData, COLOR_GRAY2BGR);
			}
			else if (functionName == "GrayLevel")
			{
				cvtColor(inputData, cvDest, COLOR_BGR2GRAY);
				cvtColor(cvDest, inputData, COLOR_GRAY2BGR);
			}
		});
}

uint8_t * COpenCLFilter::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
	Mat& inputData)
{
	uint8_t* dataOut = nullptr;

	ExecuteSafe([&]
		{

			if (hq3d == nullptr)
				hq3d = std::make_unique<Chqdn3d>(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
			else if (oldLevelDenoise != LumSpac || inputData.cols != oldwidthDenoise || inputData.rows != oldheightDenoise)
			{
				hq3d.reset(new Chqdn3d(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault));
			}
			dataOut = hq3d->ApplyDenoise3D(inputData.data, inputData.cols, inputData.rows);
		});

	return dataOut;
}

void COpenCLFilter::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
                           UMat& inputData)
{
	
	if (hq3d == nullptr)
		hq3d = std::make_unique<Chqdn3d>(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	else if (oldLevelDenoise != LumSpac || inputData.cols != oldwidthDenoise || inputData.rows != oldheightDenoise)
	{
		hq3d.reset(new Chqdn3d(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault));
	}
	ExecuteSafe([&]
		{
			UMat ycbcr;
			Mat yChannel;
			Mat yChannelOut;

			cvtColor(inputData, ycbcr, COLOR_BGR2YCrCb);

			std::vector<Mat> planes(3);
			split(ycbcr, planes);

			// Extract the Y channel
			//cv::extractChannel(ycbcr, yChannel, 0);

			uint8_t* dataOut = hq3d->ApplyDenoise3D(planes[0].data, inputData.cols, inputData.rows);

			memcpy(planes[0].data, dataOut, inputData.cols * inputData.rows);

			// Merge the the color planes back into an Lab image
			//cv::insertChannel(yChannel, ycbcr, 0);
			cv::merge(planes, ycbcr);
			// convert back to RGB
			cvtColor(ycbcr, inputData, COLOR_YCrCb2BGR);

		});
}

void COpenCLFilter::Rotate(const wxString& functionName, const int& widthOut, const int& heightOut, const double& angle,
                           UMat& inputData)
{
	ExecuteSafe([&]
		{
			UMat cvDest;
			// get rotation matrix for rotating the image around its center in pixel coordinates
			const Point2f center((inputData.cols - 1) / 2.0, (inputData.rows - 1) / 2.0);
			Mat rot = getRotationMatrix2D(center, angle, 1.0);
			// determine bounding rectangle, center not relevant
			Rect2f bbox = RotatedRect(Point2f(), inputData.size(), angle).boundingRect2f();
			// adjust transformation matrix
			rot.at<double>(0, 2) += bbox.width / 2.0 - inputData.cols / 2.0;
			rot.at<double>(1, 2) += bbox.height / 2.0 - inputData.rows / 2.0;

			warpAffine(inputData, cvDest, rot, bbox.size());
			cvDest.copyTo(inputData);
		});
}

Rect COpenCLFilter::CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle,
	float ratioX, float ratioY, int x, int y, float left, float top)
{
	// Calcul initial des positions
	float posX = static_cast<float>(x) * ratioX + left * ratioX;
	float posY = static_cast<float>(y) * ratioY + top * ratioY;

	// Gestion des rotations
	switch (angle)
	{
	case 90:
		std::swap(posX, posY);
		posX = widthIn - posX - 1;
		break;
	case 180:
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
		break;
	case 270:
		std::swap(posX, posY);
		posY = heightIn - posY - 1;
		break;
	}

	// Gestion des flips
	if (flipH == 1)
	{
		posX = widthIn - posX - 1;
	}
	if (flipV == 1)
	{
		posY = heightIn - posY - 1;
	}

	// Création du rectangle avec des coordonnées absolues
	return Rect(abs(posX), abs(posY), 0, 0);
}

void COpenCLFilter::ExecuteOpenCLCode(const wxString& programName, const wxString& functionName,
	vector<COpenCLParameter*>& vecParam, const int& width, const int& height,
	cl_mem& outBuffer)
{
	ParameterReleaseGuard releaseGuard(vecParam);

	ExecuteSafe([&]
		{
			//ocl::Context context = openCLContext->GetExecutionContext().getContext();
			ocl::Program program = openCLContext->GetProgram(programName);

			ocl::Kernel kernel(functionName, program);

			// Définition du premier argument (outBuffer)
			cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &outBuffer);
			if (err != CL_SUCCESS)
			{
				throw std::runtime_error("Failed to set kernel argument for outBuffer.");
			}

			// Ajout des autres arguments
			int numArg = 1;
			for (COpenCLParameter* parameter : vecParam)
			{
				parameter->Add(static_cast<cl_kernel>(kernel.ptr()), numArg++);
			}

			// Configuration et exécution du kernel
			size_t global_work_size[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };
			bool success = kernel.run(2, global_work_size, nullptr, true);

			if (!success)
			{
				throw std::runtime_error("Failed to execute OpenCL kernel.");
			}
		});
}

UMat COpenCLFilter::ExecuteOpenCLCode(const wxString& programName, const wxString& functionName,
	vector<COpenCLParameter*>& vecParam, const int& width, const int& height)
{
	// Une seule recherche dans la map grâce à l'insertion automatique si absent
	auto& memInfoPtr = openclMemTempMap[functionName];
	if (!memInfoPtr)
	{
		memInfoPtr = std::make_unique<OpenCLMemoryTemp>();
	}

	OpenCLMemoryTemp* memInfo = memInfoPtr.get();

	// Crée ou redimensionne le UMat uniquement si nécessaire
	if (memInfo->openclMem.empty() ||
		memInfo->openclMem.cols != width ||
		memInfo->openclMem.rows != height)
	{
		memInfo->openclMem.create(height, width, CV_8UC4);
		memInfo->cl_image = static_cast<cl_mem>(memInfo->openclMem.handle(ACCESS_WRITE));
	}

	// Récupère le handle si perdu
	if (memInfo->cl_image == nullptr)
	{
		memInfo->cl_image = static_cast<cl_mem>(memInfo->openclMem.handle(ACCESS_WRITE));
	}

	// Exécute le code OpenCL
	ExecuteOpenCLCode(programName, functionName, vecParam, width, height, memInfo->cl_image);

	return memInfo->openclMem;
}

UMat COpenCLFilter::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
	UMat& inputData, int flipH, int flipV, int angle, int ratio, bool bgraOutput)
{
	cv::UMat cvDestBgra;
    bool _useSuperResolution = false;
    //cout << "COpenCLFilter::Interpolation : " << method << endl;
    CRegardsConfigParam* regardsParam = CParamInit::getInstance();
    int superDnn = regardsParam->GetSuperResolutionType();
    int useSuperResolution = regardsParam->GetUseSuperResolution();
    if (useSuperResolution && superSampling->TestIfMethodIsValid(superDnn, (ratio / 100)) && !isDnnUsed)
        _useSuperResolution = true;

	
	//UMat cvImage;
	//inputData.copyTo(cvImage);

	try
	{
		float ratioX = static_cast<float>(inputData.cols) / rc.width;
		float ratioY = static_cast<float>(inputData.rows) / rc.height;
		if (angle == 90 || angle == 270)
		{
			ratioX = static_cast<float>(inputData.cols) / static_cast<float>(rc.height);
			ratioY = static_cast<float>(inputData.rows) / static_cast<float>(rc.width);
		}

		Rect rectGlobal;
		Rect rect_begin = CalculRect(inputData.cols, inputData.rows, widthOut, heightOut, flipH, flipV, angle, ratioX,
		                             ratioY, 0, 0, rc.x, rc.y);
		Rect rect_end = CalculRect(inputData.cols, inputData.rows, widthOut, heightOut, flipH, flipV, angle, ratioX,
		                           ratioY, widthOut, heightOut, rc.x, rc.y);
		rectGlobal.x = rect_begin.x;
		rectGlobal.y = rect_begin.y;
		rectGlobal.width = rect_end.x;
		rectGlobal.height = rect_end.y;
		if (rectGlobal.x > rectGlobal.width)
		{
			int x_end = rectGlobal.x;
			int x = rectGlobal.width;
			rectGlobal.x = x;
			rectGlobal.width = x_end - x;
		}
		else
		{
			rectGlobal.width -= rectGlobal.x;
		}

		if (rectGlobal.y > rectGlobal.height)
		{
			int y_end = rectGlobal.y;
			int y = rectGlobal.height;
			rectGlobal.y = y;
			rectGlobal.height = y_end - y;
		}
		else
		{
			rectGlobal.height -= rectGlobal.y;
		}

		if ((rectGlobal.height + rectGlobal.y) > inputData.rows)
		{
			rectGlobal.height = inputData.rows - rectGlobal.y;
		}
		if ((rectGlobal.width + rectGlobal.x) > inputData.cols)
		{
			rectGlobal.width = inputData.cols - rectGlobal.x;
		}

		//cv::UMat crop;
		inputData(rectGlobal).copyTo(cvDestBgra);
		//Mat global;
		//cvImage.copyTo(global);
		//crop.copyTo(cvImage);
		//cvImage = cvImage(rectGlobal);

		if (angle == 270)
		{
			if (flipV && flipH)
				cv::rotate(cvDestBgra, cvDestBgra, ROTATE_90_CLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvDestBgra, cvDestBgra, ROTATE_90_COUNTERCLOCKWISE);
			else
				cv::rotate(cvDestBgra, cvDestBgra, ROTATE_90_CLOCKWISE);
		}
		else if (angle == 90)
		{
			if (flipV && flipH)
				cv::rotate(cvDestBgra, cvDestBgra, ROTATE_90_COUNTERCLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvDestBgra, cvDestBgra, ROTATE_90_CLOCKWISE);
			else
				cv::rotate(cvDestBgra, cvDestBgra, ROTATE_90_COUNTERCLOCKWISE);
		}
		else if (angle == 180)
		{
			cv::rotate(cvDestBgra, cvDestBgra, ROTATE_180);
		}


		/*
		nearest neighbor interpolation
		INTER_NEAREST = 0,
		bilinear interpolation
		INTER_LINEAR = 1,
		bicubic interpolation
		INTER_CUBIC = 2,
		resampling using pixel area relation. It may be a preferred method for image decimation, as
		it gives moire'-free results. But when the image is zoomed, it is similar to the INTER_NEAREST
		method.
		INTER_AREA = 3,
		Lanczos interpolation over 8x8 neighborhood
		INTER_LANCZOS4 = 4,
		Bit exact bilinear interpolation
		INTER_LINEAR_EXACT = 5,
		Bit exact nearest neighbor interpolation. This will produce same results as
		the nearest neighbor method in PIL, scikit-image or Matlab.
		INTER_NEAREST_EXACT = 6,
		*/
		if (_useSuperResolution)
		{
			cvDestBgra = superSampling->upscaleImage(cvDestBgra, superDnn, (ratio / 100));
		}
		else if (method == 7) //AVIR INTERPOLATION NOT SUPPORTED BY OPENCL
		{
#ifdef _DEBUG
			using std::chrono::high_resolution_clock;
			using std::chrono::duration_cast;
			using std::chrono::duration;
			using std::chrono::milliseconds;

			auto t1 = high_resolution_clock::now();
#endif	

			if(!resizer->IsInit())
				resizer->Init();

			bool result = resizer->IsOk();
			if (result)
			{
				SAvirResizeParams params;
				params.linearizeGamma = true;      // correct pour une image sRGB classique
				params.sharpen = true;             // si tu agrandis et veux un rendu plus net
				params.sharpenAmount = 0.35f;
				params.dither = false;             // true seulement si tu redescends ensuite en 8 bits
				params.peakValue = 1.0f;           // buffer normalise 0..1

				cv::UMat dstRGBA;
				dstRGBA.create(heightOut, widthOut, CV_8UC4);
				

				if (cvDestBgra.channels() == 3)
				{
					cv::UMat bitmapMatrix;

					cvtColor(cvDestBgra, bitmapMatrix, cv::COLOR_BGR2BGRA);

					result = resizer->Resize(
						bitmapMatrix,
						dstRGBA,
						params);
				}
				else
				{
					result = resizer->Resize(
						cvDestBgra,
						dstRGBA,
						params);
				}

				if(result)
					dstRGBA.copyTo(cvDestBgra);

			}


			if (!result)
				resize(cvDestBgra, cvDestBgra, Size(widthOut, heightOut), method - 1);

#ifdef _DEBUG
			auto t2 = high_resolution_clock::now();

			/* Getting number of milliseconds as an integer. */
			auto ms_int = duration_cast<milliseconds>(t2 - t1);

			/* Getting number of milliseconds as a double. */
			duration<double, std::milli> ms_double = t2 - t1;


#ifdef WIN32
			OutputDebugString(L"Time taken by COpenCLFilter::Interpolation is : ");
			OutputDebugString(to_wstring(ms_int.count()).c_str());
			OutputDebugString(L" ms \n");
#else
			std::cout << "Time taken by COpenCLFilter::Interpolation is : " << ms_int.count() << "ms\n";
#endif
#endif

		}
		else if (method > 7)
		{
#ifdef _DEBUG
			using std::chrono::high_resolution_clock;
			using std::chrono::duration_cast;
			using std::chrono::duration;
			using std::chrono::milliseconds;

			auto t1 = high_resolution_clock::now();
#endif	
			// Appelle une autre version d'Interpolation pour les méthodes avancées
			int localMethod = method - 7;
			cvDestBgra = Interpolation(widthOut, heightOut, cvDestBgra, cvDestBgra.cols, cvDestBgra.rows, localMethod, bgraOutput);

#ifdef _DEBUG
			auto t2 = high_resolution_clock::now();

			/* Getting number of milliseconds as an integer. */
			auto ms_int = duration_cast<milliseconds>(t2 - t1);

			/* Getting number of milliseconds as a double. */
			duration<double, std::milli> ms_double = t2 - t1;


#ifdef WIN32
			OutputDebugString(L"Time taken by COpenCLFilter::Interpolation is : ");
			OutputDebugString(to_wstring(ms_int.count()).c_str());
			OutputDebugString(L" ms \n");
#else
			std::cout << "Time taken by COpenCLFilter::Interpolation is : " << ms_int.count() << "ms\n";
#endif
#endif
		}
		else if (cvDestBgra.cols != widthOut || cvDestBgra.rows != heightOut)
		{
			resize(cvDestBgra, cvDestBgra, Size(widthOut, heightOut), method);
		}

		if (cvDestBgra.cols != widthOut || cvDestBgra.rows != heightOut)
			resize(cvDestBgra, cvDestBgra, Size(widthOut, heightOut), method);

		//Apply Transformation

		if (flipH)
		{
			if (angle == 90 || angle == 270)
				flip(cvDestBgra, cvDestBgra, 0);
			else
				flip(cvDestBgra, cvDestBgra, 1);
		}
		if (flipV)
		{
			if (angle == 90 || angle == 270)
				flip(cvDestBgra, cvDestBgra, 1);
			else
				flip(cvDestBgra, cvDestBgra, 0);
		}
		//
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "COpenCLFilter::Interpolation exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
        std::cout << "width : " << widthOut << "height : " <<  heightOut << std::endl;
        cv::Mat image(widthOut, heightOut, CV_8UC3, cv::Scalar(0, 0, 0));
        image.copyTo(cvDestBgra);
	}


	//if (bgraOutput && cvDestBgra.channels() == 3)
	//	cvtColor(cvDestBgra, cvDestBgra, COLOR_BGR2BGRA);
	if (cvDestBgra.channels() == 4)
		cvtColor(cvDestBgra, cvDestBgra, COLOR_BGRA2BGR);

	return cvDestBgra;
    
}