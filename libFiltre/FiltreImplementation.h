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

			int GetNameFilter()
			{
				return IDM_FILTRE_SOFTEN;
			}


			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Soften();
			};
		};

		class CBrightnessAutoFilter : public CDefaultFilter
		{
		public:

			int GetNameFilter()
			{
				return IDM_BRIGHTNESSCONTRAST_AUTO;
			}

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

			bool IsOpenCLCompatible()
			{
				return false;
			}


			int GetNameFilter()
			{
				return IDM_HISTOGRAMNORMALIZE;
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

			int GetNameFilter()
			{
				return IDM_HISTOGRAMEQUALIZE;
			}

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

			int GetNameFilter()
			{
				return IDM_ROTATE90;
			}

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

			int GetNameFilter()
			{
				return IDM_ROTATE270;
			}

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

			int GetNameFilter()
			{
				return IDM_FLIPVERTICAL;
			}

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

			int GetNameFilter()
			{
				return IDM_FLIPHORIZONTAL;
			}

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

			int GetNameFilter()
			{
				return IDM_FILTREANTIBRUIT;
			}

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Median();
			};
		};

		class CErodeFilter : public CDefaultFilter
		{
		public:

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			int GetNameFilter()
			{
				return IDM_FILTRE_ERODE;
			}

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Erode();
			};
		};

		class CDilateFilter : public CDefaultFilter
		{
		public:

			int GetTypeFilter()
			{
				return SPECIAL_EFFECT;
			};

			int GetNameFilter()
			{
				return IDM_FILTRE_DILATE;
			}

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Dilate();
			};
		};

		class CSharpenFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter()
			{
				return IDM_FILTRE_SHARPEN;
			}

			virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
			{
				if (filtreEffet != nullptr)
					filtreEffet->Sharpen();
			};
		};

		class CSharpenStrongFilter : public CDefaultFilter
		{
		public:

			int GetNameFilter()
			{
				return IDM_FILTRE_SHARPENSTRONG;
			}

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

			int GetNameFilter()
			{
				return IDM_FILTRE_MOSAIQUE;
			}

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

			int GetNameFilter()
			{
				return IDM_FILTRENOISE;
			}

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

			int GetNameFilter()
			{
				return IDM_FILTRE_EMBOSS;
			}

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

			int GetNameFilter()
			{
				return IDM_GREY_LEVEL;
			}

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

			int GetNameFilter()
			{
				return IDM_IMAGE_SEPIA;
			}

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

			int GetNameFilter()
			{
				return IDM_BLACKANDWHITE;
			}

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

			int GetNameFilter()
			{
				return IDM_FILTRE_EDGE;
			}

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

			int GetNameFilter()
			{
				return IDM_NEGATIF;
			}

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

			int GetNameFilter()
			{
				return IDM_REDEYE;
			}

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