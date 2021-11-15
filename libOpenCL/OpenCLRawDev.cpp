#include <header.h>
#include "OpenCLRawDev.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "utility.h"
using namespace Regards::OpenCL;

COpenCLRawDev::COpenCLRawDev(COpenCLContext * context): width(0), height(0), sizeoutput(0)
{
	openCLProgram = nullptr;
	bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	openCLProgram = nullptr;
	this->context = context;
	paramOutput = nullptr;
}

COpenCLRawDev::~COpenCLRawDev()
{
	if(paramOutput != nullptr)
	{
		paramOutput->Release();
		delete paramOutput;
	}
}

void COpenCLRawDev::InitData(void * dataInput, int sizeInput, int sizeOutput, int width, int height, int scale)
{
	this->width = width;
	this->height = height;
	this->sizeoutput = sizeOutput;
	//img = dataInput;
	//output = new float[sizeOutput * 2];
	if(paramOutput != nullptr)
		delete paramOutput;

	paramOutput = new COpenCLParameterUShortArray();
	paramOutput->SetLibelle("input");
	((COpenCLParameterUShortArray *)paramOutput)->SetValue(context->GetContext(), (unsigned short *)dataInput, sizeInput, flag);

	cl_mem cloutput = InitData(scale);
	paramOutput = new COpenCLParameterClMem();
	((COpenCLParameterClMem *)paramOutput)->SetValue(cloutput);
}

void * COpenCLRawDev::GetOutputData(void * dataOutput, int sizeOutput, int lpass)
{
	cl_mem cloutput = OutputData(sizeOutput, lpass);
	cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), cloutput, CL_TRUE, 0, sizeOutput * sizeof(cl_ushort4), dataOutput, 0, nullptr, nullptr);
	Error::CheckError(err);
	err = clFinish(context->GetCommandQueue());
	Error::CheckError(err);

	if(cloutput != nullptr)
	{
		err = clReleaseMemObject(cloutput);
		Error::CheckError(err);
	}
	return dataOutput;
}

void * COpenCLRawDev::GetAlphaChannel(void * dataOutput, int sizeOutput)
{
	cl_mem cloutput = GetAlphaChannel(sizeOutput);
	cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), cloutput, CL_TRUE, 0, sizeOutput * sizeof(cl_float), dataOutput, 0, nullptr, nullptr);
	Error::CheckError(err);
	err = clFinish(context->GetCommandQueue());
	Error::CheckError(err);

	if(cloutput != nullptr)
	{
		err = clReleaseMemObject(cloutput);
		Error::CheckError(err);
	}
	return dataOutput;
}

void * COpenCLRawDev::GetTempData()
{
	void * data = malloc(sizeoutput * sizeof(cl_float4));
	cl_int err = clEnqueueReadBuffer(context->GetCommandQueue(), ((COpenCLParameterClMem *)paramOutput)->GetValue(), CL_TRUE, 0, sizeoutput * sizeof(cl_float4), data, 0, nullptr, nullptr);
	Error::CheckError(err);
	err = clFinish(context->GetCommandQueue());
	Error::CheckError(err);
	//delete paramOutput;

	return data;
}

void COpenCLRawDev::WaveletDenoiseNormalize(int size, int hpass, int lpass, float thold)
{
	WaveletDenoiseNormalizeValue(size, hpass, lpass, thold);
	/*
	paramOutput->Release();
	paramOutput = new COpenCLParameterClMem();
	((COpenCLParameterClMem *)paramOutput)->SetValue(cloutput);
	*/
	
}

void COpenCLRawDev::WaveletDenoiseCol(int hpass, int lpass, int sc)
{
	WaveletDenoiseByColSetValue(hpass, lpass, sc);
	/*
	paramOutput->Release();
	paramOutput = new COpenCLParameterClMem();
	((COpenCLParameterClMem *)paramOutput)->SetValue(cloutput);
	*/
	
}

void COpenCLRawDev::WaveletDenoiseRow(int lpass, int sc)
{
	WaveletDenoiseByRowSetValue(lpass, sc);
	/*
	paramOutput->Release();
	paramOutput = new COpenCLParameterClMem();
	((COpenCLParameterClMem *)paramOutput)->SetValue(cloutput);
	*/
}

void COpenCLRawDev::WaveletDenoiseChannel(int size, float threshold, float * noise)
{
	for(int lev = 0;lev < 5; lev++)
	{
		int hpass = 0;
		if(lev > 0)
			hpass = size*(((lev - 1) & 1)+1);
		int lpass = size*((lev & 1)+1);
		int sc = 1 << lev;
		float thold = threshold * noise[lev];

		WaveletDenoiseCol(hpass, lpass, sc);
		WaveletDenoiseRow(lpass, sc);
		WaveletDenoiseNormalize(size, hpass, lpass, thold);	
	}
}


cl_mem COpenCLRawDev::InitData(int scale)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			paramOutput->SetLibelle("input");
			vecParam.push_back(paramOutput);

			COpenCLParameterInt * paramScale = new COpenCLParameterInt();
			paramScale->SetValue(scale);
			paramScale->SetLibelle("scale");
			vecParam.push_back(paramScale);

			program->SetParameter(&vecParam, sizeoutput * sizeof(cl_float4), width, height, nullptr);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "InitData", width * height, sizeoutput * sizeof(cl_float4));
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}


cl_mem COpenCLRawDev::OutputData(int sizeOutput, int lpass)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			paramOutput->SetLibelle("input");
			paramOutput->SetNoDelete(true);
			vecParam.push_back(paramOutput);

			COpenCLParameterInt * paramlpass = new COpenCLParameterInt();
			paramlpass->SetValue(lpass);
			paramlpass->SetLibelle("lpass");
			vecParam.push_back(paramlpass);

			program->SetParameter(&vecParam, sizeOutput * sizeof(cl_ushort4), width, height, nullptr);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "OutputData", sizeOutput, sizeOutput * sizeof(cl_ushort4));
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}


cl_mem COpenCLRawDev::GetAlphaChannel(int sizeOutput)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			paramOutput->SetLibelle("input");
			paramOutput->SetNoDelete(true);
			vecParam.push_back(paramOutput);

			program->SetParameter(&vecParam, sizeOutput * sizeof(cl_float), width, height, nullptr);
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "GetAlphaChannel", sizeOutput, sizeOutput * sizeof(cl_float));
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}

cl_mem COpenCLRawDev::WaveletDenoiseNormalizeValue(int size, int hpass, int lpass, float thold)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterInt * paramhpass = new COpenCLParameterInt();
			paramhpass->SetValue(hpass);
			paramhpass->SetLibelle("hpass");
			vecParam.push_back(paramhpass);

			COpenCLParameterInt * paramlpass = new COpenCLParameterInt();
			paramlpass->SetValue(lpass);
			paramlpass->SetLibelle("lpass");
			vecParam.push_back(paramlpass);

			COpenCLParameterFloat * paramthold = new COpenCLParameterFloat();
			paramthold->SetValue(thold);
			paramthold->SetLibelle("thold");
			vecParam.push_back(paramthold);

			program->SetParameter(&vecParam, width, height, ((COpenCLParameterClMem *)paramOutput)->GetValue());
			program->SetKeepOutput(true);
			program->ExecuteProgram1D(programCL->GetProgram(), "WaveletDenoiseNormalize",  size, sizeoutput * sizeof(cl_float4));
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}

cl_mem COpenCLRawDev::WaveletDenoiseByColSetValue(int hpass, int lpass, int sc)
{
	//output = new float[sizeoutput*2];
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			paramOutput->SetLibelle("input");
			paramOutput->SetNoDelete(true);
			vecParam.push_back(paramOutput);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("iwidth");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("iheight");
			vecParam.push_back(paramHeight);

			COpenCLParameterInt * paramlpass = new COpenCLParameterInt();
			paramlpass->SetValue(lpass);
			paramlpass->SetLibelle("lpass");
			vecParam.push_back(paramlpass);

			COpenCLParameterInt * paramhpass = new COpenCLParameterInt();
			paramhpass->SetValue(hpass);
			paramhpass->SetLibelle("hpass");
			vecParam.push_back(paramhpass);

			COpenCLParameterInt * paramsc = new COpenCLParameterInt();
			paramsc->SetValue(sc);
			paramsc->SetLibelle("sc");
			vecParam.push_back(paramsc);
			
			program->SetParameter(&vecParam, width, height, ((COpenCLParameterClMem *)paramOutput)->GetValue());
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "WaveletDenoiseByColSetValue", sizeoutput* sizeof(cl_float4));
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}

cl_mem COpenCLRawDev::WaveletDenoiseByRowSetValue(int lpass, int sc)
{
	cl_mem outputValue = nullptr;
	if(context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_RAW");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);
			
			paramOutput->SetLibelle("input");
			paramOutput->SetNoDelete(true);
			vecParam.push_back(paramOutput);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("iwidth");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("iheight");
			vecParam.push_back(paramHeight);

			COpenCLParameterInt * paramlpass = new COpenCLParameterInt();
			paramlpass->SetValue(lpass);
			paramlpass->SetLibelle("lpass");
			vecParam.push_back(paramlpass);

			COpenCLParameterInt * paramsc = new COpenCLParameterInt();
			paramsc->SetValue(sc);
			paramsc->SetLibelle("sc");
			vecParam.push_back(paramsc);


			program->SetParameter(&vecParam, width, height, ((COpenCLParameterClMem *)paramOutput)->GetValue());
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "WaveletDenoiseByRowSetValue", sizeoutput* sizeof(cl_float4));
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}