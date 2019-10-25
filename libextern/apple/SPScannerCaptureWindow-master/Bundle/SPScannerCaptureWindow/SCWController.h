/*
 
 ------------------------------------------------------------------------------------
 SCWController.h
 ------------------------------------------------------------------------------------
 The window controller which is responsible for the whole process of interacting 
 with the scanner devices. This is the place where all the magic happens. Reading
 available scanner devices, reading the capacities of each scanner, making overview 
 and actual scans in color, b/w and grayscale, ...
 In addition the controller is using the MultiSelectionSuite.framework to enable
 partial area scans (for flatbed scanners only).
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
#import <Quartz/Quartz.h>
#import <MultiSelectionSuite/MultiSelectionSuite.h>

@protocol WindowBundleDelegate
- (void)saveContentElements:(NSArray *)elements withProperties:(NSDictionary*)properties;
@end

@interface SCWController : NSWindowController <ICScannerDeviceDelegate, ICDeviceBrowserDelegate, NSTableViewDelegate>

@property (nonatomic) IBOutlet NSImageView *scannerView;
@property (nonatomic) IBOutlet NSImageView *scanPreview;
@property (nonatomic) IBOutlet NSArrayController *scannerDevicesController;
@property (nonatomic) IBOutlet NSPopUpButton *scannerPopup;
@property (nonatomic) IBOutlet NSPopUpButton *resolutionPopup;
@property (nonatomic) IBOutlet NSMatrix *colorRadio;
@property (nonatomic) IBOutlet NSButton *improveResult;
@property (nonatomic) IBOutlet NSButton *asPdf;
@property (nonatomic) IBOutlet NSButton *closeButton;
@property (nonatomic) IBOutlet MSSCanvasController *canvasController;
@property (nonatomic) IBOutlet id<WindowBundleDelegate> delegate;
@property (nonatomic) IBOutlet NSWindow *setupWindow;
@property (nonatomic) IBOutlet NSArrayController *vendorFeaturesController;

- (instancetype) initWithDelegate:(id<WindowBundleDelegate>)delegate;
- (IBAction)startScan:(id)sender;
- (IBAction)overviewScan:(id)sender;
- (IBAction)cancelScan:(id)sender;
- (IBAction)improveImageState:(id)sender;
- (IBAction)closeSheet:(id)sender;
- (IBAction)openSetup:(id)sender;

@end



