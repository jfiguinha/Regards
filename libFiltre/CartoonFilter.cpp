#include <header.h>
#include "CartoonFilter.h"
#include "CartoonEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <BitmapDisplay.h>
using namespace Regards::Filter;

CCartoonFilter::CCartoonFilter()
{
    libelleEffectMode = L"effect.Mode";
}

CCartoonFilter::~CCartoonFilter()
{
}


int CCartoonFilter::GetTypeFilter()
{
    return IDM_FILTER_CARTOON;
}

void CCartoonFilter::AddMetadataElement(vector<CMetadata> & element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CCartoonFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	CCartoonEffectParameter * cartoonEffectParameter = (CCartoonEffectParameter *)effectParameter;
    
	this->source = source;

	vector<CMetadata> elementNoiseReduction;
	AddMetadataElement(elementNoiseReduction, "Sketch Mode", 0);
	AddMetadataElement(elementNoiseReduction, "Alien Mode", 1);
	AddMetadataElement(elementNoiseReduction, "Evil Mode", 2);
	AddMetadataElement(elementNoiseReduction, "Default", 3);
    
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


void CCartoonFilter::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput)
{
	CRegardsBitmap * bitmapOut = filtreEffet->GetBitmap(false);
	CCartoonEffectParameter * cartoonEffectParameter = (CCartoonEffectParameter *)effectParameter;
	CImageLoadingFormat image;
	image.SetPicture(bitmapOut);
	CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
	filtre->CartoonifyImage(cartoonEffectParameter->mode);

	filtreEffet->SetPreview(true);

	CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
	imageLoad->SetPicture(filtre->GetBitmap(true));
	filtreEffet->SetBitmap(imageLoad);

	delete filtre;

}

CImageLoadingFormat * CCartoonFilter::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (effectParameter != nullptr && source != nullptr)
	{
		CCartoonEffectParameter * cartoonEffectParameter = (CCartoonEffectParameter *)effectParameter;
		source->RotateExif(source->GetOrientation());
		CImageLoadingFormat image;
		image.SetPicture(source);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
		filtre->CartoonifyImage(cartoonEffectParameter->mode);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		delete filtre;
	}

	return imageLoad;
}

