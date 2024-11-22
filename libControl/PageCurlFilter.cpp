#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PageCurlFilter.h"
#include <FilterData.h>
#include <RGBAQuad.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include <GLTexture.h>

#include "effect_id.h"
using namespace Regards::Filter;
using namespace Regards::OpenGL;

CPageCurlFilter::CPageCurlFilter()
{
	initTexture = true;

}

CPageCurlFilter::~CPageCurlFilter()
{

}

void CPageCurlFilter::RenderTexture(CRenderBitmapOpenGL* renderOpenGL, const float& time, const float& invert,
                                    const int& width, const int& height, const int& left, const int& top)
{
	pictureFirst->Enable();
	pictureNext->Enable();
	GLSLShader* m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_PAGECURL");
	if (m_pShader != nullptr)
	{
		m_pShader->EnableShader();
		if (!m_pShader->SetTexture("sourceTex", pictureFirst->GetTextureID()))
		{
			printf("SetTexture sourceTex failed \n ");
		}
		if (!m_pShader->SetTexture("targetTex", pictureNext->GetTextureID()))
		{
			printf("SetTexture sourceTex failed \n ");
		}
		if (!m_pShader->SetParam("time", time))
		{
			printf("SetParam intensity failed \n ");
		}
		if (!m_pShader->SetParam("invertTex", invert))
		{
			printf("SetParam intensity failed \n ");
		}
	}

	renderOpenGL->ShowSecondBitmap(pictureNext, width, height, left, top, true);

	if (m_pShader != nullptr)
		m_pShader->DisableShader();

	//glDisable(GL_BLEND);
	pictureFirst->Disable();
	pictureNext->Disable();
}

int CPageCurlFilter::GetTypeFilter()
{
	return IDM_AFTEREFFECT_PAGECURL;
}


void CPageCurlFilter::SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	initTexture = true;
	bmpViewer->StartTransitionEffect(bmpSecond, false);
}

bool CPageCurlFilter::RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source,
                                    IBitmapDisplay* bmpViewer, CRenderBitmapOpenGL* renderOpenGL,
                                    const float& scale_factor, const int& etape)
{
	if (etape > 0 && etape < 110)
	{
		GenerateTexture(nextPicture, source, bmpViewer);
		int widthOutput = bmpViewer->GetWidth() * scale_factor;
		int heightOutput = bmpViewer->GetHeight() * scale_factor;
		RenderTexture(renderOpenGL, etape, false, widthOutput, heightOutput, 0, 0);

		return true;
	}
	return false;
}

void CPageCurlFilter::GenerateTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source,
                                      IBitmapDisplay* bmpViewer)
{
	bool init = false;
	if (initTexture || (pictureFirst->GetWidth() != bmpViewer->GetWidth() && pictureFirst->GetHeight() != bmpViewer->GetHeight()))
	{
		init = true;
		initTexture = false;
	}

	{
		CImageLoadingFormat bitmapNext;

		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			auto mat = cv::Mat(bmpViewer->GetHeight(), bmpViewer->GetWidth(), CV_8UC4,
			                   cv::Scalar(colorBack.GetBlue(), colorBack.GetGreen(), colorBack.GetRed(), 255));
			bitmapNext.SetPicture(mat);
			CImageLoadingFormat* bitmapOut = GenerateInterpolationBitmapTexture(nextPicture, bmpViewer);
			if (bitmapOut != nullptr)
			{
				bitmapNext.InsertBitmap(bitmapOut, out.x, out.y);
				//bitmapNext.Flip();
			}
			delete bitmapOut;

			mat = bitmapNext.GetMatrix().getMat();
			cv::flip(mat, mat, 0);
            Regards::Picture::CPictureArray pictureArray = Regards::Picture::CPictureArray(mat); 
			pictureNext->SetData(pictureArray);
		}
	}
	{
		CImageLoadingFormat bitmapFirst;

		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			auto mat = cv::Mat(bmpViewer->GetHeight(), bmpViewer->GetWidth(), CV_8UC4,
			                   cv::Scalar(colorBack.GetBlue(), colorBack.GetGreen(), colorBack.GetRed(), 255));
			bitmapFirst.SetPicture(mat);
			CImageLoadingFormat* bitmapOut = GenerateInterpolationBitmapTexture(source, bmpViewer);
			if (bitmapOut != nullptr)
			{
				bitmapFirst.InsertBitmap(bitmapOut, out.x, out.y);
				bitmapFirst.Flip();
			}
			delete bitmapOut;

			//mat = bitmapFirst.GetMatrix().getMat();
			Regards::Picture::CPictureArray pictureArray = bitmapFirst.GetMatrix(); 
			pictureFirst->SetData(pictureArray);
		}
	}

	
}


GLTexture* CPageCurlFilter::GetTexture(const int& numTexture)
{
	if (numTexture == 0)
		return pictureFirst;

	return pictureNext;
}
