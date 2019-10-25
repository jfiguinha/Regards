//
//  CropImageWindow.m
//  MultiSelectionSuiteApp
//
//  Created by Sascha Paulus on 14.01.15.
//  Copyright (c) 2015 Sascha Paulus. All rights reserved.
//

#import "CropImageWindow.h"

@interface CropImageWindow ()

@end

@implementation CropImageWindow

@synthesize imageExtractor;

- (NSString *)windowNibName {
    return @"CropImageWindow";
}

- (void)awakeFromNib {

    // Set an example image
    NSImage *exampleImage = [NSImage imageNamed:@"animals"];
    [self.imageView setImage:exampleImage];
    
}

#pragma mark - MSSExtractorDelegate

- (void) extractor:(MSSExtractor*)extractor extractElement:(id)element forJob:(NSString*)identifier {
    
    if ([element isKindOfClass:[NSImage class]]) {
        
        // NSImage *image = (NSImage*)element;
        
        if ([identifier isEqualToString:MSSExtractorJobCropping]) {
            
        }
    }
    
    
}


- (void) extractor:(MSSExtractor*)extractor didCompleteWithError:(NSError*)error {
    
    if(error == nil) {
        
        // If everything is good ask extractor to save the images
        NSOpenPanel *openDialog = [NSOpenPanel openPanel];
        [openDialog setCanChooseDirectories:YES];
        [openDialog setCanChooseFiles:NO];
        [openDialog setAllowsMultipleSelection:NO];
        if( [openDialog runModal] == NSModalResponseOK) {
            [extractor setSaveDirectory:[openDialog directoryURL]];
            for(NSImage *image in extractor.elements) {
                [extractor saveImage:image withType:NSJPEGFileType compression:ExtractorImageCompressionLevelNone];
            }
        }
        
    } else {
        [[NSAlert alertWithError:error] runModal];
    }
    
}

- (void) extractor:(MSSExtractor*)extractor didSaveImage:(NSURL*)fileUrl withError:(NSError*)error; {
    if(error) {
        [[NSAlert alertWithError:error] runModal];
    }
}

- (IBAction)saveImages:(id)sender {
    [self.imageExtractor prepareJob:MSSExtractorJobCropping];
}


@end
