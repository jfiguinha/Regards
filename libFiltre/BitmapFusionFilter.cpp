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
#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <opencl/cl_gl.h>
#else
#include <CL/cl.h>
#endif
using namespace Regards::Viewer;

CBitmapFusionFilter::CBitmapFusionFilter()
{
	bitmapTemp = nullptr;
	bitmapOutCopy = nullptr;
}

CBitmapFusionFilter::~CBitmapFusionFilter()
{
	if(bitmapTemp != nullptr)
		delete bitmapTemp;

	if (bitmapOutCopy != nullptr)
		delete bitmapOutCopy;

}

int CBitmapFusionFilter::GetTypeFilter()
{
	return IDM_AFTEREFFECT_FUSION;
}

CRegardsBitmap * CBitmapFusionFilter::GenerateBitmapEffect(CImageLoadingFormat * nextPicture, int etape, IBitmapDisplay * bmpViewer, wxRect &rcOut)
{
	CRegardsBitmap * bitmapOut = nullptr;

	if (etape == 0)
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
			bitmapOut  = new CRegardsBitmap(widthOutput, heightOutput);
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

	if (bitmapOut != nullptr)
		bitmapOut->SetAlphaValue(etape);

	

	rcOut.width = widthOutput;
	rcOut.height = heightOutput;
	rcOut.x = (bmpViewer->GetWidth() - widthOutput) / 2;
	rcOut.y = (bmpViewer->GetHeight() - heightOutput) / 2;
	
	return bitmapOut;

}

#ifdef RENDEROPENGL

void  CBitmapFusionFilter::GenerateTexture(GLTexture * glPicture, CImageLoadingFormat * nextPicture)
{
	CRegardsBitmap * bitmapTemp = nextPicture->GetRegardsBitmap();
	int orientation = nextPicture->GetOrientation();
	bitmapTemp->RotateExif(orientation);
	width = bitmapTemp->GetBitmapWidth();
	height = bitmapTemp->GetBitmapHeight();
	glPicture->Create(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight(), bitmapTemp->GetPtBitmap());
	glBindTexture(GL_TEXTURE_2D, glPicture->GetTextureID());
	delete bitmapTemp;
}

void CBitmapFusionFilter::GenerateBitmapOpenCLEffect(GLTexture * glPicture, CImageLoadingFormat * nextPicture, int etape, IBitmapDisplay * bmpViewer, wxRect &rcOut)
{
	COpenCLEffectVideo * openclEffectVideo = nullptr;
	cl_mem cl_nextPicture = nullptr;

	cl_int err;
	if (bmpViewer->GetOpenCLContext() != nullptr)
	{
		openclEffectVideo = new COpenCLEffectVideo(bmpViewer->GetOpenCLContext());

		try
		{
			cl_int err;
            cl_nextPicture = clCreateFromGLTexture(bmpViewer->GetOpenCLContext()->GetContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, glPicture->GetTextureID(), &err);
			err = clEnqueueAcquireGLObjects(bmpViewer->GetOpenCLContext()->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
			Error::CheckError(err);
			openclEffectVideo->SetAlphaValue(cl_nextPicture, glPicture->GetWidth(), glPicture->GetHeight(), etape);
			err = clEnqueueReleaseGLObjects(bmpViewer->GetOpenCLContext()->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
			Error::CheckError(err);
			err = clFlush(bmpViewer->GetOpenCLContext()->GetCommandQueue());
			Error::CheckError(err);

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

		float newRatio = bmpViewer->CalculPictureRatio(width, height);
		rcOut.width = width * newRatio;
		rcOut.height =height * newRatio;
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

#endif