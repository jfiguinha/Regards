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
@interface faceDetectServiceDelegate : NSObject <NSSharingServiceDelegate>
@end

@interface faceDetectServiceDelegate ()
@end

MyDetectFaceImpl::MyDetectFaceImpl( void )
{   }

MyDetectFaceImpl::~MyDetectFaceImpl( void )
{
}

@implementation faceDetectServiceDelegate

- (id)init {
    
    self = [super init];
    
    return self;
}

- (NSInteger)detectFace:(CGImageRef)image{
    //create req
	int nbFace = 0;
    VNDetectFaceRectanglesRequest *faceDetectionReq = [VNDetectFaceRectanglesRequest new];
    NSDictionary *d = [[NSDictionary alloc] init];
    //req handler
    VNImageRequestHandler *handler = [[VNImageRequestHandler alloc] initWithCGImage:image options:d];
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

- (VNDetectFaceLandmarksRequest *)drawFaceRect:(CGImageRef)image{
    //face landmark
    NSInteger i = 0;
    VNDetectFaceLandmarksRequest *faceLandmarks = [VNDetectFaceLandmarksRequest new];
    VNSequenceRequestHandler *faceLandmarksDetectionRequest = [VNSequenceRequestHandler new];
    [faceLandmarksDetectionRequest performRequests:@[faceLandmarks] onCGImage:image error:nil];
    /*
    for(VNFaceObservation *observation in faceLandmarks.results){
        //draw rect on face
        if(i == numFace)
        {
            boundingBox = observation.boundingBox;
            break;
        }
            
     }
      * */
	return faceLandmarks;
}

/*
- (NSMutableArray *)drawFaceRect:(CGImageRef)image, (NSInteger)numFace{
    //face landmark
	NSMutableArray* array = [[NSMutableArray alloc] init];;
    VNDetectFaceLandmarksRequest *faceLandmarks = [VNDetectFaceLandmarksRequest new];
    VNSequenceRequestHandler *faceLandmarksDetectionRequest = [VNSequenceRequestHandler new];
    [faceLandmarksDetectionRequest performRequests:@[faceLandmarks] onCGImage:image error:nil];
    for(VNFaceObservation *observation in faceLandmarks.results){
        //draw rect on face
        CGRect boundingBox = observation.boundingBox;
        [array addObject:[NSValue valueWithRect:boundingBox]];
        //[array addObject:[NSValue NSStringFromCGRect:boundingBox]];
        
        //CGSize size = CGSizeMake(boundingBox.size.width * self.sourceImgView.bounds.size.width, boundingBox.size.height * self.sourceImgView.bounds.size.height);
        //CGPoint origin = CGPointMake(boundingBox.origin.x * self.sourceImgView.bounds.size.width, (1-boundingBox.origin.y)*self.sourceImgView.bounds.size.height - size.height);
		//[array addObject:[NSValue valueWithCGRect:CGRectMake(origin.x, origin.y, size.width, size.height)]];
    }
	return array;
}
*/
@end

int MyDetectFaceImpl::MyDetectFace(const int &width, const int &height, uint8_t * data)
{
    faceDetectServiceDelegate * shareDelegate = [[faceDetectServiceDelegate alloc] init];
    
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

	int nbFaceDetect = [shareDelegate detectFace:imageRef];
   // NSMutableArray * listRect = [shareDelegate drawFaceRect:imageRef];
	
	CGImageRelease(imageRef);

	return nbFaceDetect;
    
}

 int MyDetectFaceImpl::DetectRectFace(const int &width, const int &height, uint8_t * data, vector<FaceRect> & listRect)
 {
        faceDetectServiceDelegate * shareDelegate = [[faceDetectServiceDelegate alloc] init];
    
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

	int nbFaceDetect = [shareDelegate detectFace:imageRef];
    VNDetectFaceLandmarksRequest * faceLandmarks = [shareDelegate drawFaceRect:imageRef];
	for(VNFaceObservation *observation in faceLandmarks.results)
    {
        /*
        CGRect boundingBox = observation.boundingBox;
        FaceRect face;
        face.x = boundingBox.origin.x;
        face.y = boundingBox.origin.y;
        face.width = boundingBox.size.width;
        face.height = boundingBox.size.height;
        */
        CGRect boundingBox = observation.boundingBox;
        float x= boundingBox.origin.x * width;
        float w= boundingBox.size.width * width;
        float h= boundingBox.size.height * height;
        float y= height*(1-boundingBox.origin.y)-h;
        
        FaceRect face;
        face.x = x;
        face.y = y;
        face.width = w;
        face.height = h;
        
        listRect.push_back(face);
    }
        
	CGImageRelease(imageRef);

	return nbFaceDetect;
 }

#endif
