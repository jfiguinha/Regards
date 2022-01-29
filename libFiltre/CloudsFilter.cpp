#include <header.h>
#include "CloudsFilter.h"
#include "CloudsEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
using namespace Regards::Filter;

CCloudsFilter::CCloudsFilter()
{
    libelleCloudsFrequency = CLibResource::LoadStringFromResource(L"LBLCLOUDSFREQUENCY",1);
    libelleCloudsAmplitude = CLibResource::LoadStringFromResource(L"LBLCLOUDSAMPLITUDE",1);
    libelleCloudsColorFrontRed = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORFRONTRED",1);
    libelleCloudsColorFrontGreen = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORFRONTGREEN",1);
    libelleCloudsColorFrontBlue = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORFRONTBLUE",1);
    libelleCloudsColorBackRed = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORBACKRED",1);
    libelleCloudsColorBackGreen = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORBACKGREEN",1);
    libelleCloudsColorBackBlue = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORBACKBLUE",1);
	libelleEffectIntensity  = CLibResource::LoadStringFromResource(L"LBLEFFECTINTENSITY",1);
}

CCloudsFilter::~CCloudsFilter()
{
    
}

int CCloudsFilter::TypeApplyFilter()
{
    return 2;
}

bool CCloudsFilter::IsOpenCLCompatible()
{
    return false;
}

wxString CCloudsFilter::GetFilterLabel()
{
    return CLibResource::LoadStringFromResource("LBLfilterClouds", 1);
}

int CCloudsFilter::GetNameFilter()
{
    return IDM_FILTRE_CLOUDS;
}

int CCloudsFilter::GetTypeFilter()
{
    return SPECIAL_EFFECT;// 
}

void CCloudsFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CCloudsEffectParameter * cloudsEffectParameter = (CCloudsEffectParameter *)effectParameter;
    
	this->source = source;

    vector<int> elementFreq;
    for (auto i = 0; i < 101; i++)
        elementFreq.push_back(i);
    
    vector<int> elementColor;
    for (auto i = 0; i < 256; i++)
        elementColor.push_back(i);
    
	filtreInterface->AddTreeInfos(libelleEffectIntensity, new CTreeElementValueInt(cloudsEffectParameter->transparency), &elementFreq);
    filtreInterface->AddTreeInfos(libelleCloudsFrequency, new CTreeElementValueInt( cloudsEffectParameter->frequence), &elementFreq);
    filtreInterface->AddTreeInfos(libelleCloudsAmplitude, new CTreeElementValueInt(cloudsEffectParameter->amplitude), &elementFreq);
    filtreInterface->AddTreeInfos(libelleCloudsColorFrontRed, new CTreeElementValueInt(cloudsEffectParameter->colorFront.GetRed()), &elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorFrontGreen, new CTreeElementValueInt(cloudsEffectParameter->colorFront.GetGreen()), &elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorFrontBlue, new CTreeElementValueInt(cloudsEffectParameter->colorFront.GetBlue()), &elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorBackRed, new CTreeElementValueInt(cloudsEffectParameter->colorBack.GetRed()), &elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorBackGreen, new CTreeElementValueInt(cloudsEffectParameter->colorBack.GetGreen()), &elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorBackBlue, new CTreeElementValueInt(cloudsEffectParameter->colorBack.GetBlue()), &elementColor);
}

void CCloudsFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CCloudsEffectParameter * cloudsEffectParameter = (CCloudsEffectParameter *)effectParameter;
    
    CTreeElementValueInt * value = (CTreeElementValueInt *)valueData;
    //Video Parameter
    if (key == libelleEffectIntensity)
    {
        cloudsEffectParameter->transparency = value->GetValue();
    }
    else if (key == libelleCloudsFrequency)
    {
        cloudsEffectParameter->frequence = value->GetValue();
    }
    else if (key == libelleCloudsAmplitude)
    {
        cloudsEffectParameter->amplitude = value->GetValue();
    }
    else if (key == L"Clouds.Octaves")
    {
        cloudsEffectParameter->octave = value->GetValue();
    }
    else if (key == libelleCloudsColorFrontRed)
    {
        cloudsEffectParameter->colorFront = CRgbaquad(value->GetValue(), cloudsEffectParameter->colorFront.GetGreen(), cloudsEffectParameter->colorFront.GetBlue());
    }
    else if (key == libelleCloudsColorFrontGreen)
    {
        cloudsEffectParameter->colorFront = CRgbaquad(cloudsEffectParameter->colorFront.GetRed(), value->GetValue(), cloudsEffectParameter->colorFront.GetBlue());
    }
    else if (key == libelleCloudsColorFrontBlue)
    {
        cloudsEffectParameter->colorFront = CRgbaquad(cloudsEffectParameter->colorFront.GetRed(), cloudsEffectParameter->colorFront.GetGreen(), value->GetValue());
    }
    else if (key == libelleCloudsColorBackRed)
    {
        cloudsEffectParameter->colorBack = CRgbaquad(value->GetValue(), cloudsEffectParameter->colorBack.GetGreen(), cloudsEffectParameter->colorBack.GetBlue());
    }
    else if (key == libelleCloudsColorBackGreen)
    {
        cloudsEffectParameter->colorBack = CRgbaquad(cloudsEffectParameter->colorBack.GetRed(), value->GetValue(), cloudsEffectParameter->colorBack.GetBlue());
    }
    else if (key == libelleCloudsColorBackBlue)
    {
        cloudsEffectParameter->colorBack = CRgbaquad(cloudsEffectParameter->colorBack.GetRed(), cloudsEffectParameter->colorBack.GetGreen(), value->GetValue());
    }
}

void CCloudsFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
    if (effectParameter != nullptr && filtreEffet != nullptr)
    {
        CCloudsEffectParameter* cloudsParameter = (CCloudsEffectParameter*)effectParameter;
        filtreEffet->CloudsFilter(cloudsParameter->colorFront, cloudsParameter->colorBack, cloudsParameter->amplitude, cloudsParameter->frequence, cloudsParameter->octave, cloudsParameter->transparency);
    }
}

bool CCloudsFilter::NeedPreview()
{
    return true;
}

CEffectParameter* CCloudsFilter::GetEffectPointer()
{
    return new CCloudsEffectParameter();
}

CEffectParameter* CCloudsFilter::GetDefaultEffectParameter()
{
    CCloudsEffectParameter* clouds = new CCloudsEffectParameter();
    clouds->colorFront = CRgbaquad(0, 0, 0);
    clouds->colorBack = CRgbaquad(255, 255, 255);
    clouds->amplitude = 1;
    clouds->frequence = 65;
    clouds->octave = 8;
    clouds->transparency = 50;
    return clouds;
}


bool CCloudsFilter::IsSourcePreview()
{
    return true;
}


void CCloudsFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
    CImageLoadingFormat* imageLoad = nullptr;
    if (effectParameter != nullptr && source != nullptr)
    {
        CImageLoadingFormat image(false);
        image.SetPicture(source);

        CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
        CCloudsEffectParameter* cloudsParameter = (CCloudsEffectParameter*)effectParameter;
        filtre->CloudsFilter(cloudsParameter->colorFront, cloudsParameter->colorBack, cloudsParameter->amplitude, cloudsParameter->frequence, cloudsParameter->octave, cloudsParameter->transparency);
        imageLoad = new CImageLoadingFormat();
        imageLoad->SetPicture(filtre->GetBitmap(true));
        delete filtre;

        filtreEffet->SetBitmap(imageLoad);

        delete imageLoad;

    }

}


void CCloudsFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}


CImageLoadingFormat* CCloudsFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
    CImageLoadingFormat* imageLoad = nullptr;
    if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
    {
        CCloudsEffectParameter* cloudsParameter = (CCloudsEffectParameter*)effectParameter;
        source->RotateExif(source->GetOrientation());
        CImageLoadingFormat image(false);
        image.SetPicture(source);
        CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
        filtre->CloudsFilter(cloudsParameter->colorFront, cloudsParameter->colorBack, cloudsParameter->amplitude, cloudsParameter->frequence, cloudsParameter->octave, cloudsParameter->transparency);
        imageLoad = new CImageLoadingFormat();
        imageLoad->SetPicture(filtre->GetBitmap(true));
        delete filtre;
    }

    return imageLoad;
}
