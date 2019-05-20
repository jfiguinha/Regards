#include <header.h>
//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "VideoFilter.h"
#include "EffectVideoParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <Metadata.h>
#include <ShowVideo.h>
#include <videostream.h>
using namespace Regards::Viewer;
using namespace Regards::Video;

CVideoFilter::CVideoFilter()
{
	effectDenoisingSigmaP = CLibResource::LoadStringFromResource(L"LBLeffectDenoisingSigmaP",1);//L"Effect.Denoising.Sigma P";
	effectDenoisingFSize =CLibResource::LoadStringFromResource(L"LBLeffectDenoisingFSize",1);// L"Effect.Denoising.FSize";
	effectDenoisingSigmaX = CLibResource::LoadStringFromResource(L"LBLeffectDenoisingSigmaX",1);//L"Effect.Denoising.Sigma X";
	effectSharpenLevel = CLibResource::LoadStringFromResource(L"LBLeffectSharpenLevel",1);//L"Effect.Sharpen.Level";
	toneConstrastLevel = CLibResource::LoadStringFromResource(L"LBLtoneConstrastLevel",1);//L"Tone.Constrast.Level";
	toneBrightnessLevel = CLibResource::LoadStringFromResource(L"LBLtoneBrightnessLevel",1);//L"Tone.Brightness.Level";
	colorBoostRed = CLibResource::LoadStringFromResource(L"LBLcolorBoostRed",1);//L"Color Boost.Red";
	colorBoostGreen = CLibResource::LoadStringFromResource(L"LBLcolorBoostGreen",1);//L"Color Boost.Green";
	colorBoostBlue = CLibResource::LoadStringFromResource(L"LBLcolorBoostBlue",1);//L"Color Boost.Blue";
	enableEffect = CLibResource::LoadStringFromResource(L"LBLenableEffect",1);//L"Effect.Enable";
	enableBlurEffect = CLibResource::LoadStringFromResource(L"LBLenableBlurEffect",1);//L"Effect.Denoising.Enable";
	enableSharpenEffect = CLibResource::LoadStringFromResource(L"LBLenableSharpenEffect",1);//L"Effect.Sharpen.Enable";
	enableColorEffect = CLibResource::LoadStringFromResource(L"LBLenableColorEffect",1);//L"Color Boost.Enable";
	enableBandCEffect = CLibResource::LoadStringFromResource(L"LBLenableBandCEffect",1);//L"Tone.Enable";
	enableGrayScale = CLibResource::LoadStringFromResource(L"LBLenableGrayScale",1);//L"Effect.Gray Scale";
    enableBicubicInterpolation = CLibResource::LoadStringFromResource(L"LBLEFFECTBICUBICINTERPOLATION", 1);//LBLEFFECTBICUBICINTERPOLATION
    enableOpenCL = CLibResource::LoadStringFromResource(L"LBLEFFECTOPENCL", 1);//LBLEFFECTOPENCL
	showFPS = CLibResource::LoadStringFromResource(L"LBLshowFPS",1);//L"Effect.Show FPS";
}

CVideoFilter::~CVideoFilter()
{
    
}

int CVideoFilter::GetTypeFilter()
{
    return IDM_FILTRE_VIDEO;
}

void CVideoFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    if(source != nullptr)
    {
        Filter(effectParameter, source->GetFilename(), filtreInterface);
    }    
}

void CVideoFilter::Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface)
{   
	CVideoEffectParameter * videoEffectParameter = (CVideoEffectParameter *)effectParameter;
	vector<int> elementContrast;
    for (auto i = -100; i < 101; i++)
        elementContrast.push_back(i);
    filtreInterface->AddTreeInfos(enableOpenCL, new CTreeElementValueInt(videoEffectParameter->enableOpenCL), &videoEffectParameter->enableOpenCL, 2, 2);
    filtreInterface->AddTreeInfos(enableBicubicInterpolation, new CTreeElementValueInt(videoEffectParameter->BicubicEnable), &videoEffectParameter->BicubicEnable, 2, 2);
	filtreInterface->AddTreeInfos(showFPS, new CTreeElementValueInt(videoEffectParameter->showFPS), &videoEffectParameter->showFPS, 2, 2);
	filtreInterface->AddTreeInfos(enableEffect, new CTreeElementValueInt(videoEffectParameter->effectEnable), &videoEffectParameter->effectEnable, 2, 2);

	filtreInterface->AddTreeInfos(enableGrayScale, new CTreeElementValueInt(videoEffectParameter->grayEnable), &videoEffectParameter->grayEnable, 2, 2);
	filtreInterface->AddTreeInfos(enableBandCEffect, new CTreeElementValueInt(videoEffectParameter->bandcEnable), &videoEffectParameter->bandcEnable, 2, 2);
    filtreInterface->AddTreeInfos(toneConstrastLevel,new CTreeElementValueInt(videoEffectParameter->contrast), &elementContrast);
    filtreInterface->AddTreeInfos(toneBrightnessLevel,new CTreeElementValueInt(videoEffectParameter->brightness), &elementContrast);

	vector<int> elementFreq;
	for (auto i = 0; i < 100; i++)
		elementFreq.push_back(i);

	filtreInterface->AddTreeInfos(enableSharpenEffect, new CTreeElementValueInt(videoEffectParameter->SharpenEnable), &videoEffectParameter->SharpenEnable, 2, 2);
	filtreInterface->AddTreeInfos(effectSharpenLevel, new CTreeElementValueInt(videoEffectParameter->sharpness), &elementFreq);

    vector<int> elementColor;
    for (auto i = -255; i < 256; i++)
        elementColor.push_back(i);
    
	filtreInterface->AddTreeInfos(enableColorEffect, new CTreeElementValueInt(videoEffectParameter->ColorBoostEnable), &videoEffectParameter->ColorBoostEnable, 2, 2);
    filtreInterface->AddTreeInfos(colorBoostRed, new CTreeElementValueInt(videoEffectParameter->color_boost[0]), &elementColor);
    filtreInterface->AddTreeInfos(colorBoostGreen, new CTreeElementValueInt(videoEffectParameter->color_boost[1]), &elementColor);
	filtreInterface->AddTreeInfos(colorBoostBlue, new CTreeElementValueInt(videoEffectParameter->color_boost[2]), &elementColor);
    
    //Denoising

	vector<int> elementExponent;
	for (auto i = 0; i < 10; i++)
		elementExponent.push_back(i);    
    filtreInterface->AddTreeInfos(enableBlurEffect, new CTreeElementValueInt(videoEffectParameter->denoiseEnable), &videoEffectParameter->denoiseEnable, 2, 2);
    filtreInterface->AddTreeInfos(effectDenoisingFSize, new CTreeElementValueInt(videoEffectParameter->exponent), &elementExponent);

/*
	vector<int> elementNbTimes;
	for (auto i = 0; i < 10; i++)
		elementNbTimes.push_back(i);


	vector<float> elementSigma;
	for (float i = 0; i < 100; i+=0.1)
		elementSigma.push_back(i);

	filtreInterface->AddTreeInfos(enableBlurEffect, new CTreeElementValueInt(videoEffectParameter->MedianEnable), &videoEffectParameter->MedianEnable, 2, 2);
	filtreInterface->AddTreeInfos(effectDenoisingSigmaP, new CTreeElementValueFloat(videoEffectParameter->sigmaP), &elementSigma, 4);
	filtreInterface->AddTreeInfos(effectDenoisingSigmaX, new CTreeElementValueFloat(videoEffectParameter->sigmaX), &elementSigma, 4);
	//filtreInterface->AddTreeInfos(effectDenoisingBSize, new CTreeElementValueInt(videoEffectParameter->bSize), &elementNbTimes);
	filtreInterface->AddTreeInfos(effectDenoisingFSize, new CTreeElementValueInt(videoEffectParameter->fSize), &elementNbTimes);
*/

}

void CVideoFilter::AddMetadataElement(vector<CMetadata> & element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CVideoFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CVideoEffectParameter * videoEffectParameter = (CVideoEffectParameter *)effectParameter;

	float value = 0.0;
	switch(valueData->GetType())
	{
		case TYPE_INT:
			{
				CTreeElementValueInt * intValue = (CTreeElementValueInt*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_FLOAT:
			{
				CTreeElementValueFloat * intValue = (CTreeElementValueFloat*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_BOOL:
			{
				CTreeElementValueBool * intValue = (CTreeElementValueBool*)valueData;
				value = intValue->GetValue();
			}
			break;
	}

	//Video Parameter
	if (key == effectDenoisingSigmaP)
	{
		videoEffectParameter->sigmaP = value;
	}
	else if (key == enableOpenCL)
	{
		videoEffectParameter->enableOpenCL = value;
	}    
	else if (key == enableBicubicInterpolation)
	{
		videoEffectParameter->BicubicEnable = value;
	}     
	else if (key == effectDenoisingSigmaX)
	{
		videoEffectParameter->sigmaX = value;
	}
	else if (key == effectSharpenLevel)
	{
		videoEffectParameter->sharpness = value / 10.0f;
	}
	else if (key == toneConstrastLevel)
	{
		//Tone mapping
		videoEffectParameter->contrast = value;
	}
	else if(key == enableGrayScale)
	{
		videoEffectParameter->grayEnable = value;
	}
	else if (key == toneBrightnessLevel)
	{
		videoEffectParameter->brightness = value;
	}
	else if (key == colorBoostRed)
	{
		//Color Boost
		videoEffectParameter->color_boost[0] = value;
	}
	else if (key == colorBoostGreen)
	{
		videoEffectParameter->color_boost[1] = value;

	}
	else if (key == colorBoostBlue)
	{
		videoEffectParameter->color_boost[2] = value;
	}
	else if (key == enableEffect)
	{
		videoEffectParameter->effectEnable = value;
	}
	else if (key == enableSharpenEffect)
	{
		videoEffectParameter->SharpenEnable = value;
	}
	else if (key == enableColorEffect)
	{
		videoEffectParameter->ColorBoostEnable = value;
	}
	else if (key == enableBandCEffect)
	{
		videoEffectParameter->bandcEnable = value;
	}
	else if (key == showFPS)
	{
		videoEffectParameter->showFPS = value;
	}
	else if (key == libelleAudio)
	{
		videoEffectParameter->streamAudioIndex = value;
		videoEffectParameter->streamAudioUpdate = 1;
	}
	else if (key == libelleVideo)
	{
		videoEffectParameter->streamVideoIndex = value;
		videoEffectParameter->streamVideoUpdate = 1;
	}
	else if (key == libelleSubtitle)
	{
		videoEffectParameter->streamSubtitleIndex = value;
		videoEffectParameter->streamSubtitleUpdate = 1;
	}
	else if (key == enableSubtitle)
	{
		videoEffectParameter->enableSubtitle = value;
	}
	else if (key == enableBlurEffect)
	{
		videoEffectParameter->denoiseEnable = value;
	}
    else if (key == effectDenoisingFSize)
	{
		videoEffectParameter->exponent = value;
	}
}