#pragma once
#include "RGBAQuad.h"
using namespace std;

class CRegardsBitmap;

namespace Regards
{
	namespace FiltreEffet
	{
		class CFiltre
		{
		public:
			CFiltre();
			virtual ~CFiltre();
			void SetParameter(CRegardsBitmap * pBitmap, CRgbaquad color);
			void Compute();
			virtual void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest) {};

		protected:
			
			int GetPosition(const int &x, const int &y);
			int bmWidth;
			int bmHeight;
			CRgbaquad color;
			CRegardsBitmap * pBitmap;
		private:

			
			int bitmapWidthSize;
			int pictureSize;
		};


		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Matrix Convolution 3x3
		///////////////////////////////////////////////////////////////////////////////////////////
		class CMatrixConvolution : public CFiltre
		{
		public:


			CMatrixConvolution(short * kernel, short Ksize, short Kfactor, short Koffset)
			{
				this->kernel = kernel;
				this->Ksize = Ksize;
				this->Kfactor = Kfactor;
				this->Koffset = Koffset;
			};
			~CMatrixConvolution(void){};

		protected:
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);

			short * kernel;
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

			CNoise(){};
			~CNoise(void){};

		protected:
			float Noise2d(int x, int y);
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Noise
		///////////////////////////////////////////////////////////////////////////////////////////
		class CMosaic : public CFiltre
		{
		public:

			CMosaic(const int &size)
			{
				this->size = size;
			};
			~CMosaic(void){};

		protected:
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);

			int size;
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Noise
		///////////////////////////////////////////////////////////////////////////////////////////
		class CPosterize : public CFiltre
		{
		public:
			explicit CPosterize(const int &level, const float &gamma = 0.6): gammaFactor(0)
			{
				long _levels = max(2, min(16, level));
				_offset = 256 / _levels;

				for (auto i = 0; i < 256; i++)
					posterize[i] = (uint8_t)(i * _offset);
			}
			;
			~CPosterize(void){};

		protected:
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);

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

			CSolarize(const int &threshold)
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
			~CSolarize(void){};

		protected:
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);

			uint8_t solarize[256];
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Swirl
		///////////////////////////////////////////////////////////////////////////////////////////
		class CSwirl : public CFiltre
		{
		public:

			struct FLOATPOINT{
				float x;
				float y;
			};

			CSwirl(float angle, float radius)
			{
				this->angle = angle;
				this->radius = radius;
			};
			~CSwirl(void){};

		protected:
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);
			float EuclideanDist(FLOATPOINT p);
			float EuclideanDist(FLOATPOINT p, FLOATPOINT q);
			float DotProduct(FLOATPOINT p, FLOATPOINT q);
			wxPoint PostFX(int x, int y, int width, int height, float radius, float angleDegree);

			float angle;
			float radius;
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Contrast
		///////////////////////////////////////////////////////////////////////////////////////////
		class CContrast : public CFiltre
		{
		public:

			CContrast(double contrast, uint8_t offset)
			{
				//double m_contrast = contrast;
				double csupp = contrast * ((double)offset - 256.0) + 128.0;

				for (auto i = 0; i< 256; i++)
				{
					int value = contrast * i + csupp;

					if (value < 0)
						value = 0;
					else if (value > 255)
						value = 255;

					m_dTemp[i] = value;
				}
			};
			~CContrast(void){};

		protected:
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);
			uint8_t m_dTemp[256];

		};
		
		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Lightness
		///////////////////////////////////////////////////////////////////////////////////////////
		class CLightness : public CFiltre
		{
		public:

			CLightness(const double &factor)
			{
				int nChange = 2.55 * factor;

				if (nChange > 255)
					nChange = 255;
				else if (nChange < -255)
					nChange = -255;

				for (auto i = 0; i< 256; i++)
				{
					int value = i + nChange;

					if (value < 0)
						value = 0;
					if (value > 255)
						value = 255;

					m_dTemp[i] = value;
				}
			};
			~CLightness(void){};

		protected:
			void PixelCompute(const int &x, const int &y, const cv::Mat & pBitsSrc, cv::Mat & pBitsDest);
			uint8_t m_dTemp[256];
		};

	}
}



