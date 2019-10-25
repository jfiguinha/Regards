/*
 
 ------------------------------------------------------------------------------------
 MSSHandle.m
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

#import "MSSHandle.h"
#import "MSSSelector.h"
#import "MSSHandleStyle.h"

@interface MSSHandle ()

@property (nonatomic) MSSHandleStyle *handleStyle;

/** Sets the frame for the handle according to the selectors frame */
- (void) makeHandleFrameWithSelectorFrame;
/** Sets the handles's HandleStyle wheather from a default style or a style which is assigned to the canvas's delegate. */
- (void)setStyle;
/** Update the tracking area */
- (void) updateTrackingArea;

@end

@implementation MSSHandle

@synthesize type = _type;
@synthesize frame = _frame;
@synthesize bounds = _bounds;
@synthesize selector = _selector;
@synthesize trackingArea = _trackingArea;

#pragma mark - Init & Alloc

/** Initilizes a handle with a type for a given MSSSelector
 @param handleType The type handle which should be created
 @param selector The selector to create the handle for
 */
- (instancetype) initWithType:(SelectorHandleType)handleType forSelector:(MSSSelector*)selector {

    self = [super init];
    if(self) {
        _selector = selector;
        [self setStyle];
        _type = handleType;
    }
    return self;
}

+ (instancetype) handleWithType:(SelectorHandleType)type forSelector:(MSSSelector*)selector {
    id instance = [[self alloc] initWithType:type forSelector:selector];
    return instance;
}

- (void)setStyle {
    if([self.selector.canvas.delegate handleStyle]!=nil) {
        self.handleStyle = [self.selector.canvas.delegate handleStyle];
    } else {
        self.handleStyle = [[MSSHandleStyle alloc] init];
    }
}

#pragma mark - Public methods

- (void) drawPath {
    
    [self makeHandleFrameWithSelectorFrame];
    
    NSBezierPath *path = self.bezierPath;
    [self.handleStyle.strokeColor setStroke];
    [path stroke];

    [NSGraphicsContext saveGraphicsState];
    NSShadow *shadow;
    if((shadow = self.shadow)!=nil) [shadow set];
    [self.handleStyle.fillColor setFill];
    [path fill];
    [NSGraphicsContext restoreGraphicsState];
    
}

#pragma mark - Internal methods


- (void) updateTrackingArea {

    NSRect trackingRect = [self.selector.canvas.delegate defaultRectForFrame:self.frame];
    if(self.trackingArea != nil) {
        [self.selector.canvas removeTrackingArea:self.trackingArea];
        _trackingArea = nil;
    }

    NSCursor *cursor = [NSCursor crosshairCursor];
    NSPoint hotSpot = NSMakePoint(9,9);
    NSBundle *frameworkBundle = [NSBundle bundleForClass:[self class]];
    NSString  *buttomTopPath = [frameworkBundle pathForImageResource:@"BottomTop"];
    NSString  *topButtomPath = [frameworkBundle pathForImageResource:@"TopBottom"];
    NSImage *buttomTop = [[NSImage alloc] initByReferencingFile:buttomTopPath];
    NSImage *topButtom = [[NSImage alloc] initByReferencingFile:topButtomPath];
    
    
    if (self.type == SelectorHandleTypeLowerLeft) {
        cursor = [[NSCursor alloc] initWithImage:buttomTop hotSpot:hotSpot];
    } else if (self.type == SelectorHandleTypeUpperLeft) {
        cursor = [[NSCursor alloc] initWithImage:topButtom hotSpot:hotSpot];
    } else if (self.type == SelectorHandleTypeUpperRight) {
        cursor = [[NSCursor alloc] initWithImage:buttomTop hotSpot:hotSpot];
    } else if (self.type == SelectorHandleTypeLowerRight) {
        cursor = [[NSCursor alloc] initWithImage:topButtom hotSpot:hotSpot];
    }
    
    NSTrackingAreaOptions options = NSTrackingActiveAlways | NSTrackingMouseEnteredAndExited;
    _trackingArea = [[NSTrackingArea alloc] initWithRect:trackingRect options:options owner:self.selector.canvas userInfo:@{@"Cursor":cursor,@"Rect":NSStringFromRect(trackingRect)}];
    [self.selector.canvas addTrackingArea:self.trackingArea];
    
}
- (void) makeHandleFrameWithSelectorFrame {

    
    CGFloat halfWidth   = self.handleStyle.size.width / 2;
    CGFloat halfHeight  = self.handleStyle.size.height /2;
    
    NSRect selectorFrame = self.selector.frame;
    CGFloat minY = selectorFrame.origin.y;
    CGFloat maxY = selectorFrame.origin.y + selectorFrame.size.height;
    CGFloat minX = selectorFrame.origin.x;
    CGFloat maxX = selectorFrame.origin.x + selectorFrame.size.width;
    
    switch (self.type) {
        case SelectorHandleTypeUpperLeft: {
            _frame = NSMakeRect(minX - halfWidth, maxY - halfHeight, self.handleStyle.size.width, self.handleStyle.size.height);
            break;
        }
        case SelectorHandleTypeUpperRight: {
            _frame = NSMakeRect(maxX - halfWidth, maxY - halfHeight, self.handleStyle.size.width, self.handleStyle.size.height);
            break;
        }
        case SelectorHandleTypeLowerLeft: {
            _frame = NSMakeRect(minX - halfWidth, minY - halfHeight, self.handleStyle.size.width, self.handleStyle.size.height);
            break;
        }
        case SelectorHandleTypeLowerRight: {
            _frame = NSMakeRect(maxX - halfWidth, minY - halfHeight, self.handleStyle.size.width, self.handleStyle.size.height);
            break;
        }
        default: {
            _frame = NSZeroRect;
            break;
        }
    }
    
    _bounds = _frame;
    
    [self updateTrackingArea];

}



- (NSShadow *) shadow {
    return nil;
}

- (NSBezierPath*) bezierPath {
    NSBezierPath *path = [NSBezierPath bezierPathWithOvalInRect:self.frame];
    [path setLineWidth:2.0f];
    return path;
}

@end
