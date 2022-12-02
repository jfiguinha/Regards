#pragma once
#include "FilterWindowParam.h"
#include "LibResource.h"
#include <effect_id.h>
namespace Regards
{
	namespace Filter
	{
		class CDefaultFilter : public CFilterWindowParam
		{
		public:
			CDefaultFilter()
			{
			};

			~CDefaultFilter() override
			{
			};

			void Filter(CEffectParameter* effectParameter, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override
			{
			};

			void Filter(CEffectParameter* effectParameter, cv::Mat & source, const wxString& filename,
			            IFiltreEffectInterface* filtreInterface) override
			{
			};

			void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
			                       const wxString& key) override
			{
			};

			int GetTypeFilter() override
			{
				return CONVOLUTION_EFFECT;
			};

			bool IsOpenCLCompatible() override
			{
				return true;
			}
		};

		class CSoftenFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_FILTRE_SOFTEN;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterSoften", 1);
			}


			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Soften();
			};
		};

		class CBrightnessAutoFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_BRIGHTNESSCONTRAST_AUTO;
			}

			int GetTypeFilter() override
			{
				return HISTOGRAM_EFFECT;
			};

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLBRIGHTNESSCONTRASTAUTO", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->BrightnessAndContrastAuto(0);
			};
		};

		class CHistogramNormalizeFilter : public CDefaultFilter
		{
		public:
			bool IsOpenCLCompatible() override
			{
				return false;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLHistogramNormalize", 1);
			}

			int GetNameFilter() override
			{
				return IDM_HISTOGRAMNORMALIZE;
			}

			int GetTypeFilter() override
			{
				return HISTOGRAM_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->HistogramNormalize();
			};
		};

		class CHistogramEqualizeFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_HISTOGRAMEQUALIZE;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLHistogramEqualize", 1);
			}

			bool IsOpenCLCompatible() override
			{
				return false;
			}

			int GetTypeFilter() override
			{
				return HISTOGRAM_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->HistogramNormalize();
			};
		};

		class CRotate90Filter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_ROTATE90;
			}

			int GetTypeFilter() override
			{
				return ROTATE_EFFECT; //return HISTOGRAM_EFFECT;
			};

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLROTATE90", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Rotate90();
			};
		};

		class CRotate270Filter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_ROTATE270;
			}

			int GetTypeFilter() override
			{
				return ROTATE_EFFECT;
			};

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLROTATE270", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Rotate270();
			};
		};

		class CFlipVerticalFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_FLIPVERTICAL;
			}

			int GetTypeFilter() override
			{
				return ROTATE_EFFECT;
			};

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLFLIPV", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->FlipVertical();
			};
		};

		class CFlipHorizontalFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_FLIPHORIZONTAL;
			}

			int GetTypeFilter() override
			{
				return ROTATE_EFFECT;
			};

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLFLIPH", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->FlipHorizontal();
			};
		};

		class CMedianFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_FILTREANTIBRUIT;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterMedian", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Median();
			};
		};

		class CErodeFilter : public CDefaultFilter
		{
		public:
			int GetTypeFilter() override
			{
				return SPECIAL_EFFECT;
			};

			int GetNameFilter() override
			{
				return IDM_FILTRE_ERODE;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterErode", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Erode();
			};
		};

		class CDilateFilter : public CDefaultFilter
		{
		public:
			int GetTypeFilter() override
			{
				return SPECIAL_EFFECT;
			};

			int GetNameFilter() override
			{
				return IDM_FILTRE_DILATE;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterDilate", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Dilate();
			};
		};

		class CSharpenFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_FILTRE_SHARPEN;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterSharpen", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Sharpen();
			};
		};

		class CSharpenStrongFilter : public CDefaultFilter
		{
		public:
			int GetNameFilter() override
			{
				return IDM_FILTRE_SHARPENSTRONG;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterSharpenStrong", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->SharpenStrong();
			};
		};

/*
		class CMosaicFilter : public CDefaultFilter
		{
		public:
			CMosaicFilter()
			{
			};

			~CMosaicFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_FILTRE_MOSAIQUE;
			}


			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterMosaic", 1);
			}

			int GetTypeFilter() override
			{
				return SPECIAL_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->FiltreMosaic();
			};
		};
*/

		class CNoiseFilter : public CDefaultFilter
		{
		public:
			CNoiseFilter()
			{
			};

			~CNoiseFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_FILTRENOISE;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterNoise", 1);
			}

			int GetTypeFilter() override
			{
				return SPECIAL_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Noise();
			};
		};

		class CEmbossFilter : public CDefaultFilter
		{
		public:
			CEmbossFilter()
			{
			};

			~CEmbossFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_FILTRE_EMBOSS;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterEmboss", 1);
			}

			int GetTypeFilter() override
			{
				return SPECIAL_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Emboss();
			};
		};

		class CGrayLevelFilter : public CDefaultFilter
		{
		public:
			CGrayLevelFilter()
			{
			};

			~CGrayLevelFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_GREY_LEVEL;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterGrey", 1);
			}

			int GetTypeFilter() override
			{
				return COLOR_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->NiveauDeGris();
			};
		};

		class CSepiaFilter : public CDefaultFilter
		{
		public:
			CSepiaFilter()
			{
			};

			~CSepiaFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_IMAGE_SEPIA;
			}

			int GetTypeFilter() override
			{
				return COLOR_EFFECT;
			};

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterSepia", 1);
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Sepia();
			};
		};

		class CNoirEtBlancFilter : public CDefaultFilter
		{
		public:
			CNoirEtBlancFilter()
			{
			};

			~CNoirEtBlancFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_BLACKANDWHITE;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterBlack", 1);
			}

			int GetTypeFilter() override
			{
				return COLOR_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->NoirEtBlanc();
			};
		};

		class CEdgeFilter : public CDefaultFilter
		{
		public:
			CEdgeFilter()
			{
			};

			~CEdgeFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_FILTRE_EDGE;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterEdge", 1);
			}

			int GetTypeFilter() override
			{
				return SPECIAL_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->FiltreEdge();
			};
		};

		class CNegatifFilter : public CDefaultFilter
		{
		public:
			CNegatifFilter()
			{
			};

			~CNegatifFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_NEGATIF;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterNegatif", 1);
			}

			int GetTypeFilter() override
			{
				return COLOR_EFFECT;
			};

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->Negatif();
			};
		};

		class CRedEyeFilter : public CDefaultFilter
		{
		public:
			CRedEyeFilter()
			{
			};

			~CRedEyeFilter() override
			{
			};

			int GetNameFilter() override
			{
				return IDM_REDEYE;
			}

			wxString GetFilterLabel() override
			{
				return CLibResource::LoadStringFromResource("LBLfilterRedEye", 1);
			}

			bool IsOpenCLCompatible() override
			{
				return false;
			}

			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
			                  const bool& preview) override
			{
				if (filtreEffet != nullptr)
					filtreEffet->RedEye();
			};
		};
	}
}
