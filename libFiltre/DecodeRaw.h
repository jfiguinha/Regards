#pragma once
#include "FilterWindowParam.h"
#include <DecodeRawPicture.h>
class CMetadata;

namespace Regards
{
	namespace Viewer
	{
		class CDecodeRaw: public CFilterWindowParam
		{
		public:
			CDecodeRaw(void);
			~CDecodeRaw(void);
            int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
            void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
            void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, CBitmapWndViewer * bitmapViewer);

        private:

			void AddMetadataElement(vector<CMetadata> & element, wxString value, int key);
			void ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput);
			int orientation = 0;
            wxString libellebright;
            wxString libellehighlight;
			wxString libellethreshold;
			wxString libelleuse_auto_wb;
			wxString libelleuse_camera_wb;
			wxString libelleuse_halfsize;
			wxString libelleuse_cameramatrix;
			wxString libelleuse_aberRedEnable;
			wxString libelleuse_aberRed;
			wxString libelleuse_aberGreenEnable;
			wxString libelleuse_aberGreen;
			wxString libelleuse_UserMultiRed;
			wxString libelleuse_UserMultiGreen;
			wxString libelleuse_UserMultiBlue;
			wxString libelleuse_UserMultiOther;
			wxString libelleuse_UserFlip;
			wxString libelleuse_UserInterpolation;
			wxString libelleuse_UserBlack;
			wxString libelleuse_UserBlackChannelRed;
			wxString libelleuse_UserBlackChannelGreen;
			wxString libelleuse_UserBlackChannelBlue;
			wxString libelleuse_UserBlackChannelOther;
			wxString libelleuse_UserSaturation;
			wxString libelleuse_med_passes;
			wxString libelleuse_noautobright;
			wxString libelleauto_bright_thr;
			wxString libelleadjust_maximum_thr;
			wxString libelleuse_fuji_rotate;
			wxString libellegreen_matching;
			wxString libelledcb_iterations;
			wxString libelledcb_enhance_fl;
			wxString libellefbdd_noiserd;
			wxString libelleeeci_refine;
			wxString libellees_med_passes;
			wxString libelleca_correc;
			wxString libellecared;
			wxString libellecablue;
			wxString libellecfaline; 
			wxString libellelinenoise;
			wxString libellecfa_clean; 
			wxString libellelclean;
			wxString libellecclean;
			wxString libellecfa_green; 
			wxString libellegreen_thresh;
			wxString libelleexp_correc; 
			wxString libelleexp_shift;
			wxString libelleexp_preser;
			wxString libellewf_debanding; 
			wxString libellewf_deband_tresholdRed;
			wxString libellewf_deband_tresholdGreen;
			wxString libellewf_deband_tresholdBlue;
			wxString libellewf_deband_tresholdOther;
			bool firstUpdate;
			CDecodeRawPicture * rawDecoder;
		};
	}
}

