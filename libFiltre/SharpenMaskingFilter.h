#pragma once
#include "FilterWindowParam.h"

namespace Regards
{
	namespace Viewer
	{
		class CSharpenMaskingFilter : public CFilterWindowParam
		{
		public:
			CSharpenMaskingFilter();
			~CSharpenMaskingFilter();
			int GetTypeFilter();
			void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface){};
			void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface);
			void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key);
		private:
			wxString libelleSharpness;
		};
	}
}

