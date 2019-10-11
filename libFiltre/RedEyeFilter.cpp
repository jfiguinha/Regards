#include <header.h>
#include "RedEyeFilter.h"
#include "EffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <Draw.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
using namespace Regards::Viewer;

CRedEyeFilter::CRedEyeFilter()
{

}

CRedEyeFilter::~CRedEyeFilter()
{
}


int CRedEyeFilter::GetTypeFilter()
{
	return IDM_REDEYE;
}

void CRedEyeFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	this->source = source;
}

void CRedEyeFilter::FilterChangeParam(CEffectParameter * effectParameter, CTreeElementValue * valueData, const wxString &key)
{

}

CImageLoadingFormat * CRedEyeFilter::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	wxRect rcZoom;
	bitmapViewer->GetDessinPt()->GetPos(rcZoom);
	if (source != nullptr)
	{
		source->RotateExif(source->GetOrientation());
		source->VertFlipBuf();
		CImageLoadingFormat image(false);
		image.SetPicture(source);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);

		filtre->RedEye(rcZoom);

		imageLoad = new CImageLoadingFormat();
		CRegardsBitmap * bitmap = filtre->GetBitmap(true);
		bitmap->VertFlipBuf();
		imageLoad->SetPicture(bitmap);
		delete filtre;
	}

	return imageLoad;
}

CImageLoadingFormat * CRedEyeFilter::ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessin)
{
	return nullptr;
}

void CRedEyeFilter::Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin)
{
	int hpos = bitmapViewer->GetHPos();
	int vpos = bitmapViewer->GetVPos();

	if (m_cDessin != nullptr)
		m_cDessin->Dessiner(dc, hpos, vpos, bitmapViewer->GetRatio(), wxColour(0, 0, 0), wxColour(0, 0, 0), wxColour(0, 0, 0), 2);
}

void CRedEyeFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput)
{
	DrawingToPicture(effectParameter, bitmapViewer, filtreEffet, m_cDessin);
}