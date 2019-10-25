/*
 
 ------------------------------------------------------------------------------------
 NSImageView+MSSScaledImage.h
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

#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>

/** Category for NSImageView to deal with the original image and the image's fileURL */
@interface NSImageView (MSSScaledImage)
/** @name Properties */
/** The original image behind the scaled image */
@property (nonatomic, readonly) NSImage *originalImage;
/** The ratio/scale between the controlView's frame and the original image */
@property (nonatomic, readonly) NSSize originalImageRatio;
/** The ratio/scale between the controlView's frame and the scaled image */
@property (nonatomic, readonly) NSSize imageRatio;
/** The frame of the scaled image */
@property (nonatomic, readonly) NSRect scaledImageRect;
/** @abstract The URL of the current image 
 @discussion If the image is changed by Drag & Drop this value will be set automatically */
@property (nonatomic) NSURL *fileURL;
@end
