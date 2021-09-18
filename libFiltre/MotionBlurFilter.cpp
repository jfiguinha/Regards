#include <header.h>
//
//  MotionBlurFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "MotionBlurFilter.h"
#include "MotionBlurEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <RenderOpenGL.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <ImageLoadingFormat.h>
using namespace Regards::Filter;

CMotionBlurFilter::CMotionBlurFilter()
{
    libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
    libelleEffectSigma = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMA",1);
    libelleEffectAngle = CLibResource::LoadStringFromResource(L"LBLEFFECTANGLE",1);
}

int CMotionBlurFilter::TypeApplyFilter()
{
	return 2;
}

CMotionBlurFilter::~CMotionBlurFilter()
{
    
}

wxString CMotionBlurFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterMotion", 1);
}


int CMotionBlurFilter::GetNameFilter()
{
	return IDM_FILTRE_MOTIONBLUR;
}

int CMotionBlurFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT;// IDM_FILTRE_MOTIONBLUR;
}

void CMotionBlurFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementAngle;
    for (auto i = 0; i < 361; i++)
        elementAngle.push_back(i);
    
    
    vector<int> elementSample;
    for (auto i = 0; i < 100; i++)
        elementSample.push_back(i);
    
    vector<int> velocity;
    for (auto i = 0; i < 101; i++)
        velocity.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(motionBlurParameter->radius), &elementSample);
    filtreInterface->AddTreeInfos(libelleEffectSigma, new CTreeElementValueInt(motionBlurParameter->sigma), &velocity);
    filtreInterface->AddTreeInfos(libelleEffectAngle, new CTreeElementValueInt(motionBlurParameter->angle), &elementAngle);
}

void CMotionBlurFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;
    
	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;

	if (motionBlurParameter != nullptr && valueInt != nullptr)
	{

		int value = valueInt->GetValue();
		//Video Parameter
		if (key == libelleEffectRadius)
		{
			motionBlurParameter->radius = value;
		}
		else if (key == libelleEffectSigma)
		{
			motionBlurParameter->sigma = value;
		}
		else if (key == libelleEffectAngle)
		{
			motionBlurParameter->angle = value;
		}
	}
}

bool CMotionBlurFilter::NeedPreview()
{
	return true;
}


void CMotionBlurFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CMotionBlurEffectParameter* motionblurEffectParameter = (CMotionBlurEffectParameter*)effectParameter;
		filtreEffet->MotionBlur(motionblurEffectParameter->radius, motionblurEffectParameter->sigma, motionblurEffectParameter->angle);
	}
}

CEffectParameter* CMotionBlurFilter::GetEffectPointer()
{
	return new CMotionBlurEffectParameter();
}

CEffectParameter* CMotionBlurFilter::GetDefaultEffectParameter()
{
	CMotionBlurEffectParameter* motionBlur = new CMotionBlurEffectParameter();
	motionBlur->radius = 20;
	motionBlur->sigma = 5;
	motionBlur->angle = 40;
	return motionBlur;
}

bool CMotionBlurFilter::IsSourcePreview()
{
	return true;
}


void CMotionBlurFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && source != nullptr)
	{
		CMotionBlurEffectParameter* motionblurEffectParameter = (CMotionBlurEffectParameter*)effectParameter;
		filtreEffet->MotionBlur(motionblurEffectParameter->radius, motionblurEffectParameter->sigma, motionblurEffectParameter->angle);
	}

}


void CMotionBlurFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CMotionBlurFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			source->RotateExif(source->GetOrientation());
			CImageLoadingFormat image(false);
			image.SetPicture(source);
			filter->SetBitmap(&image);
			
			CMotionBlurEffectParameter* motionblurEffectParameter = (CMotionBlurEffectParameter*)effectParameter;
			filter->MotionBlur(motionblurEffectParameter->radius, motionblurEffectParameter->sigma, motionblurEffectParameter->angle);
			imageLoad = new CImageLoadingFormat();
			CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
			bitmapOut->RotateExif(source->GetOrientation());
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
