/*
 
 ------------------------------------------------------------------------------------
 MSSSelectorStyle.h
 ------------------------------------------------------------------------------------
 This class provides properties for the look of a MSSSelector.
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

#import <Cocoa/Cocoa.h>

IB_DESIGNABLE
/** Represents a style object for a MSSSelector. */
@interface MSSSelectorStyle : NSObject {
    
}
/** @name Properties */
/** Determines the minimum height for a selector */
@property (nonatomic) IBInspectable CGFloat minHeight;
/** Determines the minimum width for a selector */
@property (nonatomic) IBInspectable CGFloat minWidth;
/** @abstract Determines the stroke color for a picker selector. 
 @discussion For more details about the distinction between picker and selector checkout the isPicker property. */
@property (nonatomic) IBInspectable NSColor *strokeColorPicker;
/** Determines the fill color for a picker selector.
 @discussion For more details about the distinction between picker and selector checkout the isPicker property. */
@property (nonatomic) IBInspectable NSColor *fillColorPicker;
/** Determines the stroke color for a selector if it is marked as selected. */
@property (nonatomic) IBInspectable NSColor *strokeColorSelected;
/** Determines the stroke color for a selector if it is NOT marked as selected. */
@property (nonatomic) IBInspectable NSColor *strokeColorUnselected;
/** Determines the fill color for a selector if it is marked as selected. */
@property (nonatomic) IBInspectable NSColor *fillColorSelected;
/** Determines the fill color for a selector if it is NOT marked as selected. */
@property (nonatomic) IBInspectable NSColor *fillColorUnselected;
/** Determines the width of the stroke line if selector is marked as selected */
@property (nonatomic) IBInspectable CGFloat lineWidthSelected;
/** Determines the width of the stroke line if selector is NOT marked as selected */
@property (nonatomic) IBInspectable CGFloat lineWidthUnselected;
/** Determines if the stroke line should be dashed if selector is marked as selected */
@property (nonatomic) IBInspectable BOOL lineDashedSelected;
/** Determines if the stroke line should be dashed if selector is NOT marked as selected */
@property (nonatomic) IBInspectable BOOL lineDashedUnselected;

@end
