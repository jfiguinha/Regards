#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <opencv2/opencv.hpp>
#include <RGBAQuad.h>
using namespace std;


class CCudaComputeFilter
{
public:
	CCudaComputeFilter() {};
	~CCudaComputeFilter() {};
	void ApplyEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output);
	
protected:
	virtual void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) = 0;

	unsigned char* d_input, * d_output;
};

class CSepiaFilter : public CCudaComputeFilter
{
public:
	CSepiaFilter() {};
	~CSepiaFilter() {};
protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;
};

class CPhotoFilter : public CCudaComputeFilter
{
public:
	CPhotoFilter() {};
	~CPhotoFilter() {};
	void SetParameter(float intensity, const CRgbaquad& clValue)
	{
		this->intensity = intensity;
		this->clValue = clValue;
	}
protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;

	float intensity;
	CRgbaquad clValue;
};

class CMosaicFilter : public CCudaComputeFilter
{
public:
	CMosaicFilter() {};
	~CMosaicFilter() {};
	void SetParameter(const int & fTileSize)
	{
		this->fTileSize = fTileSize;
	}
protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;

	int fTileSize;

};

class CCudaSharpenMaskingFilter : public CCudaComputeFilter
{
public:
	CCudaSharpenMaskingFilter()
	{
	};

	~CCudaSharpenMaskingFilter() {};
	void SetParameter(const cv::cuda::GpuMat& gaussian,const float & sharpness)
	{
		this->sharpness = sharpness;
		gaussian_input = (uchar*)gaussian.ptr();
	}
protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;

	float sharpness;
	uchar* gaussian_input;

};

class CCudaMedianFilter : public CCudaComputeFilter
{
public:
	CCudaMedianFilter()
	{
	};

	~CCudaMedianFilter() {};

protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;

};

class CDilateFilter : public CCudaComputeFilter
{
public:
	CDilateFilter()
	{
	};

	~CDilateFilter() {};

protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;

};


class CErodeFilter : public CCudaComputeFilter
{
public:
	CErodeFilter()
	{
	};

	~CErodeFilter() {};

protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;

};


class CSolarizationFilter : public CCudaComputeFilter
{
public:
	CSolarizationFilter()
	{
	};

	~CSolarizationFilter() {};

	void SetParameter(const float& level)
	{
		this->level = level;
	}

protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;
	float level;
};

class CPosterizationFilter : public CCudaComputeFilter
{
public:
	CPosterizationFilter()
	{
	};

	~CPosterizationFilter() {};

	void SetParameter(const int& level)
	{
		this->level = level;
	}

protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;
	int level;
};

class CDistorsionFilter : public CCudaComputeFilter
{
public:
	CDistorsionFilter()
	{
	};

	~CDistorsionFilter() {};

	void SetParameter(const float& correctionRadius)
	{
		this->correctionRadius = correctionRadius;
	}

protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;
	float correctionRadius;
};

class CSoftenFilter : public CCudaComputeFilter
{
public:
	CSoftenFilter()
	{
	};

	~CSoftenFilter() {};

protected:
	void ExecuteEffect(const cv::cuda::GpuMat& input, cv::cuda::GpuMat& output) override;

};




