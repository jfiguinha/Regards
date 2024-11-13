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
	return SPECIAL_EFFECT; // 
}

void CCartoonFilter::AddMetadataElement(vector<CMetadata>& element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CCartoonFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                            IFiltreEffectInterface* filtreInterface)
{
	auto cartoonEffectParameter = static_cast<CCartoonEffectParameter*>(effectParameter);

	this->source = source;
	this->filename = filename;
	vector<CMetadata> elementNoiseReduction;
	AddMetadataElement(elementNoiseReduction, "Default", 0);
	AddMetadataElement(elementNoiseReduction, "Sketch Mode", 1);
	AddMetadataElement(elementNoiseReduction, "Alien Mode", 2);
	AddMetadataElement(elementNoiseReduction, "Evil Mode", 3);


	filtreInterface->AddTreeInfos(libelleEffectMode, new CTreeElementValueInt(cartoonEffectParameter->mode),
	                              &elementNoiseReduction, 3, 3);
}

void CCartoonFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                       const wxString& key)
{
	auto cartoonEffectParameter = static_cast<CCartoonEffectParameter*>(effectParameter);

	if (key == libelleEffectMode)
	{
		auto valueInt = static_cast<CTreeElementValueInt*>(valueData);
		int value = valueInt->GetValue();
		cartoonEffectParameter->mode = value;
	}
}


void CCartoonFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto cartoonEffectParameter = static_cast<CCartoonEffectParameter*>(effectParameter);
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
	auto cartoonEffectParameter = new CCartoonEffectParameter();
	cartoonEffectParameter->mode = 0;
	return cartoonEffectParameter;
}


bool CCartoonFilter::IsSourcePreview()
{
	return true;
}


void CCartoonFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                              CFiltreEffet* filtreEffet, CDraw* dessing)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty())
	{
		auto cartoonEffectParameter = static_cast<CCartoonEffectParameter*>(effectParameter);
		CImageLoadingFormat image;
		image.SetPicture(source);

		auto filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), false, false, &image);
		filtre->CartoonifyImage(cartoonEffectParameter->mode);
		imageLoad = new CImageLoadingFormat();
		cv::Mat mat = filtre->GetBitmap(true);
		imageLoad->SetPicture(mat);
		delete filtre;

		filtreEffet->SetBitmap(imageLoad);

		delete imageLoad;
	}
}


void CCartoonFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                        CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                        int& heightOutput)
{
}

CImageLoadingFormat* CCartoonFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	CImageLoadingFormat* imageLoad = nullptr;
	if (effectParameter != nullptr && !source.empty() && bitmapViewer != nullptr)
	{
		auto cartoonEffectParameter = static_cast<CCartoonEffectParameter*>(effectParameter);

		CImageLoadingFormat image;
		image.SetPicture(source);
		image.RotateExif(orientation);
		auto filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), false, false, &image);
		filtre->CartoonifyImage(cartoonEffectParameter->mode);
		imageLoad = new CImageLoadingFormat();
		cv::Mat mat = filtre->GetBitmap(true);
		imageLoad->SetPicture(mat);
		delete filtre;
	}

	return imageLoad;
}
