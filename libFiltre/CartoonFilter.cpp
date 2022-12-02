#include <header.h>
#include "CartoonFilter.h"
#include "CartoonEffectParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
using namespace Regards::Filter;

CCartoonFilter::CCartoonFilter()
{
	libelleEffectMode = CLibResource::LoadStringFromResource(L"LBLEFFECTMODE", 1);
  //  libelleEffectMode = L"effect.Mode";
}

CCartoonFilter::~CCartoonFilter()
{
}

int CCartoonFilter::TypeApplyFilter()
{
	return 2;
}

wxString CCartoonFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLFILTRECARTOON", 1);
}

int CCartoonFilter::GetNameFilter()
{
	return IDM_FILTER_CARTOON;
}

int CCartoonFilter::GetTypeFilter()
{
	return SPECIAL_EFFECT;// 
}

void CCartoonFilter::AddMetadataElement(vector<CMetadata> & element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CCartoonFilter::Filter(CEffectParameter * effectParameter, cv::Mat & source, const wxString& filename, IFiltreEffectInterface * filtreInterface)
{
	CCartoonEffectParameter * cartoonEffectParameter = (CCartoonEffectParameter *)effectParameter;
    
	this->source = source;
	this->filename = filename;
	vector<CMetadata> elementNoiseReduction;
	AddMetadataElement(elementNoiseReduction, "Default", 0);
	AddMetadataElement(elementNoiseReduction, "Sketch Mode", 1);
	AddMetadataElement(elementNoiseReduction, "Alien Mode", 2);
	AddMetadataElement(elementNoiseReduction, "Evil Mode", 3);

    
	filtreInterface->AddTreeInfos(libelleEffectMode, new CTreeElementValueInt(cartoonEffectParameter->mode), &elementNoiseReduction, 3, 3);
}

void CCartoonFilter::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
	CCartoonEffectParameter * cartoonEffectParameter = (CCartoonEffectParameter *)effectParameter;

    if (key == libelleEffectMode)
    {
		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue(); 
		cartoonEffectParameter->mode = value;
    }
}


void CCartoonFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		CCartoonEffectParameter* cartoonEffectParameter = (CCartoonEffectParameter*)effectParameter;
		filtreEffet->CartoonifyImage(cartoonEffectParameter->mode);
	}
}

bool CCartoonFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CCartoonFilter::GetEffectPointer()
{
	return new CCartoonEffectParameter();
}

CEffectParameter* CCartoonFilter::GetDefaultEffectParameter()
{
	CCartoonEffectParameter* cartoonEffectParameter = new CCartoonEffectParameter();
	cartoonEffectParameter->mode = 0;
	return cartoonEffectParameter;
}


bool CCartoonFilter::IsSourcePreview()
{
	return true;
}


void CCartoonFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* dessing)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty())
	{
		CCartoonEffectParameter* cartoonEffectParameter = (CCartoonEffectParameter*)effectParameter;
		CImageLoadingFormat image;
		image.SetPicture(source);

		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), false, &image);
		filtre->CartoonifyImage(cartoonEffectParameter->mode);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;

		filtreEffet->SetBitmap(imageLoad);

		delete imageLoad;
	}

}


void CCartoonFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{


}

CImageLoadingFormat* CCartoonFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty() && bitmapViewer != nullptr)
	{
		CCartoonEffectParameter* cartoonEffectParameter = (CCartoonEffectParameter*)effectParameter;

		CImageLoadingFormat image;
		image.SetPicture(source);
		image.RotateExif(orientation);
		CFiltreEffet* filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), false, &image);
		filtre->CartoonifyImage(cartoonEffectParameter->mode);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}
