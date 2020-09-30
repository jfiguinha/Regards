#include <header.h>
//
//  CColorBalanceFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "ColorBalanceFilter.h"
#include "RgbEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <GLTexture.h>
#include <RenderOpenGL.h>
using namespace Regards::OpenGL;
using namespace Regards::Filter;

CColorBalanceFilter::CColorBalanceFilter()
{
    libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED",1);
    libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN",1);
    libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE",1);
}

CColorBalanceFilter::~CColorBalanceFilter()
{
    
}

int CColorBalanceFilter::GetTypeFilter()
{
    return IDM_COLOR_BALANCE;
}

void CColorBalanceFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = -255; i < 256; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectColorRed, new CTreeElementValueInt(rgbEffectParameter->red), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorGreen, new CTreeElementValueInt(rgbEffectParameter->green), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectColorBlue, new CTreeElementValueInt(rgbEffectParameter->blue), &elementColor);
}

void CColorBalanceFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;

	if (rgbEffectParameter != nullptr && valueInt != nullptr)
	{
		int value = valueInt->GetValue();
		//Video Parameter
		if (key == libelleEffectColorRed)
		{
			rgbEffectParameter->red = value;
		}
		else if (key == libelleEffectColorGreen)
		{
			rgbEffectParameter->green = value;
		}
		else if (key == libelleEffectColorBlue)
		{
			rgbEffectParameter->blue = value;
		}
	}
}

void CColorBalanceFilter::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;
	if (rgbEffectParameter != nullptr)
	{
		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_RGB");
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
			if (!m_pShader->SetParam("red", rgbEffectParameter->red))
			{
				printf("SetParam red failed \n ");
			}
			if (!m_pShader->SetParam("green", rgbEffectParameter->green))
			{
				printf("SetParam green failed \n ");
			}
			if (!m_pShader->SetParam("blue", rgbEffectParameter->blue))
			{
				printf("SetParam blue failed \n ");
			}
		}
	}
}


