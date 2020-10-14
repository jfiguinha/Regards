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

void CPageCurlFilter::GenerateTexture(CImageLoadingFormat * nextPicture, CImageLoadingFormat * source, IBitmapDisplay * bmpViewer)
{
    if(pictureNext == nullptr)
        pictureNext = new GLTexture();
    
    if(pictureFirst == nullptr)
        pictureFirst = new GLTexture();
    
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
			pictureFirst->Create(bitmapFirst->GetBitmapWidth(), bitmapFirst->GetBitmapHeight(), bitmapFirst->GetPtBitmap());
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