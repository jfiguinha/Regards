/*
 
 ------------------------------------------------------------------------------------
 NSImage+SCWExtension.m
 ------------------------------------------------------------------------------------
 Tiny extension of NSImage to increase the contrast and return a compressed copy
 ------------------------------------------------------------------------------------
 
 The MIT License (MIT)
 
 Copyright (c) 2014 Sascha Paulus
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 */


#import "NSImage+SCWExtension.h"
#import <QuartzCore/QuartzCore.h>

@implementation NSImage (SPExtension)

- (NSImage*) contrastEnhanced {
    
    NSImage *finalImage;
    
    CGImageSourceRef inputImageSource;
    inputImageSource = CGImageSourceCreateWithData((CFDataRef)self.TIFFRepresentation, NULL);
    CGImageRef inputImage = CGImageSourceCreateImageAtIndex(inputImageSource, 0, NULL);
    CIImage *image = [CIImage imageWithCGImage:inputImage];
    
    CIFilter *toneCurveFilter = [CIFilter filterWithName:@"CIToneCurve"];
    [toneCurveFilter setDefaults];
    [toneCurveFilter setValue:image forKey:kCIInputImageKey];
    [toneCurveFilter setValue:[CIVector vectorWithX:0.0 Y:0.0]      forKey:@"inputPoint0"];
    [toneCurveFilter setValue:[CIVector vectorWithX:0.15 Y:0.07]    forKey:@"inputPoint1"];
    [toneCurveFilter setValue:[CIVector vectorWithX:0.83 Y:0.91]    forKey:@"inputPoint2"];
    [toneCurveFilter setValue:[CIVector vectorWithX:0.91 Y:0.98]    forKey:@"inputPoint3"];
    [toneCurveFilter setValue:[CIVector vectorWithX:1.0 Y:1.0]      forKey:@"inputPoint4"];
    
    CIImage *outputImage = [toneCurveFilter outputImage];
    NSCIImageRep *rep = [NSCIImageRep imageRepWithCIImage:outputImage];
    finalImage = [[NSImage alloc] initWithSize:rep.size];
    [finalImage addRepresentation:rep];

    CFRelease(inputImageSource);
    CGImageRelease(inputImage);
    
    return finalImage;
    
}

- (NSImage*) compressedCopyWithQuality:(CGFloat)quality withOtherProperties:(NSDictionary*)otherProperties  {
    
    NSBitmapImageRep *imageRep = [[NSBitmapImageRep alloc] initWithData:self.TIFFRepresentation];
    NSMutableData * compressedData = [NSMutableData data];
    CGImageDestinationRef imageDest =  CGImageDestinationCreateWithData((CFMutableDataRef) compressedData, kUTTypeJPEG, 1, NULL);
    NSMutableDictionary *properties = [[NSMutableDictionary alloc] init];
    if(quality < 1) {
        [properties setValue:[NSNumber numberWithFloat:quality] forKey:(NSString*)kCGImageDestinationLossyCompressionQuality];
    }
    if(otherProperties) {
        [properties addEntriesFromDictionary:otherProperties];
    }
    CGImageDestinationAddImage(imageDest, [imageRep CGImage], (CFDictionaryRef) properties);
    CGImageDestinationFinalize(imageDest);
    CFRelease(imageDest);
    
    return [[NSImage alloc] initWithData:compressedData];
    
}


@end
