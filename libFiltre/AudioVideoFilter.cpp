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
#include <RegardsBitmap.h>
using namespace Regards::Filter;
using namespace Regards::Video;

CAudioVideoFilter::CAudioVideoFilter()
{
	libelleAudio =  CLibResource::LoadStringFromResource(L"LBLPARAMETERAUDIO",1);//L"Parameter.Audio";
	libelleVideo=  CLibResource::LoadStringFromResource(L"LBLPARAMETERVIDEO",1);//L"Parameter.Video";
	enableSubtitle =  CLibResource::LoadStringFromResource(L"LBLSHOWSUBTITLE",1);//"Parameter.Show Subtitle";
	libelleSubtitle=  CLibResource::LoadStringFromResource(L"LBLSUBTITLE",1);//L"Parameter.Subtitle";
}

CAudioVideoFilter::~CAudioVideoFilter()
{
    
}


int CAudioVideoFilter::GetNameFilter()
{
	return IDM_FILTRE_AUDIOVIDEO;
}

int CAudioVideoFilter::GetTypeFilter()
{
    return IDM_FILTRE_AUDIOVIDEO;
}

void CAudioVideoFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    if(source != nullptr)
    {
        Filter(effectParameter, source->GetFilename(), filtreInterface);
    }
}

void CAudioVideoFilter::Filter(CEffectParameter * effectParameter, const wxString & filename, IFiltreEffectInterface * filtreInterface)
{   
	vector<CStreamInfo> listStream = CVideoStream::GetVideoStream(filename);
	CVideoEffectParameter * videoEffectParameter = (CVideoEffectParameter *)effectParameter;

	int indexVideo = 0;
	int indexAudio = 0;
	int indexSubtitle = 0;
	vector<CMetadata> elementAudio;
	vector<CMetadata> elementVideo;
	vector<CMetadata> elementSubtitle;

	for(int i = 0;i < listStream.size();i++)
	{
		CStreamInfo streamInfo = listStream[i];
		switch(streamInfo.typeStream)
		{
		case 1:
			if(streamInfo.selected)
				indexAudio = elementAudio.size();
			AddMetadataElement(elementAudio, streamInfo.streamLibelle, streamInfo.indexStream);
			break;
		case 2:
			if(streamInfo.selected)
				indexVideo = elementVideo.size();
			AddMetadataElement(elementVideo, streamInfo.streamLibelle, streamInfo.indexStream);
			break;
		case 3:
			if(streamInfo.selected)
				indexSubtitle = elementSubtitle.size();
			AddMetadataElement(elementSubtitle, streamInfo.streamLibelle, streamInfo.indexStream);
			break;
		}
	}
	
	filtreInterface->AddTreeInfos(libelleAudio, new CTreeElementValueInt(indexAudio), &elementAudio, 3, 3);
	filtreInterface->AddTreeInfos(libelleVideo, new CTreeElementValueInt(indexVideo), &elementVideo, 3, 3);
	if(elementSubtitle.size() > 0)
	{
		filtreInterface->AddTreeInfos(enableSubtitle, new CTreeElementValueInt(videoEffectParameter->enableSubtitle), &videoEffectParameter->enableSubtitle, 2, 2);
		filtreInterface->AddTreeInfos(libelleSubtitle, new CTreeElementValueInt(indexSubtitle), &elementSubtitle, 3, 3);
	}
}

void CAudioVideoFilter::AddMetadataElement(vector<CMetadata> & element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CAudioVideoFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
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
}


