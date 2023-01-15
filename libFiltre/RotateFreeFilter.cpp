#include <header.h>
#include "RotateFreeFilter.h"
#include "FreeRotateEffectParameter.h"

#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
using namespace Regards::Filter;

CRotateFreeFilter::CRotateFreeFilter()
{
	libelleRotationAngle = CLibResource::LoadStringFromResource(L"LBLROTATIONANGLE", 1);
}

CRotateFreeFilter::~CRotateFreeFilter()
{
}

int CRotateFreeFilter::TypeApplyFilter()
{
	return 2;
}

wxString CRotateFreeFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLfilterRotate", 1);
}

int CRotateFreeFilter::GetNameFilter()
{
	return IDM_ROTATE_FREE;
}

int CRotateFreeFilter::GetTypeFilter()
{
	return ROTATE_EFFECT; //
}

bool CRotateFreeFilter::NeedToShrink()
{
	return true;
}

void CRotateFreeFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
                               IFiltreEffectInterface* filtreInterface)
{
	auto freeRotateEffectParameter = static_cast<CFreeRotateEffectParameter*>(effectParameter);

	this->source = source;
	this->filename = filename;

	vector<int> elementColor;
	for (auto i = 0; i < 361; i++)
		elementColor.push_back(i);

	filtreInterface->AddTreeInfos(libelleRotationAngle, new CTreeElementValueInt(freeRotateEffectParameter->angle),
	                              &elementColor);
}

void CRotateFreeFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
                                          const wxString& key)
{
	auto freeRotateEffectParameter = static_cast<CFreeRotateEffectParameter*>(effectParameter);

	auto valueInt = static_cast<CTreeElementValueInt*>(valueData);
	int value = valueInt->GetValue();
	//Video Parameter
	if (key == libelleRotationAngle)
	{
		freeRotateEffectParameter->angle = value;
	}

	freeRotateEffectParameter->updateEffect = true;
}

void CRotateFreeFilter::RenderEffect(CFiltreEffet* filtreEffet, CEffectParameter* effectParameter, const bool& preview)
{
	if (effectParameter != nullptr && filtreEffet != nullptr)
	{
		auto freeRotate = static_cast<CFreeRotateEffectParameter*>(effectParameter);
		filtreEffet->RotateFree(freeRotate->angle);
	}
}

bool CRotateFreeFilter::NeedPreview()
{
	return true;
}

CEffectParameter* CRotateFreeFilter::GetEffectPointer()
{
	return new CFreeRotateEffectParameter();
}

CEffectParameter* CRotateFreeFilter::GetDefaultEffectParameter()
{
	auto rotate = new CFreeRotateEffectParameter();
	rotate->angle = 50;
	return rotate;
}

bool CRotateFreeFilter::IsSourcePreview()
{
	return true;
}


void CRotateFreeFilter::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                                 CFiltreEffet* filtreEffet, CDraw* dessing)
{
	/*
	  if (effectParameter != nullptr && !source.empty())
	  {
	      CFreeRotateEffectParameter* rotate = (CFreeRotateEffectParameter*)effectParameter;
	      filtreEffet->RotateFree(rotate->angle);
	  }
	  */
}


void CRotateFreeFilter::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer,
                                           CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput,
                                           int& heightOutput)
{
	if (effectParameter != nullptr && !source.empty())
	{
		auto rotate = static_cast<CFreeRotateEffectParameter*>(effectParameter);
		filtreEffet->RotateFree(rotate->angle);
		/*
		cv::Mat preview = filtreEffet->GetBitmap(false);
		widthOutput = preview.size().width;
		heightOutput = preview.size().height;
		*/
	}
}

CImageLoadingFormat* CRotateFreeFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
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
			auto rotate = static_cast<CFreeRotateEffectParameter*>(effectParameter);
			filter->RotateFree(rotate->angle);
			cv::Mat bitmapOut = filter->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}


void CRotateFreeFilter::CancelPreview(IBitmapDisplay* bitmapViewer)
{
	if (!source.empty())
	{
		//Regards::Picture::CLibPicture libPicture;
		/*
		CImageLoadingFormat* imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(source);
		if (imageLoad != nullptr)
		{
		    bitmapViewer->UpdateBitmap(imageLoad, true);
		}
		*/
	}
}
