#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
	namespace Filter
	{
		class CSharpenMaskingFilter : public CFilterWindowParam
		{
		public:
			CSharpenMaskingFilter();
			~CSharpenMaskingFilter();
			bool IsOpenGLCompatible();
			int TypeApplyFilter();
			int GetNameFilter();
			int GetTypeFilter();
			wxString GetFilterLabel();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
			void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
			void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
			void ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID);
			void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview);
			bool NeedPreview();
			CEffectParameter* GetEffectPointer();
			CEffectParameter* GetDefaultEffectParameter();
		private:
			wxString libelleSharpness;
		};
	}
}

