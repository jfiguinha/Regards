#include <header.h>
//
//  SwirlFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "AudioVideoFilter.h"
#include "EffectVideoParameter.h"
#include <LibResource.h>
#include <FilterData.h>
#include <Metadata.h>
#include "videostream.h"

#include <effect_id.h>
#include "TreeElementValue.h"
using namespace Regards::Filter;
using namespace Regards::Video;

CAudioVideoFilter::CAudioVideoFilter()
{
	libelleAudio = CLibResource::LoadStringFromResource(L"LBLPARAMETERAUDIO", 1); //L"Parameter.Audio";
	libelleVideo = CLibResource::LoadStringFromResource(L"LBLPARAMETERVIDEO", 1); //L"Parameter.Video";
	enableSubtitle = CLibResource::LoadStringFromResource(L"LBLSHOWSUBTITLE", 1); //"Parameter.Show Subtitle";
	libelleSubtitle = CLibResource::LoadStringFromResource(L"LBLSUBTITLE", 1); //L"Parameter.Subtitle";
    libelleEffectSize = CLibResource::LoadStringFromResource(L"LBLEFFECTSIZE", 1); //"Effect.Size";
    libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED", 1);
	libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN", 1);
	libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE", 1);
}

CAudioVideoFilter::~CAudioVideoFilter()
{
}

wxString CAudioVideoFilter::GetFilterLabel()
{
	return "";
}

int CAudioVideoFilter::GetNameFilter()
{
	return IDM_FILTRE_AUDIOVIDEO;
}

int CAudioVideoFilter::GetTypeFilter()
{
	return IDM_FILTRE_AUDIOVIDEO;
}

void CAudioVideoFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                               IFiltreEffectInterface* filtreInterface)
{
	if (!source.empty())
	{
		Filter(effectParameter, filename, filtreInterface);
	}
}

void CAudioVideoFilter::Filter(CEffectParameter* effectParameter, const wxString& filename,
                               IFiltreEffectInterface* filtreInterface)
{
	vector<CStreamInfo> listStream = CVideoStream::GetVideoStream(filename);
	auto videoEffectParameter = static_cast<CVideoEffectParameter*>(effectParameter);

	int indexVideo = 0;
	int indexAudio = 0;
	int indexSubtitle = 0;
	vector<CMetadata> elementAudio;
	vector<CMetadata> elementVideo;
	vector<CMetadata> elementSubtitle;

	for (int i = 0; i < listStream.size(); i++)
	{
		CStreamInfo streamInfo = listStream[i];
		switch (streamInfo.typeStream)
		{
		case 1:
			if (streamInfo.selected)
				indexAudio = elementAudio.size();
			AddMetadataElement(elementAudio, streamInfo.streamLibelle, streamInfo.indexStream);
			break;
		case 2:
			if (streamInfo.selected)
				indexVideo = elementVideo.size();
			AddMetadataElement(elementVideo, streamInfo.streamLibelle, streamInfo.indexStream);
			break;
		case 3:
			if (streamInfo.selected)
				indexSubtitle = elementSubtitle.size();
			AddMetadataElement(elementSubtitle, streamInfo.streamLibelle, streamInfo.indexStream);
			break;
		default: ;
		}
	}

	filtreInterface->AddTreeInfos(libelleAudio, new CTreeElementValueInt(indexAudio), &elementAudio, 3, 3);
	filtreInterface->AddTreeInfos(libelleVideo, new CTreeElementValueInt(indexVideo), &elementVideo, 3, 3);
    
	if (elementSubtitle.size() > 0)
	{
        vector<float> vect{
            0.01f, 0.02f, 0.03f, 0.04f, 0.05f, 0.06f, 0.08f, 0.12f, 0.16f, 0.25f, 0.33f, 0.50f, 0.66f, 0.75f, 1.00f,
            1.33f, 1.50f, 1.66f, 2.0f, 3.00f, 4.00f, 5.00f, 6.00f, 7.00f, 8.00f, 12.00f, 16.00f
        };
        
		filtreInterface->AddTreeInfos(enableSubtitle, new CTreeElementValueInt(videoEffectParameter->enableSubtitle),
		                              &videoEffectParameter->enableSubtitle, 2, 2);
		filtreInterface->AddTreeInfos(libelleSubtitle, new CTreeElementValueInt(indexSubtitle), &elementSubtitle, 3, 3);
	
        /*
    	filtreInterface->AddTreeInfos(libelleEffectSize, new CTreeElementValueFloat(videoEffectParameter->subtitleSize, 2),
	                              &vect, 4);
                                  
    	vector<int> elementColor;
        for (auto i = 0; i < 256; i++)
            elementColor.push_back(i);

        filtreInterface->AddTreeInfos(libelleEffectColorRed, new CTreeElementValueInt(videoEffectParameter->subtitleRedColor),
                                      &elementColor);
        filtreInterface->AddTreeInfos(libelleEffectColorGreen, new CTreeElementValueInt(videoEffectParameter->subtitleGreenColor),
                                      &elementColor);
        filtreInterface->AddTreeInfos(libelleEffectColorBlue, new CTreeElementValueInt(videoEffectParameter->subtitleBlueColor),
                                      &elementColor);
             * */
    }

}

void CAudioVideoFilter::AddMetadataElement(vector<CMetadata>& element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CAudioVideoFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                          const wxString& key)
{
	auto videoEffectParameter = static_cast<CVideoEffectParameter*>(effectParameter);

	float value = 0.0;
	switch (valueData->GetType())
	{
	case TYPE_ELEMENT_INT:
		{
			auto intValue = static_cast<CTreeElementValueInt*>(valueData);
			value = intValue->GetValue();
		}
		break;
	case TYPE_ELEMENT_FLOAT:
		{
			auto intValue = static_cast<CTreeElementValueFloat*>(valueData);
			value = intValue->GetValue();
		}
		break;
	case TYPE_ELEMENT_BOOL:
		{
			auto intValue = static_cast<CTreeElementValueBool*>(valueData);
			value = intValue->GetValue();
		}
		break;
	default: ;
	}

	if (key == libelleAudio)
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
	else if (key == libelleEffectSize)
    {
        videoEffectParameter->subtitleSize = value;
    }
	else if (key == libelleEffectColorRed)
    {
        videoEffectParameter->subtitleRedColor = value;
    }
    else if (key == libelleEffectColorGreen)
    {
        videoEffectParameter->subtitleGreenColor = value;
    }
    else if (key == libelleEffectColorBlue)
    {
        videoEffectParameter->subtitleBlueColor = value;
    }
}
