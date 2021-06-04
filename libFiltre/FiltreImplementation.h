#pragma once
#include "FilterWindowParam.h"
#include <Metadata.h>
namespace Regards
{
	namespace Filter
	{
		class CDefaultFilter : public CFilterWindowParam
		{
		public:
			CDefaultFilter() {};
			~CDefaultFilter() {};
			
			virtual void Filter(CEffectParameter* effectParameter, const wxString& filename, IFiltreEffectInterface* filtreInterface) {};
			
			virtual void Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface) {};
			
			virtual void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key) {};
			
			virtual int GetTypeFilter()
			{ 
				return CONVOLUTION_EFFECT; 
			};

			virtual bool IsOpenCLCompatible()
			{
				return true;
			}
		};

		class CSoftenFilter : public CDefaultFilter
		{
		public:
			CSoftenFilter() {};
			~CSoftenFilter() {};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Soften();
			};
		};

		class CBrightnessAutoFilter : public CDefaultFilter
		{
		public:
			CBrightnessAutoFilter() {};
			~CBrightnessAutoFilter() {};

			int GetTypeFilter()
			{
				return HISTOGRAM_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->BrightnessAndContrastAuto(0);
			};
		};

		class CHistogramNormalizeFilter : public CDefaultFilter
		{
		public:
			CHistogramNormalizeFilter() {};
			~CHistogramNormalizeFilter() {};

			bool IsOpenCLCompatible()
			{
				return false;
			}

			int GetTypeFilter()
			{
				return HISTOGRAM_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->HistogramNormalize();
			};
		};

		class CHistogramEqualizeFilter : public CDefaultFilter
		{
		public:
			CHistogramEqualizeFilter() {};
			~CHistogramEqualizeFilter() {};

			bool IsOpenCLCompatible()
			{
				return false;
			}

			int GetTypeFilter()
			{
				return HISTOGRAM_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->HistogramNormalize();
			};
		};

		class CRotate90Filter : public CDefaultFilter
		{
		public:
			CRotate90Filter() {};
			~CRotate90Filter() {};

			int GetTypeFilter()
			{
				return ROTATE_EFFECT; //return HISTOGRAM_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Rotate90();
			};
		};

		class CRotate270Filter : public CDefaultFilter
		{
		public:
			CRotate270Filter() {};
			~CRotate270Filter() {};

			int GetTypeFilter()
			{
				return ROTATE_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Rotate270();
			};
		};

		class CFlipVerticalFilter : public CDefaultFilter
		{
		public:
			CFlipVerticalFilter() {};
			~CFlipVerticalFilter() {};

			int GetTypeFilter()
			{
				return ROTATE_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->FlipVertical();
			};
		};

		class CFlipHorizontalFilter : public CDefaultFilter
		{
		public:
			CFlipHorizontalFilter() {};
			~CFlipHorizontalFilter() {};

			int GetTypeFilter()
			{
				return ROTATE_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->FlipHorizontal();
			};
		};

		class CMedianFilter : public CDefaultFilter
		{
		public:
			CMedianFilter() {};
			~CMedianFilter() {};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Median();
			};
		};

		class CErodeFilter : public CDefaultFilter
		{
		public:
			CErodeFilter() {};
			~CErodeFilter() {};

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Erode();
			};
		};

		class CDilateFilter : public CDefaultFilter
		{
		public:
			CDilateFilter() {};
			~CDilateFilter() {};

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Dilate();
			};
		};

		class CSharpenFilter : public CDefaultFilter
		{
		public:
			CSharpenFilter() {};
			~CSharpenFilter() {};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Sharpen();
			};
		};

		class CSharpenStrongFilter : public CDefaultFilter
		{
		public:
			CSharpenStrongFilter() {};
			~CSharpenStrongFilter() {};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->SharpenStrong();
			};
		};

		class CMosaicFilter : public CDefaultFilter
		{
		public:
			CMosaicFilter() {};
			~CMosaicFilter() {};

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->FiltreMosaic();
			};
		};

		class CNoiseFilter : public CDefaultFilter
		{
		public:
			CNoiseFilter() {};
			~CNoiseFilter() {};

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Noise();
			};
		};

		class CEmbossFilter : public CDefaultFilter
		{
		public:
			CEmbossFilter() {};
			~CEmbossFilter() {};

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Emboss();
			};
		};

		class CGrayLevelFilter : public CDefaultFilter
		{
		public:
			CGrayLevelFilter() {};
			~CGrayLevelFilter() {};

			int GetTypeFilter()
			{
				return COLOR_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->NiveauDeGris();
			};
		};

		class CSepiaFilter : public CDefaultFilter
		{
		public:
			CSepiaFilter() {};
			~CSepiaFilter() {};

			int GetTypeFilter()
			{
				return COLOR_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Sepia();
			};
		};

		class CNoirEtBlancFilter : public CDefaultFilter
		{
		public:
			CNoirEtBlancFilter() {};
			~CNoirEtBlancFilter() {};

			int GetTypeFilter()
			{
				return COLOR_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->NoirEtBlanc();
			};
		};

		class CEdgeFilter : public CDefaultFilter
		{
		public:
			CEdgeFilter() {};
			~CEdgeFilter() {};

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->FiltreEdge();
			};
		};

		class CNegatifFilter : public CDefaultFilter
		{
		public:
			CNegatifFilter() {};
			~CNegatifFilter() {};

			int GetTypeFilter()
			{
				return COLOR_EFFECT;
			};

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Negatif();
			};
		};

		class CRedEyeFilter : public CDefaultFilter
		{
		public:
			CRedEyeFilter() {};
			~CRedEyeFilter() {};

			bool IsOpenCLCompatible()
			{
				return false;
			}

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->RedEye();
			};
		};
	}
}