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
}

COpenCLFilter::~COpenCLFilter()
{
	if (hq3d != nullptr)
		delete hq3d;
}

cl_mem COpenCLFilter::OilPaintingEffect(cl_mem inputData, int width, int height, const int &size, const int &dynRatio)
{
	context->GetContextForOpenCV().bind();
	cv::Mat cvDest;
	cv::Mat cvSrc;
	cv::UMat cvImage = GetOpenCVStruct(inputData, width, height);
	cv::cvtColor(cvImage, cvSrc, cv::COLOR_BGRA2BGR);
	cv::xphoto::oilPainting(cvSrc, cvDest, size, dynRatio, cv::COLOR_BGR2Lab);
	cv::cvtColor(cvDest, cvImage, cv::COLOR_BGR2BGRA);
	return CopyOpenCVTexture(cvImage, width, height);

}

cl_mem COpenCLFilter::Bm3d(cl_mem inputData, int width, int height, const float & fSigma)
{
	context->GetContextForOpenCV().bind();
	//bool frameStabilized = false;
	cv::UMat cvImage = GetOpenCVStruct(inputData, width, height);

	cv::UMat ycbcr;
	cv::UMat yChannel;
	cv::UMat yChannelOut;

	cv::cvtColor(cvImage, cvImage, cv::COLOR_BGRA2BGR);
	cvtColor(cvImage, ycbcr, cv::COLOR_BGR2YCrCb);

	// Extract the Y channel
	cv::extractChannel(ycbcr, yChannel, 0);

	cv::xphoto::bm3dDenoising(yChannel, yChannelOut, fSigma);

	// Merge the the color planes back into an Lab image
	cv::insertChannel(yChannelOut, ycbcr, 0);

	// convert back to RGB
	cv::cvtColor(ycbcr, cvImage, cv::COLOR_Lab2BGR);
	cv::cvtColor(cvImage, cvImage, cv::COLOR_BGR2BGRA);
	// Temporary Mat not reused, so release from memory.
	yChannel.release();

	return CopyOpenCVTexture(cvImage, width, height);
}



cl_mem COpenCLFilter::BrightnessAndContrastAuto(cl_mem inputData, int width, int height, float clipHistPercent)
{
	context->GetContextForOpenCV().bind();
	//bool frameStabilized = false;
	cv::UMat cvImage = GetOpenCVStruct(inputData, width, height);

	Regards::OpenCV::COpenCVEffect::BrightnessAndContrastAuto(cvImage);

	return CopyOpenCVTexture(cvImage, width, height);
}

cv::UMat COpenCLFilter::GetOpenCVStruct(cl_mem clImage, int width, int height)
{
	cv::UMat dst;
	cl_int err = 0;

	cl_mem_object_type mem_type = 0;
	clGetMemObjectInfo(clImage, CL_MEM_TYPE, sizeof(cl_mem_object_type), &mem_type, 0);

	cl_image_format fmt = { 0, 0 };
	clGetImageInfo(clImage, CL_IMAGE_FORMAT, sizeof(cl_image_format), &fmt, 0);

	int depth = CV_8U;
	//if (context->GetDefaultType() == OPENCL_FLOAT)
	//	depth = CV_32F;

	int type = CV_MAKE_TYPE(depth, 4);

	size_t w = width;
	size_t h = height;
	dst.create((int)h, (int)w, type);

	if (context->GetDefaultType() == OPENCL_FLOAT)
	{
		cl_mem clBuffer = (cl_mem)dst.handle(cv::ACCESS_RW);
		//cl_mem outputValue = nullptr;
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(clImage);
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("width");
			paramWidth->SetValue(width);
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("height");
			paramHeight->SetValue(height);
			vecParam.push_back(paramHeight);

			program->SetParameter(&vecParam, w, h, clBuffer);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "CopyToOpencv");
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
	}
	else
	{
		dst.create((int)h, (int)w, type);
		cl_mem clBuffer = (cl_mem)dst.handle(cv::ACCESS_RW);
		cl_command_queue q = (cl_command_queue)context->GetCommandQueue();
		err = clEnqueueCopyBuffer(q, clImage, clBuffer, 0, 0, w * h * GetSizeData(), NULL, NULL, NULL);
		Error::CheckError(err);
		clFinish(q);
	}



	return dst;
}

cl_mem COpenCLFilter::CopyOpenCVTexture(cv::UMat & dst, int width, int height)
{
	//cl_int err = 0;
	cl_mem clBuffer = (cl_mem)dst.handle(cv::ACCESS_READ);

	cl_mem outputValue = nullptr;
	COpenCLFilter openclFilter(context);
	COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_BITMAPCONVERSION");
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

cl_mem COpenCLFilter::ConvertToY(cl_mem inputData, int width, int height, const wxString & functionName)
{
	cl_mem outputValue = nullptr;
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CONVERTTOY");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(inputData);
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);

			try
			{
				program->SetParameter(&vecParam, width, height, sizeof(float) * width * height);
				program->SetKeepOutput(true);
				program->ExecuteProgram1D(programCL->GetProgram(), functionName);
				outputValue = program->GetOutput();

			}
			catch (...)
			{
				outputValue = nullptr;
			}

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
	}
	return outputValue;
}

cl_mem COpenCLFilter::ExtractBlocSize(cl_mem sourceData, const int & size, const int & marge, const int & width, const int & height, const int & x, const int & y)
{
	cl_mem outputValue = nullptr;
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CONVERTTOY");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(sourceData);
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);

			COpenCLParameterInt * paramSize = new COpenCLParameterInt();
			paramSize->SetLibelle("size");
			paramSize->SetValue(size);
			vecParam.push_back(paramSize);

			COpenCLParameterInt * paramMarge = new COpenCLParameterInt();
			paramMarge->SetLibelle("marge");
			paramMarge->SetValue(marge);
			vecParam.push_back(paramMarge);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("width");
			paramWidth->SetValue(width);
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("height");
			paramHeight->SetValue(height);
			vecParam.push_back(paramHeight);

			COpenCLParameterInt * paramX = new COpenCLParameterInt();
			paramX->SetLibelle("x");
			paramX->SetValue(x);
			vecParam.push_back(paramX);

			COpenCLParameterInt * paramY = new COpenCLParameterInt();
			paramY->SetLibelle("y");
			paramY->SetValue(y);
			vecParam.push_back(paramY);

			int sizeTraitement = size + 2 * marge;

			try
			{
				program->SetParameter(&vecParam, sizeTraitement, sizeTraitement, sizeof(float) * sizeTraitement * sizeTraitement);
				program->SetKeepOutput(true);
				program->ExecuteProgram(programCL->GetProgram(), "ExtractBlocSize");
				//program->ExecuteProgram2D(programCL->GetProgram(), "ExtractBlocSize", &vecParam, size, size);
				outputValue = program->GetOutput();

			}
			catch (...)
			{
				outputValue = nullptr;
			}

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
	}
	return outputValue;
}

void COpenCLFilter::InsertBlockSize(cl_mem sourceData, cl_mem wienerData, const int & size, const int & marge, const int & width, const int & height, const int & x, const int & y)
{
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CONVERTTOY");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * paramSource = new COpenCLParameterClMem(true);
			paramSource->SetValue(sourceData);
			paramSource->SetLibelle("output");
			paramSource->SetNoDelete(true);
			vecParam.push_back(paramSource);

			COpenCLParameterClMem * paramWiener = new COpenCLParameterClMem(true);
			paramWiener->SetValue(wienerData);
			paramWiener->SetLibelle("wiener");
			paramWiener->SetNoDelete(true);
			vecParam.push_back(paramWiener);

			COpenCLParameterInt * paramSize = new COpenCLParameterInt();
			paramSize->SetLibelle("size");
			paramSize->SetValue(size);
			vecParam.push_back(paramSize);

			COpenCLParameterInt * paramMarge = new COpenCLParameterInt();
			paramMarge->SetLibelle("marge");
			paramMarge->SetValue(marge);
			vecParam.push_back(paramMarge);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetLibelle("width");
			paramWidth->SetValue(width);
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetLibelle("height");
			paramHeight->SetValue(height);
			vecParam.push_back(paramHeight);

			COpenCLParameterInt * paramX = new COpenCLParameterInt();
			paramX->SetLibelle("x");
			paramX->SetValue(x);
			vecParam.push_back(paramX);

			COpenCLParameterInt * paramY = new COpenCLParameterInt();
			paramY->SetLibelle("y");
			paramY->SetValue(y);
			vecParam.push_back(paramY);

			try
			{
				int sizeTraitement = size + 2 * marge;
				program->ExecuteProgram2D(programCL->GetProgram(), "InsertBlockSize", &vecParam, sizeTraitement, sizeTraitement);
			}
			catch (...)
			{
			}

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
	}
}

cl_mem COpenCLFilter::InsertYValue(cl_mem inputData, cl_mem sourceData, int width, int height, const wxString & functionName)
{
	cl_mem outputValue = nullptr;
	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_CONVERTTOY");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(inputData);
			input->SetLibelle("Yinput");
			input->SetNoDelete(true);
			vecParam.push_back(input);

			COpenCLParameterClMem * source = new COpenCLParameterClMem(true);
			source->SetValue(sourceData);
			source->SetLibelle("source");
			source->SetNoDelete(true);
			vecParam.push_back(source);

			try
			{
				program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
				program->SetKeepOutput(true);
				program->ExecuteProgram1D(programCL->GetProgram(), functionName);
				outputValue = program->GetOutput();

			}
			catch (...)
			{
				outputValue = nullptr;
			}

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
	}
	return outputValue;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
cl_mem COpenCLFilter::Fusion(cl_mem inputData, cl_mem secondPictureData, const float &pourcentage, int width, int height)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		COpenCLProgram * programCL = openclFilter.GetProgram("IDR_OPENCL_FUSION");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(inputData);
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);	

			COpenCLParameterClMem * inputSecond = new COpenCLParameterClMem();
			inputSecond->SetValue(secondPictureData);
			inputSecond->SetLibelle("inputSecond");
			vecParam.push_back(inputSecond);

			COpenCLParameterFloat * prcent = new COpenCLParameterFloat();
			prcent->SetLibelle("pourcentage");
			prcent->SetValue(pourcentage);
			vecParam.push_back(prcent);

			try
			{
				program->SetParameter(&vecParam,  width, height, GetSizeData() * width * height);
				program->SetKeepOutput(true);
				program->ExecuteProgram1D(programCL->GetProgram(), "Fusion");
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
	}
	return outputValue;
}

//-----------------------------------------------------------------------------------------------
//Execute OPENCL Effect
//-----------------------------------------------------------------------------------------------
cl_mem COpenCLFilter::bilateral_filter(const int & radius, const int & preserve, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = GetProgram("IDR_BILATERALFILTER_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(inputData);
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);	

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);

			COpenCLParameterFloat * paramRadius = new COpenCLParameterFloat();
			paramRadius->SetValue(radius);
			paramRadius->SetLibelle("radius");
			vecParam.push_back(paramRadius);

			COpenCLParameterFloat * paramPreserve = new COpenCLParameterFloat();
			paramPreserve->SetValue(preserve);
			paramPreserve->SetLibelle("preserve");
			vecParam.push_back(paramPreserve);

			program->SetParameter(&vecParam,  width, height, GetSizeData() * width * height);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "bilateral_filter");
			outputValue = program->GetOutput();
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
	}
	return outputValue;
}

//NlmeansFilter
cl_mem COpenCLFilter::run2d(const int & FSIZE, const int & BSIZE,const float & SIGMA, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = GetProgram("IDR_BILATERALFILTER_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(inputData);
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);	

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);

			COpenCLParameterInt * paramfSize = new COpenCLParameterInt();
			paramfSize->SetValue(FSIZE);
			paramfSize->SetLibelle("FSIZE");
			vecParam.push_back(paramfSize);

			COpenCLParameterInt * paramBSize = new COpenCLParameterInt();
			paramBSize->SetValue(BSIZE);
			paramBSize->SetLibelle("BSIZE");
			vecParam.push_back(paramBSize);

			COpenCLParameterFloat * paramsigmaP = new COpenCLParameterFloat();
			paramsigmaP->SetValue(SIGMA);
			paramsigmaP->SetLibelle("SIGMA");
			vecParam.push_back(paramsigmaP);

			program->SetParameter(&vecParam,  width, height, GetSizeData() * width * height);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "run2d");
			outputValue = program->GetOutput();
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
	}
	return outputValue;
}

//BilateralFilter
cl_mem COpenCLFilter::bilat2(int fSize,  float sigmaX, float sigmaP, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = GetProgram("IDR_BILATERALFILTER_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
			input->SetValue(inputData);
			input->SetLibelle("input");
			input->SetNoDelete(true);
			vecParam.push_back(input);	

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);

			COpenCLParameterInt * paramfSize = new COpenCLParameterInt();
			paramfSize->SetValue(fSize);
			paramfSize->SetLibelle("fSize");
			vecParam.push_back(paramfSize);


			COpenCLParameterFloat * paramsigmaX = new COpenCLParameterFloat();
			paramsigmaX->SetValue(sigmaX);
			paramsigmaX->SetLibelle("sigmaX");
			vecParam.push_back(paramsigmaX);


			COpenCLParameterFloat * paramsigmaP = new COpenCLParameterFloat();
			paramsigmaP->SetValue(sigmaP);
			paramsigmaP->SetLibelle("sigmaP");
			vecParam.push_back(paramsigmaP);

			try
			{
				program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
				program->SetKeepOutput(true);
				program->ExecuteProgram(programCL->GetProgram(), "bilat2");
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
	}
	return outputValue;
}

cl_mem COpenCLFilter::SharpenMasking(const float &sharpness, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_SHARPENMASKING");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
				
		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterFloat * paramIntensity = new COpenCLParameterFloat();
		paramIntensity->SetLibelle("sharpness");
		paramIntensity->SetValue(sharpness);
		vecParam.push_back(paramIntensity);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::PhotoFiltre(const CRgbaquad &clValue, const int &intensity, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
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
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::RGBFilter(const int &red, const int &green, const int &blue, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
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
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::FiltreMosaic(cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_MOSAIC");
	if(programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterFloat * paramAngle = new COpenCLParameterFloat();
		paramAngle->SetLibelle("fTileSize");
		paramAngle->SetValue(5.0f);
		vecParam.push_back(paramAngle);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::Blur(const int &radius, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_BLUR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);
		
		COpenCLParameterInt * paramCoeff = new COpenCLParameterInt();
		paramCoeff->SetLibelle("size");
		paramCoeff->SetValue(radius);
		vecParam.push_back(paramCoeff);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Blur");
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
	return outputValue;
}

cl_mem COpenCLFilter::BoxBlur(const int &coeff, const wxString &functionName, cl_mem inputData, int width, int height, bool noDeleteData)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_BOXBLUR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(noDeleteData);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(noDeleteData);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * paramCoeff = new COpenCLParameterInt();
		paramCoeff->SetLibelle("coeff");
		paramCoeff->SetValue(coeff);
		vecParam.push_back(paramCoeff);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::MotionBlurCompute(const vector<double> & kernelMotion, const vector<wxPoint> & offsets, const int &size, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_MOTIONBLUR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
				
		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
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
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::FiltreConvolution(const wxString &programName, const wxString &functionName, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram(programName);
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::ErodeDilate(const wxString &functionName, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_DILATEERODE");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::Posterize(const float &level, const float &gamma, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	
		
		COpenCLParameterInt * paramLevel = new COpenCLParameterInt();
		paramLevel->SetLibelle("level");
		paramLevel->SetValue(level);
		vecParam.push_back(paramLevel);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::Solarize(const long &threshold, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramThreshold = new COpenCLParameterInt();
		paramThreshold->SetLibelle("threshold");
		paramThreshold->SetValue((int)threshold);
		vecParam.push_back(paramThreshold);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::Median(cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_MEDIAN");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "Median");
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
	return outputValue;
}

cl_mem COpenCLFilter::Noise(cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_NOISE");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;


}


cl_mem COpenCLFilter::Flip(const wxString &functionName, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_FLIP");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetNoDelete(true);
		input->SetLibelle("input");
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterInt * paramWidthOut = new COpenCLParameterInt();
		paramWidthOut->SetLibelle("widthOut");
		paramWidthOut->SetValue(width);
		vecParam.push_back(paramWidthOut);

		COpenCLParameterInt * paramHeightOut = new COpenCLParameterInt();
		paramHeightOut->SetLibelle("heightOut");
		paramHeightOut->SetValue(height);
		vecParam.push_back(paramHeightOut);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}


cl_mem COpenCLFilter::Swirl(const float &radius, const float &angle, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_SWIRL");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
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
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

cl_mem COpenCLFilter::BrightnessAndContrast(const double &brightness, const double &contrast, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
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
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
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
	return outputValue;
}

int COpenCLFilter::GetSizeData()
{
	if(context->GetDefaultType() == OPENCL_FLOAT)
		return sizeof(float) * 4;
	return  sizeof(uint8_t) * 4;
}

COpenCLProgram * COpenCLFilter::GetProgram(const wxString &numProgram)
{
	if (context != nullptr)
		return context->GetProgram(numProgram);
	return nullptr;
}

cl_mem COpenCLFilter::ColorEffect(const wxString &functionName, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_COLOR");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), functionName);
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
	return outputValue;
}


cl_mem COpenCLFilter::HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp, cl_mem inputData, int width, int height)
{
	cl_mem output = nullptr;
	cl_mem yPicture = nullptr;

	if(hq3d == nullptr)
		hq3d = new Chqdn3d(width, height, LumSpac, LumTmp);
	else if (oldLevelDenoise != LumSpac || width != oldwidthDenoise || height != oldheightDenoise)
	{
		delete hq3d;
		hq3d = new Chqdn3d(width, height, LumSpac, LumTmp);
	}

	if (context != nullptr)
	{
		COpenCLFilter openclFilter(context);
		if (inputData != nullptr)
		{
			yPicture = openclFilter.ConvertToY(inputData, width, height, "ConvertToYUchar");
		}

		long size = width * height;
		uint8_t * data_picture = new uint8_t[width * height];
		if (context != nullptr)
		{
			context->GetOutputData(yPicture, data_picture, size, flag);


		}
		uint8_t * dataOut = hq3d->ApplyDenoise3D(data_picture, width, height);

		COpenCLParameterByteArray * memDataOut = new COpenCLParameterByteArray();
		((COpenCLParameterByteArray *)memDataOut)->SetLibelle("input");
		((COpenCLParameterByteArray *)memDataOut)->SetValue(context->GetContext(), (uint8_t *)dataOut, size, flag);


		output = openclFilter.InsertYValue(memDataOut->GetValue(), inputData, width, height, "InsertYValueFromUchar");

		cl_int err;
		err = clReleaseMemObject(yPicture);
		Error::CheckError(err);
		yPicture = nullptr;

		delete memDataOut;

	}
	return output;
}

cl_mem COpenCLFilter::Rotate(const wxString &functionName, const int &widthOut, const int &heightOut, const double &angle, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_ROTATION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
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
	return outputValue;
}

cl_mem COpenCLFilter::Interpolation(const int &widthOut, const int &heightOut, const wxString &functionName, const int& method, cl_mem inputData, int width, int height, int flipH, int flipV, int angle)
{
	cl_mem outputValue = nullptr;
	
	COpenCLProgram * programCL = nullptr;
	programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetNoDelete(true);
		input->SetLibelle("input");
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
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
	return outputValue;
}

cl_mem COpenCLFilter::Denoise(const wxString &functionName, const float &sigma, const float &threshold, const float &kSigma, cl_mem inputData, int width, int height)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_DENOISE");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterFloat * paramsigma = new COpenCLParameterFloat();
		paramsigma->SetLibelle("sigma");
		paramsigma->SetValue(sigma);
		vecParam.push_back(paramsigma);

		COpenCLParameterFloat * paramthreshold = new COpenCLParameterFloat();
		paramthreshold->SetLibelle("threshold");
		paramthreshold->SetValue(threshold);
		vecParam.push_back(paramthreshold);

		COpenCLParameterFloat * paramkSigma = new COpenCLParameterFloat();
		paramkSigma->SetLibelle("kSigma");
		paramkSigma->SetValue(kSigma);
		vecParam.push_back(paramkSigma);

		try
		{
			program->SetParameter(&vecParam, width, height, GetSizeData() * width * height);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), functionName);
			outputValue = program->GetOutput();
		}
		catch (...)
		{
			outputValue = nullptr;
		}

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

cl_mem COpenCLFilter::Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const wxString &functionName, const int& method, cl_mem inputData, int width, int height, int flipH, int flipV, int angle)
{
	cl_mem outputValue = nullptr;
	COpenCLProgram * programCL = GetProgram("IDR_OPENCL_INTERPOLATION");
	if (programCL != nullptr)
	{
		vector<COpenCLParameter *> vecParam;
		COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

		COpenCLParameterClMem * input = new COpenCLParameterClMem(true);
		input->SetValue(inputData);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);	

		COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
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
	return outputValue;
}
