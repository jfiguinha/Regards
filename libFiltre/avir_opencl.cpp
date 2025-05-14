#pragma once
#include <header.h>
#include <avir_opencl.h>
#include <libResource.h>
#include "OpenCLParameter.h"
#include "OpenCLContext.h"

extern cv::ocl::OpenCLExecutionContext clExecCtx;
using namespace Regards::OpenCL;
using namespace cv;

UMat CAvirFilterOpenCL::ConvertToFloat(cv::UMat& src, const int& width, const int& height)
{
	cl_mem_flags flag;
	{
		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		//cv::Mat  paramDest(height, width, CV_32FC4, dest);
		// Crée un UMat avec le type CV_8UC4
		UMat paramSrc(height, width, CV_32FC4);
		auto clBuffer = static_cast<cl_mem>(paramSrc.handle(ACCESS_WRITE));


		vector<COpenCLParameter*> vecParam;

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		// Récupération du code source du kernel
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");
		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);

		ocl::Kernel kernel("ConvertToFloat", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		//COpenCLContext::GetOutputData(clBuffer, dest, width * height * sizeof(float) * 4, flag);

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

		return paramSrc;
	}
}


cv::UMat CAvirFilterOpenCL::GetDataOpenCLHtoV2D(cv::UMat& src)
{
	UMat paramOutput(src.size().width, src.size().height, CV_32FC4);
	cl_mem_flags flag;
	{


		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		vector<COpenCLParameter*> vecParam;
		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramOutput.handle(ACCESS_WRITE));

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);


		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(src.size().width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(src.size().height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		/*
		// Récupération du code source du kernel
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");
		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);
		*/
		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");
		ocl::Kernel kernel("GetDataHtoV2D", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(src.size().width), static_cast<size_t>(src.size().height) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
	}
	return paramOutput;
}

UMat CAvirFilterOpenCL::UpSample2D(cv::UMat& src, const int& width, const int& height, int widthSrc, int start, int outLen, int ResampleFactor)
{
	UMat paramSrc(height, width, CV_32FC4);
	cl_mem_flags flag;
	{
		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;


		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramSrc.handle(ACCESS_WRITE));

		vector<COpenCLParameter*> vecParam;

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		auto paramWidthSrc = new COpenCLParameterInt();
		paramWidthSrc->SetValue(widthSrc);
		paramWidthSrc->SetLibelle("widthSrc");
		vecParam.push_back(paramWidthSrc);

		auto paramStart = new COpenCLParameterInt();
		paramStart->SetValue(start);
		paramStart->SetLibelle("start");
		vecParam.push_back(paramStart);

		auto paramOutLen = new COpenCLParameterInt();
		paramOutLen->SetValue(outLen);
		paramOutLen->SetLibelle("outLen");
		vecParam.push_back(paramOutLen);

		auto paramsrcResampleFactor = new COpenCLParameterInt();
		paramsrcResampleFactor->SetValue(ResampleFactor);
		paramsrcResampleFactor->SetLibelle("ResampleFactor");
		vecParam.push_back(paramsrcResampleFactor);

		/*
		// Récupération du code source du kernel
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");
		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);
		*/

		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");
		ocl::Kernel kernel;
		if(src.type() == CV_8UC4)
			kernel.create("UpSample2DUchar", program);
		else
			kernel.create("UpSample2D", program);
		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

	}
	return paramSrc;
}

UMat CAvirFilterOpenCL::doResize2OpenCL2D(cv::UMat& src, const int& width, const int& height,
	int* PositionTab, int posTabSize, float* ftp, int ftpTabSize, int IntFltLen)
{
	UMat paramOutput(height, width, CV_32FC4);
	cl_mem_flags flag;
	{
		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		vector<COpenCLParameter*> vecParam;
		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramOutput.handle(ACCESS_WRITE));

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterIntArray* paramrPos = new COpenCLParameterIntArray();
		paramrPos->SetLibelle("PositionTab");
		paramrPos->SetValue((cl_context)clExecCtx.getContext().ptr(), PositionTab, posTabSize, flag);
		vecParam.push_back(paramrPos);

		COpenCLParameterFloatArray* paramrfltBank = new COpenCLParameterFloatArray();
		paramrfltBank->SetLibelle("ftp");
		paramrfltBank->SetValue((cl_context)clExecCtx.getContext().ptr(), ftp, ftpTabSize, flag);
		vecParam.push_back(paramrfltBank);

		auto paramIntFltLen = new COpenCLParameterInt();
		paramIntFltLen->SetValue(IntFltLen);
		paramIntFltLen->SetLibelle("IntFltLen");
		vecParam.push_back(paramIntFltLen);

		auto paramWidthInput = new COpenCLParameterInt();
		paramWidthInput->SetValue(src.size().width);
		paramWidthInput->SetLibelle("inputWidth");
		vecParam.push_back(paramWidthInput);

		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");
		ocl::Kernel kernel("doResize22D", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
	}
	return paramOutput;
}

UMat CAvirFilterOpenCL::doResizeOpenCL2D(cv::UMat& src, const int& width, const int& height, int* PositionTab, int posTabSize, float* ftp, int ftpTabSize, int IntFltLen)
{
	UMat paramOutput(height, width, CV_32FC4);
	cl_mem_flags flag;
	{
		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		vector<COpenCLParameter*> vecParam;
		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramOutput.handle(ACCESS_WRITE));

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterIntArray* paramrPos = new COpenCLParameterIntArray();
		paramrPos->SetLibelle("PositionTab");
		paramrPos->SetValue((cl_context)clExecCtx.getContext().ptr(), PositionTab, posTabSize, flag);
		vecParam.push_back(paramrPos);

		auto paramTabSize = new COpenCLParameterInt();
		paramTabSize->SetValue(posTabSize);
		paramTabSize->SetLibelle("tabSize");
		vecParam.push_back(paramTabSize);

		COpenCLParameterFloatArray* paramrfltBank = new COpenCLParameterFloatArray();
		paramrfltBank->SetLibelle("ftp");
		paramrfltBank->SetValue((cl_context)clExecCtx.getContext().ptr(), ftp, ftpTabSize, flag);
		vecParam.push_back(paramrfltBank);


		auto paramIntFltLen = new COpenCLParameterInt();
		paramIntFltLen->SetValue(IntFltLen);
		paramIntFltLen->SetLibelle("IntFltLen");
		vecParam.push_back(paramIntFltLen);

		auto paramWidthInput = new COpenCLParameterInt();
		paramWidthInput->SetValue(src.size().width);
		paramWidthInput->SetLibelle("inputWidth");
		vecParam.push_back(paramWidthInput);
		// Récupération du code source du kernel
		/*
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");
		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);
		*/
		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");
		ocl::Kernel kernel;
		if (src.type() == CV_8UC4)
			kernel.create("doResize2DUchar", program);
		else
			kernel.create("doResize2D", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}
	}
	return paramOutput;
}

UMat CAvirFilterOpenCL::GetDataOpenCLHtoVDither2D(cv::UMat& src, float gm, float PkOut, float TrMul0)
{
	UMat paramOutput(src.size().width, src.size().height, CV_8UC4);
	cl_mem_flags flag;
	{


		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		vector<COpenCLParameter*> vecParam;
		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramOutput.handle(ACCESS_WRITE));

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);


		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(src.size().width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(src.size().height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		auto paramGM = new COpenCLParameterFloat();
		paramGM->SetValue(gm);
		paramGM->SetLibelle("gm");
		vecParam.push_back(paramGM);

		auto paramPkOut = new COpenCLParameterFloat();
		paramPkOut->SetValue(PkOut);
		paramPkOut->SetLibelle("PkOut");
		vecParam.push_back(paramPkOut);

		auto paramTrMul0 = new COpenCLParameterFloat();
		paramTrMul0->SetValue(TrMul0);
		paramTrMul0->SetLibelle("TrMul0");
		vecParam.push_back(paramTrMul0);

		/*
		// Récupération du code source du kernel
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");
		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);
		*/

		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");
		ocl::Kernel kernel("GetDataHtoVDither2D", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(src.size().height), static_cast<size_t>(src.size().width) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

	}
	return paramOutput;
}

UMat CAvirFilterOpenCL::doFilterOpenCL2D(cv::UMat& src, const int& width, const int& height,
	const float* f, int flen, int step)
{
	UMat paramOutput(height, width, CV_32FC4);
	cl_mem_flags flag;
	{
		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		vector<COpenCLParameter*> vecParam;
		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramOutput.handle(ACCESS_WRITE));

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidtSrc = new COpenCLParameterInt();
		paramWidtSrc->SetValue(src.size().width);
		paramWidtSrc->SetLibelle("widthSrc");
		vecParam.push_back(paramWidtSrc);

		auto paramHeightSrc = new COpenCLParameterInt();
		paramHeightSrc->SetValue(src.size().height);
		paramHeightSrc->SetLibelle("heightSrc");
		vecParam.push_back(paramHeightSrc);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(width);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(height);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterFloatArray* paramrfltBank = new COpenCLParameterFloatArray();
		paramrfltBank->SetLibelle("f");
		paramrfltBank->SetValue((cl_context)clExecCtx.getContext().ptr(), (float*)f, flen, flag);
		vecParam.push_back(paramrfltBank);

		auto paramIntFltLen = new COpenCLParameterInt();
		paramIntFltLen->SetValue(flen);
		paramIntFltLen->SetLibelle("flen");
		vecParam.push_back(paramIntFltLen);

		auto paramIntStep = new COpenCLParameterInt();
		paramIntStep->SetValue(step);
		paramIntStep->SetLibelle("step");
		vecParam.push_back(paramIntStep);

		/*
		// Récupération du code source du kernel
		//wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");

		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);
		*/

		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");

		ocl::Kernel kernel;
		if (src.type() == CV_8UC4)
			kernel.create("doFilter2DUchar", program);
		else
			kernel.create("doFilter2D", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(width), static_cast<size_t>(height) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

		return paramOutput;
	}
}


UMat CAvirFilterOpenCL::doFilterOpenCL2DV(cv::UMat& src, const float* f, int flen, int step)
{
	UMat paramOutput(src.size().width, src.size().height, CV_32FC4);
	cl_mem_flags flag;
	{
		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		vector<COpenCLParameter*> vecParam;
		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramOutput.handle(ACCESS_WRITE));

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidtSrc = new COpenCLParameterInt();
		paramWidtSrc->SetValue(src.size().width);
		paramWidtSrc->SetLibelle("widthSrc");
		vecParam.push_back(paramWidtSrc);

		auto paramHeightSrc = new COpenCLParameterInt();
		paramHeightSrc->SetValue(src.size().height);
		paramHeightSrc->SetLibelle("heightSrc");
		vecParam.push_back(paramHeightSrc);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(src.size().height);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(src.size().width);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterFloatArray* paramrfltBank = new COpenCLParameterFloatArray();
		paramrfltBank->SetLibelle("f");
		paramrfltBank->SetValue((cl_context)clExecCtx.getContext().ptr(), (float*)f, flen, flag);
		vecParam.push_back(paramrfltBank);

		auto paramIntFltLen = new COpenCLParameterInt();
		paramIntFltLen->SetValue(flen);
		paramIntFltLen->SetLibelle("flen");
		vecParam.push_back(paramIntFltLen);

		auto paramIntStep = new COpenCLParameterInt();
		paramIntStep->SetValue(step);
		paramIntStep->SetLibelle("step");
		vecParam.push_back(paramIntStep);

		/*
		// Récupération du code source du kernel
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");
		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);
		*/

		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");
		ocl::Kernel kernel;
		kernel.create("doFilter2DV", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(src.size().width), static_cast<size_t>(src.size().height) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

		return paramOutput;
	}
}

UMat CAvirFilterOpenCL::doFilterOpenCL2DLastStep(cv::UMat& src, const float* f, int flen, int step, float gm, float PkOut, float TrMul0)
{
	UMat paramOutput(src.size().width, src.size().height, CV_8UC4);
	cl_mem_flags flag;
	{
		bool useMemory = (cv::ocl::Device::getDefault().type() == CL_DEVICE_TYPE_GPU) ? false : true;
		flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;

		vector<COpenCLParameter*> vecParam;
		// Crée un UMat avec le type CV_8UC4

		auto clBuffer = static_cast<cl_mem>(paramOutput.handle(ACCESS_WRITE));

		auto clInputBuffer = static_cast<cl_mem>(src.handle(ACCESS_READ));
		auto input = new COpenCLParameterClMem(true);
		input->SetValue(clInputBuffer);
		input->SetLibelle("input");
		input->SetNoDelete(true);
		vecParam.push_back(input);

		auto paramWidtSrc = new COpenCLParameterInt();
		paramWidtSrc->SetValue(src.size().width);
		paramWidtSrc->SetLibelle("widthSrc");
		vecParam.push_back(paramWidtSrc);

		auto paramHeightSrc = new COpenCLParameterInt();
		paramHeightSrc->SetValue(src.size().height);
		paramHeightSrc->SetLibelle("heightSrc");
		vecParam.push_back(paramHeightSrc);

		auto paramWidth = new COpenCLParameterInt();
		paramWidth->SetValue(src.size().height);
		paramWidth->SetLibelle("width");
		vecParam.push_back(paramWidth);

		auto paramHeight = new COpenCLParameterInt();
		paramHeight->SetValue(src.size().width);
		paramHeight->SetLibelle("height");
		vecParam.push_back(paramHeight);

		COpenCLParameterFloatArray* paramrfltBank = new COpenCLParameterFloatArray();
		paramrfltBank->SetLibelle("f");
		paramrfltBank->SetValue((cl_context)clExecCtx.getContext().ptr(), (float*)f, flen, flag);
		vecParam.push_back(paramrfltBank);

		auto paramIntFltLen = new COpenCLParameterInt();
		paramIntFltLen->SetValue(flen);
		paramIntFltLen->SetLibelle("flen");
		vecParam.push_back(paramIntFltLen);

		auto paramIntStep = new COpenCLParameterInt();
		paramIntStep->SetValue(step);
		paramIntStep->SetLibelle("step");
		vecParam.push_back(paramIntStep);

		auto paramGM = new COpenCLParameterFloat();
		paramGM->SetValue(gm);
		paramGM->SetLibelle("gm");
		vecParam.push_back(paramGM);

		auto paramPkOut = new COpenCLParameterFloat();
		paramPkOut->SetValue(PkOut);
		paramPkOut->SetLibelle("PkOut");
		vecParam.push_back(paramPkOut);

		auto paramTrMul0 = new COpenCLParameterFloat();
		paramTrMul0->SetValue(TrMul0);
		paramTrMul0->SetLibelle("TrMul0");
		vecParam.push_back(paramTrMul0);

		/*
		// Récupération du code source du kernel
		wxString kernelSource = CLibResource::GetOpenCLUcharProgram("IDR_OPENCL_AVIR");
		cv::ocl::ProgramSource programSource(kernelSource);
		ocl::Context context = clExecCtx.getContext();

		// Compilation du kernel
		String errmsg;
		String buildopt = ""; // Options de compilation (vide par défaut)
		ocl::Program program = context.getProg(programSource, buildopt, errmsg);
		*/

		ocl::Program program = COpenCLContext::GetProgram("IDR_OPENCL_AVIR");
		ocl::Kernel kernel;
		kernel.create("doFilter2DLastStep", program);

		// Définition du premier argument (outBuffer)
		cl_int err = clSetKernelArg(static_cast<cl_kernel>(kernel.ptr()), 0, sizeof(cl_mem), &clBuffer);
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

		size_t global_work_size[2] = { static_cast<size_t>(src.size().width), static_cast<size_t>(src.size().height) };
		bool success = kernel.run(2, global_work_size, nullptr, true);
		if (!success)
		{
			throw std::runtime_error("Failed to execute OpenCL kernel.");
		}

		for (COpenCLParameter* parameter : vecParam)
		{
			if (!parameter->GetNoDelete())
			{
				delete parameter;
				parameter = nullptr;
			}
		}

		return paramOutput;
	}
}