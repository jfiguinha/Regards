#include <header.h>

//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "BitmapFusionFilter.h"

#include <ImageLoadingFormat.h>
#include <GLTexture.h>
#include <BitmapDisplay.h>
#include <effect_id.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <opencl/cl_gl.h>
#else
#endif
using namespace Regards::Filter;


CBitmapFusionFilter::CBitmapFusionFilter(): _bmpSecond(nullptr), width(0), height(0)
{
	pictureNext = new GLTexture();
}

CBitmapFusionFilter::~CBitmapFusionFilter()
{
	CBitmapFusionFilter::DeleteTexture();
}

int CBitmapFusionFilter::GetTypeFilter()
{
	return IDM_AFTEREFFECT_FUSION;
}

CImageLoadingFormat* CBitmapFusionFilter::GenerateInterpolationBitmapTexture(CImageLoadingFormat* nextPicture,
                                                                        IBitmapDisplay* bmpViewer)
{
	CImageLoadingFormat* picture = new CImageLoadingFormat();
	const float newRatio = bmpViewer->CalculPictureRatio(nextPicture->GetWidth(), nextPicture->GetHeight());
	const int widthOutput = nextPicture->GetWidth() * newRatio;
	const int heightOutput = nextPicture->GetHeight() * newRatio;

	cv::Mat _out;
	cv::resize(nextPicture->GetOpenCVPicture(), _out, cv::Size(widthOutput, heightOutput), cv::INTER_CUBIC);
	picture->SetPicture(_out);
	out.width = widthOutput;
	out.height = heightOutput;
	out.x = (bmpViewer->GetWidth() - widthOutput) / 2;
	out.y = (bmpViewer->GetHeight() - heightOutput) / 2;

	
	return picture;
}

void CBitmapFusionFilter::SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer,
                                              CImageLoadingFormat* bmpSecond)
{
	DeleteTexture();

	if (start)
		bmpViewer->StartTransitionEffect(bmpSecond, false);
	else
		bmpViewer->StopTransitionEffect(bmpSecond);
}


void CBitmapFusionFilter::GenerateEffectTexture(CImageLoadingFormat* nextPicture, IBitmapDisplay* bmpViewer)
{
	CImageLoadingFormat * temp = GenerateInterpolationBitmapTexture(nextPicture, bmpViewer);
	if (temp != nullptr)
	{
		GenerateTexture(temp);
		delete temp;
	}
}

void CBitmapFusionFilter::AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL,
                                      IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor,
                                      const bool& isNext, float& ratio)
{
	cv::Mat mat = nextPicture->GetOpenCVPicture();
	const int orientation = nextPicture->GetOrientation();
	CImageLoadingFormat bitmapTemp;
	bitmapTemp.SetPicture(mat);
	bitmapTemp.RotateExif(orientation);


	const float newRatio = bmpViewer->CalculPictureRatio(bitmapTemp.GetWidth(), bitmapTemp.GetHeight());
	const int widthOutput = bitmapTemp.GetWidth() * newRatio;
	const int heightOutput = bitmapTemp.GetHeight() * newRatio;

	if (pictureNext == nullptr || pictureNext->GetWidth() != widthOutput || pictureNext->GetHeight() != heightOutput)
		GenerateEffectTexture(nextPicture, bmpViewer);

	if (renderOpenGL != nullptr)
		renderOpenGL->ShowSecondBitmapWithAlpha(GetTexture(0), etape, out.width * scale_factor,
		                                        out.height * scale_factor, out.x * scale_factor, out.y * scale_factor);
}

void CBitmapFusionFilter::GenerateTexture(CImageLoadingFormat * bitmap)
{
	if (pictureNext == nullptr)
		pictureNext = new GLTexture();
	cv::Mat mat = bitmap->GetOpenCVPicture();
	pictureNext->SetData(mat);
	glBindTexture(GL_TEXTURE_2D, pictureNext->GetTextureID());
}

void CBitmapFusionFilter::DeleteTexture()
{
	if (pictureNext != nullptr)
		delete(pictureNext);
	pictureNext = nullptr;
}

GLTexture* CBitmapFusionFilter::GetTexture(const int& numTexture)
{
	return pictureNext;
}
