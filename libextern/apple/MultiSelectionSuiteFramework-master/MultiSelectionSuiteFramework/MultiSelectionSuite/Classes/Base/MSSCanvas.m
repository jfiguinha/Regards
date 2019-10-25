
/*
 
 ------------------------------------------------------------------------------------
 MSSCanvas.m
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

#import "MSSCanvas.h"
#import "MSSSelector.h"
#import "MSSHandle.h"


@implementation MSSCanvas

#pragma mark - Redrawing

- (void)awakeFromNib {
    self.allowIntersections = YES;
}

- (void)drawRect:(NSRect)dirtyRect {
    
    /** Draw all selectors */
    for (MSSSelector *selector in self.delegate.selectors) {
        [selector drawPath];
    }
    /** Draw the current selector (not part of self.delegate.selectors yet)  */
    if(self.delegate.currentSelector) {
        [self.delegate.currentSelector drawPath];
    }
}

#pragma mark - Events

- (void)mouseDown:(NSEvent *)theEvent {
    [self.delegate mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent {
    [self.delegate mouseUp:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent {
    [self.delegate mouseDragged:theEvent];
}

- (void)keyDown:(NSEvent *)theEvent {
    [self.delegate keyDown:theEvent];
}


#pragma First Responder

- (BOOL)acceptsFirstResponder {
    return YES;
}

@end
