#include <header.h>
//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "SwirlFilter.h"
#include "SwirlEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <RenderOpenGL.h>
using namespace Regards::Viewer;

CSwirlFilter::CSwirlFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
    libelleEffectAngle = CLibResource::LoadStringFromResource(L"LBLEFFECTANGLE",1);
}

CSwirlFilter::~CSwirlFilter()
{
    
}

int CSwirlFilter::GetTypeFilter()
{
    return IDM_FILTRE_SWIRL;
}

void CSwirlFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{   
   	CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
    
    swirlEffectParameter->bitmapWidth = source->GetBitmapWidth();
    swirlEffectParameter->bitmapHeight = source->GetBitmapHeight();
    
    vector<int> elementAngle;
    for (auto i = 0; i < 360; i++)
        elementAngle.push_back(i);
    
    vector<int> elementRadius;
    for (auto i = 0; i < swirlEffectParameter->bitmapWidth; i++)
        elementRadius.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(swirlEffectParameter->radius), &elementRadius);
    filtreInterface->AddTreeInfos(libelleEffectAngle, new CTreeElementValueInt(swirlEffectParameter->angle), &elementAngle);
}

void CSwirlFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
    
	this->source = source;

	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleEffectRadius)
    {
        swirlEffectParameter->radius = value;
    }
    if (key == libelleEffectAngle)
    {
        swirlEffectParameter->angle = value;
    }
}


void CSwirlFilter::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;
	if (swirlEffectParameter != nullptr)
	{
		/*
uniform float widthScreen;
uniform float heightScreen;
uniform float widthBitmap;
uniform float heightBitmap;
uniform float left;
uniform float top;

// Swirl effect parameters
uniform float radius;
uniform float angleDegree;
		*/

		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_SWIRL");
		if (m_pShader != nullptr)
		{
			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("textureScreen", textureID))
			{
				printf("SetTexture textureScreen failed \n ");
			}
			if (!m_pShader->SetParam("widthScreen", renderOpenGL->GetWidth()))
			{
				printf("SetParam widthBitmap failed \n ");
			}
			if (!m_pShader->SetParam("heightScreen", renderOpenGL->GetHeight()))
			{
				printf("SetParam heightBitmap failed \n ");
			}
			if (!m_pShader->SetParam("widthBitmap", swirlEffectParameter->bitmapWidth))
			{
				printf("SetParam widthBitmap failed \n ");
			}
			if (!m_pShader->SetParam("heightBitmap", swirlEffectParameter->bitmapHeight))
			{
				printf("SetParam heightBitmap failed \n ");
			}
			if (!m_pShader->SetParam("left", 0))
			{
				printf("SetParam widthBitmap failed \n ");
			}
			if (!m_pShader->SetParam("top", 0))
			{
				printf("SetParam heightBitmap failed \n ");
			}
			if (!m_pShader->SetParam("radius", swirlEffectParameter->radius))
			{
				printf("SetParam radius failed \n ");
			}
			if (!m_pShader->SetParam("angleDegree", swirlEffectParameter->angle))
			{
				printf("SetParam angleDegree failed \n ");
			}
		}
	}
}
