//
//  FilterWindowParam.hpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <FiltreEffectInterface.h>
#include <EffectParameter.h>
#include <MouseUpdate.h>

class CFiltreEffet;
class CRegardsBitmap;
class CImageLoadingFormat;
class IBitmapDisplay;

namespace Regards{namespace OpenGL{
class GLSLShader;
}
}
using namespace Regards::OpenGL;

class CFilterWindowParam : public IMouseUpdate
{
public:
    CFilterWindowParam();
    virtual ~CFilterWindowParam();

	virtual wxString GetFilterLabel() = 0;

	virtual int TypeApplyFilter() {
		return 3;
	};
	virtual CEffectParameter * GetDefaultEffectParameter() {
		return new CEffectParameter();
	};
	virtual CEffectParameter * GetEffectPointer()
	{
		return new CEffectParameter();
	}
	virtual bool IsSourcePreview()
	{
		return false;
	}
	virtual int GetNameFilter() = 0;
    virtual int GetTypeFilter() = 0;
	virtual bool NeedPreview() { return false; };
	virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview) {};
    virtual void Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface) = 0;
    virtual void FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key) = 0;
	virtual CImageLoadingFormat * ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer);
	virtual CImageLoadingFormat * ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing);
	virtual void ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int& widthOutput, int& heightOutput);
	virtual void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin);
	static CImageLoadingFormat * RenderEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, int numFiltre);
	virtual void ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID);
	virtual void DisableOpenGLShader();
	virtual bool IsOpenGLCompatible();
	virtual bool IsOpenCLCompatible();
	virtual bool SupportMouseSelection();
	virtual bool SupportMouseClick();
	virtual void SetCursor();
	static void InitFilterOpenCLCompatible();
    //static void SetOpenCLCompatible(const bool & openCLCompatible);
	virtual CDraw* GetDrawingPt()
	{
		return nullptr;
	}

	void CancelPreview(IBitmapDisplay* bitmapViewer) {};
protected:

	static void RotateExif(const int & orientation, CFiltreEffet * filtre);
	virtual void Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin);
	virtual void DrawingToPicture(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin);
	CRegardsBitmap * source;
	Regards::OpenGL::GLSLShader * m_pShader;
	static bool supportOpenCL;
};
