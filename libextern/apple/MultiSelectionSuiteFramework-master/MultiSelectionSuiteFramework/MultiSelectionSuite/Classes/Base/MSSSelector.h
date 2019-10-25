/*
 
 ------------------------------------------------------------------------------------
 MSSSelector.h
 ------------------------------------------------------------------------------------
 This class is the actual selection path on a MSSCanvas. A canvas can have
 multiple selectors. Basically a selector is a NSBezierPath which interacts with 
 the mouse activities. Movement and Resizing are controlled by the canvasController.
 Their two types of selectors: picker and selector. A normal selector stays drawn
 on the canvas and a picker selector is just for the purpose of selecting/collecting 
 available selectors and mark them as selected. The NSBezierPaths for the picker and 
 for the selector can be overwritten. Each selector has four handles for resizing.
 Every selector can also have a different look (stroke and fill colors).
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
#import "MSSCanvas.h"
#import "MSSHandle.h"

/** Represents a designable selection rectangle with handles to resize */
@interface MSSSelector : NSObject {
    
}
/** @name Properties */
/** The MSSCanvas on which the selector is drawn */
@property (nonatomic, readonly) MSSCanvas *canvas;
/** The selector's frame */
@property (nonatomic, readonly) NSRect frame;
/** Determines if the selector is selected */
@property (nonatomic, readonly) BOOL isSelected;
/** Determines if the selector is just a picker
 
 A non-picker selector stays drawn on the canvas and a picker selector is just for the purpose of selecting/collecting available selectors and mark them as selected.
 */
@property (nonatomic, readonly) BOOL isPicker;
/** Determines the current SelectorAction */
@property (nonatomic, readonly) SelectorAction action;
/** Holds all handles of the selector */
@property (nonatomic, readonly) NSMutableArray *handles;
/** Determines the current handle */
@property (nonatomic, readonly) MSSHandle *currentHandle;
/** The actual NSBezierPath of the selector  */
@property (nonatomic, readonly) NSBezierPath *selectorBezierPath;
/** @abstract The actual NSBezierPath of the selector if the selector is a picker
 @discussion For more details about the distinction between picker and selector checkout the isPicker property.
 */
@property (nonatomic, readonly) NSBezierPath *pickerBezierPath;
/** The current NSTrackingArea of the handle  */
@property (nonatomic, readonly) NSTrackingArea *trackingArea;
/** @name Methods */
/** 
 @abstract Creates a selector or picker for a given MSSCanvas
 @param rect The frame for the selector
 @param canvas The canvas on which the selector should be drawn
 @param picker Determines if the new selector should be a picker
 @discussion For more details about the distinction between picker and selector checkout the isPicker property.
 */
+ (instancetype) selectorWithFrame:(NSRect)rect forCanvas:canvas asPicker:(BOOL)picker;
/** Determines if a selector's handle has been touched and prepare the corresponding action for the selector 
 @param rect The frame, which could have touched the handle.
 */
- (BOOL) touchedHandle:(NSRect)rect;
/** Determines if the selector's frame intersects with a given NSRect 
 @param rect The frame, which could have intersected the selector.
 */
- (BOOL) intersectsFrame:(NSRect)rect;
/** Draws the selector's NSBezierPath */
- (void) drawPath;
/** Marks the selector as selected */
- (void) setSelected;
/** Marks the selector as unselected */
- (void) setUnselected;
/** Moves the selector to a given point 
 @param point The point on the canvas to move the selector to.
 */
- (void) moveSelectorToPoint:(CGPoint)point;
/** Resizes the selector from the current origin to a given point 
 @param point The point on the canvas to resize the selector to.
 */
- (void) resizeToPoint:(CGPoint)point;
/** Determines if the selector can be dropped on the canvas 
 
 Depending on whether intersections with other selectors is allowed or not or the minimum rectangle size for new selectors does not apply, the dropping of the current selector can be denied. In this case the current selector will be discarded, regardless if the selector is new or already existed and only has been resized or moved.
 
 */
- (BOOL) canDropOnCanvas;
/** Determines if the selector's frame crosses the canvas boundaries */
- (BOOL) touchedBoundaries;
/** Prepares the selector for a certain action 
 @param action The SelectorAction to prepare the selector for.
 */
- (void) prepareAction:(SelectorAction)action;
/** Removes all tracking areas for this selector */
- (void) removeTrackingAreas;

@end
