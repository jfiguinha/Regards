#pragma once
#include "RGBAQuad.h"
using namespace std;

class CRegardsBitmap;

namespace Regards::FiltreEffet
{
	class CFiltre
	{
	public:
		CFiltre();
		virtual ~CFiltre();
		void SetParameter(cv::Mat& pBitmap, CRgbaquad color);
		void Compute();

		virtual void PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest)
		{
		};

	protected:
		int bmWidth;
		int bmHeight;
		CRgbaquad color;
		cv::Mat pBitsSrc;
	};


	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Matrix Convolution 3x3
	///////////////////////////////////////////////////////////////////////////////////////////
	class CMatrixConvolution : public CFiltre
	{
	public:
		CMatrixConvolution(short* kernel, short Ksize, short Kfactor, short Koffset)
		{
			this->kernel = kernel;
			this->Ksize = Ksize;
			this->Kfactor = Kfactor;
			this->Koffset = Koffset;
		};

		~CMatrixConvolution(void) override
		{
		};

	protected:
		void PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest) override;

		short* kernel;
		short Ksize;
		short Kfactor;
		short Koffset;
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Noise
	///////////////////////////////////////////////////////////////////////////////////////////
	class CNoise : public CFiltre
	{
	public:
		CNoise()
		{
		};

		~CNoise(void) override
		{
		};

	protected:
		float Noise2d(int x, int y);
		void PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest) override;
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Noise
	///////////////////////////////////////////////////////////////////////////////////////////
	class CMosaic : public CFiltre
	{
	public:
		CMosaic(const int& size)
		{
			this->size = size;
		};

		~CMosaic(void) override
		{
		};

	protected:
		void PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest) override;

		int size;
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Noise
	///////////////////////////////////////////////////////////////////////////////////////////
	class CPosterize : public CFiltre
	{
	public:
		explicit CPosterize(const int& level, const float& gamma = 0.6): gammaFactor(0)
		{
			long _levels = max(2, min(16, level));
			_offset = 256 / _levels;

			for (auto i = 0; i < 256; i++)
				posterize[i] = static_cast<uint8_t>(i * _offset);
		}
		;

		~CPosterize(void) override
		{
		};

	protected:
		void PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest) override;

		long _offset;
		float posterize[256];
		float gammaFactor;
	};


	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Solarize
	///////////////////////////////////////////////////////////////////////////////////////////
	class CSolarize : public CFiltre
	{
	public:
		CSolarize(const int& threshold)
		{
			//long _threshold = max(0, min(255, threshold));

			for (auto i = 0; i < 256; i++)
			{
				if (i > threshold)
					solarize[i] = 255 - i;
				else
					solarize[i] = i;
			}
		};

		~CSolarize(void) override
		{
		};

	protected:
		void PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest) override;

		uint8_t solarize[256];
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Swirl
	///////////////////////////////////////////////////////////////////////////////////////////
	class CSwirl : public CFiltre
	{
	public:
		struct FLOATPOINT
		{
			float x;
			float y;
		};

		CSwirl(float angle, float radius)
		{
			this->angle = angle;
			this->radius = radius;
		};

		~CSwirl(void) override
		{
		};

	protected:
		void PixelCompute(const int& x, const int& y, const cv::Mat& pBitsSrc, cv::Mat& pBitsDest) override;
		float EuclideanDist(FLOATPOINT p);
		float EuclideanDist(FLOATPOINT p, FLOATPOINT q);
		float DotProduct(FLOATPOINT p, FLOATPOINT q);
		wxPoint PostFX(int x, int y, int width, int height, float radius, float angleDegree);

		float angle;
		float radius;
	};
}
