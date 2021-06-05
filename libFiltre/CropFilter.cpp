#include <header.h>
#include "CropFilter.h"
#include "EffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <Draw.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include <Crop.h>
using namespace Regards::Filter;

CCropFilter::CCropFilter()
{

}

CCropFilter::~CCropFilter()
{
}

CDraw* CCropFilter::GetDrawingPt()
{
	return new CCrop();
}

bool CCropFilter::IsOpenCLCompatible()
{
	return false;
}

void CCropFilter::SetCursor()
{
	::wxSetCursor(wxCursor(wxCURSOR_CROSS));
}

bool CCropFilter::SupportMouseClick()
{
	return true;
}

bool CCropFilter::SupportMouseSelection()
{
	return true;
}

int CCropFilter::GetTypeFilter()
{
	return IDM_CROP;
}

int CCropFilter::TypeApplyFilter()
{
	return 2;
}

int CCropFilter::GetNameFilter()
{
	return IDM_CROP;
}


wxString CCropFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLCROP", 1);
}

void CCropFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	this->source = source;
}

void CCropFilter::FilterChangeParam(CEffectParameter * effectParameter, CTreeElementValue * valueData, const wxString &key)
{

}

CImageLoadingFormat * CCropFilter::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	wxRect rcZoom;
	bitmapViewer->GetDessinPt()->GetPos(rcZoom);
	if (source != nullptr)
	{
		source->RotateExif(source->GetOrientation());

		source->VertFlipBuf();

		CRegardsBitmap * bitmapOut = source->CropBitmap(rcZoom.x, rcZoom.y, rcZoom.width, rcZoom.height);

		bitmapOut->VertFlipBuf();

		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(bitmapOut);
	}

	return imageLoad;
}

CImageLoadingFormat * CCropFilter::ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessin)
{


	return nullptr;
}

void CCropFilter::Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin)
{
	int hpos = bitmapViewer->GetHPos();
	int vpos = bitmapViewer->GetVPos();

	if (m_cDessin != nullptr)
		m_cDessin->Dessiner(dc, hpos, vpos, bitmapViewer->GetRatio(), wxColour(0, 0, 0), wxColour(0, 0, 0), wxColour(0, 0, 0), 2);
}

void CCropFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput)
{
	DrawingToPicture(effectParameter, bitmapViewer, filtreEffet, m_cDessin);
}

void CCropFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{

}

bool CCropFilter::NeedPreview()
{
	return true;
}


