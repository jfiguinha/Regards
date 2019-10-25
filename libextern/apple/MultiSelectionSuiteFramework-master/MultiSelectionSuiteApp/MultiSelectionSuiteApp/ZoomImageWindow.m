//
//  ZoomImageWindow.m
//  MultiSelectionSuiteApp
//
//  Created by Sascha Paulus on 14.01.15.
//  Copyright (c) 2015 Sascha Paulus. All rights reserved.
//

#import "ZoomImageWindow.h"

@interface ZoomImageWindow ()

@end

@implementation ZoomImageWindow

@synthesize zoomCanvas;
@synthesize zoomCanvasController;
@synthesize zoomExtractor;

- (NSString *)windowNibName {
    return @"ZoomImageWindow";
}


- (void)awakeFromNib {

    
    // Setup without interface builder
    self.zoomCanvas = [[MSSCanvas alloc] init];
    self.zoomCanvasController = [MSSCanvasController canvasControllerForCanvas:zoomCanvas withContentView:self.sourceView];
    [self.zoomCanvas setDelegate:zoomCanvasController];
    self.zoomExtractor = [[MSSZoomExtractor alloc] init];
    [self.zoomExtractor setCanvasController:zoomCanvasController];
    [self.zoomExtractor setDelegate:self];
    [self.zoomExtractor prepareJob:MSSExtractorJobZoom];
    [self.zoomCanvasController setSingleSelectionMode:YES];
    [self.zoomCanvasController setSelectorStyle:self.zoomSelectorStyle];
    [self.zoomCanvasController setHandleStyle:self.zoomHandleStyle];
    [self.zoomCanvasController setContentExtractor:self.zoomExtractor];
}

#pragma mark - MSSExtractorDelegate

- (void) extractor:(MSSExtractor*)extractor extractElement:(id)element forJob:(NSString*)identifier {
    
    if ([element isKindOfClass:[NSImage class]]) {
        
        NSImage *image = (NSImage*)element;
        
        if(identifier == MSSExtractorJobZoom) {
            [self.zoomTargetView setImage:image];
        }
    }
    
    
}


- (void) extractor:(MSSExtractor*)extractor didCompleteWithError:(NSError*)error {
    }

- (void) extractor:(MSSExtractor*)extractor didSaveImage:(NSURL*)fileUrl withError:(NSError*)error; {
}


@end
