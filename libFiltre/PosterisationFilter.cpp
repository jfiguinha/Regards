#include <header.h>
//
//  PosterisationFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PosterisationFilter.h"
#include "PosterisationEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

CPosterisationFilter::CPosterisationFilter()
{
    libelleEffectLevel = CLibResource::LoadStringFromResource(L"LBLEFFECTLEVEL",1);
    libelleEffectGamma = CLibResource::LoadStringFromResource(L"LBLEFFECTGAMMA",1);
}

CPosterisationFilter::~CPosterisationFilter()
{
    
}

int CPosterisationFilter::GetTypeFilter()
{
    return ID_AJUSTEMENT_POSTERISATION;
}

void CPosterisationFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    vector<int> elementGamma;
    for (auto i = 0; i < 11; i++)
        elementGamma.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectLevel, new CTreeElementValueInt(posterisationEffectParameter->level), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectGamma, new CTreeElementValueInt(posterisationEffectParameter->gamma * 10), &elementGamma);
}

void CPosterisationFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;

	if (posterisationEffectParameter != nullptr && valueInt != nullptr)
	{
		int value = valueInt->GetValue();
		//Video Parameter
		if (key == libelleEffectLevel)
		{
			posterisationEffectParameter->level = value;
		}
		if (key == libelleEffectGamma)
		{
			posterisationEffectParameter->gamma = value / 10.0;
		}
	}
}

void CPosterisationFilter::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;
	if (posterisationEffectParameter != nullptr)
	{
		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_POSTERIZE");
		if (m_pShader != nullptr)
		{
			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("textureScreen", textureID))
			{
				printf("SetTexture textureScreen failed \n ");
			}
			if (!m_pShader->SetParam("level", posterisationEffectParameter->level))
			{
				printf("SetParam red failed \n ");
			}
			if (!m_pShader->SetParam("gamma", posterisationEffectParameter->gamma))
			{
				printf("SetParam green failed \n ");
			}
		}
	}
}


