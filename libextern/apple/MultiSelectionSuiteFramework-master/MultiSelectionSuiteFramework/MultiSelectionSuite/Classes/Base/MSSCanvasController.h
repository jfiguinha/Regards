/*
 
 ------------------------------------------------------------------------------------
 MSSCanvasController.h
 ------------------------------------------------------------------------------------
 The canvasController handles any interaction between the canvas, the mouse 
 and other objects. The controller moves and resizes the canvas's selectors.
 Optional canvasController can provide the styles for the selectors and the handles. 
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

#import <Cocoa/Cocoa.h>
#import "MSSCanvas.h"
#import "MSSSelectorStyle.h"
#import "MSSHandleStyle.h"

@class MSSExtractor;

IB_DESIGNABLE
/** Represents a ViewController customized to control a MSSCanvas object */
@interface MSSCanvasController : NSViewController <MSSCanvasDelegate>
/** @name Properties */
/** Holds all selectors on the canvas */
@property (nonatomic, readonly) NSArray *selectors;
/** Determines the canvas */
@property (atomic) IBOutlet MSSCanvas* view;
/** Determines the chosen SelectorStyle */
@property (nonatomic) IBOutlet MSSSelectorStyle *selectorStyle;
/** Determines the chosen HandleStyle */
@property (nonatomic) IBOutlet MSSHandleStyle *handleStyle;
/** Determines if the canvas has selectors or not */
@property (nonatomic, readonly) BOOL hasSelectors;
/** The view with will contain the canvas as subview */
@property (nonatomic) IBOutlet NSView *contentView;
/** Determines if the canvas can have only one selector */
@property (nonatomic) IBInspectable BOOL singleSelectionMode;
/** The object which extracts the content from the contentView */
@property (nonatomic) IBOutlet MSSExtractor *contentExtractor;

/** @name Methods */
- (NSRect) defaultRectForFrame:(NSRect)frame;
- (void)removeAllSelectorsFromCanvas;
- (void) prepareController;
+ (instancetype)canvasControllerForCanvas:(MSSCanvas*)theCanvas withContentView:(NSView*)theContentView;


@end
