#include <header.h>
#include "OpenCLFilter.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "RegardsBitmap.h"
#include "utility.h"
#include <OpenCVEffect.h>
#include <opencv2/core/ocl.hpp>
#include <opencv2/core.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/imgproc.hpp>

using namespace Regards::OpenCL;


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


int COpenCLFilter::GetRgbaBitmap(void* cl_image, const cv::UMat& inputData)
{
	COpenCLProgram* programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
		int width = inputData.cols;
		int height = inputData.rows;

		vector<COpenCLParameter*> vecParam;
		COpenCLExecuteProgram* program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem* paramOutput = new COpenCLParameterClMem();
		paramOutput->SetLibelle("input");
		paramOutput->SetValue(clBuffer);
		paramOutput->SetNoDelete(true);
		vecParam.push_back(paramOutput);

		COpenCLParameterInt* paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt* paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);


		COpenCLParameterInt* paramRGBA = new COpenCLParameterInt();

#if defined(__x86_64__) || defined(_M_AMD64)	
		paramRGBA->SetValue(0);
#elif defined(__APPLE__)
		paramRGBA->SetValue(1);
#endif 
		paramRGBA->SetLibelle("rgba");
		vecParam.push_back(paramRGBA);

		program->SetKeepOutput(true);
		program->SetParameter(&vecParam, width, height, (cl_mem)cl_image);
		program->ExecuteProgram(programCL->GetProgram(), "BitmapToOpenGLTexture");

		delete program;
		vecParam.clear();
	}

	return 0;
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
		Regards::OpenCV::COpenCVEffect::BrightnessAndContrastAuto(inputData);
	}
	catch (cv::Exception& e)
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
	}
}

cv::UMat COpenCLFilter::GetOpenCVStruct(cl_mem clImage, int width, int height)
{
	cv::UMat cvImage;

	int depth = (context->GetDefaultType() == OPENCL_FLOAT) ? CV_32F : CV_8U;
	int type = CV_MAKE_TYPE(depth, 4);
	cvImage.create((int)height, (int)width, type);
	cl_mem clBuffer = (cl_mem)cvImage.handle(cv::ACCESS_RW);
	cl_command_queue q = context->GetCommandQueue();
	cl_int err = clEnqueueCopyBuffer(q, clImage, clBuffer, 0, 0, width * height * GetSizeData(), NULL, NULL, NULL);
	Error::CheckError(err);
	clFinish(q);

	err = clReleaseMemObject(clImage);
	Error::CheckError(err);
	clImage = nullptr;

	return cvImage;
}

cl_mem COpenCLFilter::CopyOpenCVTexture(cv::UMat & dst, int width, int height)
{
	//cl_int err = 0;
	cl_mem clBuffer = (cl_mem)dst.handle(cv::ACCESS_READ);

	cl_mem outputValue = nullptr;
	
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_BITMAPCONVERSION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem *	dataImage = new COpenCLParameterClMem();
		dataImage->SetNoDelete(true);
		dataImage->SetValue(clBuffer);
		vecParam.push_back(dataImage);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetLibelle("width");
		paramWidth->SetValue(width);
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetLibelle("height");
		paramHeight->SetValue(height);
		vecParam.push_back(paramHeight);

		program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
		program->SetKeepOutput(true);
		program->ExecuteProgram1D(programCL->GetProgram(), "ImportFromOpencv");
		outputValue = program->GetOutput();

		delete program;

		for (COpenCLParameter * parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

		vecParam.clear();
	}


	return outputValue;
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
	cv::UMat paramSrc;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			
			//void COpenCLExecuteProgram::SetParameter(vector<COpenCLParameter*>* vecParam, int width, int height, cl_mem output)
			//program->SetParameter(&vecParam, inputData.cols, inputData.rows, (cl_mem)paramSrc.handle(cv::ACCESS_RW));
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "SharpenMasking");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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

	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
}

void COpenCLFilter::PhotoFiltre(const CRgbaquad &clValue, const int &intensity, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "PhotoFiltre");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
}

void COpenCLFilter::RGBFilter(const int &red, const int &green, const int &blue, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "RGBFiltre");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
}

void COpenCLFilter::FiltreMosaic(cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Mosaic");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
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
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
}

void COpenCLFilter::MotionBlurCompute(const vector<double> & kernelMotion, const vector<wxPoint> & offsets, const int &size, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "MotionBlur");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
}

void COpenCLFilter::FiltreConvolution(const wxString &programName, const wxString &functionName, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
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
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "Posterisation");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
}

void COpenCLFilter::Solarize(const long &threshold, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "Solarization");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
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
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Noise");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
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
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Swirl");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);
}

void COpenCLFilter::BrightnessAndContrast(const double &brightness, const double &contrast, cv::UMat & inputData)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, inputData.cols, inputData.rows, GetSizeData() * inputData.cols * inputData.rows);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "LightAndContrast");
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, inputData.cols, inputData.rows);

}

int COpenCLFilter::GetSizeData()
{
	return  sizeof(uint8_t) * 4;
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

	}
}

void COpenCLFilter::Rotate(const wxString &functionName, const int &widthOut, const int &heightOut, const double &angle, cv::UMat & inputData)
{

	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
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
			program->SetParameter(&vecParam, widthOut, heightOut, GetSizeData() * widthOut * heightOut);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	inputData = GetOpenCVStruct(outputValue, widthOut, heightOut);

}

cv::UMat COpenCLFilter::Interpolation(const int &widthOut, const int &heightOut, const wxString &functionName, const int& method, cv::UMat & inputData, int flipH, int flipV, int angle)
{
	cl_mem outputValue = nullptr;
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_INTERPOLATION");

	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(clBuffer);
		input->SetNoDelete(true);
		input->SetLibelle("input");
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

		COpenCLParameterInt * paramflipH = new COpenCLParameterInt();
		paramflipH->SetLibelle("flipH");
		paramflipH->SetValue(flipH);
		vecParam.push_back(paramflipH);

		COpenCLParameterInt * paramflipV = new COpenCLParameterInt();
		paramflipV->SetLibelle("flipV");
		paramflipV->SetValue(flipV);
		vecParam.push_back(paramflipV);

		COpenCLParameterInt * paramangle = new COpenCLParameterInt();
		paramangle->SetLibelle("angle");
		paramangle->SetValue(angle);
		vecParam.push_back(paramangle);

		COpenCLParameterInt* paramtype = new COpenCLParameterInt();
		paramtype->SetLibelle("type");
		paramtype->SetValue(method);
		vecParam.push_back(paramtype);

		try
		{
			program->SetParameter(&vecParam,  widthOut, heightOut, GetSizeData() * widthOut * heightOut);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	return GetOpenCVStruct(outputValue, widthOut, heightOut);
}

cv::UMat COpenCLFilter::Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const wxString &functionName, const int& method, cv::UMat & inputData, int flipH, int flipV, int angle)
{
	cl_mem clBuffer = (cl_mem)inputData.handle(cv::ACCESS_RW);
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
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

		COpenCLParameterFloat * left = new COpenCLParameterFloat();
		left->SetLibelle("left");
		left->SetValue(rc.x);
		vecParam.push_back(left);

		COpenCLParameterFloat * top = new COpenCLParameterFloat();
		top->SetLibelle("top");
		top->SetValue(rc.y);
		vecParam.push_back(top);

		COpenCLParameterFloat * bitmapWidth = new COpenCLParameterFloat();
		bitmapWidth->SetLibelle("bitmapWidth");
		bitmapWidth->SetValue(rc.width);
		vecParam.push_back(bitmapWidth);

		COpenCLParameterFloat * bitmapHeight = new COpenCLParameterFloat();
		bitmapHeight->SetLibelle("bitmapHeight");
		bitmapHeight->SetValue(rc.height);
		vecParam.push_back(bitmapHeight);

		COpenCLParameterInt * paramflipH = new COpenCLParameterInt();
		paramflipH->SetLibelle("flipH");
		paramflipH->SetValue(flipH);
		vecParam.push_back(paramflipH);

		COpenCLParameterInt * paramflipV = new COpenCLParameterInt();
		paramflipV->SetLibelle("flipV");
		paramflipV->SetValue(flipV);
		vecParam.push_back(paramflipV);

		COpenCLParameterInt * paramangle = new COpenCLParameterInt();
		paramangle->SetLibelle("angle");
		paramangle->SetValue(angle);
		vecParam.push_back(paramangle);

		COpenCLParameterInt* paramtype = new COpenCLParameterInt();
		paramtype->SetLibelle("type");
		paramtype->SetValue(method);
		vecParam.push_back(paramtype);

		try
		{
			program->SetKeepOutput(true);
			program->SetParameter(&vecParam,  widthOut, heightOut, GetSizeData() * widthOut * heightOut);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			outputValue = program->GetOutput();
		}
		catch(...)
		{
			outputValue = nullptr;
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
	return GetOpenCVStruct(outputValue, widthOut, heightOut);
}
