//
//  FilterWindowParam.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <FiltreEffectInterface.h>
#include <TreeElementValue.h>
#include <EffectParameter.h>
#include <MouseUpdate.h>
class CFiltreEffet;
class CRegardsBitmap;
class CImageLoadingFormat;

namespace Regards
{
	namespace Control
	{
		class CBitmapWndViewer;
	}
}

using namespace Regards::Control;

class CFilterWindowParam : public IMouseUpdate
{
public:
    CFilterWindowParam();
    virtual ~CFilterWindowParam();
    virtual int GetTypeFilter() = 0;
    virtual void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key) = 0;
	virtual CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer);
	virtual CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * dessing);
	virtual void ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput);
	static CImageLoadingFormat * RenderEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, int numFiltre);

protected:
	virtual void Drawing(wxMemoryDC * dc, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * m_cDessin);
	virtual void DrawingToPicture(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin);
	CRegardsBitmap * source;
};
