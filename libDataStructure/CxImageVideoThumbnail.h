//
//  CxImageVideoThumbnail.h
//  Regards.libDataStructure
//
//  Created by figuinha jacques on 11/08/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#pragma once

#ifdef __APPLE__

class CRegardsBitmap;

class CCxImageVideoThumbnail
{
public:
    
    CCxImageVideoThumbnail(){};
    ~CCxImageVideoThumbnail(){};
    
    CRegardsBitmap * image = nullptr;
    int rotation = 0;
    int percent = 0;
    int timePosition = 0;
};

#else

class CxImage;

class CCxImageVideoThumbnail
{
public:
    
    CCxImageVideoThumbnail(){};
    ~CCxImageVideoThumbnail(){};
    
    CxImage * image = nullptr;
    int rotation = 0;
    int percent = 0;
    int timePosition = 0;
};
#endif