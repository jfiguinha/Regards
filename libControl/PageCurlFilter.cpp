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
#include <RegardsBitmap.h>
#include <BitmapDisplay.h>
#include <GLTexture.h>
using namespace Regards::Filter;
using namespace Regards::OpenGL;

CPageCurlFilter::CPageCurlFilter()
{
	initTexture = true;
	pictureFirst = new GLTexture();
}

CPageCurlFilter::~CPageCurlFilter()
{
	if (bitmapNext != nullptr)
	{
		delete bitmapNext;
	}
	if (bitmapFirst != nullptr)
	{
		delete bitmapFirst;
	}

	if (pictureFirst != nullptr)
		delete(pictureFirst);
}

void CPageCurlFilter::RenderTexture(CRenderBitmapOpenGL* renderOpenGL, const float& time, const float& invert, const int& width, const int& height, const int& left, const int& top)
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

void CPageCurlFilter::SetTransitionBitmap(const bool& openCL, const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	bmpViewer->StartTransitionEffect(bmpSecond, false);
}

bool CPageCurlFilter::RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source, IBitmapDisplay* bmpViewer, CRenderBitmapOpenGL* renderOpenGL, const float& scale_factor, const int& etape)
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

void CPageCurlFilter::GenerateTexture(CImageLoadingFormat * nextPicture, CImageLoadingFormat * source, IBitmapDisplay * bmpViewer)
{
	wxRect out;
	{
		bool init = false;
		if (bitmapNext == nullptr)
		{
			bitmapNext = new CRegardsBitmap(bmpViewer->GetWidth(), bmpViewer->GetHeight());
			init = true;
		}
		else if (initTexture || (bitmapNext->GetBitmapWidth() != bmpViewer->GetWidth() && bitmapNext->GetBitmapHeight() != bmpViewer->GetHeight()))
		{
			delete bitmapNext;
			bitmapNext = new CRegardsBitmap(bmpViewer->GetWidth(), bmpViewer->GetHeight());
			init = true;
		}

		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			bitmapNext->SetBackgroundColor(CRgbaquad(colorBack.GetRed(), colorBack.GetGreen(), colorBack.GetBlue(), 255));
			CRegardsBitmap * bitmapOut = GenerateBitmapTexture(nextPicture, bmpViewer, out);
			if (bitmapOut != nullptr)
				bitmapNext->InsertBitmap(bitmapOut, out.x, out.y);
			delete bitmapOut;
            if(pictureNext == nullptr)
               pictureNext = new GLTexture();
			pictureNext->Create(bitmapNext->GetBitmapWidth(), bitmapNext->GetBitmapHeight(), bitmapNext->GetPtBitmap());
		}
        
        if(pictureNext == nullptr)
        {
            pictureNext = new GLTexture();
            pictureNext->Create(bitmapNext->GetBitmapWidth(), bitmapNext->GetBitmapHeight(), bitmapNext->GetPtBitmap());
        }
	}

	{
		if (bitmapTemp != nullptr)
			delete bitmapTemp;
		bitmapTemp = nullptr;

		if (bitmapOutCopy != nullptr)
			delete bitmapOutCopy;
		bitmapOutCopy = nullptr;

		bool init = false;
		if (bitmapFirst == nullptr)
		{
			bitmapFirst = new CRegardsBitmap(bmpViewer->GetWidth(), bmpViewer->GetHeight());
			init = true;
		}
		else if (initTexture || (bitmapFirst->GetBitmapWidth() != bmpViewer->GetWidth() && bitmapFirst->GetBitmapHeight() != bmpViewer->GetHeight()))
		{
			delete bitmapFirst;
			bitmapFirst = new CRegardsBitmap(bmpViewer->GetWidth(), bmpViewer->GetHeight());
			init = true;
		}

		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			bitmapFirst->SetBackgroundColor(CRgbaquad(colorBack.GetRed(), colorBack.GetGreen(), colorBack.GetBlue(), 255));
			CRegardsBitmap * bitmapOut = GenerateBitmapTexture(source, bmpViewer, out);
			if (bitmapOut != nullptr)
				bitmapFirst->InsertBitmap(bitmapOut, out.x, out.y);
			delete bitmapOut;
            if(pictureFirst == nullptr)
                pictureFirst = new GLTexture();
			pictureFirst->Create(bitmapFirst->GetBitmapWidth(), bitmapFirst->GetBitmapHeight(), bitmapFirst->GetPtBitmap());
		}
        
        if(pictureFirst == nullptr)
        {
            pictureFirst = new GLTexture();
            pictureFirst->Create(bitmapNext->GetBitmapWidth(), bitmapNext->GetBitmapHeight(), bitmapNext->GetPtBitmap());
        }
	}

	initTexture = false;
}


GLTexture * CPageCurlFilter::GetTexture(const int &numTexture)
{
	if (numTexture == 0)
		return pictureFirst;

	return pictureNext;
}