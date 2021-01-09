#include <header.h>
//
//  Chqdn3dFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "hqdn3dFilter.h"
#include "hqdn3dEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <Draw.h>
#include <BitmapDisplay.h>
using namespace Regards::Filter;

Chqdn3dFilter::Chqdn3dFilter()
{
	/*
	libelleLumSpac = CLibResource::LoadStringFromResource(L"LBLEFFECTLUMSPAC",1);
    libelleChromSpac = CLibResource::LoadStringFromResource(L"LBLEFFECTCHROMSPAC",1);
    libelleLumTmp = CLibResource::LoadStringFromResource(L"LBLEFFECTLUMTMP",1);
    libelleChromTmp = CLibResource::LoadStringFromResource(L"LBLEFFECTCHROMTMP",1);
	*/
	libelleLumSpac = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1);
}

Chqdn3dFilter::~Chqdn3dFilter()
{
    
}

int Chqdn3dFilter::GetTypeFilter()
{
    return IDM_FILTREHQDN3D;
}

void Chqdn3dFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    Chqdn3dEffectParameter * hqdn3dParameter = (Chqdn3dEffectParameter *)effectParameter;
 
	this->source = source;

    vector<int> elementIntensity;
    for (auto i = 0; i < 255; i++)
        elementIntensity.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleLumSpac, new CTreeElementValueInt(hqdn3dParameter->LumSpac), &elementIntensity);
    //filtreInterface->AddTreeInfos(libelleChromSpac, new CTreeElementValueInt(hqdn3dParameter->ChromSpac), &elementIntensity);
   // filtreInterface->AddTreeInfos(libelleLumTmp, new CTreeElementValueInt(hqdn3dParameter->LumTmp), &elementIntensity);
    //filtreInterface->AddTreeInfos(libelleChromTmp, new CTreeElementValueInt(hqdn3dParameter->ChromTmp), &elementIntensity);
}

void Chqdn3dFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    Chqdn3dEffectParameter * hqdn3dParameter = (Chqdn3dEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleLumSpac)
    {
        hqdn3dParameter->LumSpac = value;
    }
    else if (key == libelleChromSpac)
    {
        hqdn3dParameter->ChromSpac = value;
    }
    else if (key == libelleLumTmp)
    {
        hqdn3dParameter->LumTmp = value;
    }
    else if (key == libelleChromTmp)
    {
        hqdn3dParameter->ChromTmp = value;
    }
}

/*
void Chqdn3dFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput)
{
	CRegardsBitmap * bitmapOut = filtreEffet->GetBitmap(false);
	Chqdn3dEffectParameter * hqdn3dParameter = (Chqdn3dEffectParameter *)effectParameter;
	CImageLoadingFormat image;
	image.SetPicture(bitmapOut);
	CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
	filtre->HQDn3D(hqdn3dParameter->LumSpac, hqdn3dParameter->ChromSpac, hqdn3dParameter->LumTmp, hqdn3dParameter->ChromTmp);

	filtreEffet->SetPreview(true);

	CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
	imageLoad->SetPicture(filtre->GetBitmap(true));
	filtreEffet->SetBitmap(imageLoad);

	delete filtre;

}

CImageLoadingFormat * Chqdn3dFilter::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		Chqdn3dEffectParameter * hqdn3dParameter = (Chqdn3dEffectParameter *)effectParameter;
		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image;
		image.SetPicture(source);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		filtre->HQDn3D(hqdn3dParameter->LumSpac, hqdn3dParameter->ChromSpac, hqdn3dParameter->LumTmp, hqdn3dParameter->ChromTmp);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}
*/