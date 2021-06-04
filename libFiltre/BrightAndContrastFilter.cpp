#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BrightAndContrastFilter.h"
#include "BrightAndContrastEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <RenderOpenGL.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CBrightAndContrastFilter::CBrightAndContrastFilter()
{
    libelleEffectContrast = CLibResource::LoadStringFromResource(L"LBLEFFECTCONTRAST",1);
    libelleEffectLightness = CLibResource::LoadStringFromResource(L"LBLEFFECTBRIGHTNESS",1);
}

CBrightAndContrastFilter::~CBrightAndContrastFilter()
{
    
}

bool CBrightAndContrastFilter::IsOpenGLCompatible()
{
    return true;
}

int CBrightAndContrastFilter::TypeApplyFilter()
{
    return 2;
}

int CBrightAndContrastFilter::GetTypeFilter()
{
    return COLOR_EFFECT; //return IDM_IMAGE_LIGHTCONTRAST;
}

void CBrightAndContrastFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;
	
	this->source = source;

    vector<int> elementColor;
    for (int i = -100; i < 101; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectContrast,new CTreeElementValueInt(BrightAndContrastEffectParameter->contrast), &elementColor);
    filtreInterface->AddTreeInfos(libelleEffectLightness,new CTreeElementValueInt(BrightAndContrastEffectParameter->brightness), &elementColor);
}

void CBrightAndContrastFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;
    //Video Parameter
	CTreeElementValueInt * value = (CTreeElementValueInt *)valueData;

    if (key == libelleEffectContrast)
    {
        BrightAndContrastEffectParameter->contrast = value->GetValue();
    }
    else if (key == libelleEffectLightness)
    {
        BrightAndContrastEffectParameter->brightness = value->GetValue();
    }
}


void CBrightAndContrastFilter::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	CBrightAndContrastEffectParameter * brightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;
	if (brightAndContrastEffectParameter != nullptr)
	{
        printf("GLSLShader IDR_GLSL_BRIGHTANDCONTRAST_SHADER \n " );
		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_BRIGHTANDCONTRAST_SHADER");
		if (m_pShader != nullptr)
		{
			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("textureScreen", textureID))
			{
				printf("SetTexture textureScreen failed \n ");
			}
			if (!m_pShader->SetParam("contrast", brightAndContrastEffectParameter->contrast))
			{
				printf("SetParam contrast failed \n ");
			}
			if (!m_pShader->SetParam("brightness", brightAndContrastEffectParameter->brightness))
			{
				printf("SetParam brightness failed \n ");
			}
		}
	}
}

void CBrightAndContrastFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
    if (effectParameter != nullptr && filtreEffet != nullptr)
    {
        CBrightAndContrastEffectParameter* brightAndContrast = (CBrightAndContrastEffectParameter*)effectParameter;
        filtreEffet->BrightnessAndContrast(brightAndContrast->brightness, brightAndContrast->contrast);
    }
}

bool CBrightAndContrastFilter::NeedPreview()
{
    return true;
}

CEffectParameter* CBrightAndContrastFilter::GetEffectPointer()
{
    return new CBrightAndContrastEffectParameter();
}

CEffectParameter* CBrightAndContrastFilter::GetDefaultEffectParameter()
{
    CBrightAndContrastEffectParameter* brightness = new CBrightAndContrastEffectParameter();
    brightness->brightness = 20;
    brightness->contrast = 20;
    return brightness;
}