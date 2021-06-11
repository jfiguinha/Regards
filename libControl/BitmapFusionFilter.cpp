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
	pictureNext = new GLTexture();
}

CBitmapFusionFilter::~CBitmapFusionFilter()
{
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
	//bitmapTemp->SetAlphaValue(0);

	float newRatio = bmpViewer->CalculPictureRatio(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight());
	int widthOutput = bitmapTemp->GetBitmapWidth() * newRatio;
	int heightOutput = bitmapTemp->GetBitmapHeight() * newRatio;

	CRegardsBitmap* bitmapOut = new CRegardsBitmap(widthOutput, heightOutput);
	CInterpolationBicubic interpolation;
	interpolation.Execute(bitmapTemp, bitmapOut);

	delete bitmapTemp;

	out.width = widthOutput;
	out.height = heightOutput;
	out.x = (bmpViewer->GetWidth() - widthOutput) / 2;
	out.y = (bmpViewer->GetHeight() - heightOutput) / 2;

	return bitmapOut;
}

void CBitmapFusionFilter::SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	if (start)
	{
		GenerateEffectTexture(bmpSecond, bmpViewer);
		bmpViewer->StartTransitionEffect(bmpSecond, false);
	}
	else
	{
		bmpViewer->StopTransitionEffect(bmpSecond);
	}
}

void CBitmapFusionFilter::GenerateEffectTexture(CImageLoadingFormat* nextPicture, IBitmapDisplay* bmpViewer)
{
	CRegardsBitmap* temp = GenerateInterpolationBitmapTexture(nextPicture, bmpViewer);
	if (temp != nullptr)
	{
		GenerateTexture(temp);
		delete temp;
	}

}

void CBitmapFusionFilter::AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio)
{
	CRegardsBitmap * bitmapTemp = nextPicture->GetRegardsBitmap(true);
	int orientation = nextPicture->GetOrientation();
	bitmapTemp->RotateExif(orientation);
	//bitmapTemp->SetAlphaValue(0);

	float newRatio = bmpViewer->CalculPictureRatio(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight());
	int widthOutput = bitmapTemp->GetBitmapWidth() * newRatio;
	int heightOutput = bitmapTemp->GetBitmapHeight() * newRatio;
    
    delete bitmapTemp;
    
    if(pictureNext == nullptr || pictureNext->GetWidth() != widthOutput || pictureNext->GetHeight() != heightOutput)
        GenerateEffectTexture(nextPicture, bmpViewer);

	if (renderOpenGL != nullptr)
		renderOpenGL->ShowSecondBitmapWithAlpha(GetTexture(0), etape, out.width * scale_factor, out.height * scale_factor, out.x * scale_factor, out.y * scale_factor);

}

#ifdef RENDEROPENGL

void  CBitmapFusionFilter::GenerateTexture(CRegardsBitmap* bitmap)
{
    if(pictureNext == nullptr)
        pictureNext = new GLTexture();
	pictureNext->Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
	glBindTexture(GL_TEXTURE_2D, pictureNext->GetTextureID());
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