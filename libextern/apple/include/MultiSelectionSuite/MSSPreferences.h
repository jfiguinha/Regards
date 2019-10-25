/*
 
 ------------------------------------------------------------------------------------
 MSSPreferences.h
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

#import <Foundation/Foundation.h>

FOUNDATION_EXPORT NSString * const MSSExtractorJobUnknown;
FOUNDATION_EXPORT NSString * const MSSExtractorJobCropping;
FOUNDATION_EXPORT NSString * const MSSExtractorJobOverviewScan;
FOUNDATION_EXPORT NSString * const MSSExtractorJobImageScan;
FOUNDATION_EXPORT NSString * const MSSExtractorJobZoom;

/** Determines the type of a selector handle */
typedef NS_ENUM(NSInteger, SelectorHandleType) {
    /** No handle is selected or handle has no type */
    SelectorHandleTypeNoHandle,
    /** Selected handle is on the upper left */
    SelectorHandleTypeUpperLeft,
    /** Selected handle is on the upper right */
    SelectorHandleTypeUpperRight,
    /** Selected handle is on the lower left */
    SelectorHandleTypeLowerLeft,
    /** Selected handle is on the upper right */
    SelectorHandleTypeLowerRight,
};

/** Defines to current action of a selector */
typedef NS_ENUM(NSInteger, SelectorAction) {
    /** Selector has no action yet */
    SelectorActionNone,
    /** Selector is about to resize */
    SelectorActionResize,
    /** Selector is about to move */
    SelectorActionMove,
    /** Selector is about to initialize */
    SelectorActionInit,
};

/** Determines the current selector position in the canvas boundaries */
typedef NS_ENUM(NSInteger, SelectorBoundariesPosition) {
    /** Selector is in boundaries */
    SelectorBoundariesPositionInBounderies,
    /** Selector is out of boundaries (left side) */
    SelectorBoundariesPositionLeftOuterBounderies,
    /** Selector is out of boundaries (right side) */
    SelectorBoundariesPositionRightOuterBounderies,
    /** Selector is out of boundaries (top side) */
    SelectorBoundariesPositionUpperOuterBounderies,
    /** Selector is out of boundaries (bottom side) */
    SelectorBoundariesPositionLowerOuterBounderies,
};

/** Defines the struture of a boundary */
typedef struct {
    /** Position (x,y) of the boundary */
    CGFloat position;
    /** Size (width,height) of the boundary */
    CGFloat size;
} Boundary;

/** Defines the boundaries of an object */
typedef struct {
    /** Boundary on the top side */
    Boundary top;
    /** Boundary on the bottom side */
    Boundary bottom;
    /** Boundary on the left side */
    Boundary left;
    /** Boundary on the right side */
    Boundary right;
} Boundaries;


/** Defines how much images should be compress */
typedef NS_ENUM(NSInteger, ExtractorImageCompressionLevel) {
    /** No compression (Image quality 100%) */
    ExtractorImageCompressionLevelNone,
    /** Little compression (Image quality 80%)  */
    ExtractorImageCompressionLevelLow,
    /** Medium compression (Image quality 60%)  */
    ExtractorImageCompressionLevelMiddle,
    /** High compression (Image quality 30%)  */
    ExtractorImageCompressionLevelHigh,
    /** Highest compression (Image quality 10%)  */
    ExtractorImageCompressionLevelHighest,
};
