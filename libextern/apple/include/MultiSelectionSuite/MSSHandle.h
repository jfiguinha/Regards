/*
 
 ------------------------------------------------------------------------------------
 MSSHandle.h
 ------------------------------------------------------------------------------------
 A handle is a little NSBezierPath (by default a rectangle) element.
 The handles are used to resize a MSSSelector starting from the corners.
 By default every selector has four handles (one for each corner).
 In order to know which corner handle is selected the handle has a type.
 Of course the NSBezierPath can be overwritten. This applies for the NSShadow, too.
 Every handle can also have a different look (stroke and fill color).
 The look is defined by a style which is determinated in the canvasController.
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

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import "MSSPreferences.h"

@class MSSSelector;

/** Designable NSBezierPath to resize a MSSSelection from different corners */
@interface MSSHandle : NSObject {
    
}
/** @name Properties */
/** The handle's MSSSelector */
@property (nonatomic, readonly) MSSSelector* selector;
/** The type (SelectorHandleType) of the handle */
@property (nonatomic, readonly) SelectorHandleType type;
/** The handle's frame */
@property (nonatomic, readonly) NSRect frame;
/** The handle's boundaries */
@property (nonatomic, readonly) NSRect bounds;
/** The handle's NSShadow */
@property (nonatomic, readonly) NSShadow *shadow;
/** The actual NSBezierPath of the handle  */
@property (nonatomic, readonly) NSBezierPath *bezierPath;
/** The current NSTrackingArea of the handle  */
@property (nonatomic, readonly) NSTrackingArea *trackingArea;
/** @name Methods */
/** Creates a handle with type for a given MSSSelector 
 @param type The type handle which should be created
 @param selector The selector to create the handle for
 */
+ (instancetype) handleWithType:(SelectorHandleType)type forSelector:(MSSSelector*)selector;
/** Draws the handle on the canvas */
- (void) drawPath;

@end
