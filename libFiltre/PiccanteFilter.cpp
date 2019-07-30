#include <header.h>
#include "PiccanteFilter.h"
 //#include <FileUtility.h>
 #include <climits>
 #include <RegardsBitmap.h>
 #include <RegardsFloatBitmap.h>
#include <ImageLoadingFormat.h>
#include <ConvertUtility.h>
#include <climits>
//This means that we disable Eigen; some functionalities cannot be used.
//For example, estimating the camera response function
#define PIC_DISABLE_EIGEN

//This means that OpenGL acceleration layer is disabled
#define PIC_DISABLE_OPENGL

//This means we do not use QT for I/O
#define PIC_DISABLE_QT

#include <cstring>
#include <random>
#include <iostream>
#include <functional>
#include <piccante.hpp>
//#include <libPicture.h>
#if defined(__x86_64__) || defined(_M_AMD64)
#include <xmmintrin.h>
#endif

float CPiccanteFilter::clamp ( float val, float minval, float maxval )
{
    // Branchless SSE clamp.
    // return minss( maxss(val,minval), maxval );
#if defined(__x86_64__) || defined(_M_AMD64)
    _mm_store_ss( &val, _mm_min_ss( _mm_max_ss(_mm_set_ss(val),_mm_set_ss(minval)), _mm_set_ss(maxval) ) );
	return val;
#else
	return min(max(val, minval), maxval);
#endif
   
}


void CPiccanteFilter::GetPictureDimensions(const wxString & fileName, int & width, int & height)
{
    pic::Image img;
    img.Read(CConvertUtility::ConvertToStdString(fileName));
    width = img.width;
    height = img.height;
}

void CPiccanteFilter::LoadPicture(const wxString & fileName, const bool &isThumbnail, CImageLoadingFormat * bitmap)
{
    pic::Image img;
    img.Read(CConvertUtility::ConvertToStdString(fileName));
    unsigned char * data = nullptr;
    int width = img.width;
    int height = img.height;
    int channels = img.channels;
    float *dataWriter = img.data; 



    if(width > 0 && height > 0 && dataWriter != nullptr)
    {
        if(isThumbnail)
        {

            unsigned char *tmp = ConvertHDR2LDR(dataWriter,  img.dataUC, width * height * channels, pic::LT_NOR);
            if( img.dataUC == NULL) {
                data = tmp;
            }                            
            
            
            CRegardsBitmap * picture = new CRegardsBitmap(width, height);                
            uint8_t * pictureData = picture->GetPtBitmap();
            int shiftG = 1;
            int shiftB = 2;

            if(channels==1) 
            {
                shiftG = 0;
                shiftB = 0;
            }                
                       
            int k = 0;
            for(int j = (height - 1); j > -1; j--) 
            {
                int cj = j * width;

                for(int i = 0; i < width; i++, k+=4) {
                    int c = (cj + i) * channels;
                    //From RGB to BGR
                    pictureData[k] = data[c + shiftB];
                    pictureData[k+1] = data[c + shiftG];
                    pictureData[k+2] = data[c    ];
                    pictureData[k+3] = 0;
                }
            }
            
            if (picture != nullptr)
            {
                bitmap->SetPicture(picture);
                bitmap->SetFilename(fileName);
            }                         
        }
        else
        {
            
            CRegardsFloatBitmap * picture = new CRegardsFloatBitmap(width, height);                
            float * pictureData = picture->GetData();
            int size = width * height * channels;
            switch(channels)
            {
                case 3:
                case 4:
                    {
                        int k = 0;
                        for (int i = height - 1; i >= 0; i--)
                        {
                            for (int j = 0; j < width; j++, k+=4)
                            {
                                int position = i * width * channels + j * channels;
                                pictureData[k] =  clamp(dataWriter[position+ 2], 0.0f, 1.0f);
                                pictureData[k+1] =  clamp(dataWriter[position + 1], 0.0f, 1.0f);
                                pictureData[k+2] =  clamp(dataWriter[position], 0.0f, 1.0f);
                                pictureData[k+3] =  clamp(dataWriter[position + 3], 0.0f, 1.0f);
                            }
                        }                        
                    }
                    break;
                case 1:
                    {
                        int k = 0;
                        int position = 0;
                        for (int i = 0; i < height; i++)
                        {
                            for (int j = 0; j < width; j++, k+=4, position++)
                            {
                                pictureData[k] =  clamp(dataWriter[position], 0.0f, 1.0f);
                                pictureData[k+1] =  clamp(dataWriter[position], 0.0f, 1.0f);
                                pictureData[k+2] =  clamp(dataWriter[position], 0.0f, 1.0f);
                                pictureData[k+3] =  1.0f;
                            }
                        }                              
                    }
                    break;
            }

            
            if (picture != nullptr)
            {
                bitmap->SetPicture(picture);
                bitmap->SetFilename(fileName);
            }                       

        }                
    }          
}

void CPiccanteFilter::BestExposure(CRegardsFloatBitmap * & floatBitmap, const float &tmoValue)
{
    printf("Reading an HDR file...");
    pic::Image img(floatBitmap->GetWidth(),floatBitmap->GetHeight(),4);
    img.data = floatBitmap->GetData();
    //img.data = new float[floatBitmap->GetWidth() * floatBitmap->GetHeight() * 4];
    //memcpy(img.data,floatBitmap->GetData(),floatBitmap->GetWidth() * floatBitmap->GetHeight() * 4 * sizeof(float));

    if(img.isValid()) 
    {
        printf("OK\n");

        //we estimate the best exposure for this HDR image
        float fstop = pic::FindBestExposure(&img);

        printf("The best exposure value is: %f f-stops\n", fstop);

        pic::FilterSimpleTMO fltSimpleTMO(tmoValue, fstop);

        pic::Image *imgToneMapped = fltSimpleTMO.Process(Single(&img), NULL);

        imgToneMapped->ChangeOwnership(true);
        floatBitmap->data = imgToneMapped->data;
        //memcpy(floatBitmap->GetData(), imgToneMapped->data, floatBitmap->GetSize());
        
        delete imgToneMapped;
        
    } 
    else 
    {
        printf("No it is not a valid file!\n");
    }    
}

void CPiccanteFilter::FilterBilateral2DS(CRegardsFloatBitmap * & floatBitmap, const float & sigma_s, const float & sigma_r)
{
    printf("Reading an HDR file...");
    pic::Image img(floatBitmap->GetWidth(),floatBitmap->GetHeight(),4);
    img.data = floatBitmap->GetData();
    //img.data = new float[floatBitmap->GetWidth() * floatBitmap->GetHeight() * 4];
    //memcpy(img.data,floatBitmap->GetData(),floatBitmap->GetWidth() * floatBitmap->GetHeight() * 4 * sizeof(float));
    pic::Image *imgToneMapped = NULL;
    if(img.isValid()) 
    {
        pic::FilterBilateral2DS flt(sigma_s, sigma_r);
        imgToneMapped = flt.Process(Single(&img), NULL);

        imgToneMapped->ChangeOwnership(true);
        floatBitmap->data = imgToneMapped->data;
        //memcpy(floatBitmap->GetData(), imgToneMapped->data, floatBitmap->GetSize());
        
        delete imgToneMapped;
        
    } 
    else 
    {
        printf("No it is not a valid file!\n");
    }  
}

void CPiccanteFilter::FilterKuwahara(CRegardsFloatBitmap * & floatBitmap, const int &kernelSize)
{
    printf("Reading an HDR file...");
    pic::Image img(floatBitmap->GetWidth(),floatBitmap->GetHeight(),4);
    img.data = floatBitmap->GetData();
    //img.data = new float[floatBitmap->GetWidth() * floatBitmap->GetHeight() * 4];
    //memcpy(img.data,floatBitmap->GetData(),floatBitmap->GetWidth() * floatBitmap->GetHeight() * 4 * sizeof(float));
    pic::Image *imgToneMapped = NULL;
    if(img.isValid()) 
    {
        pic::FilterKuwahara fltK(11);
        imgToneMapped = fltK.Process(Single(&img), NULL);

        imgToneMapped->ChangeOwnership(true);
        floatBitmap->data = imgToneMapped->data;
        //memcpy(floatBitmap->GetData(), imgToneMapped->data, floatBitmap->GetSize());
        
        delete imgToneMapped;
        
    } 
    else 
    {
        printf("No it is not a valid file!\n");
    }  
}
