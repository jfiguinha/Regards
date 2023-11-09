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

namespace Regards::OpenGL
{
	class GLSLShader;
}

using namespace Regards::OpenGL;

class CFilterWindowParam : public IMouseUpdate
{
public:
	CFilterWindowParam();
	virtual ~CFilterWindowParam();

	virtual wxString GetFilterLabel() = 0;

	virtual int TypeApplyFilter()
	{
		return 3;
	};

	virtual CEffectParameter* GetDefaultEffectParameter()
	{
		return new CEffectParameter();
	};

	virtual CEffectParameter* GetEffectPointer()
	{
		return new CEffectParameter();
	}

	bool IsSourcePreview() override
	{
		return false;
	}

	virtual int GetNameFilter() = 0;
	virtual int GetTypeFilter() = 0;
	virtual bool NeedPreview() { return false; };

	virtual void RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
	{
	};
	virtual void Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
	                    IFiltreEffectInterface* filtreInterface) = 0;
	virtual void Filter(CEffectParameter* effectParameter, const wxString& filename,
	                    IFiltreEffectInterface* filtreInterface) = 0;
	virtual void FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
	                               const wxString& key) = 0;
	virtual CImageLoadingFormat* ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer);
	CImageLoadingFormat* ApplyMouseMoveEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
	                                          CDraw* dessing) override;
	void ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet,
	                        CDraw* dessing, int& widthOutput, int& heightOutput) override;
	void ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
	                              CFiltreEffet* filtreEffet, CDraw* m_cDessin) override;
	static CImageLoadingFormat* RenderEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
	                                         int numFiltre);
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

	void CancelPreview(IBitmapDisplay* bitmapViewer) override
	{
	};

protected:
	void ApplyExifToPoint(wxPoint& pt, int numExif, const int& width, const int& height);
	static void RotateExif(const int& orientation, CFiltreEffet* filtre);
	virtual void Drawing(wxMemoryDC* dc, IBitmapDisplay* bitmapViewer, CDraw* m_cDessin);
	virtual void DrawingToPicture(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
	                              CFiltreEffet* filtreEffet, CDraw* m_cDessin);
	cv::Mat source;
	wxString filename = "";
	int orientation;
	GLSLShader* m_pShader;
	static bool supportOpenCL;
};
