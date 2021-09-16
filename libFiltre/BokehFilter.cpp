#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BokehFilter.h"
#include "BokehEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <BitmapDisplay.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <FaceDetector.h>
using namespace Regards::Filter;
using namespace Regards::OpenCV;
CBokehFilter::CBokehFilter()
{
	libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS", 1);
	libelleEffectBoxSize = CLibResource::LoadStringFromResource(L"LBLEFFECTBOXSIZE", 1);
}

int CBokehFilter::TypeApplyFilter()
{
	return 2;
}

CBokehFilter::~CBokehFilter()
{

}

wxString CBokehFilter::GetFilterLabel()
{
	return "Bokeh Effect";//CLibResource::LoadStringFromResource("LBLBokehDENOISING", 1);
}


int CBokehFilter::GetNameFilter()
{
	return IDM_FILTRE_BOKEH;
}

int CBokehFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT; //
}

void CBokehFilter::Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface)
{
	CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;

	this->source = source;

	vector<int> elementColor;
	for (auto i = 0; i < 100; i++)
	{
		if(i%2 != 0)
			elementColor.push_back(i);
	}
	filtreInterface->AddTreeInfos(libelleEffectRadius, new CTreeElementValueInt(BokehEffectParameter->radius), &elementColor);
	filtreInterface->AddTreeInfos(libelleEffectBoxSize, new CTreeElementValueInt(BokehEffectParameter->boxSize), &elementColor);

}

void CBokehFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key)
{
	CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;
	//Video Parameter
	float value = 0.0;
	switch (valueData->GetType())
	{
	case TYPE_ELEMENT_INT:
	{
		CTreeElementValueInt* intValue = (CTreeElementValueInt*)valueData;
		value = intValue->GetValue();
	}
	break;
	case TYPE_ELEMENT_FLOAT:
	{
		CTreeElementValueFloat* intValue = (CTreeElementValueFloat*)valueData;
		value = intValue->GetValue();
	}
	break;
	case TYPE_ELEMENT_BOOL:
	{
		CTreeElementValueBool* intValue = (CTreeElementValueBool*)valueData;
		value = intValue->GetValue();
	}
	break;
	}
	
	if (key == libelleEffectRadius)
	{
		BokehEffectParameter->radius = value;
	}
	else if (key == libelleEffectBoxSize)
	{
		BokehEffectParameter->boxSize = value;
	}
}

void CBokehFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;
		filtreEffet->BokehEffect(BokehEffectParameter->radius, BokehEffectParameter->boxSize, nbFace, faceRect);
	}
}

bool CBokehFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CBokehFilter::GetEffectPointer()
{
	return new CBokehEffectParameter();
}

CEffectParameter* CBokehFilter::GetDefaultEffectParameter()
{
	return new CBokehEffectParameter();
}


void CBokehFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
	CRegardsBitmap* bitmapOut = filtreEffet->GetBitmap(false);
	CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;
	if(bitmapOut != nullptr && BokehEffectParameter != nullptr)
	{
		CImageLoadingFormat image;
		image.SetPicture(bitmapOut);

		if(oldWidth != bitmapOut->GetBitmapWidth() || oldHeight != bitmapOut->GetBitmapHeight())
		{
			bitmapOut->VertFlipBuf();

			//Find Face on source
			cv::Mat dst;
			cv::Mat image_local(bitmapOut->GetBitmapHeight(), bitmapOut->GetBitmapWidth(), CV_8UC4, bitmapOut->GetPtBitmap());
			cv::cvtColor(image_local, dst, cv::COLOR_BGRA2BGR);

			//Extract Face and Get Rectangle
			CFaceDetector face_detector(true);
			vector<cv::Rect> listFace = face_detector.GetRectFace(dst);
			if (listFace.size() > 0)
			{
				faceRect.SetX(listFace[0].x);
				faceRect.SetY(listFace[0].y);
				faceRect.SetWidth(listFace[0].width);
				faceRect.SetHeight(listFace[0].height);
			}

			nbFace = listFace.size();

			bitmapOut->VertFlipBuf();

			oldWidth = bitmapOut->GetBitmapWidth();
			oldHeight = bitmapOut->GetBitmapHeight();
		}
				
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		filtre->BokehEffect(BokehEffectParameter->radius, BokehEffectParameter->boxSize, nbFace, faceRect);

		DrawingToPicture(effectParameter, bitmapViewer, filtre, m_cDessin);

		filtreEffet->SetPreview(true);

		CImageLoadingFormat* imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		filtreEffet->SetBitmap(imageLoad);

		delete filtre;
	}


}

CImageLoadingFormat* CBokehFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image(false);
		image.SetPicture(source);

		wxRect faceRect;
		int nbFace = 0;

		source->VertFlipBuf();

		//Find Face on source
		cv::Mat dst;
		cv::Mat image_local(source->GetBitmapHeight(), source->GetBitmapWidth(), CV_8UC4, source->GetPtBitmap());
		cv::cvtColor(image_local, dst, cv::COLOR_BGRA2BGR);

		//Extract Face and Get Rectangle
		CFaceDetector face_detector(true);
		vector<cv::Rect> listFace = face_detector.GetRectFace(dst);
		if (listFace.size() > 0)
		{
			faceRect.SetX(listFace[0].x);
			faceRect.SetY(listFace[0].y);
			faceRect.SetWidth(listFace[0].width);
			faceRect.SetHeight(listFace[0].height);
		}

		nbFace = listFace.size();

		source->VertFlipBuf();
		
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		CBokehEffectParameter* BokehEffectParameter = (CBokehEffectParameter*)effectParameter;
		filtre->BokehEffect(BokehEffectParameter->radius, BokehEffectParameter->boxSize, nbFace, faceRect);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}
