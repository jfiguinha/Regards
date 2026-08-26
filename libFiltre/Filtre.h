#pragma once
#include "RGBAQuad.h"
using namespace std;

;

namespace Regards::FiltreEffet
{
	class CFiltre
	{
	public:
		CFiltre();
		virtual ~CFiltre() = default;
		void SetParameter(cv::Mat& pBitmap, CRgbaquad color);
		void Compute();

		virtual void ProcessOpenCV(cv::Mat& image)
		{}


	protected:
		int bmWidth;
		int bmHeight;
		CRgbaquad color;
		cv::Mat pBitsSrc;
	};


	class CLUTFilter : public CFiltre
	{
	public:

		explicit CLUTFilter(cv::Mat lut)
			: lut_(std::move(lut))
		{}

	protected:

		void ProcessOpenCV(cv::Mat& image) override
		{
			cv::LUT(image, lut_, image);
		}

	private:

		cv::Mat lut_;
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Matrix Convolution 3x3
	///////////////////////////////////////////////////////////////////////////////////////////
	class CMatrixConvolution : public CFiltre
	{
	public:

		CMatrixConvolution(const short* kernel,
			int kernelSize,
			int kernelFactor = 0,
			int kernelOffset = 0);

		~CMatrixConvolution() override = default;

	protected:

		void ProcessOpenCV(cv::Mat& image) override;

	private:

		cv::Mat kernelMat;
		int offset = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Noise
	///////////////////////////////////////////////////////////////////////////////////////////
	class CNoise : public CFiltre
	{
	public:

		CNoise() = default;
		~CNoise() override = default;

	protected:

		void ProcessOpenCV(cv::Mat& image) override;

	private:

		float Noise2d(int x, int y);

		cv::Mat CreateNoise(const cv::Size& size);
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Noise
	///////////////////////////////////////////////////////////////////////////////////////////
	class CMosaic : public CFiltre
	{
	public:

		explicit CMosaic(int size)
			: blockSize(std::max(1, size))
		{}

		~CMosaic() override = default;

	protected:

		void ProcessOpenCV(cv::Mat& image) override;

	private:

		int blockSize;
	};

	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Noise
	///////////////////////////////////////////////////////////////////////////////////////////
	class CPosterize : public CFiltre
	{
	public:

		explicit CPosterize(int level)
		{
			BuildLUT(level);
		}

	protected:

		void ProcessOpenCV(cv::Mat& image) override
		{
			cv::LUT(image, lut, image);
		}

	private:

		cv::Mat lut;

		void BuildLUT(int level)
		{
			level = std::clamp(level, 2, 16);

			lut.create(1, 256, CV_8U);

			uchar* p = lut.ptr<uchar>();

			const double scale =
				255.0 / (level - 1);

			for (int i = 0; i < 256; i++)
			{
				p[i] = cv::saturate_cast<uchar>(
					std::round(
						std::round(i / scale) * scale
					)
				);
			}
		}
	};
	
	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Solarize
	///////////////////////////////////////////////////////////////////////////////////////////
	class CSolarize : public CFiltre
	{
	public:

		explicit CSolarize(int threshold)
		{
			lut.create(1, 256, CV_8U);

			uchar* p = lut.ptr<uchar>();

			for (int i = 0; i < 256; i++)
			{
				p[i] =
					(i > threshold)
					? 255 - i
					: i;
			}
		}


	protected:

		void ProcessOpenCV(cv::Mat& image) override
		{
			cv::LUT(image, lut, image);
		}

	private:

		cv::Mat lut;
	};
	///////////////////////////////////////////////////////////////////////////////////////////
	//Filtre Swirl
	class CSwirl : public CFiltre
	{
	public:

		CSwirl(float angle, float radius)
			: angle(angle),
			radius(radius)
		{}

	protected:

		void ProcessOpenCV(cv::Mat& image) override;

	private:

		void BuildMaps(const cv::Size& size,
			cv::Mat& mapX,
			cv::Mat& mapY);

		float angle;
		float radius;
	};
}
