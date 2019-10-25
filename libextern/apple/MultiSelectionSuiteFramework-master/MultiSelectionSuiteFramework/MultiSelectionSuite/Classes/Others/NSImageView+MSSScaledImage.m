/*
 
 ------------------------------------------------------------------------------------
 NSimageView+MSSScaledImage.m
 ------------------------------------------------------------------------------------
 Category for NSImageView to deal with the original image
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

#import "NSImageView+MSSScaledImage.h"
#import <objc/runtime.h>

@implementation NSImageView (MSSScaledImage)

@dynamic fileURL;

- (void) setFileURL:(NSURL*)url {
    objc_setAssociatedObject(self, @selector(fileURL), url, OBJC_ASSOCIATION_RETAIN);
}

- (NSURL*) fileURL {
    return objc_getAssociatedObject(self, @selector(fileURL));
}

- (NSImage*) originalImage {

    CGFloat scale = 1.0f;
    // If fileURL exists and the file is a PDF scale it up by 2.5
    NSString *fileType;
    if(self.fileURL) {
        CGImageSourceRef isr = CGImageSourceCreateWithURL((__bridge CFURLRef)(self.fileURL), NULL);
        fileType = (__bridge NSString*)CGImageSourceGetType(isr);
        CFRelease(isr);
    }
    if(fileType && [fileType isEqualToString:@"com.adobe.pdf"]) {
        scale = 2.5;
    }
    
    NSImageRep *imageRep = [self.image.representations objectAtIndex:0];
    CGFloat w = fmaxf(imageRep.size.width, imageRep.pixelsWide);
    CGFloat h = fmaxf(imageRep.size.height, imageRep.pixelsHigh);
    NSImage *image = [[NSImage alloc] initWithSize:NSMakeSize(w*scale, h*scale)];
    [image addRepresentation:imageRep];
    return image;
}

- (NSSize) originalImageRatio {

    
    NSImage *actualImage = [self originalImage];
    
    NSSize imageSize = NSMakeSize(actualImage.size.width, actualImage.size.height);
    NSRect frameRect = [self.cell drawingRectForBounds:self.bounds];
    NSSize frameSize = frameRect.size;
    
    NSSize ratio = NSZeroSize;
    CGFloat ratioX = frameSize.width / imageSize.width;
    CGFloat ratioY = frameSize.height / imageSize.height;
    CGFloat scale = 1.0f;
    
    switch (self.imageScaling) {
        case NSImageScaleProportionallyDown:
            scale = fminf(ratioX, ratioY);
            scale = fminf(scale, 1.0f);
            ratio = NSMakeSize(scale, scale);
            break;
        case NSImageScaleProportionallyUpOrDown:
            scale = fminf(ratioX, ratioY);
            ratio = NSMakeSize(scale, scale);
            break;
        case NSImageScaleAxesIndependently:
            ratio = NSMakeSize(ratioX, ratioY);
            break;
        default:
            ratio = NSMakeSize(scale, scale);
            break;
    }
    
    return ratio;

    
}

- (NSSize) imageRatio {

    NSSize imageSize = self.image.size;
    
    NSRect frameRect = [self.cell drawingRectForBounds:self.bounds];
    NSSize frameSize = frameRect.size;
    
    NSSize ratio = NSZeroSize;
    CGFloat ratioX = frameSize.width / imageSize.width;
    CGFloat ratioY = frameSize.height / imageSize.height;
    CGFloat scale = 1.0f;
    
    switch (self.imageScaling) {
        case NSImageScaleProportionallyDown:
            scale = fminf(ratioX, ratioY);
            scale = fminf(scale, 1.0f);
            ratio = NSMakeSize(scale, scale);
            break;
        case NSImageScaleProportionallyUpOrDown:
            scale = fminf(ratioX, ratioY);
            ratio = NSMakeSize(scale, scale);
            break;
        case NSImageScaleAxesIndependently:
            ratio = NSMakeSize(ratioX, ratioY);
            break;
        default:
            ratio = NSMakeSize(scale, scale);
            break;
    }
    
    return ratio;
    
}

- (NSRect) scaledImageRect {
    
    NSRect scaledRect = NSZeroRect;
    
    NSSize imageSize = self.image.size;
    if (imageSize.width < 1.0 || imageSize.height < 1.0) {
        return scaledRect;
    }
    NSRect frameRect = [self.cell drawingRectForBounds:self.bounds];
    NSSize frameSize = frameRect.size;
    NSSize ratio = self.imageRatio;
    
    
    CGFloat scaledWidth = imageSize.width * ratio.width;
    CGFloat scaledHeight = imageSize.height * ratio.height;
    NSSize scaledSize = NSMakeSize(scaledWidth, scaledHeight);
    
    CGFloat Xmin = 0.0f;
    CGFloat Xmax = frameSize.width - scaledSize.width;
    CGFloat verticalCenter = (frameSize.width - scaledSize.width) / 2;
    CGFloat Ymin = 0.0f;
    CGFloat Ymax = frameSize.height - scaledSize.height;
    CGFloat horizontalCenter = (frameSize.height - scaledSize.height) / 2;
    
    switch (self.imageAlignment) {
        case NSImageAlignCenter:
            scaledRect = NSMakeRect(verticalCenter, horizontalCenter, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignLeft:
            scaledRect = NSMakeRect(Xmin, horizontalCenter, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignRight:
            scaledRect = NSMakeRect(Xmax, horizontalCenter, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignTop:
            scaledRect = NSMakeRect(verticalCenter, Ymax, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignTopLeft:
            scaledRect = NSMakeRect(Xmin, Ymax, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignTopRight:
            scaledRect = NSMakeRect(Xmax, Ymax, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignBottom:
            scaledRect = NSMakeRect(verticalCenter, Ymin, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignBottomLeft:
            scaledRect = NSMakeRect(Xmin, Ymin, scaledSize.width, scaledSize.height);
            break;
        case NSImageAlignBottomRight:
            scaledRect = NSMakeRect(Xmax, Ymin, scaledSize.width, scaledSize.height);
            break;
    }
    
    scaledRect = NSOffsetRect(scaledRect, frameRect.origin.x, frameRect.origin.y);
    
    return scaledRect;
}

/** Overwrites the default draggingEntered in order to intercept the draged fileUrl */
- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender {
    
    NSPasteboard *pboard = [sender draggingPasteboard];
    NSDragOperation sourceDragMask = [sender draggingSourceOperationMask];
    
    if([[pboard types] containsObject:NSURLPboardType]) {
        if(sourceDragMask & NSDragOperationGeneric) {
            self.fileURL = [NSURL URLFromPasteboard:pboard];
            return NSDragOperationCopy;
        }
        
    }
    return NSDragOperationNone;
}


@end
