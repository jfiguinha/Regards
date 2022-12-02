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
	pictureFirst = new GLTexture();
}

CPageCurlFilter::~CPageCurlFilter()
{
	CPageCurlFilter::DeleteTexture();
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

void CPageCurlFilter::DeleteTexture()
{
	if (pictureNext != nullptr)
		delete(pictureNext);
	pictureNext = nullptr;

	if (pictureFirst != nullptr)
		delete(pictureFirst);
	pictureFirst = nullptr;
}

void CPageCurlFilter::SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	DeleteTexture();

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
	if (initTexture || pictureFirst == nullptr || pictureNext == nullptr || (pictureFirst->GetWidth() != bmpViewer->
		GetWidth() && pictureFirst->GetHeight() != bmpViewer->GetHeight()))
	{
		init = true;
	}

	{
		cv::Mat bitmapNext;

		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			bitmapNext = cv::Mat(bmpViewer->GetHeight(), bmpViewer->GetWidth(), CV_8UC4, cv::Scalar(colorBack.GetRed(), colorBack.GetGreen(), colorBack.GetBlue(), 255));
			CImageLoadingFormat * bitmapOut = GenerateInterpolationBitmapTexture(nextPicture, bmpViewer);
			if (bitmapOut != nullptr)
			{
				cv::Mat mat = bitmapOut->GetOpenCVPicture();
				mat.copyTo(bitmapNext(cv::Rect(out.x, out.y, bitmapOut->GetWidth(), bitmapOut->GetHeight())));
			}
			delete bitmapOut;
			if (pictureNext == nullptr)
				pictureNext = new GLTexture();
			pictureNext->SetData(bitmapNext);
		}

	}
	{
		cv::Mat bitmapFirst;

		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			bitmapFirst = cv::Mat(bmpViewer->GetHeight(), bmpViewer->GetWidth(), CV_8UC4, cv::Scalar(colorBack.GetRed(), colorBack.GetGreen(), colorBack.GetBlue(), 255));
			CImageLoadingFormat* bitmapOut = GenerateInterpolationBitmapTexture(source, bmpViewer);
			if (bitmapOut != nullptr)
			{
				cv::Mat mat = bitmapOut->GetOpenCVPicture();
				mat.copyTo(bitmapFirst(cv::Rect(out.x, out.y, bitmapOut->GetWidth(), bitmapOut->GetHeight())));
			}
			delete bitmapOut;
			if (pictureFirst == nullptr)
				pictureFirst = new GLTexture();
			pictureFirst->SetData(bitmapFirst);
		}
	}

	initTexture = false;
}


GLTexture* CPageCurlFilter::GetTexture(const int& numTexture)
{
	if (numTexture == 0)
		return pictureFirst;

	return pictureNext;
}
