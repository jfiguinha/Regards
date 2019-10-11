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
class IBitmapDisplay;

class CFilterWindowParam : public IMouseUpdate
{
public:
    CFilterWindowParam();
    virtual ~CFilterWindowParam();
    virtual int GetTypeFilter() = 0;
    virtual void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key) = 0;
	virtual CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer);
	virtual CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing);
	virtual void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput);
	static CImageLoadingFormat * RenderEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, int numFiltre);

protected:

	static void RotateExif(const int & orientation, CFiltreEffet * filtre);
	virtual void Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin);
	virtual void DrawingToPicture(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin);
	CRegardsBitmap * source;
};
