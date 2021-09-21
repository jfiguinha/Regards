#include <header.h>
#include "SharpenMaskingFilter.h"
#include "SharpenMaskingParameter.h"
#include <FilterData.h>
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <RenderOpenGL.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>

#include "MotionBlurFilter.h"
using namespace Regards::Filter;

CSharpenMaskingFilter::CSharpenMaskingFilter()
{
	libelleSharpness = CLibResource::LoadStringFromResource(L"LBLSHARPNESS", 1);
}

int CSharpenMaskingFilter::TypeApplyFilter()
{
	return 2;
}

bool CSharpenMaskingFilter::IsOpenGLCompatible()
{
	return false;
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

void CSharpenMaskingFilter::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
	CSharpenMaskingEffectParameter * sharpenMaskingParameter = (CSharpenMaskingEffectParameter *)effectParameter;

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

void CSharpenMaskingFilter::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	CSharpenMaskingEffectParameter * sharpenMaskingParameter = (CSharpenMaskingEffectParameter *)effectParameter;
	if (sharpenMaskingParameter != nullptr)
	{
        printf("GLSLShader IDR_GLSL_SHADER_SHARPENMASKING \n " );
		m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_SHADER_SHARPENMASKING");
		if (m_pShader != nullptr)
		{
			m_pShader->EnableShader();
			if (!m_pShader->SetTexture("textureScreen", textureID))
			{
				printf("SetTexture textureScreen failed \n ");
			}
			if (!m_pShader->SetParam("fWidth", renderOpenGL->GetWidth()))
			{
				printf("SetParam sharpness failed \n ");
			}
			if (!m_pShader->SetParam("fHeight", renderOpenGL->GetHeight()))
			{
				printf("SetParam sharpness failed \n ");
			}
			if (!m_pShader->SetParam("sharpness", sharpenMaskingParameter->sharpness))
			{
				printf("SetParam sharpness failed \n ");
			}
		}
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
	if (effectParameter != nullptr && source != nullptr)
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
	if (effectParameter != nullptr && source != nullptr && bitmapViewer != nullptr)
	{
		CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
		if (filter != nullptr)
		{
			imageLoad = new CImageLoadingFormat();
			CSharpenMaskingEffectParameter* sharpenParameter = (CSharpenMaskingEffectParameter*)effectParameter;
			filter->SharpenMasking(sharpenParameter->sharpness);
			CRegardsBitmap* bitmapOut = filter->GetBitmap(true);
			bitmapOut->RotateExif(source->GetOrientation());
			imageLoad->SetPicture(bitmapOut);
		}
	}

	return imageLoad;
}
