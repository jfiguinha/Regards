/*
 
 ------------------------------------------------------------------------------------
 MSSCanvasController.m
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

#import "MSSCanvasController.h"
#import "MSSSelector.h"
#import "MSSHandle.h"
#import "NSImageView+MSSScaledImage.h"
#import "MSSExtractor.h"

@interface MSSCanvasController ()

/** All selectors */
@property (nonatomic) NSMutableArray *_selectors;
/** Current selector */
@property (nonatomic) MSSSelector *currentSelector;
/** Current handle */
@property (nonatomic) MSSHandle *handle;
/** Determines the canvas frame based on the contentView type */
@property (nonatomic, readonly) NSRect canvasFrame;

@property (nonatomic) BOOL cursorCanBeChanged;


/** Marks all selectors as unselected and redraws canvas if needed
 @param redraw Selectors should be redrawn after change
 */
- (void) markAllUnselected:(BOOL)redraw;
/** Marks all selectors as selected and redraws canvas if needed
 @param redraw Selectors should be redrawn after change
 */
- (void) markAllSelected:(BOOL)redraw;
/** Discards all selectors from the canvas which are marked as selected */
- (void) removeMarkedSelectorsFromCanvas;
/** Creates a new selector and defines him as the current selector object
 @param frame The initial frame of the new selector
 */
- (void) addSelectorWithFrame:(NSRect)frame;
/** @abstract Creates a new picker selector and defines him as the current selector object.
 
 @discussion For more details about the distinction between picker and selector checkout the isPicker property of the MSSSelector class.
 @param frame The initial frame of the new picker
 */
- (void) setPickerWithFrame:(NSRect)frame;
/** Determines if the canvas has current selector object or not */
- (BOOL) hasSelector;
/** Sets or replaces the canvas in the view's subviews */
- (void) arrangeCanvas;
/** Reset the canvas and remove all selections */
- (void) resetCanvas;
/** Will be called whenever something on the contentView has changed
 @param notification The notification with information about the change
 */
- (void) contentViewDidChange:(NSNotification*)notification;


@end

@implementation MSSCanvasController

#pragma mark - Init

- (instancetype)initWithCanvas:(MSSCanvas*)theCanvas withContentView:(NSView*)theContentView {
    self = [super init];
    if(self) {
        self.view = theCanvas;
        self.contentView = theContentView;
        [self prepareController];
    }
    return self;
}

+ (instancetype)canvasControllerForCanvas:(MSSCanvas*)theCanvas withContentView:(NSView*)theContentView {
    return [[self alloc] initWithCanvas:theCanvas withContentView:theContentView];
}

- (void)awakeFromNib {
    
    [self prepareController];
}

- (void) prepareController {
    
    self.cursorCanBeChanged = YES;

    NSKeyValueObservingOptions options = NSKeyValueObservingOptionNew|NSKeyValueObservingOptionOld;

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(contentViewDidChange:) name:NSViewFrameDidChangeNotification object:self.contentView];
    
    if([self.contentView isKindOfClass:[NSImageView class]]) {
        
        [(NSImageView*)self.contentView setRefusesFirstResponder:YES];
        
        NSArray *keyPaths = @[@"self.contentView.image",
                              @"self.contentView.image.size",
                              @"self.contentView.imageAlignment",
                              @"self.contentView.imageFrameStyle",
                              @"self.contentView.imageScaling"];
        
        for(NSString *keyPath in keyPaths) {
            
            [self addObserver:self forKeyPath:keyPath options:options context:NULL];
        }
    }
    
    
    self._selectors = [NSMutableArray array];
    self.currentSelector = nil;
    
    [self arrangeCanvas];

}


#pragma mark - Observation

- (void) resetCanvas {
    [self removeAllSelectorsFromCanvas];
    [self arrangeCanvas];
}

- (void) contentViewDidChange:(NSNotification*)notification {
    [self resetCanvas];
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    [self resetCanvas];
}

#pragma mark - Events

/** @abstract If the left mouse button is pressed and no selector is on the canvas or the command key is pressed add a new selector to the canvas and define him as the current selector object. Otherwise determine if an existing selector has been touched. If yes, mark this selector as selected and define him as the current selector object. If not, create a picker selector.
 @param theEvent An object encapsulating information about the mouse-down event.
 @discussion For more details about the distinction between picker and selector checkout the isPicker property of the MSSSelector class.
 */
- (void)mouseDown:(NSEvent *)theEvent {
    
    self.cursorCanBeChanged = NO;
    [self.view.window disableCursorRects];
    
    [self markAllUnselected:YES];
    
    NSPoint currentPoint    = [self.view convertPoint:[theEvent locationInWindow] fromView:nil];
    NSRect initalFrame      = NSMakeRect(currentPoint.x, currentPoint.y, 0, 0);
    self.currentSelector    = nil;
    
    if(([theEvent modifierFlags] & NSCommandKeyMask) || self.selectors.count == 0) {
        [self addSelectorWithFrame:initalFrame];
    } else {
        BOOL didHitSelector = NO;
        
        for(MSSSelector *selector in self.selectors) {
            if((didHitSelector = [selector intersectsFrame:initalFrame])) {
                self.currentSelector = selector;
                [self.currentSelector touchedHandle:initalFrame];
                break;
            }
        }
        if(!didHitSelector) {
            [self setPickerWithFrame:initalFrame];
        }
    }    
    
    [self.view setNeedsDisplay:YES];
}

/** If the left mouse button has been released and the canvas has selectors determine if the current selector object is a picker. If yes, mark every intersecting selection as selected. If not, try to drop the current selector object on the canvas. If this is not possible discard the current selector object. 
 @param theEvent An object encapsulating information about the mouse-up event.
 */
- (void)mouseUp:(NSEvent *)theEvent {

    self.cursorCanBeChanged = YES;
    
    if(self.hasSelector) {
        if(self.currentSelector.isPicker) {
            for(MSSSelector *selector in self.selectors) {
                [selector intersectsFrame:self.currentSelector.frame];
            }
            self.currentSelector = nil;
        } else {
            if(self.currentSelector.canDropOnCanvas) {
                [self performSelector:@selector(canvasHasChanged)];
                
            } else {
                [self._selectors removeObject:self.currentSelector];
                self.currentSelector = nil;
            }
        }
    }
    [self.view setNeedsDisplay:YES];

}


/** If the left mouse button is pressed, the mouse has been moved and the canvas has selectors execute the corresponding action of the current selection object. 
 @param theEvent An object encapsulating information about the mouse-dragged event.
 */
- (void)mouseDragged:(NSEvent *)theEvent {
    
    NSPoint currentPoint = [self.view convertPoint:[theEvent locationInWindow] fromView:nil];
    
    if(self.hasSelector) {
        
        if(self.currentSelector.action == SelectorActionInit || self.currentSelector.action == SelectorActionResize) {
            [self.currentSelector resizeToPoint:currentPoint];
            [self.view setNeedsDisplay:YES];
            if([self.currentSelector touchedBoundaries]) {
                [self.view setNeedsDisplay:YES];
            }
        } else if (self.currentSelector.action == SelectorActionMove) {
            [self.currentSelector moveSelectorToPoint:currentPoint];
            [self.view setNeedsDisplay:YES];
            if([self.currentSelector touchedBoundaries]) {
                [self.view setNeedsDisplay:YES];
            }
        }
    }
    
}

/** If the DEL or BACKSPACE key is pressed delete all as selected marked selections. 
 If CMD + A key is pressed mark all selectors as selected.
 If the ESC key is pressed mark all selectors as unselected. 
 @param theEvent An object encapsulating information about the key-down event.
 */
- (void)keyDown:(NSEvent *)theEvent {
    
    if(theEvent.keyCode == 51 || theEvent.keyCode == 117) {
        [self removeMarkedSelectorsFromCanvas];
    } else if(theEvent.keyCode == 0 && theEvent.modifierFlags&NSCommandKeyMask) {
        [self markAllSelected:YES];
    } else if(theEvent.keyCode == 53) {
        [self markAllUnselected:YES];
    }
}

- (void)mouseEntered:(NSEvent *)theEvent {
    if(self.cursorCanBeChanged) {
        NSDictionary *userInfo = (NSDictionary *)[theEvent userData];
        NSCursor *cursor = [userInfo objectForKey:@"Cursor"];
        [cursor set];
    }
}


- (void)mouseExited:(NSEvent *)theEvent {
    
    if(self.cursorCanBeChanged) {
        [[NSCursor arrowCursor] set];
    }
}

- (BOOL)acceptsFirstResponder {
    return YES;
}


#pragma mark - Public methods

- (NSRect) defaultRectForFrame:(NSRect)frame {
    
    NSRect result = NSZeroRect;
    
    CGFloat newX = frame.origin.x;
    CGFloat newY = frame.origin.y;
    CGFloat newW = frame.size.width;
    CGFloat newH = frame.size.height;
    
    if(frame.size.width < 0) {
        newX = frame.origin.x + frame.size.width;
        newW = fabs(frame.size.width);
    }
    
    if(frame.size.height < 0) {
        newY = frame.origin.y + frame.size.height;
        newH = fabs(frame.size.height);
    }
    
    result = NSMakeRect(newX, newY, newW, newH);
    
    return result;
    
}

- (void) removeAllSelectorsFromCanvas {
    [self markAllSelected:NO];
    [self removeMarkedSelectorsFromCanvas];
}

- (BOOL) hasSelectors {
    
    if(self.selectors && self.selectors.count > 0) {
        return YES;
    }
    return NO;
    
}

#pragma mark - Other methods

- (void) markAllUnselected:(BOOL)redraw {
    
    for(MSSSelector *selector in self.selectors) {
        [selector setUnselected];
    }
    
    if(redraw) {
        [self.view setNeedsDisplay:YES];
    }
}

- (void) markAllSelected:(BOOL)redraw {
    
    for(MSSSelector *selector in self.selectors) {
        [selector setSelected];
    }
    
    if(redraw) {
        [self.view setNeedsDisplay:YES];
    }
}

- (void) removeMarkedSelectorsFromCanvas {

    
    self.currentSelector = nil;
    NSMutableIndexSet *discardedItems = [NSMutableIndexSet indexSet];
    for(int i=0;i<self.selectors.count;i++) {
        MSSSelector *item = (self.selectors)[i];
        if([item isSelected]) {
            [discardedItems addIndex:i];
            [item removeTrackingAreas];
        }
    }
    [self._selectors removeObjectsAtIndexes:discardedItems];
    [self.view setNeedsDisplay:YES];
    [[NSCursor arrowCursor] set];
    [self performSelector:@selector(canvasHasChanged)];
    
    
}

- (void) canvasHasChanged {
    
    if(self.contentExtractor) {
        [self.contentExtractor canvasHasChanged];
    }
}


- (void) addSelectorWithFrame:(NSRect)frame {
    
    if(self.singleSelectionMode == NO || self.hasSelectors == NO) {
        MSSSelector *newSelector = [MSSSelector selectorWithFrame:frame forCanvas:self.view asPicker:NO];
        [self._selectors addObject:newSelector];
        self.currentSelector = newSelector;        
    }

}


- (void) setPickerWithFrame:(NSRect)frame {
    self.currentSelector = [MSSSelector selectorWithFrame:frame forCanvas:self.view asPicker:YES];
}

- (BOOL) hasSelector {
    return (self.currentSelector != nil);
}

- (NSArray*) selectors {
    return self._selectors;
}

- (NSRect) canvasFrame {
    
    if([self.contentView isKindOfClass:[NSImageView class]]) {
        return [(NSImageView*)self.contentView scaledImageRect];
    } else {
        return self.contentView.bounds;
    }
}

- (void) arrangeCanvas {
    
    self.view.frame = self.canvasFrame;
    if([self.contentView.subviews indexOfObject:self.view] != NSNotFound) {
        [self.contentView replaceSubview:self.view with:self.view];
    } else {
        [self.contentView addSubview:self.view];
    }
}


@end
