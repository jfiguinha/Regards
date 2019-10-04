#include <header.h>
//
//  FilterWindowParam.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterWindowParam.h"
#include <BitmapWndViewer.h>
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>

CFilterWindowParam::CFilterWindowParam()
{
    
}

CFilterWindowParam::~CFilterWindowParam()
{
    
}


CImageLoadingFormat * CFilterWindowParam::ApplyEffect(CEffectParameter * effectParameter, CBitmapWndViewer * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = nullptr;
	if (source != nullptr)
	{
		CImageLoadingFormat image(false);
		image.SetPicture(source);
		CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), bitmapViewer->GetOpenCLContext(), &image);
		filtre->RenderEffect(GetTypeFilter(), effectParameter);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(filtre->GetBitmap(true));
		imageLoad->SetOrientation(bitmapViewer->GetOrientation());
		delete filtre;
	}

	return imageLoad;
}