//
//  ZoomImageWindow.h
//  MultiSelectionSuiteApp
//
//  Created by Sascha Paulus on 14.01.15.
//  Copyright (c) 2015 Sascha Paulus. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <MultiSelectionSuite/MultiSelectionSuite.h>

@interface ZoomImageWindow : NSWindowController  <MSSExtractorDelegate>

@property (nonatomic) MSSCanvasController *zoomCanvasController;
@property (nonatomic) MSSCanvas *zoomCanvas;
@property (nonatomic) MSSZoomExtractor *zoomExtractor;
@property (nonatomic) IBOutlet NSImageView *sourceView;
@property (nonatomic) IBOutlet NSImageView *zoomTargetView;
@property (nonatomic) IBOutlet MSSSelectorStyle *zoomSelectorStyle;
@property (nonatomic) IBOutlet MSSHandleStyle *zoomHandleStyle;


@end
