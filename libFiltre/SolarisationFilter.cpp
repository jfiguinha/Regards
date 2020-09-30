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
using namespace Regards::Filter;

CSolarisationFilter::CSolarisationFilter()
{
    libelleEffectThreshold = CLibResource::LoadStringFromResource(L"LBLEFFECTTHRESHOLD",1);
}

CSolarisationFilter::~CSolarisationFilter()
{
    
}

int CSolarisationFilter::GetTypeFilter()
{
    return IDM_AJUSTEMENT_SOLARISATION;
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

