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

		protected:
			virtual void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest){};
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
		//Filtre Niveau de Gris
		///////////////////////////////////////////////////////////////////////////////////////////
		class CGrayScale : public CFiltre
		{
		public:
			CGrayScale(){};
			~CGrayScale(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Niveau de Gris
		///////////////////////////////////////////////////////////////////////////////////////////
		class CSharpenMasking : public CFiltre
		{
		public:
			CSharpenMasking(const float &sharpness){ this->sharpness = sharpness; };
			~CSharpenMasking(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
			float sharpness;
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Noir et Blanc
		///////////////////////////////////////////////////////////////////////////////////////////
		class CBlackAndWhite : public CFiltre
		{
		public:
			CBlackAndWhite(){};
			~CBlackAndWhite(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Median
		///////////////////////////////////////////////////////////////////////////////////////////
		class CMedian : public CFiltre
		{
		public:
			CMedian(void){};
			~CMedian(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

		private:
			short quick_select(short * arr, int n);


			
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Negatif
		///////////////////////////////////////////////////////////////////////////////////////////
		class CNegatif : public CFiltre
		{
		public:
			CNegatif(){};
			~CNegatif(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Photo Filtre
		///////////////////////////////////////////////////////////////////////////////////////////
		class CPhotoFiltre : public CFiltre
		{
		public:
			CPhotoFiltre(CRgbaquad colorValue, int iDensity)
			{
				this->colorValue = colorValue;

				int m_iDiff = 255 - iDensity;
				this->diff = m_iDiff / 255.0;
				this->iDensity = float(iDensity) / 255.0;

				r = colorValue.GetFRed() * this->iDensity;
				g = colorValue.GetFGreen() * this->iDensity;
				b = colorValue.GetFBlue() * this->iDensity;
			};
			~CPhotoFiltre(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

		private:
			CRgbaquad colorValue;
			float r;
			float g;
			float b;
			float iDensity;
			float diff;
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre CRgbaquad
		///////////////////////////////////////////////////////////////////////////////////////////
		class CRgbFiltre : public CFiltre
		{
		public:
			CRgbFiltre(const int &m_lRValue, const int &m_lGValue, const int &m_lBValue)
			{
				this->m_lRValue = m_lRValue;
				this->m_lGValue = m_lGValue;
				this->m_lBValue = m_lBValue;
			};
			~CRgbFiltre(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

		private:
			int m_lRValue;
			int m_lGValue;
			int m_lBValue;
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Sepia
		///////////////////////////////////////////////////////////////////////////////////////////
		class CSepiaFiltre : public CFiltre
		{
		public:
			CSepiaFiltre()
			{
			};
			~CSepiaFiltre(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

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
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

			short * kernel;
			short Ksize;
			short Kfactor;
			short Koffset;
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Dilate
		///////////////////////////////////////////////////////////////////////////////////////////
		class CDilate : public CFiltre
		{
		public:

			CDilate(){};
			~CDilate(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Dilate
		///////////////////////////////////////////////////////////////////////////////////////////
		class CErode : public CFiltre
		{
		public:

			CErode(){};
			~CErode(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
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
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
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
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

			int size;
		};

		///////////////////////////////////////////////////////////////////////////////////////////
		//Filtre Noise
		///////////////////////////////////////////////////////////////////////////////////////////
		class CPosterize : public CFiltre
		{
		public:

			CPosterize(const int &level, const float &gamma = 0.6)
			{
				long _levels = max(2, min(16, level));
				_offset = 256 / _levels;

				for (auto i = 0; i < 256; i++)
					posterize[i] = (uint8_t)(i * _offset);

			};
			~CPosterize(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

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
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);

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
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
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
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
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
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
			uint8_t m_dTemp[256];
		};

		class CBilateral : public CFiltre
		{
		public:
			CBilateral(const int & fSize, const float & sigmaX, const float & sigmaP)
			{ 
				this->fSize = fSize; 
				this->sigmaX = sigmaX; 
				this->sigmaP = sigmaP; 
			};
			~CBilateral(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
			int fSize;
			float sigmaX;
			float sigmaP;
		};

		class CNlmeans : public CFiltre
		{
		public:
			CNlmeans( const int & FSIZE, const int & BSIZE,const float & SIGMA)
			{ 
				this->fSize = FSIZE; 
				this->bSize = BSIZE; 
				this->sigma = SIGMA; 
			};
			~CNlmeans(void){};

		protected:
			void PixelCompute(const int &x, const int &y, uint8_t * & pBitsSrc, uint8_t * & pBitsDest);
			int fSize;
			int bSize;
			float sigma;
		};
	}
}



