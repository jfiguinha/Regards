#include <header.h>
#include "OpenCLBm3D.h"
#include "OpenCLContext.h"
#include "OpenCLExecuteProgram.h"
#include "OpenCLProgram.h"
#include "utility.h"

#define ENABLE_PROFILING 0
#define UNROLL 1

#define DISTANCE_IN_SPARSE 0

#define BLOCK_SIZE 8
#define BLOCK_SIZE_HALF 4
#define BLOCK_SIZE_SQ 64

#define WINDOW_SIZE 39
#define WINDOW_SIZE_HALF 19

#define STEP_SIZE 3
// Multiple of STEP_SIZE
#define SPLIT_SIZE_X (3*STEP_SIZE)
#define SPLIT_SIZE_Y (3*STEP_SIZE)

#define WINDOW_STEP_SIZE_1 1
#define WINDOW_STEP_SIZE_2 1

#define MAX_BLOCK_COUNT_1 16
// 32 causes crash on CPU
#define MAX_BLOCK_COUNT_2 16

#define USE_KAISER_WINDOW 1

#define DCT_1D 0
#define HAAR_1D 1
#define TRANSFORM_METHOD_1D HAAR_1D

#define D_THRESHOLD_1 (3 * 2500)
#define D_THRESHOLD_2 (3 * 400)

using namespace Regards::OpenCL;

unsigned next_multiple(const unsigned x, const unsigned n) {
	return (x + n - 1) & ~(n - 1);
}

COpenCLBm3D::COpenCLBm3D(COpenCLContext * context): width(0), height(0), sizeoutput(0), opencl_type(0)
{
	openCLProgram = nullptr;
	bool useMemory = (context->GetDeviceType() == CL_DEVICE_TYPE_GPU) ? false : true;
	flag = useMemory ? CL_MEM_USE_HOST_PTR : CL_MEM_COPY_HOST_PTR;
	openCLProgram = nullptr;
	this->context = context;
	paramInput = nullptr;
	basicPicture = nullptr;
	wienerPicture = nullptr;
}

COpenCLBm3D::~COpenCLBm3D()
{
	if (paramInput != nullptr)
	{
		paramInput->Release();
		delete paramInput;
	}
	if (basicPicture != nullptr)
	{
		cl_int err;
		err = clReleaseMemObject(basicPicture);
		Error::CheckError(err);
		basicPicture = nullptr;
	}
	if (wienerPicture != nullptr)
	{
		cl_int err;
		err = clReleaseMemObject(wienerPicture);
		Error::CheckError(err);
		wienerPicture = nullptr;
	}
}



void COpenCLBm3D::InitData(cl_mem dataInput, int sizeInput, int width, int height)
{
	this->width = width;
	this->height = height;
	this->sizeoutput = sizeInput;
	paramInput = new COpenCLParameterClMem();
	paramInput->SetLibelle("input");
	((COpenCLParameterClMem *)paramInput)->SetValue(dataInput);
}

void COpenCLBm3D::StepOne(const float &sigma)
{
	const size_t ls[2] = { 16, 8 };
	const int gx_d = next_multiple((unsigned)ceil(width / (double)STEP_SIZE), ls[0]);
	const int gy_d = next_multiple((unsigned)ceil(height / (double)STEP_SIZE), ls[1]);
	const int tot_items_d = gx_d * gy_d;

	COpenCLParameterShortArray * similar_coords = new COpenCLParameterShortArray(true, CL_MEM_READ_WRITE);
	COpenCLParameterUCharArray * block_counts = new COpenCLParameterUCharArray(true, CL_MEM_READ_WRITE);

	const size_t block_counts_size = tot_items_d * sizeof(cl_uchar);
	unsigned char * blockValue = new unsigned char[block_counts_size];

	const size_t similar_coords_size = MAX_BLOCK_COUNT_2 * tot_items_d * sizeof(cl_short) * 2;

	short * shortValue = new short[similar_coords_size];

	//size_t max_size = max(block_counts_size, similar_coords_size);
	similar_coords->SetValue(context->GetContext(), shortValue, similar_coords_size, flag);
	block_counts->SetValue(context->GetContext(), blockValue, block_counts_size, flag);

	CalculDistanceStep1(similar_coords, block_counts, 0, 0);
	basicPicture = ExecuteBasicFilter(sigma, similar_coords, block_counts);

	if (similar_coords != nullptr)
	{
		similar_coords->Release();
		delete similar_coords;
	}

	if (block_counts != nullptr)
	{
		block_counts->Release();
		delete block_counts;
	}
}

void COpenCLBm3D::StepTwo(const float &sigma)
{
	const size_t ls[2] = { 16, 8 };
	const int gx_d = next_multiple((unsigned)ceil(width / (double)STEP_SIZE), ls[0]);
	const int gy_d = next_multiple((unsigned)ceil(height / (double)STEP_SIZE), ls[1]);
	const int tot_items_d = gx_d * gy_d;

	COpenCLParameterShortArray * similar_coords = new COpenCLParameterShortArray(true, CL_MEM_READ_WRITE);
	COpenCLParameterUCharArray * block_counts = new COpenCLParameterUCharArray(true, CL_MEM_READ_WRITE);

	const size_t block_counts_size = tot_items_d * sizeof(cl_uchar);
	unsigned char * blockValue = new unsigned char[block_counts_size];

	const size_t similar_coords_size = MAX_BLOCK_COUNT_2 * tot_items_d * sizeof(cl_short) * 2;

	short * shortValue = new short[similar_coords_size];

	//size_t max_size = max(block_counts_size, similar_coords_size);
	similar_coords->SetValue(context->GetContext(), shortValue, similar_coords_size, flag);
	block_counts->SetValue(context->GetContext(), blockValue, block_counts_size, flag);


	CalculDistanceStep2(basicPicture, similar_coords, block_counts, 0, 0);
	wienerPicture = ExecuteWienerFilter(sigma, basicPicture, similar_coords, block_counts);

	if (similar_coords != nullptr)
	{
		similar_coords->Release();
		delete similar_coords;
	}

	if (block_counts != nullptr)
	{
		block_counts->Release();
		delete block_counts;
	}
}

void COpenCLBm3D::ExecuteFilter(const float &sigma)
{
	StepOne(sigma);
	StepTwo(sigma);

}

cl_mem COpenCLBm3D::GetBasicImage()
{
	return basicPicture;
}

cl_mem COpenCLBm3D::GetWienerImage()
{
	return wienerPicture;
}

cl_mem COpenCLBm3D::ExecuteBasicFilter(const float &sigma, COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts)
{
	const size_t ls[2] = { 16, 8 };
	const int gx_d = next_multiple((unsigned)ceil(width / (double)STEP_SIZE), ls[0]);
	const int gy_d = next_multiple((unsigned)ceil(height / (double)STEP_SIZE), ls[1]);
	const int tot_items_d = gx_d * gy_d;

//	const size_t gx = next_multiple((unsigned)ceil(width / (double)SPLIT_SIZE_X), ls[0]);
//	const size_t gy = next_multiple((unsigned)ceil(height / (double)SPLIT_SIZE_Y), ls[1]);
//	const size_t tot_items = gx * gy;

	/*
	__kernel void bm3d_basic_filter(
	__global float * output,
	const __global float *input,
	const int width,
	const int height,
	const float fSigma,
	const __global int2 * block_count_similar_coords,
	const int global_size_x_d,
	const int tot_globals_d)
	*/

	cl_mem outputValue = nullptr;
	if (context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_BM3D");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			paramInput->SetLibelle("input");
			paramInput->SetNoDelete(true);
			vecParam.push_back(paramInput);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);

			COpenCLParameterFloat * paramSigma = new COpenCLParameterFloat();
			paramSigma->SetValue(sigma);
			paramSigma->SetLibelle("fSigma");
			vecParam.push_back(paramSigma);

			similar_coords->SetLibelle("similar_coords");
			similar_coords->SetNoDelete(true);
			vecParam.push_back(similar_coords);

			block_counts->SetLibelle("block_counts");
			block_counts->SetNoDelete(true);
			vecParam.push_back(block_counts);

			COpenCLParameterInt * paramGlobalSizeX = new COpenCLParameterInt();
			paramGlobalSizeX->SetValue(gx_d);
			paramGlobalSizeX->SetLibelle("global_size_x_d");
			vecParam.push_back(paramGlobalSizeX);

			COpenCLParameterInt * paramTot_globals_d = new COpenCLParameterInt();
			paramTot_globals_d->SetValue(tot_items_d);
			paramTot_globals_d->SetLibelle("tot_globals_d");
			vecParam.push_back(paramTot_globals_d);

			program->SetParameter(&vecParam, sizeoutput * sizeof(float), width, height, nullptr);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "bm3d_basic_filter");
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}


cl_mem COpenCLBm3D::ExecuteWienerFilter(const float &sigma, const cl_mem & basicPicture, COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts)
{
	const size_t ls[2] = { 16, 8 };
	const int gx_d = next_multiple((unsigned)ceil(width / (double)STEP_SIZE), ls[0]);
	const int gy_d = next_multiple((unsigned)ceil(height / (double)STEP_SIZE), ls[1]);
	const int tot_items_d = gx_d * gy_d;

//	const size_t gx = next_multiple((unsigned)ceil(width / (double)SPLIT_SIZE_X), ls[0]);
//	const size_t gy = next_multiple((unsigned)ceil(height / (double)SPLIT_SIZE_Y), ls[1]);
//	const size_t tot_items = gx * gy;

	/*
	__global float * output,
	const __global float * input,
	const __global float * basic,
	int width,
	int height,
	const float fSigma,
	const __global int2 * block_count_similar_coords,
	const int global_size_x_d,
	const int tot_globals_d
	*/

	cl_mem outputValue = nullptr;
	if (context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_BM3D");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			paramInput->SetLibelle("input");
			paramInput->SetNoDelete(true);
			vecParam.push_back(paramInput);

			COpenCLParameterClMem * parambasicPicture = new COpenCLParameterClMem();
			parambasicPicture->SetValue(basicPicture);
			parambasicPicture->SetLibelle("basic");
			parambasicPicture->SetNoDelete(true);
			vecParam.push_back(parambasicPicture);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);

			COpenCLParameterFloat * paramSigma = new COpenCLParameterFloat();
			paramSigma->SetValue(sigma);
			paramSigma->SetLibelle("fSigma");
			vecParam.push_back(paramSigma);

			similar_coords->SetLibelle("similar_coords");
			similar_coords->SetNoDelete(true);
			vecParam.push_back(similar_coords);

			block_counts->SetLibelle("block_counts");
			block_counts->SetNoDelete(true);
			vecParam.push_back(block_counts);

			COpenCLParameterInt * paramGlobalSizeX = new COpenCLParameterInt();
			paramGlobalSizeX->SetValue(gx_d);
			paramGlobalSizeX->SetLibelle("global_size_x_d");
			vecParam.push_back(paramGlobalSizeX);

			COpenCLParameterInt * paramTot_globals_d = new COpenCLParameterInt();
			paramTot_globals_d->SetValue(tot_items_d);
			paramTot_globals_d->SetLibelle("tot_globals_d");
			vecParam.push_back(paramTot_globals_d);

			program->SetParameter(&vecParam, sizeoutput * sizeof(float), width, height, nullptr);
			program->SetKeepOutput(true);
			program->ExecuteProgram(programCL->GetProgram(), "bm3d_wiener_filter");
			outputValue = program->GetOutput();
			delete program;
			vecParam.clear();
		}
	}
	return outputValue;
}


void COpenCLBm3D::CalculDistanceStep2(const cl_mem & basicPicture, COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts, int i, int j)
{
//	const int hard_threshold = D_THRESHOLD_1 * BLOCK_SIZE_SQ;
	const int wiener_threshold = D_THRESHOLD_2 * BLOCK_SIZE_SQ;
//	const int max_block_count_1 = MAX_BLOCK_COUNT_1;
	const int max_block_count_2 = MAX_BLOCK_COUNT_2;
//	const int window_step_size_1 = WINDOW_STEP_SIZE_1;
	const int window_step_size_2 = WINDOW_STEP_SIZE_2;

	size_t ls[2] = { 16, 8 };
	const size_t gx_d = next_multiple((unsigned)ceil(width / (double)STEP_SIZE), ls[0]);
	const size_t gy_d = next_multiple((unsigned)ceil(height / (double)STEP_SIZE), ls[1]);
//	const int tot_items_d = gx_d * gy_d;

	const size_t gx = next_multiple((unsigned)ceil(width / (double)SPLIT_SIZE_X), ls[0]);
	const size_t gy = next_multiple((unsigned)ceil(height / (double)SPLIT_SIZE_Y), ls[1]);
//	const size_t tot_items = gx * gy;

	size_t gs_d[2] = { gx_d, gy_d };
//	size_t gs[2] = { gx, gy };
	size_t offset[2] = { i*gx, j*gy };
	/*
	const int width,
	const int height,
	//__global short* similar_coords,
	//__global uchar* block_counts,
	const int threshold,
	const int max_block_count,
	const int window_step_size
	*/

//	cl_mem outputValue = nullptr;
	if (context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_BM3D");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			COpenCLParameterClMem * parambasicPicture = new COpenCLParameterClMem();
			parambasicPicture->SetValue(basicPicture);
			parambasicPicture->SetLibelle("input");
			parambasicPicture->SetNoDelete(true);
			vecParam.push_back(parambasicPicture);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);

			similar_coords->SetLibelle("similar_coords");
			similar_coords->SetNoDelete(true);
			vecParam.push_back(similar_coords);

			block_counts->SetLibelle("block_counts");
			block_counts->SetNoDelete(true);
			vecParam.push_back(block_counts);

			COpenCLParameterInt * paramThreshold = new COpenCLParameterInt();
			paramThreshold->SetValue(wiener_threshold);
			paramThreshold->SetLibelle("threshold");
			vecParam.push_back(paramThreshold);

			COpenCLParameterInt * paramBlockCount = new COpenCLParameterInt();
			paramBlockCount->SetValue(max_block_count_2);
			paramBlockCount->SetLibelle("max_block_count");
			vecParam.push_back(paramBlockCount);

			COpenCLParameterInt * paramWindowStepSize = new COpenCLParameterInt();
			paramWindowStepSize->SetValue(window_step_size_2);
			paramWindowStepSize->SetLibelle("window_step_size");
			vecParam.push_back(paramWindowStepSize);

			program->ExecuteProgram2D(programCL->GetProgram(), "calc_distances", &vecParam, offset, gs_d, ls);
			delete program;
			vecParam.clear();

		}
	}
}

void COpenCLBm3D::CalculDistanceStep1(COpenCLParameterShortArray * similar_coords, COpenCLParameterUCharArray * block_counts, int i, int j)
{
	const int hard_threshold = D_THRESHOLD_1 * BLOCK_SIZE_SQ;
//	const int wiener_threshold = D_THRESHOLD_2 * BLOCK_SIZE_SQ;
	const int max_block_count_1 = MAX_BLOCK_COUNT_1;
//	const int max_block_count_2 = MAX_BLOCK_COUNT_2;
	const int window_step_size_1 = WINDOW_STEP_SIZE_1;
//	const int window_step_size_2 = WINDOW_STEP_SIZE_2;

	size_t ls[2] = { 16, 8 };
	const size_t gx_d = next_multiple((unsigned)ceil(width / (double)STEP_SIZE), ls[0]);
	const size_t gy_d = next_multiple((unsigned)ceil(height / (double)STEP_SIZE), ls[1]);
//	const int tot_items_d = gx_d * gy_d;

	const size_t gx = next_multiple((unsigned)ceil(width / (double)SPLIT_SIZE_X), ls[0]);
	const size_t gy = next_multiple((unsigned)ceil(height / (double)SPLIT_SIZE_Y), ls[1]);
//	const size_t tot_items = gx * gy;

	size_t gs_d[2] = { gx_d, gy_d };
//	size_t gs[2] = { gx, gy };
	size_t offset[2] = { i*gx, j*gy };


	/*
	const int width,
	const int height,
	//__global short* similar_coords,
	//__global uchar* block_counts,
	const int threshold,
	const int max_block_count,
	const int window_step_size
	*/

//	cl_mem outputValue = nullptr;
	if (context != nullptr)
	{
		COpenCLProgram * programCL = context->GetProgram("IDR_OPENCL_BM3D");
		if (programCL != nullptr)
		{
			vector<COpenCLParameter *> vecParam;
			COpenCLExecuteProgram * program = new COpenCLExecuteProgram(context, flag);

			paramInput->SetLibelle("input");
			paramInput->SetNoDelete(true);
			vecParam.push_back(paramInput);

			COpenCLParameterInt * paramWidth = new COpenCLParameterInt();
			paramWidth->SetValue(width);
			paramWidth->SetLibelle("width");
			vecParam.push_back(paramWidth);

			COpenCLParameterInt * paramHeight = new COpenCLParameterInt();
			paramHeight->SetValue(height);
			paramHeight->SetLibelle("height");
			vecParam.push_back(paramHeight);

			similar_coords->SetLibelle("similar_coords");
			similar_coords->SetNoDelete(true);
			vecParam.push_back(similar_coords);

			block_counts->SetLibelle("block_counts");
			block_counts->SetNoDelete(true);
			vecParam.push_back(block_counts);

			COpenCLParameterInt * paramThreshold = new COpenCLParameterInt();
			paramThreshold->SetValue(hard_threshold);
			paramThreshold->SetLibelle("threshold");
			vecParam.push_back(paramThreshold);

			COpenCLParameterInt * paramBlockCount = new COpenCLParameterInt();
			paramBlockCount->SetValue(max_block_count_1);
			paramBlockCount->SetLibelle("max_block_count");
			vecParam.push_back(paramBlockCount);

			COpenCLParameterInt * paramWindowStepSize = new COpenCLParameterInt();
			paramWindowStepSize->SetValue(window_step_size_1);
			paramWindowStepSize->SetLibelle("window_step_size");
			vecParam.push_back(paramWindowStepSize);

			program->ExecuteProgram2D(programCL->GetProgram(), "calc_distances", &vecParam, offset, gs_d, ls);
			delete program;
			vecParam.clear();
		}
	}

}
