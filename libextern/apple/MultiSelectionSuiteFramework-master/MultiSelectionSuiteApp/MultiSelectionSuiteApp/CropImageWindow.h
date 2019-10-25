//
//  CropImageWindow.h
//  MultiSelectionSuiteApp
//
//  Created by Sascha Paulus on 14.01.15.
//  Copyright (c) 2015 Sascha Paulus. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <MultiSelectionSuite/MultiSelectionSuite.h>

@interface CropImageWindow : NSWindowController <MSSExtractorDelegate>

@property (nonatomic) IBOutlet NSImageView *imageView;
@property (nonatomic) IBOutlet MSSImageExtractor *imageExtractor;

- (IBAction)saveImages:(id)sender;

@end
