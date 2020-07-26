#include <header.h>
//
//  PhotoFiltreFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PhotoFiltreFilter.h"
#include "PhotoFiltreEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <RenderOpenGL.h>
using namespace Regards::Viewer;

CPhotoFiltreFilter::CPhotoFiltreFilter()
{
    libelleEffectIntensity = CLibResource::LoadStringFromResource(L"LBLEFFECTINTENSITY",1);
    libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED",1);
    libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN",1);
    libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE",1);
}

CPhotoFiltreFilter::~CPhotoFiltreFilter()
{
    
}

int CPhotoFiltreFilter::GetTypeFilter()
{
    return ID_AJUSTEMENT_PHOTOFILTRE;
}

void CPhotoFiltreFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CPhotoFiltreEffectParameter * photoEffectParameter = (CPhotoFiltreEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    vector<int> intensity;
    for (auto i = 0; i < 101; i++)
        intensity.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectIntensity, new CTreeElementValueInt(photoEffectParameter->intensity), &intensity);
    filtreInterface->AddTreeInfos(libelleEffectColorRed, new CTreeElementValueInt(photoEffectParameter->red), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorGreen, new CTreeElementValueInt(photoEffectParameter->green), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorBlue, new CTreeElementValueInt(photoEffectParameter->blue), &elementColor);
}

void CPhotoFiltreFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CPhotoFiltreEffectParameter * photoEffectParameter = (CPhotoFiltreEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();
    //Video Parameter
    if (key == libelleEffectColorRed)
    {
        photoEffectParameter->red = value;
    }
    else if (key == libelleEffectColorGreen)
    {
        photoEffectParameter->green = value;
    }
    else if (key == libelleEffectColorBlue)
    {
        photoEffectParameter->blue = value;
    }
    else if (key == libelleEffectIntensity)
    {
        photoEffectParameter->intensity = value;
    }
}

void CPhotoFiltreFilter::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	CPhotoFiltreEffectParameter * photoEffectParameter = (CPhotoFiltreEffectParameter *)effectParameter;
	if (photoEffectParameter != nullptr)
	{
		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_PHOTOFILTER");
		if (m_pShader != nullptr)
		{
			/*
			uniform float widthScreen;
			uniform float heightScreen;
			uniform float red;
			uniform float green;
			uniform float blue;
			uniform float left;
			uniform float top;
			*/
			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("textureScreen", textureID))
			{
				printf("SetTexture textureScreen failed \n ");
			}
			if (!m_pShader->SetParam("red", photoEffectParameter->red))
			{
				printf("SetParam red failed \n ");
			}
			if (!m_pShader->SetParam("green", photoEffectParameter->green))
			{
				printf("SetParam green failed \n ");
			}
			if (!m_pShader->SetParam("blue", photoEffectParameter->blue))
			{
				printf("SetParam blue failed \n ");
			}
			if (!m_pShader->SetParam("intensity", photoEffectParameter->intensity))
			{
				printf("SetParam blue failed \n ");
			}
		}
	}
}
