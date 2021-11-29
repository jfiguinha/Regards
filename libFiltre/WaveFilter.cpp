#include <header.h>
//
//  CWaveFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "WaveFilter.h"
#include "WaveEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include <Draw.h>
#include <Selection.h>
using namespace Regards::Filter;

int CWaveFilter::TypeApplyFilter()
{
	return 2;
}

CDraw* CWaveFilter::GetDrawingPt()
{
	return new CSelection();
}

CWaveFilter::CWaveFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
	libelleEffectScale = CLibResource::LoadStringFromResource(L"LBLEFFECTSCALE", 1);
    libelleEffectHeight = CLibResource::LoadStringFromResource(L"LBLEFFECTHEIGHT", 1);

}

bool CWaveFilter::IsOpenCLCompatible()
{
	return false;
}

CWaveFilter::~CWaveFilter()
{
    
}

bool CWaveFilter::SupportMouseClick()
{
	return true;
}

wxString CWaveFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLWAVEFILTER", 1);
}


int CWaveFilter::GetNameFilter()
{
	return IDM_WAVE_EFFECT;
}

int CWaveFilter::GetTypeFilter()
{
    return IDM_WAVE_EFFECT;
}

void CWaveFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CWaveEffectParameter * waveParameter = (CWaveEffectParameter *)effectParameter;
	
	this->source = source;

    vector<int> elementIntensity;
    for (int i = 0; i < 40; i++)
        elementIntensity.push_back(i);
    
    vector<int> elementColor;
    for (int i = 0; i < 255; i++)
        elementColor.push_back(i);
    
    vector<int> elementBitmapHeight;
    for (int i = 0; i < source->GetBitmapHeight() / 2; i++)
        elementBitmapHeight.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectHeight, new CTreeElementValueInt(waveParameter->height), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectScale, new CTreeElementValueInt(waveParameter->scale), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(waveParameter->radius), &elementColor);
}

void CWaveFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CWaveEffectParameter * waveParameter = (CWaveEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleEffectHeight)
    {
		waveParameter->height = value;
    }
    else if (key == libelleEffectRadius)
    {
		waveParameter->radius = value;
    }
    else if (key == libelleEffectScale)
    {
		waveParameter->scale = value;
    }
}
void CWaveFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int& widthOutput, int& heightOutput)
{

}


CImageLoadingFormat * CWaveFilter::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer)
	{
		CImageLoadingFormat image(false);
		source->RotateExif(source->GetOrientation());
		image.SetPicture(source);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);

		wxPoint pt;
		bitmapViewer->GetDessinPt()->GetPoint(pt);

		CWaveEffectParameter * waveEffectParameter = (CWaveEffectParameter *)effectParameter;
		int radius = waveEffectParameter->radius;
		int scale = waveEffectParameter->scale;
		filtre->WaveFilter(pt.x, image.GetHeight() - pt.y, image.GetHeight(), radius, scale);

		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}


void CWaveFilter::Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin)
{
	int hpos = bitmapViewer->GetHPos();
	int vpos = bitmapViewer->GetVPos();

	if (m_cDessin != nullptr)
		m_cDessin->Dessiner(dc, hpos, vpos, bitmapViewer->GetRatio(), wxColour(0, 0, 0), wxColour(0, 0, 0), wxColour(0, 0, 0), 2);
}


bool CWaveFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CWaveFilter::GetEffectPointer()
{
	return new CWaveEffectParameter();
}

CEffectParameter* CWaveFilter::GetDefaultEffectParameter()
{
	return new CWaveEffectParameter();
}

bool CWaveFilter::IsSourcePreview()
{
	return true;
}


void CWaveFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		CImageLoadingFormat image(false);
		image.SetPicture(source);
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);

		wxPoint pt;
		bitmapViewer->GetDessinPt()->GetPoint(pt);
		ApplyExifToPoint(pt, source->GetOrientation(), source->GetBitmapWidth(), source->GetBitmapHeight());
		//Calcul Point with Exif info

		CWaveEffectParameter* waveEffectParameter = (CWaveEffectParameter*)effectParameter;
		int radius = waveEffectParameter->radius;
		int scale = waveEffectParameter->scale;
		filtre->WaveFilter(pt.x, image.GetHeight() - pt.y, image.GetHeight(), radius, scale);

		imageLoad = new CImageLoadingFormat();
		
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;

		filtreEffet->SetBitmap(imageLoad);

		delete imageLoad;
	}


}