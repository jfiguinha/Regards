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
#include <RegardsBitmap.h>
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
	m_cDessin->Dessiner(dc, 0, 0, 1, wxColour(30, 30, 30), wxColour(30, 30, 30), wxColour(255, 255, 255), 2);
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

void CFilterWindowParam::ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput)
{
	if (CFiltreData::IsPiccanteCompatible(GetTypeFilter()))
	{
		filtreEffet->SetPreview(true);
		CRegardsFloatBitmap * test = filtreEffet->GetFloatBitmap(false);
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
		if (CFiltreData::IsOpenCLCompatible(GetTypeFilter()))
			filtreEffet->RenderEffect(GetTypeFilter(), effectParameter);
		else
		{
			CRegardsBitmap * bitmap = filtreEffet->GetBitmap(false);
			if (bitmap != nullptr)
			{
				CImageLoadingFormat image;
				image.SetPicture(bitmap);
				CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
				filtre->RenderEffect(GetTypeFilter(), effectParameter);
				CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
				imageLoad->SetPicture(filtre->GetBitmap(true));
				filtreEffet->SetBitmap(imageLoad);
				delete filtre;
			}
		}

		widthOutput = filtreEffet->GetWidth();
		heightOutput = filtreEffet->GetHeight();
	}
}

CImageLoadingFormat * CFilterWindowParam::RenderEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, int numFiltre)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (CFiltreData::IsPiccanteCompatible(numFiltre))
	{
		CRegardsFloatBitmap * bitmap = bitmapViewer->GetFloatBitmap(true);
		switch (numFiltre)
		{
		case IDM_BEST_EXPOSURE:
			CPiccanteFilter::BestExposure(bitmap);
			break;
		case IDM_FILTER_KUWAHARA:
			CPiccanteFilter::FilterKuwahara(bitmap);
			break;
		case IDM_FILTER_BILATERAL2DS:
			CPiccanteFilter::FilterBilateral2DS(bitmap);
			break;
		}
		CImageLoadingFormat imageLoadFormat(true);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(bitmap);
		imageLoad->SetOrientation(bitmapViewer->GetOrientation());
	}
	else if (CFiltreData::IsOpenCLCompatible(numFiltre))
	{
		CFiltreEffet * filtre = bitmapViewer->GetFiltreEffet();
		if (filtre != nullptr)
		{
			filtre->SetPreview(false);
			filtre->RenderEffect(numFiltre, effectParameter);
			imageLoad = new CImageLoadingFormat();
			CRegardsBitmap * bitmapOut = filtre->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
			imageLoad->SetOrientation(bitmapViewer->GetOrientation());
		}
	}
	else
	{
		CRegardsBitmap * bitmap = bitmapViewer->GetBitmap(true);
		if (bitmap != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(bitmap);
			CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
			filtre->RenderEffect(numFiltre, effectParameter);
			imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(filtre->GetBitmap(true));
			imageLoad->SetOrientation(bitmapViewer->GetOrientation());

			delete filtre;
		}
	}

	return imageLoad;
}

CImageLoadingFormat * CFilterWindowParam::ApplyEffect(CEffectParameter * effectParameter, CBitmapWndViewer * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = CFilterWindowParam::RenderEffect(effectParameter, bitmapViewer, GetTypeFilter());
	return imageLoad;
}