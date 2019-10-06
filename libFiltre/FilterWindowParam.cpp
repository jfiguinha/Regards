#include <header.h>
//
//  FilterWindowParam.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterWindowParam.h"
#include <BitmapWndViewer.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <PiccanteFilter.h>
CFilterWindowParam::CFilterWindowParam()
{
    
}

CFilterWindowParam::~CFilterWindowParam()
{
    
}

CImageLoadingFormat * CFilterWindowParam::ApplyMouseMoveEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * dessing)
{
	return nullptr;
}

void CFilterWindowParam::Drawing(wxMemoryDC * dc, Regards::Control::CBitmapWndViewer * bitmapViewer, CDraw * m_cDessin)
{
	m_cDessin->Dessiner(dc, bitmapViewer->GetHPos(), bitmapViewer->GetVPos(), bitmapViewer->GetRatio(), wxColour(30, 30, 30), wxColour(30, 30, 30), wxColour(255, 255, 255), 2);
}

void CFilterWindowParam::DrawingToPicture(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin)
{
	if (m_cDessin != nullptr)
	{
		wxImage image = filtreEffet->GetwxImage();
		wxBitmap bitmap = wxBitmap(image);
		wxMemoryDC dc;
		dc.SelectObject(bitmap);
		wxRect rc(0, 0, image.GetWidth(), image.GetHeight());
		wxImage render = filtreEffet->GetwxImage();
		Drawing(&dc, bitmapViewer, m_cDessin);
		dc.SelectObject(wxNullBitmap);
		CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
		wxImage * local_image = new wxImage(bitmap.ConvertToImage());
		imageLoad->SetPicture(local_image);
		filtreEffet->SetBitmap(imageLoad);
	}
}

void CFilterWindowParam::ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing)
{
	if (CFiltreData::IsPiccanteCompatible(GetTypeFilter()))
	{
		CRegardsFloatBitmap * test = filtreEffet->GetFloatBitmap(true);
		switch (GetTypeFilter())
		{
		case IDM_BEST_EXPOSURE:
			CPiccanteFilter::BestExposure(test);
			break;
		case IDM_FILTER_KUWAHARA:
			CPiccanteFilter::FilterKuwahara(test);
			break;
		case IDM_FILTER_BILATERAL2DS:
			CPiccanteFilter::FilterBilateral2DS(test);
			break;
		}

		CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(test);
		filtreEffet->SetBitmap(imageLoad);
	}
	else
	{
		filtreEffet->SetPreview(true);
		filtreEffet->RenderEffect(GetTypeFilter(), effectParameter);
	}
}

CImageLoadingFormat * CFilterWindowParam::ApplyEffect(CEffectParameter * effectParameter, CBitmapWndViewer * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (source != nullptr)
	{
		if (CFiltreData::IsPiccanteCompatible(GetTypeFilter()))
		{
			CRegardsFloatBitmap * test = bitmapViewer->GetFloatBitmap(true);
			switch (GetTypeFilter())
			{
			case IDM_BEST_EXPOSURE:
				CPiccanteFilter::BestExposure(test);
				break;
			case IDM_FILTER_KUWAHARA:
				CPiccanteFilter::FilterKuwahara(test);
				break;
			case IDM_FILTER_BILATERAL2DS:
				CPiccanteFilter::FilterBilateral2DS(test);
				break;
			}

			imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(test);
			imageLoad->SetOrientation(bitmapViewer->GetOrientation());
		}
		else
		{
			CFiltreEffet * filtre = nullptr;
			CImageLoadingFormat image(false);
			image.SetPicture(source);

			if (CFiltreData::IsOpenCLCompatible(GetTypeFilter()))
				filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), bitmapViewer->GetOpenCLContext(), &image);
			else
				filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);

			filtre->RenderEffect(GetTypeFilter(), effectParameter);
			imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(filtre->GetBitmap(true));
			imageLoad->SetOrientation(bitmapViewer->GetOrientation());
			delete filtre;
		}


	}

	return imageLoad;
}