#include <header.h>
#include "SharpenMaskingFilter.h"
#include "SharpenMaskingParameter.h"
#include <FilterData.h>

#include <LibResource.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>

using namespace Regards::Filter;

CSharpenMaskingFilter::CSharpenMaskingFilter()
{
	libelleSharpness = CLibResource::LoadStringFromResource(L"LBLSHARPNESS", 1);
}

int CSharpenMaskingFilter::TypeApplyFilter()
{
	return 2;
}

CSharpenMaskingFilter::~CSharpenMaskingFilter()
{
}

wxString CSharpenMaskingFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLSharpenMasking", 1);
}

int CSharpenMaskingFilter::GetNameFilter()
{
	return IDM_SHARPENMASKING;
}

int CSharpenMaskingFilter::GetTypeFilter()
{
	return CONVOLUTION_EFFECT; //return IDM_SHARPENMASKING;
}

void CSharpenMaskingFilter::Filter(CEffectParameter * effectParameter, cv::Mat & source, const wxString& filename, IFiltreEffectInterface * filtreInterface)
{
	CSharpenMaskingEffectParameter * sharpenMaskingParameter = (CSharpenMaskingEffectParameter *)effectParameter;
	this->filename = filename;
	this->source = source;

	vector<int> elementFreq;
	for (auto i = 0; i < 100; i++)
		elementFreq.push_back(i);

	filtreInterface->AddTreeInfos(libelleSharpness, new CTreeElementValueInt(sharpenMaskingParameter->sharpness), &elementFreq);
}

void CSharpenMaskingFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CSharpenMaskingEffectParameter * sharpenMaskingParameter = (CSharpenMaskingEffectParameter *)effectParameter;

	CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
    int value = valueInt->GetValue();

	//Video Parameter
	if (key == libelleSharpness)
	{
		sharpenMaskingParameter->sharpness = (float)value / 10.0f;
	}
}


void CSharpenMaskingFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CSharpenMaskingEffectParameter* sharpenParameter = (CSharpenMaskingEffectParameter*)effectParameter;
		filtreEffet->SharpenMasking(sharpenParameter->sharpness);
	}
}

bool CSharpenMaskingFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CSharpenMaskingFilter::GetEffectPointer()
{
	return new CSharpenMaskingEffectParameter();
}

CEffectParameter* CSharpenMaskingFilter::GetDefaultEffectParameter()
{
	CSharpenMaskingEffectParameter* sharpen = new CSharpenMaskingEffectParameter();
	sharpen->sharpness = 1.0f;
	return sharpen;
}


bool CSharpenMaskingFilter::IsSourcePreview()
{
	return true;
}


void CSharpenMaskingFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	if (effectParameter != nullptr && !source.empty())
	{
		CSharpenMaskingEffectParameter* sharpenParameter = (CSharpenMaskingEffectParameter*)effectParameter;
		filtreEffet->SharpenMasking(sharpenParameter->sharpness);
	}

}


void CSharpenMaskingFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{

}

CImageLoadingFormat* CSharpenMaskingFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty() && bitmapViewer != nullptr)
	{
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(source);
			image.RotateExif(orientation);
			filter->SetBitmap(&image);
			
			imageLoad = new CImageLoadingFormat();
			CSharpenMaskingEffectParameter* sharpenParameter = (CSharpenMaskingEffectParameter*)effectParameter;
			filter->SharpenMasking(sharpenParameter->sharpness);
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
