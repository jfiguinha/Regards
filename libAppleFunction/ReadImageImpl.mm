//
//  MyObject.m
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//
#ifdef __APPLE__
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include "ReadImageImpl.h"
#include "wx/uri.h"

ReadImageImpl::ReadImageImpl( void )
{   }

ReadImageImpl::~ReadImageImpl( void )
{
}


void ReadImageImpl::GetDimension(const wxString & filePicture, int &width, int &height)
{

    wxURI photo = wxURI(wxString( wxT("file://") ) + filePicture);
    wxCFStringRef uri(photo.BuildURI());
    NSURL *url = [NSURL URLWithString: uri.AsNSString()];
    NSData * imageData = [[NSData alloc] initWithContentsOfURL: url];
    
    CGImageSourceRef source = CGImageSourceCreateWithData((__bridge CFDataRef)imageData, NULL);
    CGImageRef imageRef =  CGImageSourceCreateImageAtIndex(source, 0, NULL);
    
    width = CGImageGetWidth(imageRef);
    height = CGImageGetHeight(imageRef);
   
}

unsigned char * ReadImageImpl::ReadImage(const wxString & filePicture, int &width, int &height)
{
    std::cout << "Picture : " << filePicture << std::endl;
    wxURI photo = wxURI(wxString( wxT("file://") ) + filePicture);
    wxCFStringRef uri(photo.BuildURI());
    NSURL *url = [NSURL URLWithString: uri.AsNSString()];
    NSData * imageData = [[NSData alloc] initWithContentsOfURL: url];
    
    std::cout << "toto 2 : " << std::endl;
    
    CGImageSourceRef source = CGImageSourceCreateWithData((__bridge CFDataRef)imageData, NULL);
    CGImageRef imageRef =  CGImageSourceCreateImageAtIndex(source, 0, NULL);
    
     std::cout << "toto 3 : " << std::endl;
    
    width = CGImageGetWidth(imageRef);
    height = CGImageGetHeight(imageRef);
    
     std::cout << "toto 4 : " << std::endl;
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    unsigned char *rawData = (unsigned char*) calloc(height * width * 4, sizeof(unsigned char));
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
                                                 
     std::cout << "toto 5 : " << std::endl;
     
    CGColorSpaceRelease(colorSpace);

    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    return rawData;
}
#endif