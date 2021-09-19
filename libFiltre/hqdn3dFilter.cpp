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

#include "TreeElementValue.h"
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

int Chqdn3dFilter::TypeApplyFilter()
{
    return 2;
}

wxString Chqdn3dFilter::GetFilterLabel()
{
    return CLibResource::LoadStringFromResource("LBLFILTREHQDN3D", 1);
}


int Chqdn3dFilter::GetNameFilter()
{
    return IDM_FILTREHQDN3D;
}

int Chqdn3dFilter::GetTypeFilter()
{
    return CONVOLUTION_EFFECT; //
}

void Chqdn3dFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    Chqdn3dEffectParameter * hqdn3dParameter = (Chqdn3dEffectParameter *)effectParameter;
 
	this->source = source;

    vector<int> elementIntensity;
    for (auto i = 0; i < 26; i++)
    {
    	if(i%2 == 1)
			elementIntensity.push_back(i);
    }
    
    filtreInterface->AddTreeInfos(libelleLumSpac, new CTreeElementValueInt(hqdn3dParameter->LumSpac), &elementIntensity);

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


void Chqdn3dFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput)
{
}

CImageLoadingFormat * Chqdn3dFilter::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
        CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
        if (filter != nullptr)
        {
            Chqdn3dEffectParameter* hqdn3dParameter = (Chqdn3dEffectParameter*)effectParameter;
            filter->HQDn3D(hqdn3dParameter->LumSpac, hqdn3dParameter->ChromSpac, hqdn3dParameter->LumTmp, hqdn3dParameter->ChromTmp);
            imageLoad = new CImageLoadingFormat();
            CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
            bitmapOut->RotateExif(source->GetOrientation());
            imageLoad->SetPicture(bitmapOut);
        }
	}

	return imageLoad;
}

void Chqdn3dFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
    if (effectParameter != nullptr && filtreEffet != nullptr)
    {
        Chqdn3dEffectParameter* hq3dn = (Chqdn3dEffectParameter*)effectParameter;
        filtreEffet->HQDn3D(hq3dn->LumSpac, hq3dn->ChromSpac, hq3dn->LumTmp, hq3dn->ChromTmp);
    }
}

bool Chqdn3dFilter::NeedPreview()
{
    return true;
}

CEffectParameter* Chqdn3dFilter::GetEffectPointer()
{
    return new Chqdn3dEffectParameter();
}

CEffectParameter* Chqdn3dFilter::GetDefaultEffectParameter()
{
    Chqdn3dEffectParameter* hq3deffect = new Chqdn3dEffectParameter();
    hq3deffect->LumSpac = 4;
    hq3deffect->ChromSpac = 4;
    hq3deffect->LumTmp = 3;
    hq3deffect->ChromTmp = 3;
    return hq3deffect;
}


bool Chqdn3dFilter::IsSourcePreview()
{
	return true;
}


void Chqdn3dFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && source != nullptr)
	{
        Chqdn3dEffectParameter* hq3dn = (Chqdn3dEffectParameter*)effectParameter;
        filtreEffet->HQDn3D(hq3dn->LumSpac, hq3dn->ChromSpac, hq3dn->LumTmp, hq3dn->ChromTmp);
	}

}


