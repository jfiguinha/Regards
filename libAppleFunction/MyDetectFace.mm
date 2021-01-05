//
//  MyObject.m
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//
#ifdef __APPLE__
#import <Foundation/Foundation.h>
#import <CoreML/CoreML.h>
#import <Vision/Vision.h>
#import "RegardsInclude.h"
#include "MyDetectFaceImpl.h"
#import <AppKit/AppKit.h>
#include "wx/osx/private.h"
#include "wx/uri.h"
@end

MyDetectFaceImpl::MyDetectFaceImpl( void )
{   }

MyDetectFaceImpl::~MyDetectFaceImpl( void )
{
}

- (NSInteger)detectFace:(CIImage*)image{
    //create req
	int nbFace = 0;
    VNDetectFaceRectanglesRequest *faceDetectionReq = [VNDetectFaceRectanglesRequest new];
    NSDictionary *d = [[NSDictionary alloc] init];
    //req handler
    VNImageRequestHandler *handler = [[VNImageRequestHandler alloc] initWithCIImage:image options:d];
    //send req to handler
    [handler performRequests:@[faceDetectionReq] error:nil];
    
    //is there a face?
    for(VNFaceObservation *observation in faceDetectionReq.results){
        if(observation){
			//[self drawFaceRect:image];
			nbFace++;
        }
    }
	
	return nbFace;
}

- (NSMutableArray *)drawFaceRect:(CIImage*)image{
    //face landmark
	NSMutableArray* array = [NSMutableArray mutableArray];
    VNDetectFaceLandmarksRequest *faceLandmarks = [VNDetectFaceLandmarksRequest new];
    VNSequenceRequestHandler *faceLandmarksDetectionRequest = [VNSequenceRequestHandler new];
    [faceLandmarksDetectionRequest performRequests:@[faceLandmarks] onCIImage:image error:nil];
    for(VNFaceObservation *observation in faceLandmarks.results){
        //draw rect on face
        CGRect boundingBox = observation.boundingBox;
        CGSize size = CGSizeMake(boundingBox.size.width * self.sourceImgView.bounds.size.width, boundingBox.size.height * self.sourceImgView.bounds.size.height);
        CGPoint origin = CGPointMake(boundingBox.origin.x * self.sourceImgView.bounds.size.width, (1-boundingBox.origin.y)*self.sourceImgView.bounds.size.height - size.height);
		[array addObject:[NSValue valueWithCGRect:CGRectMake(origin.x, origin.y, size.width, size.height)]];
    }
	return array;
}


int MyDetectFaceImpl::MyDetectFace(const int &width, const int &height, uint8_t * data)
{
	CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, 
															  data, 
															  width*height*4, 
															  NULL);

	int bitsPerComponent = 8;
	int bitsPerPixel = 32;
	int bytesPerRow = 4*width;
	CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
	CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
	CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
	CGImageRef imageRef = CGImageCreate(width,
										height,
										8,
										32,
										4*width,colorSpaceRef,
										bitmapInfo,
										provider,NULL,NO,renderingIntent);

	int nbFaceDetect = [self detectFace:image];
	
	CGImageRelease(image);

	return nbFaceDetect;
    
}
#endif