/*
 
 ------------------------------------------------------------------------------------
 MSSSelector.m
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

#import "MSSSelector.h"
#import "MSSHandle.h"
#import "MSSSelectorStyle.h"
#import "MSSExtractor.h"

@interface MSSSelector ()

@property NSColor *strokeColor;
@property NSColor *fillColor;
@property NSRect lastFrame;
@property (nonatomic) MSSSelectorStyle *selectorStyle;

/** Determines the canvas boundaries */
@property (nonatomic,readonly) Boundaries canvasBoundaries;
@property (nonatomic) Boundaries boundaries;
/** Determines if the selector's frame has the minimum size */
@property (nonatomic, readonly) BOOL isBigEnough;
/** Determines if and where the selector's frame crosses the canvas vertical boundaries.
 @param frame The current frame of the selector
 */
- (SelectorBoundariesPosition) verticalInBounds:(NSRect)frame;
/** Determines if and where the selector's frame crosses the canvas horizontal boundaries.
 @param frame The current frame of the selector
 */
- (SelectorBoundariesPosition) horizontalInBounds:(NSRect)frame;
/** Draw the selector's handles */
- (void) drawHandles;
/** Determines if the selector's frame intersects with other selectors.
 @param selectors The other selectors which could have been intersected.
 */
- (BOOL) intersectsOthers:(NSArray*)selectors;
/** Prepares the boundaries based on the canvas boundaries, the current action and the selector's current frame. */
- (void) prepareBoundaries;
/** Rearranged the selector's origin based on the current handle. */
- (void) makeOriginAtHandle;
/** Redefines the frame's origin to a normal (lower-left) origin */
- (void) restoreOrigin;
/** Sets the selector's SelectorStyle wheather from a default style or a style which is assigned to the canvas's delegate. */
- (void) setStyle;

/** Update the tracking area */
- (void) updateTrackingArea;

@end


@implementation MSSSelector

@synthesize handles = _handles;
@synthesize isSelected = _isSelected;
@synthesize isPicker = _isPicker;
@synthesize frame = _frame;
@synthesize action = _action;
@synthesize currentHandle = _currentHandle;
@synthesize canvas = _canvas;
@synthesize trackingArea = _trackingArea;

#pragma mark - Init & Alloc

/** Initilizes a selector or picker for a given MSSCanvas
 @param rect The frame for the selector
 @param canvas The canvas on which the selector should be drawn
 @param picker Determines if the new selector should be a picker
 @discussion For more details about the distinction between picker and selector checkout the isPicker property.
 */
- (instancetype)initWithFrame:(NSRect)rect forCanvas:canvas asPicker:(BOOL)picker {

    self = [super init];
    if(self) {
        
        _frame = rect;
        _canvas = canvas;
        [self setStyle];
        _isPicker = picker;
        [self prepareAction:SelectorActionInit];
        _currentHandle = nil;
        [self setSelected];
        
        _handles = [[NSMutableArray alloc] init];
        [_handles addObject:[MSSHandle handleWithType:SelectorHandleTypeUpperLeft forSelector:self]];
        [_handles addObject:[MSSHandle handleWithType:SelectorHandleTypeUpperRight forSelector:self]];
        [_handles addObject:[MSSHandle handleWithType:SelectorHandleTypeLowerLeft forSelector:self]];
        [_handles addObject:[MSSHandle handleWithType:SelectorHandleTypeLowerRight forSelector:self]];
        
        
    }
    return self;
    
}

+ (instancetype) selectorWithFrame:(NSRect)rect forCanvas:canvas asPicker:(BOOL)picker {
    return [[self alloc] initWithFrame:rect forCanvas:canvas asPicker:picker];
}

- (void)setStyle {
    if([self.canvas.delegate selectorStyle]!=nil) {
        self.selectorStyle = [self.canvas.delegate selectorStyle];
    } else {
        self.selectorStyle = [[MSSSelectorStyle alloc] init];
    }
}

#pragma mark - Draw

- (void) drawPath {

    NSBezierPath *path = self.isPicker ? self.pickerBezierPath : self.selectorBezierPath;
    [self.strokeColor setStroke];
    [path stroke];
    [self.fillColor setFill];
    [path fill];
    [self drawHandles];
    

}

#pragma mark - Public methods

- (void) setSelected {
    self.strokeColor = self.isPicker ? self.selectorStyle.strokeColorPicker : self.selectorStyle.strokeColorSelected;
    self.fillColor = self.isPicker ? self.selectorStyle.fillColorPicker : self.selectorStyle.fillColorSelected;
    _isSelected = YES;
}
- (void) setUnselected {
    self.strokeColor = self.isPicker ? self.selectorStyle.strokeColorPicker : self.selectorStyle.strokeColorUnselected;
    self.fillColor = self.isPicker ? self.selectorStyle.fillColorPicker : self.selectorStyle.fillColorUnselected;
    _isSelected = NO;
}

- (BOOL) intersectsFrame:(NSRect)rect {
    
    if(CGRectIntersectsRect(rect, self.frame)) {
        [self setSelected];
        return YES;
    }
    if([self touchedHandle:rect] != SelectorHandleTypeNoHandle) {
        [self setSelected];
        return YES;
    }
    return NO;
}

- (BOOL) touchedHandle:(NSRect)rect {
    
    for(MSSHandle *handle in self.handles) {
        if(CGRectIntersectsRect(rect, handle.bounds)) {
            _currentHandle = handle;
            [self prepareAction:SelectorActionResize];
            return YES;
        }
    }
    [self prepareAction:SelectorActionMove];
    return NO;
}

- (void) restoreOrigin {
    _frame = [self.canvas.delegate defaultRectForFrame:self.frame];
}

- (void) prepareBoundaries {
    
    Boundaries boundaries;
    
    switch (self.action) {
        case SelectorActionNone: {

            boundaries.right.position = self.frame.origin.x;
            boundaries.right.size = self.canvasBoundaries.right.position - boundaries.right.position;
            
            boundaries.left.position = self.frame.origin.x;
            boundaries.left.size = self.canvasBoundaries.left.position - boundaries.left.position;
            
            boundaries.top.position = self.frame.origin.y;
            boundaries.top.size = self.canvasBoundaries.top.position - boundaries.top.position;
            
            boundaries.bottom.position = self.frame.origin.y;
            boundaries.bottom.size = self.canvasBoundaries.bottom.position - boundaries.bottom.position;

            break;
        }
            
        case SelectorActionInit: {
            
            boundaries.right.position = self.frame.origin.x;
            boundaries.right.size = self.canvasBoundaries.right.position - boundaries.right.position;
            
            boundaries.left.position = self.frame.origin.x;
            boundaries.left.size = self.canvasBoundaries.left.position - boundaries.left.position;
            
            boundaries.top.position = self.frame.origin.y;
            boundaries.top.size = self.canvasBoundaries.top.position - boundaries.top.position;
            
            boundaries.bottom.position = self.frame.origin.y;
            boundaries.bottom.size = self.canvasBoundaries.bottom.position - boundaries.bottom.position;
            
            break;
        }
            
        case SelectorActionMove: {

            boundaries.right.position = self.canvasBoundaries.right.position - self.frame.size.width;
            boundaries.right.size = self.frame.size.width;
            
            boundaries.left.position = self.canvasBoundaries.left.position;
            boundaries.left.size = self.frame.size.width;
            
            boundaries.top.position = self.canvasBoundaries.top.position - self.frame.size.height;
            boundaries.top.size = self.frame.size.height;
            
            boundaries.bottom.position = self.canvasBoundaries.bottom.position;
            boundaries.bottom.size = self.frame.size.height;

            break;
        }
        case SelectorActionResize: {

            CGFloat X0 = self.frame.origin.x + self.frame.size.width;
            boundaries.right.position = self.canvasBoundaries.right.position;
            boundaries.right.size = X0 - boundaries.right.position;
            
            boundaries.left.position = self.canvasBoundaries.left.position;
            boundaries.left.size = self.frame.origin.x + self.frame.size.width;
            
            CGFloat Y0 = self.frame.origin.y + self.frame.size.height;
            boundaries.top.position = self.canvasBoundaries.top.position;
            boundaries.top.size = Y0 - boundaries.top.position;
            
            boundaries.bottom.position = self.canvasBoundaries.bottom.position;
            boundaries.bottom.size = self.frame.origin.y + self.frame.size.height;
            
            break;
        }
        default:
            break;
    }
    
    self.boundaries = boundaries;
   
}

- (void) makeOriginAtHandle {

    
    CGFloat pointX,pointY;
    NSPoint point = NSZeroPoint;
    if(self.currentHandle.type != SelectorHandleTypeNoHandle) {
        pointX  = self.currentHandle.bounds.origin.x + (self.currentHandle.bounds.size.width/2);
        pointY  = self.currentHandle.bounds.origin.y + (self.currentHandle.bounds.size.height/2);
        point   = NSMakePoint(pointX, pointY);
    }
   
    
    NSPoint origin  = self.frame.origin;
    NSSize size     = self.frame.size;
    BOOL doUpdate   = NO;
   
    switch (self.currentHandle.type) {
        case SelectorHandleTypeUpperLeft: {
            origin = NSMakePoint(point.x, point.y);
            size = NSMakeSize(fabs(size.width), fabs(size.height)*-1);
            doUpdate = YES;
            break;
        }
        case SelectorHandleTypeUpperRight: {
            origin = NSMakePoint(point.x, point.y);
            size = NSMakeSize(fabs(size.width)*-1, fabs(size.height)*-1);
            doUpdate = YES;
            break;
        }
        case SelectorHandleTypeLowerLeft: {
            origin = NSMakePoint(point.x, point.y);
            size = NSMakeSize(fabs(size.width), fabs(size.height));
            doUpdate = YES;
            break;
        }
        case SelectorHandleTypeLowerRight: {
            origin = NSMakePoint(point.x, point.y);
            size = NSMakeSize(fabs(size.width)*-1, fabs(size.height));
            doUpdate = YES;
            break;
        }
        default:
            break;
    }
    
    if(doUpdate) {
        _frame = NSMakeRect(origin.x, origin.y, size.width, size.height);
    }
    
    [self prepareBoundaries];
    
}


- (void) resizeToPoint:(CGPoint)point {
    
    NSPoint origin  = self.frame.origin;
    NSSize size     = self.frame.size;
    CGFloat vDif    = (point.x - origin.x);
    CGFloat hDif    = (point.y - origin.y);
    BOOL doUpdate   = NO;
    NSRect newRect  = NSZeroRect;

    switch (self.currentHandle.type) {
        case SelectorHandleTypeNoHandle: {
            newRect = NSMakeRect(origin.x, origin.y, vDif, hDif);
            doUpdate = YES;
            break;
        }
        default: {
            newRect = NSMakeRect(point.x, point.y, size.width - vDif, size.height - hDif);
            doUpdate = YES;
            break;
        }
    }
   
    if(doUpdate) {
 
        size.width = newRect.size.width;
        origin.x = newRect.origin.x;
        
        size.height = newRect.size.height;
        origin.y = newRect.origin.y;
        
        _frame = NSMakeRect(origin.x, origin.y, size.width, size.height);
        
    }
  
}

- (BOOL) touchedBoundaries {

    if(self.isPicker) {
        return NO;
    }
    
    BOOL retValue = NO;
    
    SelectorBoundariesPosition Xpos = [self verticalInBounds:self.frame];
    SelectorBoundariesPosition Ypos = [self horizontalInBounds:self.frame];
    
    if(Xpos != SelectorBoundariesPositionInBounderies) {
        
        retValue = YES;
        
        if(Xpos == SelectorBoundariesPositionRightOuterBounderies) {
            _frame.origin.x = self.boundaries.right.position;
            _frame.size.width = self.boundaries.right.size;
        } else if(Xpos == SelectorBoundariesPositionLeftOuterBounderies) {
            _frame.origin.x = self.boundaries.left.position;
            _frame.size.width = self.boundaries.left.size;
        }

    }
    if(Ypos != SelectorBoundariesPositionInBounderies) {
        
        retValue = YES;
        
        if(Ypos == SelectorBoundariesPositionUpperOuterBounderies) {
            _frame.origin.y = self.boundaries.top.position;
            _frame.size.height = self.boundaries.top.size;
        } else if(SelectorBoundariesPositionLowerOuterBounderies) {
            _frame.origin.y = self.boundaries.bottom.position;
            _frame.size.height = self.boundaries.bottom.size;
        }
    }

    
    [self performSelector:@selector(selectorHasChanged)];

    return retValue;

}

- (void) selectorHasChanged {
    
    [self updateTrackingArea];
    [[self.canvas.delegate contentExtractor] selectorHasChanged:self];
}

- (void) moveSelectorToPoint:(CGPoint)point {
    
    CGFloat newX = point.x - self.frame.size.width / 2;
    CGFloat newY = point.y - self.frame.size.height / 2;
    NSRect target = self.frame;
    NSRect newRect = NSMakeRect(newX, newY, self.frame.size.width, self.frame.size.height);

    target.origin.x = newRect.origin.x;
    target.origin.y = newRect.origin.y;

    _frame = target;

}

- (BOOL) canDropOnCanvas {
    
    NSMutableArray *theOthers = [NSMutableArray array];
    for(MSSSelector* selector in [self.canvas.delegate selectors]) {
        if([selector isNotEqualTo:self]) {
            [theOthers addObject:selector];
        }
    }
    
    BOOL drop = !((!self.isBigEnough) || (!self.canvas.allowIntersections && [self intersectsOthers:theOthers]));
    if(drop) {
        [self restoreOrigin];
    }
    return drop;
}

- (void) removeTrackingAreas {
    for(MSSHandle* handle in self.handles) {
        [self.canvas removeTrackingArea:handle.trackingArea];
    }
    [self.canvas removeTrackingArea:self.trackingArea];
}

#pragma mark - Internal methods

- (void) updateTrackingArea {
    NSRect trackingRect = [self.canvas.delegate defaultRectForFrame:self.frame];
    
    MSSHandle * firstHandle = (MSSHandle*)[self.handles objectAtIndex:0];
    CGFloat hW = firstHandle.frame.size.width;
    CGFloat hH = firstHandle.frame.size.height;
    CGFloat x = trackingRect.origin.x + hW;
    CGFloat y = trackingRect.origin.y + hH;
    CGFloat w = trackingRect.size.width - (hW*2);
    CGFloat h = trackingRect.size.height - (hH*2);
    trackingRect = NSMakeRect(x,y,w,h);
    
    if(self.trackingArea != nil) {
        [self.canvas removeTrackingArea:self.trackingArea];
        _trackingArea = nil;
    }
    NSTrackingAreaOptions options = NSTrackingActiveAlways | NSTrackingMouseEnteredAndExited;
    _trackingArea = [[NSTrackingArea alloc] initWithRect:trackingRect options:options owner:self.canvas userInfo:@{@"Cursor":[NSCursor openHandCursor],@"Rect":NSStringFromRect(trackingRect)}];
    [self.canvas addTrackingArea:self.trackingArea];
}


- (Boundaries) canvasBoundaries {
    
    Boundaries result;
    result.left.position = 0;
    result.top.position = self.canvas.frame.size.height;
    result.right.position = self.canvas.frame.size.width;
    result.bottom.position = 0;
    
    return result;
    
}

- (SelectorBoundariesPosition) verticalInBounds:(NSRect)frame {
    
    NSRect nextRect = [self.canvas.delegate defaultRectForFrame:frame];
    CGFloat Xmin = nextRect.origin.x;
    CGFloat Xmax = nextRect.origin.x + nextRect.size.width;
    if(Xmax > self.canvasBoundaries.right.position) {
        return SelectorBoundariesPositionRightOuterBounderies;
    }
    if(Xmin < self.canvasBoundaries.left.position) {
        return SelectorBoundariesPositionLeftOuterBounderies;
    }
    return SelectorBoundariesPositionInBounderies;
}

- (SelectorBoundariesPosition) horizontalInBounds:(NSRect)frame {
    
    NSRect nextRect = [self.canvas.delegate defaultRectForFrame:frame];
    CGFloat Ymin = nextRect.origin.y;
    CGFloat Ymax = nextRect.origin.y + nextRect.size.height;
    if(Ymax > self.canvasBoundaries.top.position) {
        return SelectorBoundariesPositionUpperOuterBounderies;
    }
    if(Ymin < self.canvasBoundaries.bottom.position) {
        return SelectorBoundariesPositionLowerOuterBounderies;
    }
    return SelectorBoundariesPositionInBounderies;
}

- (void) drawHandles {

    if(self.isSelected && !self.isPicker) {
        for(MSSHandle *handle in self.handles) {
            [handle drawPath];
        }
    }
}

- (BOOL) intersectsOthers:(NSArray*)selectors {
    for(MSSSelector *selector in selectors) {
        if(CGRectEqualToRect(selector.frame, self.frame) == NO) {
            if([self intersectsFrame:selector.frame]) {
                return YES;
            }
        }
    }
    return NO;
}

- (BOOL) isBigEnough {
    CGFloat w = fabs(self.frame.size.width);
    CGFloat h = fabs(self.frame.size.height);
    if((w < self.selectorStyle.minWidth || h < self.selectorStyle.minHeight)) {
        return NO;
    } else {
        return YES;
    }
}

- (void) prepareAction:(SelectorAction)action {

    _action = action;
    
    switch (action) {
            
        case SelectorActionMove: {
            [self prepareBoundaries];
            break;
        }
        case SelectorActionResize: {
            [self makeOriginAtHandle];
            break;
        }
        case SelectorActionInit: {
            [self prepareBoundaries];
            break;
        }
        default:
            break;
    }
    
}

- (SelectorAction)action {
    return _action;
}



#pragma mark - Properties

- (NSBezierPath*) pickerBezierPath {
    
    NSBezierPath *path = [NSBezierPath bezierPathWithRect:self.frame];
    CGFloat lineWidth = self.isSelected ? self.selectorStyle.lineWidthSelected : self.selectorStyle.lineWidthUnselected;
    path.lineWidth = lineWidth;
    BOOL lineDash = ((self.isSelected && self.selectorStyle.lineDashedSelected) || (self.isSelected && self.selectorStyle.lineDashedUnselected));
    if(lineDash) {
        const CGFloat dash_pattern[] = {5};
        [path setLineDash:dash_pattern count:1 phase:0];
    }
    return path;
}

- (NSBezierPath*) selectorBezierPath {
    
    NSBezierPath *path = [NSBezierPath bezierPathWithRect:self.frame];
    CGFloat lineWidth = self.isSelected ? self.selectorStyle.lineWidthSelected : self.selectorStyle.lineWidthUnselected;
    path.lineWidth = lineWidth;
    BOOL lineDash = ((self.isSelected && self.selectorStyle.lineDashedSelected) || (!self.isSelected && self.selectorStyle.lineDashedUnselected));
    if(lineDash) {
        const CGFloat dash_pattern[] = {5};
        [path setLineDash:dash_pattern count:1 phase:0];
    }
    return path;
    
}


@end
