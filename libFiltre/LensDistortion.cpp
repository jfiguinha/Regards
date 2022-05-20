#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "LensDistortion.h"
#include "LensDistortionEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <BitmapDisplay.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
using namespace Regards::Filter;

CLensDistortion::CLensDistortion()
{
    libelleEffectStrength = CLibResource::LoadStringFromResource(L"LBLFILTERSTRENGTH", 1); //"Effect.Strength";
}

int CLensDistortion::TypeApplyFilter()
{
	return 2;
}

CLensDistortion::~CLensDistortion()
{
    
}

wxString CLensDistortion::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLLENSDISTORTION", 1);
}


int CLensDistortion::GetNameFilter()
{
	return IDM_FILTRELENSCORRECTION;
}

int CLensDistortion::GetTypeFilter()
{
	return CONVOLUTION_EFFECT; //
}

void CLensDistortion::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CLensDistortionEffectParameter * lensEffectParameter = (CLensDistortionEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementColor;
    for (auto i = 1; i < 1000; i+=10)
	{
		elementColor.push_back(i);
	}

    
    filtreInterface->AddTreeInfos(libelleEffectStrength,new CTreeElementValueInt(lensEffectParameter->strength), &elementColor);
}

void CLensDistortion::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CLensDistortionEffectParameter * lensEffectParameter = (CLensDistortionEffectParameter *)effectParameter;
    //Video Parameter
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

    if (key == libelleEffectStrength)
    {
        lensEffectParameter->strength = value;
    }

}

void CLensDistortion::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{

}

bool CLensDistortion::NeedPreview()
{
	return true;
}

CEffectParameter* CLensDistortion::GetEffectPointer()
{
	return new CLensDistortionEffectParameter();
}

CEffectParameter* CLensDistortion::GetDefaultEffectParameter()
{
	return new CLensDistortionEffectParameter();
}


bool CLensDistortion::IsSourcePreview()
{
	return true;
}


void CLensDistortion::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
    CLensDistortionEffectParameter * lensEffectParameter = (CLensDistortionEffectParameter *)effectParameter;
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image(false);
		image.SetPicture(source);
        
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		filtre->LensDistortionFilter(lensEffectParameter->strength);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;

		filtreEffet->SetBitmap(imageLoad);
		delete imageLoad;
	}
}


void CLensDistortion::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
}

CImageLoadingFormat* CLensDistortion::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		CLensDistortionEffectParameter * lensEffectParameter = (CLensDistortionEffectParameter *)effectParameter;
		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image(false);
		image.SetPicture(source);
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		filtre->LensDistortionFilter(lensEffectParameter->strength);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}
