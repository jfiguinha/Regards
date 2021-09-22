#include <header.h>
#include "DecodeRaw.h"
#include "DecodeRawParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <Metadata.h>
#include <ConvertUtility.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include "DecodeRawPicture.h"
#include <wx/busyinfo.h>
using namespace Regards::Filter;


CDecodeRaw::CDecodeRaw()
{
	libellebright = "Effect.Brightness";
	libellehighlight = "Effect.Highlight";
	libellethreshold = "Effect.Treshold";
	libelleuse_auto_wb = "Effect.AutoWB";
	libelleuse_camera_wb = "Effect.CameraWB";

}

CDecodeRaw::~CDecodeRaw()
{
	if (rawDecoder != nullptr)
		delete rawDecoder;
	rawDecoder = nullptr;
}


bool CDecodeRaw::IsOpenCLCompatible()
{
	return false;
}

int CDecodeRaw::TypeApplyFilter()
{
	return 2;
}

wxString CDecodeRaw::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLBLACKROOM", 1);
}


int CDecodeRaw::GetNameFilter()
{
	return IDM_DECODE_RAW;
}

int CDecodeRaw::GetTypeFilter()
{
	return IDM_DECODE_RAW;
}

bool CDecodeRaw::NeedPreview()
{
	return true;
}


void CDecodeRaw::AddMetadataElement(vector<CMetadata>& element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

CEffectParameter* CDecodeRaw::GetEffectPointer()
{
	return new CDecodeRawParameter();
}

void CDecodeRaw::Filter(CEffectParameter* effectParameter, CRegardsBitmap* source, IFiltreEffectInterface* filtreInterface)
{
	CDecodeRawParameter* decodeParameter = (CDecodeRawParameter*)effectParameter;

	if (source != nullptr)
		orientation = source->GetOrientation();

	rawDecoder = new CDecodeRawPicture(CConvertUtility::ConvertToStdString(source->GetFilename()));

	vector<int> elementColor;
	for (auto i = 0; i < 10; i++)
		elementColor.push_back(i);

	vector<float> elementBright;
	for (float i = 1.0; i < 10.0; i += 1.0)
		elementBright.push_back(i);


	vector<int> elementValue;
	for (auto i = -100; i < 101; i++)
		elementValue.push_back(i);
	

	filtreInterface->AddTreeInfos(libellebright, new CTreeElementValueFloat(decodeParameter->bright, 1), &elementBright, 4);
	filtreInterface->AddTreeInfos(libellehighlight, new CTreeElementValueInt(decodeParameter->highlight), &elementColor);
	filtreInterface->AddTreeInfos(libellethreshold, new CTreeElementValueInt(decodeParameter->threshold), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_auto_wb, new CTreeElementValueInt(decodeParameter->use_auto_wb), &decodeParameter->use_auto_wb, 2, 2);
	filtreInterface->AddTreeInfos(libelleuse_camera_wb, new CTreeElementValueInt(decodeParameter->use_camera_wb), &decodeParameter->use_camera_wb, 2, 2);

}

void CDecodeRaw::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData, const wxString& key)
{
	CDecodeRawParameter* decodeParameter = (CDecodeRawParameter*)effectParameter;

	//Video Parameter
	if (key == libellebright)
	{
		CTreeElementValueFloat* valueInt = (CTreeElementValueFloat*)valueData;
		float value = valueInt->GetValue();
		decodeParameter->bright = value;
	}
	else if (key == libellehighlight)
	{
		CTreeElementValueInt* valueInt = (CTreeElementValueInt*)valueData;
		int value = valueInt->GetValue();
		decodeParameter->highlight = value;
	}
	else if (key == libellethreshold)
	{
		CTreeElementValueInt* valueInt = (CTreeElementValueInt*)valueData;
		int value = valueInt->GetValue();
		decodeParameter->threshold = value;
	}
	else if (key == libelleuse_auto_wb)
	{
		CTreeElementValueBool* valueInt = (CTreeElementValueBool*)valueData;
		int value = valueInt->GetValue();
		decodeParameter->use_auto_wb = value;
	}
	else if (key == libelleuse_camera_wb)
	{
		CTreeElementValueBool* valueInt = (CTreeElementValueBool*)valueData;
		int value = valueInt->GetValue();
		decodeParameter->use_camera_wb = value;
	}

	decodeParameter->update = true;
}


CImageLoadingFormat* CDecodeRaw::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	if (effectParameter == nullptr || bitmapViewer == nullptr || rawDecoder == nullptr)
		return nullptr;

	CDecodeRawParameter* rawParameter = (CDecodeRawParameter*)effectParameter;
	wxBusyInfo wait("Please wait, working...");
	return rawDecoder->DecodePicture(rawParameter);
}

void CDecodeRaw::ApplyPreviewEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin, int& widthOutput, int& heightOutput)
{
	if (effectParameter == nullptr || bitmapViewer == nullptr || rawDecoder == nullptr)
		return;

	wxString filename = bitmapViewer->GetFilename();

	CDecodeRawParameter* rawParameter = (CDecodeRawParameter*)effectParameter;

	if(rawParameter->update)
	{
		wxBusyInfo wait("Please wait, working...");
		CImageLoadingFormat* imageLoad = rawDecoder->DecodePicture(rawParameter);
		if (imageLoad != nullptr)
		{
			if (firstUpdate)
			{
				bitmapViewer->UpdateBitmap(imageLoad, true);
				firstUpdate = false;
			}
			else
			{
				bitmapViewer->UpdateBitmap(imageLoad, false);
			}
		}
		rawParameter->update = false;
	}

}