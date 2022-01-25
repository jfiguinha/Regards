#include <header.h>
#include "OpenCLFilter.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "RegardsBitmap.h"
#include "utility.h"
#include <CvPlot/cvplot.h>
#include <opencv2/xphoto.hpp>
#include <opencv2/dnn_superres.hpp>
#include <FileUtility.h>
#include <mutex>
using namespace Regards::OpenCL;
using namespace cv;
using namespace dnn;
using namespace dnn_superres;

#define EDSR 7
#define ESPCN 8
#define FSRCNN 9
#define LapSRN 10

bool isUsed = false;
std::mutex muDnnSuperResImpl;

string GenerateModelPath(string modelName, int scale)
{
	wxString path = "";
#ifdef WIN32
	path = CFileUtility::GetResourcesFolderPath() + "\\model\\" + modelName + "_x" + to_string(scale) + ".pb";
#else
	path = CFileUtility::GetResourcesFolderPath() + "/model/pose_iter_160000.caffemodel";
#endif

	return path.ToStdString();
}

bool TestIfMethodIsValid(int method, int scale)
{
	if (method == EDSR && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	else if (method == ESPCN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	else if (method == FSRCNN && (scale == 2 || scale == 3 || scale == 4))
	{
		return true;
	}
	else if (method == LapSRN && (scale == 2 || scale == 4 || scale == 8))
	{
		return true;
	}
	return false;
}

cv::UMat COpenCLFilter::upscaleImage(cv::UMat img, int method, int scale)
{
	isUsed = true;
	UMat outputImage;
	try
	{
		//muDnnSuperResImpl.lock();

		DnnSuperResImpl sr;
		

		switch (method)
		{
		case EDSR: 
			{
				string algorithm = "edsr";
				sr.readModel(GenerateModelPath("EDSR",scale));
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
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	isUsed = false;
	return outputImage;
}



COpenCLFilter::COpenCLFilter(COpenCLContext * context)
{
	bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	this->context = context;
	hq3d = nullptr;
}

COpenCLFilter::~COpenCLFilter()
{
	if (hq3d != nullptr)
		delete hq3d;
}


void COpenCLFilter::BilateralEffect(cv::UMat & inputData, const int& fSize, const int& sigmaX, const int& sigmaP)
{
	try
	{
		cv::UMat cvSrc;
		cv::cvtColor(inputData, cvSrc, cv::COLOR_BGRA2BGR);
		cv::bilateralFilter(cvSrc, cvSrc, fSize, sigmaX, sigmaP, cv::BORDER_DEFAULT);
		cv::cvtColor(cvSrc, inputData, cv::COLOR_BGR2BGRA);
		cvSrc.release();

	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

}


void COpenCLFilter::NlMeans(cv::UMat & inputData, const int& h, const int& hColor, const int& templateWindowSize, const int& searchWindowSize)
{
	cv::UMat cvDest;
	try
	{
		//context->GetContextForOpenCV().bind();
		
		cv::UMat cvSrc;
		cv::cvtColor(inputData, cvSrc, cv::COLOR_BGRA2BGR);
		cv::fastNlMeansDenoisingColored(cvSrc, cvDest, h, hColor, templateWindowSize, searchWindowSize);
		cv::cvtColor(cvDest, inputData, cv::COLOR_BGR2BGRA);
		cvSrc.release();

	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::Bm3d(cv::UMat & inputData, const float & fSigma)
{
	try
	{
		cv::UMat cvDest;
		cv::UMat ycbcr;
		cv::UMat yChannel;
		cv::UMat yChannelOut;

		cv::cvtColor(inputData, cvDest, cv::COLOR_BGRA2BGR);
		cvtColor(cvDest, ycbcr, cv::COLOR_BGR2YCrCb);

		// Extract the Y channel
		cv::extractChannel(ycbcr, yChannel, 0);

		cv::xphoto::bm3dDenoising(yChannel, yChannelOut, fSigma);

		// Merge the the color planes back into an Lab image
		cv::insertChannel(yChannelOut, ycbcr, 0);

		// convert back to RGB
		cv::cvtColor(ycbcr, cvDest, cv::COLOR_YCrCb2BGR);
		cv::cvtColor(cvDest, inputData, cv::COLOR_BGR2BGRA);
		// Temporary Mat not reused, so release from memory.
		yChannel.release();
		ycbcr.release();
		yChannelOut.release();
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}



void COpenCLFilter::BrightnessAndContrastAuto(cv::UMat & inputData, float clipHistPercent)
{
	try
	{
		int histSize = 256;
		float alpha, beta;
		double minGray = 0, maxGray = 0;

		cv::UMat gray;
		cvtColor(inputData, gray, cv::COLOR_BGRA2GRAY);

		if (clipHistPercent == 0)
		{
			// keep full available range
			minMaxLoc(gray, &minGray, &maxGray);
		}
		else
		{
			cv::UMat h;

			std::vector<int> channels = { 0 }; // Analyze only the channel 0.
			std::vector<int> hsize = { 256 };
			//Quantize the intensities in the image using 256 levels even if all the levels are not present.
			std::vector<float> hranges = { 0, 256 }; // The range is between 0 - 255 (so less than 256).

			calcHist(std::vector<cv::UMat>(1, gray), channels, cv::noArray(), h, hsize, hranges);

			cv::Mat hist;

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
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void COpenCLFilter::Fusion(cv::UMat& inputData, const cv::UMat &secondPictureData, const float &pourcentage)
{
	try
	{
		cv::UMat dst;
		float beta = (1.0 - pourcentage);
		cv::addWeighted(inputData, pourcentage, secondPictureData, beta, 0.0, dst);
		dst.copyTo(inputData);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::SharpenMasking(const float &sharpness, cv::UMat & inputData)
{
	cv::UMat dest;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_SHARPENMASKING");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
				
		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterFloat * paramIntensity = new COpenCLParameterFloat();
		paramIntensity->SetLibelle("sharpness");
		paramIntensity->SetValue(sharpness);
		vecParam.push_back(paramIntensity);

		try
		{
			
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "SharpenMasking");
			
		}
		catch(...)
		{
			
		}

		delete program;

        for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::PhotoFiltre(const CRgbaquad &clValue, const int &intensity, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cv::UMat dest;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramIntensity = new COpenCLParameterInt();
		paramIntensity->SetLibelle("intensity");
		paramIntensity->SetValue(intensity);
		vecParam.push_back(paramIntensity);

		COLORData color = { clValue.GetRed(), clValue.GetGreen(), clValue.GetBlue(), 0 };

		COpenCLParameterColorData * paramColor = new COpenCLParameterColorData();
		paramColor->SetLibelle("color");
		paramColor->SetValue(context->GetContext(), &color, flag);
		vecParam.push_back(paramColor);
		
		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "PhotoFiltre");
			
		}
		catch(...)
		{
			
		}
		delete program;

	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::RGBFilter(const int &red, const int &green, const int &blue, cv::UMat & inputData)
{
	cv::UMat dest;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COLORData color = { red, green, blue, 0 };

		COpenCLParameterColorData * paramColor = new COpenCLParameterColorData();
		paramColor->SetLibelle("color");
		paramColor->SetValue(context->GetContext(), &color, flag);
		vecParam.push_back(paramColor);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "RGBFiltre");
			
		}
		catch(...)
		{
			
		}
		delete program;

		for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::FiltreMosaic(cv::UMat & inputData)
{
	cv::UMat dest;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_MOSAIC");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterFloat * paramAngle = new COpenCLParameterFloat();
		paramAngle->SetLibelle("fTileSize");
		paramAngle->SetValue(5.0f);
		vecParam.push_back(paramAngle);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Mosaic");
			
		}
		catch(...)
		{
			
		}
		delete program;

		for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::Blur(const int &radius, cv::UMat & inputData)
{
	try
	{
		cv::blur(inputData, inputData, cv::Size(radius, radius));
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::BoxBlur(const int &coeff, const wxString &functionName, cv::UMat & inputData, bool noDeleteData)
{
	cv::UMat dest;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_BOXBLUR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(noDeleteData);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(noDeleteData);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * paramCoeff = new COpenCLParameterInt();
		paramCoeff->SetLibelle("coeff");
		paramCoeff->SetValue(coeff);
		vecParam.push_back(paramCoeff);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			
		}
		catch(...)
		{
			
		}
		delete program;

	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::MotionBlurCompute(const vector<double> & kernelMotion, const vector<wxPoint> & offsets, const int &size, cv::UMat & inputData)
{
	cv::UMat dest;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_MOTIONBLUR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
				
		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		float * kernel = new float[kernelMotion.size()];
		for (auto i = 0; i < kernelMotion.size(); i++)
			kernel[i] = kernelMotion[i];

		COpenCLParameterFloatArray * paramkernelMotion = new COpenCLParameterFloatArray();
		paramkernelMotion->SetLibelle("kernelMotion");
		paramkernelMotion->SetValue(context->GetContext(), kernel, size, flag);
		vecParam.push_back(paramkernelMotion);

		int * offsetsMotion = new int[kernelMotion.size() * 2];
		for (auto i = 0, j = 0; i < offsets.size(); i++, j += 2)
		{
			offsetsMotion[j] = offsets[i].x;
			offsetsMotion[j + 1] = offsets[i].y;
		}

		COpenCLParameterIntArray * paramoffsets = new COpenCLParameterIntArray();
		paramoffsets->SetLibelle("offsets");
		paramoffsets->SetValue(context->GetContext(), offsetsMotion, size * 2, flag);
		vecParam.push_back(paramoffsets);
	

		COpenCLParameterInt * paramkernelSize = new COpenCLParameterInt();
		paramkernelSize->SetLibelle("kernelSize");
		paramkernelSize->SetValue(size);
		vecParam.push_back(paramkernelSize);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "MotionBlur");
			
		}
		catch(...)
		{
			
		}
		delete program;

		delete[] kernel;
		delete[] offsetsMotion;

	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::FiltreConvolution(const wxString &programName, const wxString &functionName, cv::UMat & inputData)
{
	cv::UMat dest;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	
	COpenCLProgram * programCL = GetProgram(programName);
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			
		}
		catch(...)
		{
			
		}
		delete program;

	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::ErodeDilate(const wxString &functionName, cv::UMat & inputData)
{
	try
	{
		if(functionName == "Erode")
			cv::erode(inputData, inputData, cv::Mat());
		else if(functionName == "Dilate")
			cv::dilate(inputData, inputData, cv::Mat());

	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Posterize(const float &level, const float &gamma, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cv::UMat dest;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	
		
		COpenCLParameterInt * paramLevel = new COpenCLParameterInt();
		paramLevel->SetLibelle("level");
		paramLevel->SetValue(level);
		vecParam.push_back(paramLevel);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "Posterisation");
			
		}
		catch(...)
		{
			
		}
		delete program;

	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::Solarize(const long &threshold, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cv::UMat dest;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramThreshold = new COpenCLParameterInt();
		paramThreshold->SetLibelle("threshold");
		paramThreshold->SetValue((int)threshold);
		vecParam.push_back(paramThreshold);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "Solarization");
			
		}
		catch(...)
		{
			
		}
		delete program;

	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::Median(cv::UMat & inputData)
{
	try
	{
		cv::medianBlur(inputData, inputData, 3);

	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

void COpenCLFilter::Noise(cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cv::UMat dest;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_NOISE");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Noise");
			
		}
		catch(...)
		{
			
		}
		delete program;


	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}


void COpenCLFilter::Flip(const wxString &functionName, cv::UMat & inputData)
{
	if (functionName == "FlipVertical")
	{
		cv::flip(inputData, inputData, 0);
	}
	else
	{
		cv::flip(inputData, inputData, 1);
	}
}


void COpenCLFilter::Swirl(const float &radius, const float &angle, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cv::UMat dest;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_SWIRL");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterFloat * paramRadius = new COpenCLParameterFloat();
		paramRadius->SetLibelle("radius");
		paramRadius->SetValue(radius);
		vecParam.push_back(paramRadius);

		COpenCLParameterFloat * paramAngle = new COpenCLParameterFloat();
		paramAngle->SetLibelle("angle");
		paramAngle->SetValue(angle);
		vecParam.push_back(paramAngle);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Swirl");
			
		}
		catch(...)
		{
			
		}
		delete program;



	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();
}

void COpenCLFilter::BrightnessAndContrast(const double &brightness, const double &contrast, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cv::UMat dest;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterFloat * paramConstrast = new COpenCLParameterFloat();
		paramConstrast->SetLibelle("contrast");
		paramConstrast->SetValue(contrast);
		vecParam.push_back(paramConstrast);

		COpenCLParameterFloat * paramLightness = new COpenCLParameterFloat();
		paramLightness->SetLibelle("light");
		paramLightness->SetValue(brightness);
		vecParam.push_back(paramLightness);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create((int)inputData.rows, (int)inputData.cols, type);
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "LightAndContrast");
			
		}
		catch(...)
		{
			
		}
		delete program;



	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();

}

int COpenCLFilter::GetSizeData()
{
	return  sizeof(uint8_t);
}

COpenCLProgram * COpenCLFilter::GetProgram(const wxString &numProgram)
{
	if (context != nullptr)
		return context->GetProgram(numProgram);
	return nullptr;
}

void COpenCLFilter::ColorEffect(const wxString &functionName, cv::UMat & inputData)
{
	cv::UMat cvDest;
	try
	{
		if (functionName == "Sepia")
		{
			cvtColor(inputData, cvDest, cv::COLOR_BGRA2BGR);

			cv::Mat kernel =
				(cv::Mat_<float>(3, 3)
					<<
					0.272, 0.534, 0.131,
					0.349, 0.686, 0.168,
					0.393, 0.769, 0.189);

			cv::transform(cvDest, cvDest, kernel);
			cv::cvtColor(cvDest, inputData, cv::COLOR_BGR2BGRA);
		}
		else if (functionName == "Negatif")
		{
			cv::bitwise_not(inputData, inputData);
		}
		else if (functionName == "NoirEtBlanc")
		{
			cv::cvtColor(inputData, cvDest, cv::COLOR_BGRA2GRAY);
			cv::threshold(cvDest, cvDest, 127, 255, cv::THRESH_BINARY);
			cv::cvtColor(cvDest, inputData, cv::COLOR_GRAY2BGRA);
		}
		else if (functionName == "GrayLevel")
		{
			cv::cvtColor(inputData, cvDest, cv::COLOR_BGRA2GRAY);
			cv::cvtColor(cvDest, inputData, cv::COLOR_GRAY2BGRA);
		}
		cvDest.release();
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}


void COpenCLFilter::HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp, cv::UMat & inputData)
{
	if(hq3d == nullptr)
		hq3d = new Chqdn3d(inputData.cols, inputData.rows, LumSpac, LumTmp);
	else if (oldLevelDenoise != LumSpac || inputData.cols != oldwidthDenoise || inputData.rows != oldheightDenoise)
	{
		delete hq3d;
		hq3d = new Chqdn3d(inputData.cols, inputData.rows, LumSpac, LumTmp);
	}

	if (context != nullptr)
	{
		cv::UMat cvDest;
		cv::UMat ycbcr;
		cv::Mat yChannel;
		cv::Mat yChannelOut;

		cv::cvtColor(inputData, cvDest, cv::COLOR_BGRA2BGR);
		cvtColor(cvDest, ycbcr, cv::COLOR_BGR2YCrCb);

		std::vector<cv::Mat> planes(3);
		cv::split(ycbcr, planes);

		// Extract the Y channel
		//cv::extractChannel(ycbcr, yChannel, 0);

		uint8_t* dataOut = hq3d->ApplyDenoise3D(planes[0].data, inputData.cols, inputData.rows);

		memcpy(planes[0].data, dataOut, inputData.cols * inputData.rows);

		// Merge the the color planes back into an Lab image
		//cv::insertChannel(yChannel, ycbcr, 0);
		cv::merge(planes, ycbcr);
		// convert back to RGB
		cv::cvtColor(ycbcr, cvDest, cv::COLOR_YCrCb2BGR);
		cv::cvtColor(cvDest, inputData, cv::COLOR_BGR2BGRA);

		cvDest.release();
		ycbcr.release();
		yChannel.release();
		yChannelOut.release();

	}
}

void COpenCLFilter::Rotate(const wxString &functionName, const int &widthOut, const int &heightOut, const double &angle, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cv::UMat dest;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_ROTATION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(inputData.cols);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(inputData.rows);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
		paramWidthOut->SetLibelle("widthOut");
		paramWidthOut->SetValue(widthOut);
		vecParam.push_back(paramWidthOut);

		COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
		paramHeightOut->SetLibelle("heightOut");
		paramHeightOut->SetValue(heightOut);
		vecParam.push_back(paramHeightOut);

		COpenCLParameterFloat * paramAngle = new COpenCLParameterFloat();
		paramAngle->SetLibelle("angle");
		paramAngle->SetValue(angle);
		vecParam.push_back(paramAngle);

		try
		{
			int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
			int type = CV_MAKE_TYPE(depth, 4);
			dest.create(heightOut, widthOut, type);
			program->SetParameter(&vecParam, widthOut, heightOut, (cl_mem)dest.handle(cv::ACCESS_RW));
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
		}
		catch(...)
		{
		}
		delete program;


	for (COpenCLParameter * parameter : vecParam)
		{
			if(!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
		vecParam.clear();
	}
	inputData.release();
	dest.copyTo(inputData);
	dest.release();

}

cv::Rect COpenCLFilter::CalculRect(int widthIn, int heightIn, int widthOut, int heightOut, int flipH, int flipV, int angle, float ratioX, float ratioY, int x, int y, float left, float top)
{
	cv::Rect rect;
	float posX = (float)x * ratioX + left * ratioX;
	float posY = (float)y * ratioY + top * ratioY;

	if (angle == 270)
	{
		int srcx = posY;
		int srcy = posX;

		posX = srcx;
		posY = srcy;

		posX = widthIn - posX - 1;
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

		posY = heightIn - posY - 1;
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


cv::UMat COpenCLFilter::Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int& method, cv::UMat & inputData, int flipH, int flipV, int angle, int ratio)
{
	cv::UMat cvImage;
	//inputData.copyTo(cvImage);
	cv::cvtColor(inputData, cvImage, cv::COLOR_BGRA2BGR);

	try
	{



		float ratioX = (float)inputData.cols / rc.width;
		float ratioY = (float)inputData.rows / rc.height;
		if (angle == 90)
		{
			ratioX = (float)inputData.cols / (float)rc.height;
			ratioY = (float)inputData.rows / (float)rc.width;
		}
		else if (angle == 270)
		{
			ratioX = (float)inputData.cols / (float)rc.height;
			ratioY = (float)inputData.rows / (float)rc.width;
		}

		cv::Rect rectGlobal;
		cv::Rect rect_begin = CalculRect(inputData.cols, inputData.rows, widthOut, heightOut, flipH, flipV, angle, ratioX, ratioY, 0, 0, rc.x, rc.y);
		cv::Rect rect_end = CalculRect(inputData.cols, inputData.rows, widthOut, heightOut, flipH, flipV, angle, ratioX, ratioY, widthOut, heightOut, rc.x, rc.y);
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

		if ((rectGlobal.height + rectGlobal.y) > cvImage.rows)
		{
			rectGlobal.height = cvImage.rows - rectGlobal.y;
		}
		if ((rectGlobal.width + rectGlobal.x) > cvImage.cols)
		{
			rectGlobal.width = cvImage.cols - rectGlobal.x;
		}

		cvImage = cvImage(rectGlobal);


		if (angle == 90)
		{
			cv::rotate(cvImage, cvImage, cv::ROTATE_90_CLOCKWISE);
		}
		else if (angle == 270)
		{
			cv::rotate(cvImage, cvImage, cv::ROTATE_90_COUNTERCLOCKWISE);
		}
		else if (angle == 180)
		{
			cv::rotate(cvImage, cvImage, cv::ROTATE_180);
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
		if (ratio != 100)
		{
			if (method > 6 && TestIfMethodIsValid(method, (ratio / 100)) && !isUsed)
			{
				cvImage = upscaleImage(cvImage, method, (ratio / 100));
			}
			else
			{
				if (method > 6)
				{
					cv::resize(cvImage, cvImage, cv::Size(widthOut, heightOut), INTER_NEAREST_EXACT);
				}
				else
					cv::resize(cvImage, cvImage, cv::Size(widthOut, heightOut), method);
			}
		}

		if (cvImage.cols != widthOut || cvImage.rows != heightOut)
			cv::resize(cvImage, cvImage, cv::Size(widthOut, heightOut), cv::INTER_NEAREST_EXACT);

		//Apply Transformation
		if (flipH)
		{
			cv::flip(cvImage, cvImage, 1);
		}
		if (flipV)
		{
			cv::flip(cvImage, cvImage, 0);
		}
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}

	cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2BGRA);
	return cvImage;
}
