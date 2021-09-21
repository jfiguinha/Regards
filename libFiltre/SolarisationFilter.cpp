#include <header.h>
//
//  SolarisationFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "SolarisationFilter.h"
#include "SolarisationEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <RenderOpenGL.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Filter;

CSolarisationFilter::CSolarisationFilter()
{
    libelleEffectThreshold = CLibResource::LoadStringFromResource(L"LBLEFFECTTHRESHOLD",1);
}

CSolarisationFilter::~CSolarisationFilter()
{
    
}

int CSolarisationFilter::TypeApplyFilter()
{
	return 2;
}

bool CSolarisationFilter::IsOpenGLCompatible()
{
	return false;
}

wxString CSolarisationFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterSolarize", 1);
}

int CSolarisationFilter::GetNameFilter()
{
	return IDM_AJUSTEMENT_SOLARISATION;
}

int CSolarisationFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //return IDM_AJUSTEMENT_SOLARISATION;
}

void CSolarisationFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectThreshold, new CTreeElementValueInt(solarisationEffectParameter->threshold), &elementColor);
}

void CSolarisationFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;

	if (solarisationEffectParameter != nullptr && valueInt != nullptr)
	{
		int value = valueInt->GetValue();
		//Video Parameter
		if (key == libelleEffectThreshold)
		{
			solarisationEffectParameter->threshold = value;
		}
	}
}

void CSolarisationFilter::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;
	if (solarisationEffectParameter != nullptr)
	{
        printf("GLSLShader IDR_GLSL_SHADER_SOLARIZE \n " );
		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_SOLARIZE");
		if (m_pShader != nullptr)
		{
			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("textureScreen", textureID))
			{
				printf("SetTexture textureScreen failed \n ");
			}
			if (!m_pShader->SetParam("threshold", solarisationEffectParameter->threshold))
			{
				printf("SetParam sharpness failed \n ");
			}
		}
	}
}

void CSolarisationFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CSolarisationEffectParameter* solarisationEffectParameter = (CSolarisationEffectParameter*)effectParameter;
		filtreEffet->Solarize(solarisationEffectParameter->threshold);
	}
}

bool CSolarisationFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CSolarisationFilter::GetEffectPointer()
{
	return new CSolarisationEffectParameter();
}

CEffectParameter* CSolarisationFilter::GetDefaultEffectParameter()
{
	CSolarisationEffectParameter* solarisation = new CSolarisationEffectParameter();
	solarisation->threshold = 50;
	return solarisation;
}

bool CSolarisationFilter::IsSourcePreview()
{
	return true;
}


void CSolarisationFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{

	if (effectParameter != nullptr && source != nullptr)
	{
		CSolarisationEffectParameter* solarisationEffectParameter = (CSolarisationEffectParameter*)effectParameter;
		filtreEffet->Solarize(solarisationEffectParameter->threshold);
	}
}


void CSolarisationFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CSolarisationFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && bitmapViewer != nullptr)
	{
		CFiltreEffet* filtre = bitmapViewer->GetFiltreEffet();

		if (source != nullptr && filtre != nullptr)
		{
			imageLoad = new CImageLoadingFormat();
			CSolarisationEffectParameter* solarisationEffectParameter = (CSolarisationEffectParameter*)effectParameter;
			filtre->Solarize(solarisationEffectParameter->threshold);
			CRegardsBitmap* bitmapOut = filtre->GetBitmap(true);
			bitmapOut->RotateExif(source->GetOrientation());
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}

