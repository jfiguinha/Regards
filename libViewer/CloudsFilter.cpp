#include "CloudsFilter.h"
#include <vector>
#include "CloudsEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>

using namespace Regards::Viewer;

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
}

CCloudsFilter::~CCloudsFilter()
{
    
}

int CCloudsFilter::GetTypeFilter()
{
    return IDM_FILTRE_CLOUDS;
}

void CCloudsFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CCloudsEffectParameter * cloudsEffectParameter = (CCloudsEffectParameter *)effectParameter;
    
    vector<int> elementFreq;
    for (int i = 0; i < 101; i++)
        elementFreq.push_back(i);
    
    vector<int> elementColor;
    for (int i = 0; i < 256; i++)
        elementColor.push_back(i);
    
    filtreInterface->AddTreeInfos(libelleCloudsFrequency, cloudsEffectParameter->frequence * 100, elementFreq);
    filtreInterface->AddTreeInfos(libelleCloudsAmplitude, cloudsEffectParameter->amplitude * 100, elementFreq);
    filtreInterface->AddTreeInfos(libelleCloudsColorFrontRed, cloudsEffectParameter->colorFront.GetRed(), elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorFrontGreen, cloudsEffectParameter->colorFront.GetGreen(), elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorFrontBlue, cloudsEffectParameter->colorFront.GetBlue(), elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorBackRed, cloudsEffectParameter->colorBack.GetRed(), elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorBackGreen, cloudsEffectParameter->colorBack.GetGreen(), elementColor);
    filtreInterface->AddTreeInfos(libelleCloudsColorBackBlue, cloudsEffectParameter->colorBack.GetBlue(), elementColor);
}

void CCloudsFilter::FilterChangeParam(CEffectParameter * effectParameter, const int &valueData, const wxString &key)
{
    CCloudsEffectParameter * cloudsEffectParameter = (CCloudsEffectParameter *)effectParameter;
    
    float value = valueData;
    //Video Parameter
    if (key == libelleCloudsFrequency)
    {
        cloudsEffectParameter->frequence = value / 100.0;
    }
    if (key == libelleCloudsAmplitude)
    {
        cloudsEffectParameter->amplitude = value / 100.0;
    }
    if (key == L"Clouds.Octaves")
    {
        cloudsEffectParameter->octave = value;
    }
    if (key == libelleCloudsColorFrontRed)
    {
        cloudsEffectParameter->colorFront = CRgbaquad(value, cloudsEffectParameter->colorFront.GetGreen(), cloudsEffectParameter->colorFront.GetBlue());
    }
    if (key == libelleCloudsColorFrontGreen)
    {
        cloudsEffectParameter->colorFront = CRgbaquad(cloudsEffectParameter->colorFront.GetRed(), value, cloudsEffectParameter->colorFront.GetBlue());
    }
    if (key == libelleCloudsColorFrontBlue)
    {
        cloudsEffectParameter->colorFront = CRgbaquad(cloudsEffectParameter->colorFront.GetRed(), cloudsEffectParameter->colorFront.GetGreen(), value);
    }
    if (key == libelleCloudsColorBackRed)
    {
        cloudsEffectParameter->colorBack = CRgbaquad(value, cloudsEffectParameter->colorBack.GetGreen(), cloudsEffectParameter->colorBack.GetBlue());
    }
    if (key == libelleCloudsColorBackGreen)
    {
        cloudsEffectParameter->colorBack = CRgbaquad(cloudsEffectParameter->colorBack.GetRed(), value, cloudsEffectParameter->colorBack.GetBlue());
    }
    if (key == libelleCloudsColorBackBlue)
    {
        cloudsEffectParameter->colorBack = CRgbaquad(cloudsEffectParameter->colorBack.GetRed(), cloudsEffectParameter->colorBack.GetGreen(), value);
    }
}