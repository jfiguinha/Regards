/*
 
 ------------------------------------------------------------------------------------
 MSSHandleStyle.m
 ------------------------------------------------------------------------------------
 This class provides properties for the look of a MSSHandle.
 If maintained via interface builder it is possible set the values via IB UI.
 Tipp: For a quick start please read the README document.
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

#import "MSSHandleStyle.h"

@implementation MSSHandleStyle

- (instancetype) init {
    self = [super init];
    if(self != nil) {
        
        CGFloat red = 0.1054f;
        CGFloat green = 0.4531f;
        CGFloat blue = 0.7929f;
        self.size           = NSMakeSize(8.5f, 8.5f);
        self.strokeColor    = [NSColor whiteColor];
        self.fillColor      = [NSColor colorWithCalibratedRed:red green:green blue:blue alpha:1.0f];
    }
    return self;
}


@end
