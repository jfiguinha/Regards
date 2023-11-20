#include <header.h>
#include "OpenCLFilter.h"

#include "utility.h"
#include <opencv2/xphoto.hpp>
#include <opencv2/dnn_superres.hpp>
#include <FileUtility.h>
#include <mutex>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <opencv2/core/ocl.hpp>
#include <LibResource.h>
#if defined __APPLE__
#include <OpenCL/cl_gl.h>
#else
#endif

using namespace Regards::OpenCL;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;

#define EDSR 0
#define ESPCN 1
#define FSRCNN 2
#define LapSRN 3

bool isUsed = false;
std::mutex muDnnSuperResImpl;
int numTexture = -1;


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
	wxString path = "";
#ifdef WIN32
	path = CFileUtility::GetResourcesFolderPath() + "\\model\\" + modelName + "_x" + to_string(scale) + ".pb";
#else
	path = CFileUtility::GetResourcesFolderPath() + "/model/" + modelName + "_x" + to_string(scale) + ".pb";
#endif

	return CConvertUtility::ConvertToStdString(path);
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
	isUsed = true;
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
	isUsed = false;
	return outputImage;
}


COpenCLFilter::COpenCLFilter()
{
	bool useMemory = (ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	hq3d = nullptr;
	superSampling = new CSuperSampling();
}

COpenCLFilter::~COpenCLFilter()
{
	if (hq3d != nullptr)
		delete hq3d;
	delete superSampling;
}


void COpenCLFilter::BilateralEffect(UMat& inputData, const int& fSize, const int& sigmaX, const int& sigmaP)
{
	try
	{
		UMat dest;
		bilateralFilter(inputData, dest, fSize, sigmaX, sigmaP, BORDER_DEFAULT);
		dest.copyTo(inputData);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "BilateralEffect exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::NlMeans(UMat& inputData, const int& h, const int& hColor, const int& templateWindowSize,
                            const int& searchWindowSize)
{
	try
	{
		UMat ycbcr;
		UMat yChannel;
		UMat yChannelOut;

		cvtColor(inputData, ycbcr, COLOR_BGR2YCrCb);

		// Extract the Y channel
		extractChannel(ycbcr, yChannel, 0);
		extractChannel(ycbcr, yChannel, 0);

		fastNlMeansDenoising(yChannel, yChannelOut, h, templateWindowSize, searchWindowSize);

		// Merge the the color planes back into an Lab image
		insertChannel(yChannelOut, ycbcr, 0);

		// convert back to RGB
		cvtColor(ycbcr, inputData, COLOR_YCrCb2BGR);

		// Temporary Mat not reused, so release from memory.
		yChannel.release();
		ycbcr.release();
		yChannelOut.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "NlMeans exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::Bm3d(UMat& inputData, const float& fSigma)
{
	try
	{
		UMat ycbcr;
		UMat yChannel;
		UMat yChannelOut;

		cvtColor(inputData, ycbcr, COLOR_BGR2YCrCb);

		// Extract the Y channel
		extractChannel(ycbcr, yChannel, 0);

		xphoto::bm3dDenoising(yChannel, yChannelOut, fSigma);

		// Merge the the color planes back into an Lab image
		insertChannel(yChannelOut, ycbcr, 0);

		// convert back to RGB
		cvtColor(ycbcr, inputData, COLOR_YCrCb2BGR);

		// Temporary Mat not reused, so release from memory.
		yChannel.release();
		ycbcr.release();
		yChannelOut.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "Bm3d exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::BrightnessAndContrastAuto(UMat& inputData, float clipHistPercent)
{
	try
	{
		int histSize = 256;
		float alpha, beta;
		double minGray = 0, maxGray = 0;

		UMat gray;
		cvtColor(inputData, gray, COLOR_BGR2GRAY);

		if (clipHistPercent == 0)
		{
			// keep full available range
			minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			UMat h;

			std::vector<int> channels = {0}; // Analyze only the channel 0.
			std::vector<int> hsize = {256};
			//Quantize the intensities in the image using 256 levels even if all the levels are not present.
			std::vector<float> hranges = {0, 256}; // The range is between 0 - 255 (so less than 256).

			calcHist(std::vector<UMat>(1, gray), channels, noArray(), h, hsize, hranges);

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
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "BrightnessAndContrastAuto exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void COpenCLFilter::Fusion(UMat& inputData, const UMat& secondPictureData, const float& pourcentage)
{
	try
	{
		UMat dst;
		float beta = (1.0 - pourcentage);
		addWeighted(inputData, pourcentage, secondPictureData, beta, 0.0, dst);
		dst.copyTo(inputData);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "Fusion exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::SharpenMasking(const float& sharpness, UMat& inputData)
{
	try
	{
		UMat cvDestBgra;
		double sigma = 1;
		cv::GaussianBlur(inputData, cvDestBgra, Size(), sigma, sigma);

		UMat dest;
		UMat inputDataBgra;
		cvtColor(cvDestBgra, cvDestBgra, COLOR_BGR2BGRA);
		cvtColor(inputData, inputDataBgra, COLOR_BGR2BGRA);

		vector<COpenCLParameter*> vecParam;
		auto clBuffer = static_cast<cl_mem>(inputDataBgra.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto clBuffer_out = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
		auto gauss = new COpenCLParameterClMem(true);
		gauss->SetValue(clBuffer_out);
		gauss->SetLibelle("gaussian");
		gauss->SetNoDelete(true);
		vecParam.push_back(gauss);


		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.size().width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.size().height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		auto paramThreshold = new COpenCLParameterFloat();
		paramThreshold->SetLibelle("sharpness");
		paramThreshold->SetValue(sharpness);
		vecParam.push_back(paramThreshold);

		dest = ExecuteOpenCLCode("IDR_OPENCL_SHARPENMASKING", "SharpenMasking", vecParam, inputData.size().width,
		                         inputData.size().height);

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

		cvtColor(dest, inputData, COLOR_BGRA2BGR);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "SharpenMasking exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::PhotoFiltre(const CRgbaquad& clValue, const int& intensity, UMat& inputData)
{
	try
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
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "PhotoFiltre exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::RGBFilter(const int& red, const int& green, const int& blue, UMat& inputData)
{
	try
	{
		UMat out;
		auto color = Scalar(blue, green, red);
		add(inputData, color, out);
		out.copyTo(inputData);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "RGBFilter exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::FiltreMosaic(UMat& inputData, const int& size)
{
	try
	{
		UMat dest;
		UMat cvDestBgra;
		cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

		vector<COpenCLParameter*> vecParam;
		auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		auto paramAngle = new COpenCLParameterFloat();
		paramAngle->SetLibelle("fTileSize");
		paramAngle->SetValue(size);
		vecParam.push_back(paramAngle);

		dest = ExecuteOpenCLCode("IDR_OPENCL_MOSAIC", "Mosaic", vecParam, inputData.size().width,
		                         inputData.size().height);

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

		cvtColor(dest, inputData, COLOR_BGRA2BGR);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "FiltreMosaic exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Blur(const int& radius, UMat& inputData)
{
	try
	{
		blur(inputData, inputData, Size(radius, radius));
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "Blur exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::GaussianBlur(const int& radius, const int& boxSize, UMat& inputData)
{
	try
	{
		cv::GaussianBlur(inputData, inputData, Size(boxSize, boxSize), radius);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "GaussianBlur exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::MotionBlurCompute(const vector<double>& kernelMotion, const vector<wxPoint>& offsets,
                                      const int& size,
                                      UMat& inputData)
{
	UMat dest;
	UMat cvDestBgra;
	cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);

	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(inputData.cols);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(inputData.rows);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);

	auto kernel = new float[kernelMotion.size()];
	for (auto i = 0; i < kernelMotion.size(); i++)
		kernel[i] = kernelMotion[i];

	auto paramkernelMotion = new COpenCLParameterFloatArray();
	paramkernelMotion->SetLibelle("kernelMotion");
	paramkernelMotion->SetValue(static_cast<cl_context>(ocl::Context::getDefault(false).ptr()), kernel, size, flag);
	vecParam.push_back(paramkernelMotion);

	auto offsetsMotion = new int[kernelMotion.size() * 2];
	for (auto i = 0, j = 0; i < offsets.size(); i++, j += 2)
	{
		offsetsMotion[j] = offsets[i].x;
		offsetsMotion[j + 1] = offsets[i].y;
	}

	auto paramoffsets = new COpenCLParameterIntArray();
	paramoffsets->SetLibelle("offsets");
	paramoffsets->SetValue(static_cast<cl_context>(ocl::Context::getDefault(false).ptr()), offsetsMotion, size * 2,
	                       flag);
	vecParam.push_back(paramoffsets);


	auto paramkernelSize = new COpenCLParameterInt();
	paramkernelSize->SetLibelle("kernelSize");
	paramkernelSize->SetValue(size);
	vecParam.push_back(paramkernelSize);

	dest = ExecuteOpenCLCode("IDR_OPENCL_MOTIONBLUR", "MotionBlur", vecParam, inputData.size().width,
	                         inputData.size().height);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}


	delete[] kernel;
	delete[] offsetsMotion;

	cvtColor(dest, inputData, COLOR_BGRA2BGR);
}

void COpenCLFilter::Emboss(UMat& inputData)
{
	try
	{
		// Construct kernel (all entries initialized to 0)
		Mat kernel(3, 3, CV_32F, Scalar(0));
		kernel.at<float>(0, 0) = -1.0;
		kernel.at<float>(2, 2) = 1.0;
		UMat dest;
		filter2D(inputData, dest, inputData.depth(), kernel);
		dest.copyTo(inputData);
		dest.release();
		kernel.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "Emboss exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Sharpen(UMat& inputData)
{
	try
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
		dest.release();
		kernel.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "Sharpen exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::SharpenStrong(UMat& inputData)
{
	try
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
		dest.release();
		kernel.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "SharpenStrong exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Edge(UMat& inputData)
{
	try
	{
		UMat dest;
		cvtColor(inputData, dest, COLOR_BGR2GRAY);

		Mat img_blur;
		cv::GaussianBlur(dest, img_blur, Size(3, 3), 0, 0);
		UMat edges;
		Canny(img_blur, edges, 100, 200, 3, false);

		cvtColor(edges, inputData, COLOR_GRAY2BGR);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "Edge exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::FiltreConvolution(const wxString& programName, const wxString& functionName, UMat& inputData)
{
	UMat dest;
	UMat cvDestBgra;
	cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);

	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(cvDestBgra.cols);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(cvDestBgra.rows);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);

	dest = ExecuteOpenCLCode(programName, functionName, vecParam, cvDestBgra.size().width, cvDestBgra.size().height);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	cvtColor(dest, inputData, COLOR_BGRA2BGR);
}

void COpenCLFilter::ErodeDilate(const wxString& functionName, UMat& inputData)
{
	try
	{
		if (functionName == "Erode")
			erode(inputData, inputData, Mat());
		else if (functionName == "Dilate")
			dilate(inputData, inputData, Mat());
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "ErodeDilate exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Posterize(const float& level, const float& gamma, UMat& inputData)
{
	UMat dest;
	UMat cvDestBgra;
	cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);

	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(cvDestBgra.size().width);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(cvDestBgra.size().height);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);

	auto paramLevel = new COpenCLParameterInt();
	paramLevel->SetLibelle("level");
	paramLevel->SetValue(level);
	vecParam.push_back(paramLevel);

	dest = ExecuteOpenCLCode("IDR_OPENCL_COLOR", "Posterisation", vecParam, inputData.size().width,
	                         inputData.size().height);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	cvtColor(dest, inputData, COLOR_BGRA2BGR);
}


void COpenCLFilter::LensDistortion(const float& strength, UMat& inputData)
{
	UMat dest;
	UMat cvDestBgra;
	cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

	double _strength = static_cast<double>(strength) / 100;
	double correctionRadius = sqrt(pow(inputData.rows, 2) + pow(inputData.cols, 2)) / _strength;

	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);

	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(cvDestBgra.size().width);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(cvDestBgra.size().height);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);


	auto paramLevel = new COpenCLParameterFloat();
	paramLevel->SetLibelle("correctionRadius");
	paramLevel->SetValue(correctionRadius);
	vecParam.push_back(paramLevel);

	dest = ExecuteOpenCLCode("IDR_OPENCL_DISTORTION", "Distortion", vecParam, inputData.size().width,
	                         inputData.size().height);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	cvtColor(dest, inputData, COLOR_BGRA2BGR);
}

int COpenCLFilter::GetRgbaBitmap(cl_mem cl_image, UMat& u)
{
	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(u.handle(ACCESS_READ));

	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);

	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(u.size().width);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(u.size().height);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);

#ifdef __APPLE__
	ExecuteOpenCLCode("IDR_OPENCL_BITMAPCONVERSION", "BitmapToOpenGLTextureApple", vecParam, u.size().width, u.size().height, cl_image);// program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTextureApple");
#else
	ExecuteOpenCLCode("IDR_OPENCL_BITMAPCONVERSION", "BitmapToOpenGLTexture", vecParam, u.size().width, u.size().height,
	                  cl_image); //program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");
#endif


	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	return 0;
}


void COpenCLFilter::Solarize(const long& threshold, UMat& inputData)
{
	UMat dest;
	UMat cvDest;
	UMat cvDestBgra;
	cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);


	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(cvDestBgra.size().width);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(cvDestBgra.size().height);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);

	auto paramThreshold = new COpenCLParameterInt();
	paramThreshold->SetLibelle("threshold");
	paramThreshold->SetValue(static_cast<int>(threshold));
	vecParam.push_back(paramThreshold);

	dest = ExecuteOpenCLCode("IDR_OPENCL_COLOR", "Solarization", vecParam, inputData.size().width,
	                         inputData.size().height);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	cvtColor(dest, inputData, COLOR_BGRA2BGR);
}

void COpenCLFilter::Median(UMat& inputData)
{
	try
	{
		medianBlur(inputData, inputData, 3);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "Median exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Noise(UMat& inputData)
{
	UMat dest;
	UMat cvDestBgra;
	cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));
	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);

	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(inputData.cols);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(inputData.rows);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);

	dest = ExecuteOpenCLCode("IDR_OPENCL_NOISE", "Noise", vecParam, inputData.size().width, inputData.size().height);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	cvtColor(dest, inputData, COLOR_BGRA2BGR);
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
	UMat dest;
	UMat cvDest;
	UMat cvDestBgra;
	cvtColor(inputData, cvDestBgra, COLOR_BGR2BGRA);

	vector<COpenCLParameter*> vecParam;
	auto clBuffer = static_cast<cl_mem>(cvDestBgra.handle(ACCESS_READ));

	auto input = new COpenCLParameterClMem(true);
	input->SetValue(clBuffer);
	input->SetLibelle("input");
	input->SetNoDelete(true);
	vecParam.push_back(input);

	auto paramWidth = new COpenCLParameterInt();
	paramWidth->SetValue(inputData.cols);
	paramWidth->SetLibelle("width");
	vecParam.push_back(paramWidth);

	auto paramHeight = new COpenCLParameterInt();
	paramHeight->SetValue(inputData.rows);
	paramHeight->SetLibelle("height");
	vecParam.push_back(paramHeight);

	auto paramRadius = new COpenCLParameterFloat();
	paramRadius->SetLibelle("radius");
	paramRadius->SetValue(radius);
	vecParam.push_back(paramRadius);

	auto paramAngle = new COpenCLParameterFloat();
	paramAngle->SetLibelle("angle");
	paramAngle->SetValue(angle);
	vecParam.push_back(paramAngle);


	dest = ExecuteOpenCLCode("IDR_OPENCL_SWIRL", "Swirl", vecParam, inputData.size().width, inputData.size().height);

	for (COpenCLParameter* parameter : vecParam)
	{
		if (!parameter->GetNoDelete())
		{
			delete parameter;
			parameter = nullptr;
		}
	}

	cvtColor(dest, inputData, COLOR_BGRA2BGR);
}

void COpenCLFilter::BrightnessAndContrast(const double& brightness, const double& contrast, UMat& inputData)
{
	UMat cvDest;
	try
	{
		convertScaleAbs(inputData, cvDest, contrast / 100.0f, brightness);
		cvDest.copyTo(inputData);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::ColorEffect(const wxString& functionName, UMat& inputData)
{
	UMat cvDest;
	try
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
		cvDest.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

uint8_t * COpenCLFilter::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
	Mat& inputData)
{
	uint8_t* dataOut = nullptr;
	if (hq3d == nullptr)
		hq3d = new Chqdn3d(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	else if (oldLevelDenoise != LumSpac || inputData.cols != oldwidthDenoise || inputData.rows != oldheightDenoise)
	{
		delete hq3d;
		hq3d = new Chqdn3d(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	}
	try
	{
		dataOut = hq3d->ApplyDenoise3D(inputData.data, inputData.cols, inputData.rows);
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
	return dataOut;
}

void COpenCLFilter::HQDn3D(const double& LumSpac, const double& temporalLumaDefault, const double& temporalSpatialLumaDefault,
                           UMat& inputData)
{
	if (hq3d == nullptr)
		hq3d = new Chqdn3d(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	else if (oldLevelDenoise != LumSpac || inputData.cols != oldwidthDenoise || inputData.rows != oldheightDenoise)
	{
		delete hq3d;
		hq3d = new Chqdn3d(inputData.cols, inputData.rows, LumSpac, temporalLumaDefault, temporalSpatialLumaDefault);
	}
	try
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

		ycbcr.release();
		yChannel.release();
		yChannelOut.release();
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Rotate(const wxString& functionName, const int& widthOut, const int& heightOut, const double& angle,
                           UMat& inputData)
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

	rot.release();
	inputData.release();
	cvDest.copyTo(inputData);
	cvDest.release();
}

Rect COpenCLFilter::CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle,
                               float ratioX, float ratioY, int x, int y, float left, float top)
{
	Rect rect;
	float posX = static_cast<float>(x) * ratioX + left * ratioX;
	float posY = static_cast<float>(y) * ratioY + top * ratioY;

	if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posY = heightIn - posY - 1;
		//
	}
	else if (angle == 180)
	{
		posX = widthIn - posX - 1;
		posY = heightIn - posY - 1;
	}
	else if (angle == 90)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
	}

	if (angle == 90 || angle == 270)
	{
		if (flipV == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipH == 1)
		{
			posY = heightIn - posY - 1;
		}
	}
	else
	{
		if (flipH == 1)
		{
			posX = widthIn - posX - 1;
		}

		if (flipV == 1)
		{
			posY = heightIn - posY - 1;
		}
	}

	rect.x = abs(posX);
	rect.y = abs(posY);
	return rect;
}

void COpenCLFilter::ExecuteOpenCLCode(const wxString& programName, const wxString& functionName,
                                      vector<COpenCLParameter*>& vecParam, const int& width, const int& height,
                                      cl_mem& outBuffer)
{
	wxString kernelSource = CLibResource::GetOpenCLUcharProgram(programName);
	ocl::ProgramSource programSource(kernelSource);
	ocl::Context context = ocl::Context::getDefault(false);

	// Compile the kernel code
	String errmsg;
	String buildopt = ""; // cv::format("-D dstT=%s", cv::ocl::typeToStr(paramSrc.depth()));
	ocl::Program program = context.getProg(programSource, buildopt, errmsg);

	ocl::Kernel kernel(functionName, program);

	//cl_mem clBuffer = (cl_mem)paramSrc.handle(cv::ACCESS_WRITE);

	cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &outBuffer);
	if (err > 0)
		throw "OpenCL Kernel Error";

	int numArg = 1;
	for (auto it = vecParam.begin(); it != vecParam.end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		parameter->Add(static_cast<cl_kernel>(kernel.ptr()), numArg++);
	}

	size_t global_work_size[2] = {static_cast<size_t>(width), static_cast<size_t>(height)};
	//size_t localThreads[2] = { 16, 16 };
	bool success = kernel.run(2, global_work_size, nullptr, true);
	if (!success)
	{
		cout << "Failed running the kernel..." << endl;
		if (err > 0)
			throw "OpenCL Kernel run";
		return;
	}

	for (auto it = vecParam.begin(); it != vecParam.end(); ++it)
	{
		COpenCLParameter* parameter = *it;
		if (!parameter->GetNoDelete())
			parameter->Release();
	}
}

UMat COpenCLFilter::ExecuteOpenCLCode(const wxString& programName, const wxString& functionName,
                                      vector<COpenCLParameter*>& vecParam, const int& width, const int& height)
{
	UMat paramSrc;


	int depth = CV_8U;
	int type = CV_MAKE_TYPE(depth, 4);
	paramSrc.create(height, width, type);

	auto clBuffer = static_cast<cl_mem>(paramSrc.handle(ACCESS_WRITE));

	ExecuteOpenCLCode(programName, functionName, vecParam, width, height, clBuffer);

	return paramSrc;
}


UMat COpenCLFilter::Interpolation(const int& widthOut, const int& heightOut, const wxRect& rc, const int& method,
                                  UMat& inputData, int flipH, int flipV, int angle, int ratio)
{
	UMat cvImage;
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
		inputData(rectGlobal).copyTo(cvImage);
		//Mat global;
		//cvImage.copyTo(global);
		//crop.copyTo(cvImage);
		//cvImage = cvImage(rectGlobal);

		if (angle == 270)
		{
			if (flipV && flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_CLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_COUNTERCLOCKWISE);
			else
				cv::rotate(cvImage, cvImage, ROTATE_90_CLOCKWISE);
		}
		else if (angle == 90)
		{
			if (flipV && flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_COUNTERCLOCKWISE);
			else if (flipV || flipH)
				cv::rotate(cvImage, cvImage, ROTATE_90_CLOCKWISE);
			else
				cv::rotate(cvImage, cvImage, ROTATE_90_COUNTERCLOCKWISE);
		}
		else if (angle == 180)
		{
			cv::rotate(cvImage, cvImage, ROTATE_180);
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
		CRegardsConfigParam* regardsParam = CParamInit::getInstance();
		int superDnn = regardsParam->GetSuperResolutionType();
		int useSuperResolution = regardsParam->GetUseSuperResolution();
		if (useSuperResolution && superSampling->TestIfMethodIsValid(superDnn, (ratio / 100)) && !isUsed)
		{
			cvImage = superSampling->upscaleImage(cvImage, superDnn, (ratio / 100));
		}
		else if (cvImage.cols != widthOut || cvImage.rows != heightOut)
		{
			resize(cvImage, cvImage, Size(widthOut, heightOut), method);
		}
		/*
		else
		{
			cv::resize(cvImage, cvImage, cv::Size(widthOut+2, heightOut+2), method);
		}
		*/

		if (cvImage.cols != widthOut || cvImage.rows != heightOut)
			resize(cvImage, cvImage, Size(widthOut, heightOut), method);

		//Apply Transformation

		if (flipH)
		{
			if (angle == 90 || angle == 270)
				flip(cvImage, cvImage, 0);
			else
				flip(cvImage, cvImage, 1);
		}
		if (flipV)
		{
			if (angle == 90 || angle == 270)
				flip(cvImage, cvImage, 1);
			else
				flip(cvImage, cvImage, 0);
		}
		//
	}
	catch (Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "COpenCLFilter::Interpolation exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
        cv::Mat image(widthOut, heightOut, CV_8UC3, cv::Scalar(0, 0, 0));
        image.copyTo(cvImage);
	}


	return cvImage;
}
