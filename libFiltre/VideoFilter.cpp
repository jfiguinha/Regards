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
#include "videostream.h"
#include <effect_id.h>
using namespace Regards::Filter;
using namespace Regards::Video;

CVideoFilter::CVideoFilter()
{
	effectDenoising = CLibResource::LoadStringFromResource(L"LBLeffectDenoising", 1);
	effectDenoisingSigmaU = CLibResource::LoadStringFromResource(L"LBLeffectDenoisingSigmaU",1);//L"Effect.Denoising.Sigma P";
	effectDenoisingThreshold =CLibResource::LoadStringFromResource(L"LBLeffectDenoisingThreshold",1);// L"Effect.Denoising.FSize";
	effectDenoisingSigmaK = CLibResource::LoadStringFromResource(L"LBLeffectDenoisingSigmaK",1);//L"Effect.Denoising.Sigma X";
	effectSharpenLevel = CLibResource::LoadStringFromResource(L"LBLeffectSharpenLevel",1);//L"Effect.Sharpen.Level";
	toneConstrastLevel = CLibResource::LoadStringFromResource(L"LBLtoneConstrastLevel",1);//L"Tone.Constrast.Level";
	toneBrightnessLevel = CLibResource::LoadStringFromResource(L"LBLtoneBrightnessLevel",1);//L"Tone.Brightness.Level";
	colorBoostRed = CLibResource::LoadStringFromResource(L"LBLcolorBoostRed",1);//L"Color Boost.Red";
	colorBoostGreen = CLibResource::LoadStringFromResource(L"LBLcolorBoostGreen",1);//L"Color Boost.Green";
	colorBoostBlue = CLibResource::LoadStringFromResource(L"LBLcolorBoostBlue",1);//L"Color Boost.Blue";
	enableEffect = CLibResource::LoadStringFromResource(L"LBLenableEffect",1);//L"Effect.Enable";
	enableDenoiseEffect = CLibResource::LoadStringFromResource(L"LBLenableDenoiseEffect",1);//L"Effect.Denoising.Enable";
	enableOpenglDenoising = CLibResource::LoadStringFromResource(L"LBLenableOpenGLDenoiseEffect", 1);//L"Effect.Denoising.Enable";
	enableSharpenEffect = CLibResource::LoadStringFromResource(L"LBLenableSharpenEffect",1);//L"Effect.Sharpen.Enable";
	enableColorEffect = CLibResource::LoadStringFromResource(L"LBLenableColorEffect",1);//L"Color Boost.Enable";
	enableBandCEffect = CLibResource::LoadStringFromResource(L"LBLenableBandCEffect",1);//L"Tone.Enable";
	enableVHSEffect = CLibResource::LoadStringFromResource(L"LBLFILTERVHS", 1); //"Effect.Vhs";
	enableGrayScale = CLibResource::LoadStringFromResource(L"LBLenableGrayScale",1);//L"Effect.Gray Scale";
    enableBicubicInterpolation = CLibResource::LoadStringFromResource(L"LBLEFFECTBICUBICINTERPOLATION", 1);//LBLEFFECTBICUBICINTERPOLATION
    enableOpenCL = CLibResource::LoadStringFromResource(L"LBLEFFECTOPENCL", 1);//LBLEFFECTOPENCL
	libelleAutoContrast = CLibResource::LoadStringFromResource(L"LBLVideoAutoContrast", 1);//LBLEFFECTOPENCL
	libelleStabilize = CLibResource::LoadStringFromResource(L"LBLVideoStabilize", 1);//LBLEFFECTOPENCL
	libelleStabilizeNbFrame = CLibResource::LoadStringFromResource(L"LBLVideoStabilizeFrame", 1);//LBLEFFECTOPENCL
	showFPS = CLibResource::LoadStringFromResource(L"LBLshowFPS",1);//L"Effect.Show FPS";
	enableFilmgrain = CLibResource::LoadStringFromResource(L"LBLFILMGRAIN", 1);
	libelleScale = CLibResource::LoadStringFromResource(L"LBLPICTUREFORMAT", 1);
	libelleZoom = CLibResource::LoadStringFromResource(L"LBLZOOMPICTURE", 1);
}

CVideoFilter::~CVideoFilter()
{
    
}

wxString CVideoFilter::GetFilterLabel()
{
	return "";
}


int CVideoFilter::GetNameFilter()
{
	return IDM_FILTRE_VIDEO;
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
	//int supportOpenCL = COpenCLEngine::SupportOpenCL();

	CVideoEffectParameter * videoEffectParameter = (CVideoEffectParameter *)effectParameter;
	vector<int> elementContrast;
    for (auto i = -100; i < 101; i++)
        elementContrast.push_back(i);

	/*
			tabRatio.push_back(1.1f);
		tabRatio.push_back(1.33f);
		tabRatio.push_back(1.66f);
		tabRatio.push_back(1.85f);
		tabRatio.push_back(2.35f);
	*/
	vector<CMetadata> formatPicture;
	AddMetadataElement(formatPicture, "auto", 0);
	AddMetadataElement(formatPicture, "1.33", 1);
	AddMetadataElement(formatPicture, "1.66", 2);
	AddMetadataElement(formatPicture, "1.85", 3);
	AddMetadataElement(formatPicture, "2.35", 4);

	vector<CMetadata> zoomPicture;
	for(int i = 0;i < videoEffectParameter->tabZoom.size();i++)
		AddMetadataElement(zoomPicture, to_string(videoEffectParameter->tabZoom[i]), i);

	filtreInterface->AddTreeInfos(libelleScale, new CTreeElementValueInt(videoEffectParameter->ratioSelect), &formatPicture, 3, 3);
	filtreInterface->AddTreeInfos(showFPS, new CTreeElementValueInt(videoEffectParameter->showFPS), &videoEffectParameter->showFPS, 2, 2);
	filtreInterface->AddTreeInfos(enableEffect, new CTreeElementValueInt(videoEffectParameter->effectEnable), &videoEffectParameter->effectEnable, 2, 2);
	filtreInterface->AddTreeInfos(libelleAutoContrast, new CTreeElementValueInt(videoEffectParameter->autoConstrast), &videoEffectParameter->autoConstrast, 2, 2);
	filtreInterface->AddTreeInfos(enableFilmgrain, new CTreeElementValueInt(videoEffectParameter->filmgrainenable), &videoEffectParameter->filmgrainenable, 2, 2);
	filtreInterface->AddTreeInfos(enableGrayScale, new CTreeElementValueInt(videoEffectParameter->grayEnable), &videoEffectParameter->grayEnable, 2, 2);
	filtreInterface->AddTreeInfos(enableVHSEffect, new CTreeElementValueInt(videoEffectParameter->vhsEnable), &videoEffectParameter->vhsEnable, 2, 2);
	filtreInterface->AddTreeInfos(enableBandCEffect, new CTreeElementValueInt(videoEffectParameter->bandcEnable), &videoEffectParameter->bandcEnable, 2, 2);
    filtreInterface->AddTreeInfos(toneConstrastLevel,new CTreeElementValueInt(videoEffectParameter->contrast), &elementContrast);
    filtreInterface->AddTreeInfos(toneBrightnessLevel,new CTreeElementValueInt(videoEffectParameter->brightness), &elementContrast);

	vector<int> elementFreqStab;
	for (auto i = 2; i < 100; i++)
		elementFreqStab.push_back(i);

	filtreInterface->AddTreeInfos(libelleStabilize, new CTreeElementValueInt(videoEffectParameter->stabilizeVideo), &videoEffectParameter->stabilizeVideo, 2, 2);
	filtreInterface->AddTreeInfos(libelleStabilizeNbFrame, new CTreeElementValueInt(videoEffectParameter->stabilizeImageBuffere), &elementFreqStab);

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
   
	filtreInterface->AddTreeInfos(enableDenoiseEffect, new CTreeElementValueInt(videoEffectParameter->denoiseEnable), &videoEffectParameter->denoiseEnable, 2, 2);
	
	vector<float> elementDenoiseLevel;
	for (float i = 0; i < 30; i += 1)
		elementDenoiseLevel.push_back(i);

	filtreInterface->AddTreeInfos(effectDenoising, new CTreeElementValueFloat(videoEffectParameter->denoisingLevel), &elementDenoiseLevel, 4);

	filtreInterface->AddTreeInfos(enableOpenglDenoising, new CTreeElementValueInt(videoEffectParameter->openglDenoise), &videoEffectParameter->openglDenoise, 2, 2);

	vector<float> elementSigma;
	for (float i = 1; i < 30; i+=2)
		elementSigma.push_back(i);

	filtreInterface->AddTreeInfos(effectDenoisingSigmaU, new CTreeElementValueFloat(videoEffectParameter->uSigma), &elementSigma, 4);
	filtreInterface->AddTreeInfos(effectDenoisingThreshold, new CTreeElementValueFloat(videoEffectParameter->uThreshold), &elementDenoiseLevel, 4);
	filtreInterface->AddTreeInfos(effectDenoisingSigmaK, new CTreeElementValueFloat(videoEffectParameter->uKSigma), &elementSigma, 4);


	//filtreInterface->AddTreeInfos(effectDenoisingBSize, new CTreeElementValueInt(videoEffectParameter->bSize), &elementNbTimes);
	//filtreInterface->AddTreeInfos(effectDenoisingFSize, new CTreeElementValueInt(videoEffectParameter->fSize), &elementNbTimes);


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
		case TYPE_ELEMENT_INT:
			{
				CTreeElementValueInt * intValue = (CTreeElementValueInt*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_ELEMENT_FLOAT:
			{
				CTreeElementValueFloat * intValue = (CTreeElementValueFloat*)valueData;
				value = intValue->GetValue();
			}
			break;
		case TYPE_ELEMENT_BOOL:
			{
				CTreeElementValueBool * intValue = (CTreeElementValueBool*)valueData;
				value = intValue->GetValue();
			}
			break;
		default: ;
	}

	//Video Parameter
	
	if (key == effectDenoisingSigmaU)
	{
		videoEffectParameter->uSigma = value;
	}
	/*
	else if (key == enableOpenCL)
	{
		videoEffectParameter->enableOpenCL = value;
	}    */
	else if (key == effectDenoising)
	{
		videoEffectParameter->denoisingLevel = value;
	}
	else if (key == enableVHSEffect)
	{
		videoEffectParameter->vhsEnable = value;
	}
	else if (key == libelleStabilizeNbFrame)
	{
		videoEffectParameter->stabilizeImageBuffere = value;
	}
	else if (key == libelleAutoContrast)
	{
		videoEffectParameter->autoConstrast = value;
	}
	else if (key == libelleStabilize)
	{
		videoEffectParameter->stabilizeVideo = value;
	}
	else if (key == libelleScale)
	{
		videoEffectParameter->zoomUpdate = true;
		videoEffectParameter->ratioSelect = value;
	}
	else if (key == libelleZoom)
	{
		videoEffectParameter->zoomUpdate = true;
		videoEffectParameter->zoomSelect = value;
	}
	else if (key == enableFilmgrain)
	{
		videoEffectParameter->filmgrainenable = value;
	}
	else if (key == enableBicubicInterpolation)
	{
		videoEffectParameter->BicubicEnable = value;
	}    
	/*
	else if (key == effectDenoisingSigmaK)
	{
		videoEffectParameter->uKSigma = value;
	}*/
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
	else if (key == enableDenoiseEffect)
	{
		videoEffectParameter->denoiseEnable = value;
	}
    else if (key == enableOpenglDenoising)
	{
		videoEffectParameter->openglDenoise = value;
	}
}
