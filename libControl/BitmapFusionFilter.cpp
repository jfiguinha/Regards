#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BitmapFusionFilter.h"
#include "BitmapFusionEffectParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
#include <InterpolationBicubic.h>
#include <utility.h>
#include <OpenCLEffectVideo.h>
#include <GLTexture.h>
#include <BitmapDisplay.h>
#include <OpenCLContext.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <opencl/cl_gl.h>
#else
#include <CL/cl.h>
#endif
using namespace Regards::Filter;

CBitmapFusionFilter::CBitmapFusionFilter()
{
	bitmapTemp = nullptr;
	bitmapOutCopy = nullptr;
	pictureNext = new GLTexture();
}

CBitmapFusionFilter::~CBitmapFusionFilter()
{
	if(bitmapTemp != nullptr)
		delete bitmapTemp;

	if (bitmapOutCopy != nullptr)
		delete bitmapOutCopy;

	if (pictureNext != nullptr)
		delete(pictureNext);
}

int CBitmapFusionFilter::GetTypeFilter()
{
	return IDM_AFTEREFFECT_FUSION;
}

CRegardsBitmap* CBitmapFusionFilter::GenerateInterpolationBitmapTexture(CImageLoadingFormat* nextPicture, IBitmapDisplay* bmpViewer)
{
	CRegardsBitmap * bitmapTemp = nextPicture->GetRegardsBitmap(true);
	int orientation = nextPicture->GetOrientation();
	bitmapTemp->RotateExif(orientation);
	bitmapTemp->SetAlphaValue(0);

	float newRatio = bmpViewer->CalculPictureRatio(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight());
	int widthOutput = bitmapTemp->GetBitmapWidth() * newRatio;
	int heightOutput = bitmapTemp->GetBitmapHeight() * newRatio;

	CRegardsBitmap* bitmapOut = new CRegardsBitmap(widthOutput, heightOutput);
	CInterpolationBicubic interpolation;
	interpolation.Execute(bitmapTemp, bitmapOut);

	delete bitmapTemp;

	return bitmapOut;
}

CRegardsBitmap * CBitmapFusionFilter::GenerateBitmapTexture(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut)
{
	CRegardsBitmap * bitmapOut = nullptr;

	if (bitmapTemp == nullptr)
	{
		bitmapTemp = nextPicture->GetRegardsBitmap(true);
		int orientation = nextPicture->GetOrientation();
		bitmapTemp->RotateExif(orientation);
	}

	float newRatio = bmpViewer->CalculPictureRatio(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight());
	int widthOutput = bitmapTemp->GetBitmapWidth() * newRatio;
	int heightOutput = bitmapTemp->GetBitmapHeight() * newRatio;

	if (bitmapOutCopy != nullptr)
	{
		if (bitmapOutCopy->GetBitmapWidth() != widthOutput || bitmapOutCopy->GetBitmapHeight() != heightOutput)
		{
			bitmapOut = new CRegardsBitmap(widthOutput, heightOutput);
			CInterpolationBicubic interpolation;
			interpolation.Execute(bitmapTemp, bitmapOut);
		}
		else
		{
			bitmapOut = new CRegardsBitmap();
			*bitmapOut = *bitmapOutCopy;
		}

	}
	else
	{
		bitmapOutCopy = new CRegardsBitmap(widthOutput, heightOutput);
		CInterpolationBicubic interpolation;
		interpolation.Execute(bitmapTemp, bitmapOutCopy);

		bitmapOut = new CRegardsBitmap();
		*bitmapOut = *bitmapOutCopy;
	}

	rcOut.width = widthOutput;
	rcOut.height = heightOutput;
	rcOut.x = (bmpViewer->GetWidth() - widthOutput) / 2;
	rcOut.y = (bmpViewer->GetHeight() - heightOutput) / 2;

	return bitmapOut;
}

void CBitmapFusionFilter::SetTransitionBitmap(const bool& openCL, const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	if (start)
	{
		GenerateEffectTexture(bmpSecond, openCL, bmpViewer);
		bmpViewer->StartTransitionEffect(bmpSecond, false);
	}
	else
	{
		bmpViewer->StopTransitionEffect(bmpSecond);
	}
}

void CBitmapFusionFilter::GenerateEffectTexture(CImageLoadingFormat* nextPicture, const bool& isOpenCL, IBitmapDisplay* bmpViewer)
{
	CImageLoadingFormat tempSecond;
	if (isOpenCL)
	{
		CRegardsBitmap* temp = GenerateInterpolationBitmapTexture(nextPicture, bmpViewer);
		tempSecond.SetPicture(temp);
		GenerateTexture(temp);
	}
	else
		tempSecond.SetPicture(GenerateInterpolationBitmapTexture(nextPicture, bmpViewer));

	if (isOpenCL)
		GenerateBitmapOpenCLEffect(&tempSecond, bmpViewer, out);
	else
		GenerateBitmapEffect(&tempSecond, bmpViewer, out);
}

void CBitmapFusionFilter::AfterRender(CImageLoadingFormat* nextPicture, const bool& isOpenCL, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio)
{
	wxRect local;
	float newRatio = bmpViewer->CalculPictureRatio(nextPicture->GetWidth(), nextPicture->GetHeight());
	local.width = nextPicture->GetWidth() * newRatio;
	local.height = nextPicture->GetHeight() * newRatio;
	local.x = (bmpViewer->GetWidth() - out.width) / 2;
	local.y = (bmpViewer->GetHeight() - out.height) / 2;


	if (local.width != out.width)
	{
		GenerateEffectTexture(nextPicture, isOpenCL, bmpViewer);
	}

	if (renderOpenGL != nullptr)
		renderOpenGL->ShowSecondBitmapWithAlpha(GetTexture(0), etape, out.width * scale_factor, out.height * scale_factor, out.x * scale_factor, out.y * scale_factor);

}


void CBitmapFusionFilter::GenerateBitmapEffect(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut)
{
    if(pictureNext == nullptr)
        pictureNext = new GLTexture();
    
	CRegardsBitmap * bitmapOut = GenerateBitmapTexture(nextPicture, bmpViewer, rcOut);
	if (bitmapOut != nullptr)
	{
		pictureNext->Create(bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight(), bitmapOut->GetPtBitmap());
		delete bitmapOut;
	}
}

#ifdef RENDEROPENGL

void  CBitmapFusionFilter::GenerateTexture(CRegardsBitmap* bitmap)
{
    if(pictureNext == nullptr)
        pictureNext = new GLTexture();
	pictureNext->Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
	glBindTexture(GL_TEXTURE_2D, pictureNext->GetTextureID());
}

void CBitmapFusionFilter::GenerateBitmapOpenCLEffect(CImageLoadingFormat * nextPicture, IBitmapDisplay * bmpViewer, wxRect &rcOut)
{
	COpenCLEffectVideo * openclEffectVideo = nullptr;
	cl_mem cl_nextPicture = nullptr;

	//cl_int err;
	if (bmpViewer->GetOpenCLContext() != nullptr)
	{
		openclEffectVideo = new COpenCLEffectVideo(bmpViewer->GetOpenCLContext());

		try
		{
			cl_int err;
            cl_nextPicture = clCreateFromGLTexture(bmpViewer->GetOpenCLContext()->GetContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, pictureNext->GetTextureID(), &err);
			err = clEnqueueAcquireGLObjects(bmpViewer->GetOpenCLContext()->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
			Error::CheckError(err);
			openclEffectVideo->GetRgbaBitmap(cl_nextPicture);
			err = clEnqueueReleaseGLObjects(bmpViewer->GetOpenCLContext()->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
			Error::CheckError(err);
			err = clFlush(bmpViewer->GetOpenCLContext()->GetCommandQueue());
			Error::CheckError(err);
		}
		catch (...)
		{

		}

		float newRatio = bmpViewer->CalculPictureRatio(nextPicture->GetWidth(), nextPicture->GetHeight());
		rcOut.width = nextPicture->GetWidth() * newRatio;
		rcOut.height = nextPicture->GetHeight() * newRatio;
		rcOut.x = (bmpViewer->GetWidth() - rcOut.width) / 2;
		rcOut.y = (bmpViewer->GetHeight() - rcOut.height) / 2;
	}

	if (openclEffectVideo != nullptr)
		delete openclEffectVideo;
	openclEffectVideo = nullptr;

	if (cl_nextPicture != nullptr)
	{
		clReleaseMemObject(cl_nextPicture);
		cl_nextPicture = nullptr;
	}

}

void CBitmapFusionFilter::DeleteTexture()
{
	if (pictureNext != nullptr)
		delete(pictureNext);
	pictureNext = nullptr;
}

GLTexture * CBitmapFusionFilter::GetTexture(const int &numTexture)
{
	return pictureNext;
}

#endif