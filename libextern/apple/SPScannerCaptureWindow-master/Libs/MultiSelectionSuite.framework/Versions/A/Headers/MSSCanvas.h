/*
 
 ------------------------------------------------------------------------------------
 MSSCanvas.h
 ------------------------------------------------------------------------------------
 The canvas is a normal NSView which can add as subview on any other view. 
 This is the place where the MSSSelector(s) are drawn. A canvas can have multiple 
 selectors. The canvas is controlled by the canvasController who is responsible for 
 almost every interaction between the canvas, the selectors and the mouse.
 The frame of the canvas will be arranged automatically.
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
#import <Cocoa/Cocoa.h>
#import "MSSHandle.h"
#import "MSSSelectorStyle.h"
#import "MSSHandleStyle.h"

@class MSSCanvas;
@class MSSSelector;
@class MSSExtractor;

/** Defines requirements for an eventSubscriber object to receive events */
@protocol MSSCanvasEventSubscriber
/** @name Methods */
/** Will be called whenever the canvas has changed */
- (void) canvasHasChanged;
/** Will be called wheneever a selector has changed 
 @param selector The selector which has been changed
 */
- (void) selectorHasChanged:(MSSSelector*)selector;
@end

/** Defines requirements for a controller object to manage a MSSCanvas */
@protocol MSSCanvasDelegate

/** @name Properties */
/** Holds all selectors on the canvas */
@property (nonatomic, readonly) NSArray *selectors;
/** Determines the current selector */
@property (atomic) IBOutlet MSSCanvas* view;
/** Determines the chosen SelectorStyle */
@property (nonatomic) IBOutlet MSSSelectorStyle *selectorStyle;
/** Determines the chosen HandleStyle */
@property (nonatomic) IBOutlet MSSHandleStyle *handleStyle;
/** Determines if the canvas has selectors or not */
@property (nonatomic, readonly) BOOL hasSelectors;
/** The view with will contain the canvas as subview */
@property (nonatomic) IBOutlet NSView *contentView;
/** Determines the current selector */
@property (nonatomic, readonly) MSSSelector *currentSelector;
/** Determines if the canvas can have only one selector */
@property (nonatomic) IBInspectable BOOL singleSelectionMode;
/** The object which extracts the content from the contentView */
@property (nonatomic) IBOutlet MSSExtractor *contentExtractor;

/** @name Methods */
/** Returns a frame with normal (lower-left) origin based on the current frame. 
 @param frame The frame which should be normalized
 */
- (NSRect) defaultRectForFrame:(NSRect)frame;
/** Discards all selectors from the canvas which are marked as selected */
- (void)removeAllSelectorsFromCanvas;
/** Prepares to controller after a canvas has been set */
- (void) prepareController;

/** Creates a canvasController for a canvas and a contentView
 @param theCanvas The canvas to create a controller for
 @param theContentView The view (e.g. NSImageView) which provides the content
 */
+ (instancetype)canvasControllerForCanvas:(MSSCanvas*)theCanvas withContentView:(NSView*)theContentView;

/** @name Eventhandling */
- (void)mouseDown:(NSEvent *)theEvent;
- (void)mouseDragged:(NSEvent *)theEvent;
- (void)mouseUp:(NSEvent *)theEvent;
- (void)keyDown:(NSEvent *)theEvent;

@end

IB_DESIGNABLE
/** Represents a canvas view on which the selectors will be drawn */
@interface MSSCanvas : NSView
/** @name Delegation */
/** The controller of the canvas */
@property (nonatomic) IBOutlet id<MSSCanvasDelegate> delegate;
/** @name Properties */
/** Determines if intersections between selections are allowed. (Default: YES) */
@property (nonatomic) IBInspectable BOOL allowIntersections;

@end
