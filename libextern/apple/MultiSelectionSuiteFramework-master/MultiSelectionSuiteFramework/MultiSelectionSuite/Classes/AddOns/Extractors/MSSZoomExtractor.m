/*
 
 -----------------------------------------------------------------------------------------
 MSSZoomExtractor.m
 -----------------------------------------------------------------------------------------
 The extractor is an Add-On for SelectorSuite to extract content from the contentView
 of the canvasController. This derived version extracts one part from the contentView's
 original image based on the selection and "zooms" it by using the original size.
 Tipp: For a quick start please read the README document.
 -----------------------------------------------------------------------------------------
 
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

#import "MSSZoomExtractor.h"
#import "MSSError.h"
#import "MSSPreferences.h"
#import "NSImageView+MSSScaledImage.h"
#import "MSSSelector.h"

@implementation MSSZoomExtractor

- (void) prepareJob:(NSString *)identifier {
    [super prepareJob:identifier];
}

- (void)selectorHasChanged:(MSSSelector *)selector {
    
    if(selector!=nil) {
        
        NSRect frame = [self.canvasController defaultRectForFrame:selector.frame];
        NSSize ratio = [(NSImageView*)self.canvasController.contentView originalImageRatio];
        CGFloat x = frame.origin.x / ratio.width;
        CGFloat y = frame.origin.y / ratio.height;
        CGFloat w = frame.size.width / ratio.width;
        CGFloat h = frame.size.height / ratio.height;
        NSRect sourceFrame = NSMakeRect(x, y, w, h);
        NSRect targetFrame = NSMakeRect(0, 0, w*1.0f, h*1.0f);
        NSImage *targetImage = [[NSImage alloc] initWithSize:targetFrame.size];
        if(targetImage.size.width > 0 && targetFrame.size.height > 0) {
            [targetImage lockFocus];
            [[(NSImageView*)self.canvasController.contentView originalImage] drawInRect:targetFrame fromRect:sourceFrame operation:NSCompositeCopy fraction:1.0f];
            [targetImage unlockFocus];
            [self didExtractElement:targetImage withError:nil];
            
        }
    }
}


@end
