/*
 
 ------------------------------------------------------------------------------------
 SCWController.m
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


#import "SCWController.h"
#import "NSImage+SCWExtension.h"
#import "SCWScannedUnit.h"
#import "SCWError.h"

NSString * const kKeyPathScannerState = @"self.selectedScanner.selectedFunctionalUnit.state";
NSString * const kKeyPathScannerProgress = @"self.selectedScanner.selectedFunctionalUnit.scanProgressPercentDone";

@interface SCWController ()

@property (nonatomic) CGFloat scanProgressPercentDone;
@property (nonatomic) BOOL readyForOverview;
@property (nonatomic) BOOL readyForCancel;
@property (nonatomic) BOOL readyForScan;

@property (nonatomic) ICScannerDevice *selectedScanner;
@property (nonatomic, getter=checkResolutions) NSArray *scanResolutions;
@property (nonatomic) NSUInteger scanResolution;
@property (nonatomic, readonly) NSUInteger scanColor;

@property (nonatomic) SCWScannedUnit *previewScan;
@property (nonatomic) NSMutableArray *scannedPages;
@property (nonatomic) ICScannerBandData *lastBandData;
@property (nonatomic) NSMutableData *scanData;
@property (nonatomic) NSMutableArray *scanAreas;
@property (nonatomic) NSInteger currentArea;
@property (nonatomic) ICDeviceBrowser *deviceBrowser;

@end

@implementation SCWController

@synthesize selectedScanner;
@synthesize scanColor, scanResolution;
@synthesize deviceBrowser;
@synthesize previewScan;
@synthesize scannedPages;
@synthesize scanAreas;
@synthesize scanProgressPercentDone;


#pragma mark - Init

- (instancetype)init {
    self = [super init];
    if(self) {
        [self prepareController];
    }
    return self;
}

- (instancetype) initWithDelegate:(id<WindowBundleDelegate>)delegate {
    self = [super init];
    if(self) {
        [self setDelegate:delegate];
        [self prepareController];
    }
    return self;
}

- (void)awakeFromNib {
    [self prepareController];
}

- (void) prepareController {

    self.scanResolution = 300;
    self.deviceBrowser = [[ICDeviceBrowser alloc] init];
    self.deviceBrowser.delegate = self;
    [self.deviceBrowser setBrowsedDeviceTypeMask:(ICDeviceLocationTypeLocal|ICDeviceTypeMaskScanner)];
    [self.deviceBrowser start];
    [self setObservation];
    self.scannedPages = [[NSMutableArray alloc] init];
    [self.scannerPopup setAction:@selector(scannerPopUpChanged:)];
    [self.scannerPopup setTarget:self];
}

- (void) setObservation {
    
    NSKeyValueObservingOptions options = (NSKeyValueObservingOptionNew|NSKeyValueObservingOptionOld);
    [self addObserver:self forKeyPath:kKeyPathScannerState options:options context:nil];
    [self addObserver:self forKeyPath:kKeyPathScannerProgress options:options context:nil];
    
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    
    if([keyPath isEqualToString:kKeyPathScannerState]) {
        self.readyForScan = [self checkReadyForScan];
        self.readyForOverview = [self checkReadyForOverview];
        self.readyForCancel = [self checkReadyForCancel];
        self.scanResolutions = [self checkResolutions];
    } else if([keyPath isEqualToString:kKeyPathScannerProgress]) {
        self.scanProgressPercentDone = self.selectedScanner.selectedFunctionalUnit.scanProgressPercentDone;
    }
}

- (NSString *)windowNibName {
    return @"SCWWindow";
}

#pragma mark - ICScannerDeviceDelegate

- (void)device:(ICDevice *)device didCloseSessionWithError:(NSError *)error {
    
    self.selectedScanner = nil;
    [self.vendorFeaturesController removeObjects:self.vendorFeaturesController.arrangedObjects];
    
    if(error) {
        [self forwardErrorToExtractorsDelegate:error];
    }
    
}

- (void)device:(ICDevice *)device didOpenSessionWithError:(NSError *)error {
    
    ICScannerDevice *scanner = (ICScannerDevice *)device;
    ICScannerFunctionalUnit *funit = scanner.selectedFunctionalUnit;
    if(funit.type != ICScannerFunctionalUnitTypeFlatbed) {
        self.canvasController.contentView = nil;
    }
    if(error) {
        [self forwardErrorToExtractorsDelegate:error];
    }
}

- (void)didRemoveDevice:(ICDevice *)device {
    
    if(self.selectedScanner!=nil && [self.selectedScanner hasOpenSession]) {
        [self.selectedScanner requestCloseSession];
    }
    
}

- (void)deviceDidBecomeReady:(ICDevice *)device {
    [self renderVendorFeatures];
    [self resetScanAreas];
}


- (void)scannerDevice:(ICScannerDevice *)scanner didScanToBandData:(ICScannerBandData *)data {
    
    if(self.scanData==nil) {
        self.scanData = [NSMutableData data];
    }
    [self.scanData appendData:data.dataBuffer];
    self.lastBandData = data;
}

- (void)scannerDevice:(ICScannerDevice *)scanner didCompleteScanWithError:(NSError *)error {
    if(!error) {
        
        [self convertScanToImage];
        
        if(self.scanAreas.count > (self.currentArea+1)) {
            self.currentArea++;
            [self scanArea:scanner withIndex:self.currentArea];
        } else {
            [self forwardErrorToExtractorsDelegate:error];
            [self resetScanAreas];
        }
    }
}


- (void)scannerDevice:(ICScannerDevice *)scanner didCompleteOverviewScanWithError:(NSError *)error {
    
    if( !error ) {
        ICScannerFunctionalUnit *funit = scanner.selectedFunctionalUnit;
        CGFloat w = funit.physicalSize.width * funit.scaleFactor;
        CGFloat h = funit.physicalSize.height * funit.scaleFactor;
        NSSize size = NSMakeSize(w,h);
        NSImage *image = [[NSImage alloc] initWithCGImage:funit.overviewImage size:size];
        [self.canvasController.contentExtractor didExtractElement:image withError:nil];
    } else {
        [self forwardErrorToExtractorsDelegate:error];
    }
}

#pragma mark - ReadyStates & Properties

- (BOOL) scannerIsReadyAndNotBusy:(BOOL)postErrors {
    
    if(self.selectedScanner == nil) {
        if(postErrors) {
            NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : SCW_ERROR_DESC(SCWErrorNoScannerDeviceSelected) };
            NSError *error = [NSError errorWithDomain:SCWErrorDomain code:SCWErrorNoScannerDeviceSelected userInfo:errorDesc];
            [self.canvasController.contentExtractor didExtractElement:nil withError:error];
        }
        return NO;
    }
    
    ICScannerFunctionalUnit*  funit = self.selectedScanner.selectedFunctionalUnit;
    if(funit.state != ICScannerFunctionalUnitStateReady) {
        if(postErrors) {
            NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : SCW_ERROR_DESC(SCWErrorScannerDeviceNotReadyOrBusy) };
            NSError *error = [NSError errorWithDomain:SCWErrorDomain code:SCWErrorScannerDeviceNotReadyOrBusy userInfo:errorDesc];
            [self.canvasController.contentExtractor didExtractElement:nil withError:error];
        }
        return NO;
    }
    
    if(funit.scanInProgress || funit.overviewScanInProgress) {
        return NO;
    }
    
    return YES;
    
}

- (BOOL) checkReadyForOverview {
    
    BOOL retValue = YES;
    
    retValue = [self scannerIsReadyAndNotBusy:NO];
    
    if(retValue) {
        ICScannerFunctionalUnit*  funit = self.selectedScanner.selectedFunctionalUnit;
        return funit.canPerformOverviewScan;
    }
    
    return retValue;
    
}

- (BOOL) checkReadyForScan {
    BOOL retValue = YES;
    retValue = [self scannerIsReadyAndNotBusy:NO];
    return retValue;
}

- (BOOL) checkReadyForCancel {
    
    if(self.selectedScanner) {
        ICScannerFunctionalUnit *funit = self.selectedScanner.selectedFunctionalUnit;
        return (funit.scanInProgress || funit.overviewScanInProgress);
    }
    return NO;
}

- (NSArray *)checkResolutions {
    
    NSMutableArray *resolutions = [[NSMutableArray alloc] init];
    if(self.selectedScanner) {
        [self.selectedScanner.selectedFunctionalUnit.preferredResolutions enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
            [resolutions addObject:[NSNumber numberWithInteger:idx]];
        }];
        
    }
    return resolutions;
}


- (NSUInteger)scanColor {
    return [[self.colorRadio selectedCell] tag];
}

#pragma mark - NSTableViewDelegate




- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    
    NSTableCellView *cell = nil;
    
    if([tableColumn.identifier isEqualToString:@"NameColumn"]) {
        cell = [tableView makeViewWithIdentifier:@"NameColumnCell" owner:self];        
    } else if([tableColumn.identifier isEqualToString:@"ValueColumn"]) {
        
        ICScannerFeature *feature = [self.vendorFeaturesController.arrangedObjects objectAtIndex:row];
        if([feature isKindOfClass:[ICScannerFeatureBoolean class]]) {
            
            cell = [tableView makeViewWithIdentifier:@"ValueColumnBoolean" owner:self];
            
        } else if([feature isKindOfClass:[ICScannerFeatureEnumeration class]]) {
            cell = [tableView makeViewWithIdentifier:@"ValueColumnEnum" owner:self];
        }
        
        
    }
    
    
    return cell;
    
}

#pragma mark - ICDeviceBrowserDelegate

- (void)deviceBrowser:(ICDeviceBrowser*)browser didAddDevice:(ICDevice*)addedDevice moreComing:(BOOL)moreComing {
    if ( (addedDevice.type & ICDeviceTypeMaskScanner) == ICDeviceTypeScanner )
    {
        if(![self.scannerDevicesController.arrangedObjects containsObject:addedDevice]) {
            [self.scannerDevicesController addObject:addedDevice];
            if([self.scannerDevicesController.arrangedObjects count] == 1) {
                [self scannerPopUpChanged:nil];
            }
        }
    }
}

- (void)deviceBrowser:(ICDeviceBrowser*)browser didRemoveDevice:(ICDevice*)removedDevice moreGoing:(BOOL)moreGoing {
    [self.scannerDevicesController removeObject:removedDevice];
}

- (void)deviceBrowser:(ICDeviceBrowser*)browser deviceDidChangeName:(ICDevice*)device {
}


- (void)deviceBrowser:(ICDeviceBrowser*)browser deviceDidChangeSharingState:(ICDevice*)device {
}


- (void)deviceBrowser:(ICDeviceBrowser*)browser requestsSelectDevice:(ICDevice*)device {
}


#pragma mark - NSPopUpButton

- (IBAction)scannerPopUpChanged:(id)sender {
    
    
    
    if(self.scannerDevicesController.arrangedObjects > 0 && self.scannerPopup.indexOfSelectedItem >= 0) {
        
        ICDevice *device = (ICDevice*)[self.scannerDevicesController.arrangedObjects objectAtIndex:self.scannerPopup.indexOfSelectedItem];
        
        
        if( device != self.selectedScanner && [self.selectedScanner hasOpenSession]) {
            [self.selectedScanner requestCloseSession];
        }
        
        BOOL isScanner = (device.type & ICDeviceTypeScanner) == ICDeviceTypeScanner;
        
        if(isScanner) {
            
            ICScannerDevice *scanner = (ICScannerDevice*)device;
            
            ICScannerFunctionalUnit *funit = scanner.selectedFunctionalUnit;
            
            if ( funit.type != ICScannerFunctionalUnitTypeFlatbed
                && funit.type != ICScannerFunctionalUnitTypePositiveTransparency
                && funit.type != ICScannerFunctionalUnitTypeNegativeTransparency
                && funit.type != ICScannerFunctionalUnitTypeDocumentFeeder
                ) {
                NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : SCW_ERROR_DESC(SCWErrorScannerDeviceUnsupported) };
                NSError *error = [NSError errorWithDomain:SCWErrorDomain code:SCWErrorScannerDeviceUnsupported userInfo:errorDesc];
                [self.canvasController.contentExtractor didExtractElement:nil withError:error];

            } else {
                [scanner setDelegate:self];
                [self setSelectedScanner:scanner];
                [scanner requestOpenSession];
            }

            
            
        } else {
            
            NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : SCW_ERROR_DESC(SCWErrorNoScannerDeviceSelected) };
            NSError *error = [NSError errorWithDomain:SCWErrorDomain code:SCWErrorNoScannerDeviceSelected userInfo:errorDesc];
            [self.canvasController.contentExtractor didExtractElement:nil withError:error];
            
        }
    }
}


- (void) renderVendorFeatures {
    
    ICScannerFunctionalUnit *fu = self.selectedScanner.selectedFunctionalUnit;
    
    [self.vendorFeaturesController removeObjects:[self.vendorFeaturesController arrangedObjects]];
    
    
    for(id feature in fu.vendorFeatures) {
        if([feature isKindOfClass:[ICScannerFeatureBoolean class]]) {
            [self.vendorFeaturesController addObject:feature];
        } else if([feature isKindOfClass:[ICScannerFeatureEnumeration class]]) {
            [self.vendorFeaturesController addObject:feature];
        } else if([feature isKindOfClass:[ICScannerFeatureRange class]]) {
            // NOT SUPPORTED YET
        } else if([feature isKindOfClass:[ICScannerFeatureTemplate class]]) {
            // NOT SUPPORTED YET
        }
        
    }
}

#pragma mark - MSSExtractorDelegate

- (void)extractor:(MSSExtractor *)extractor extractElement:(id)element forJob:(NSString *)identifier {
    
    
    if ([element isKindOfClass:[NSImage class]]) {
        
        NSImage *image = (NSImage*)element;
        SCWScannedUnit *page = [SCWScannedUnit unitWithImage:image withResolution:self.scanResolution];
        
        if(identifier == MSSExtractorJobOverviewScan) {
            
            self.previewScan = page;
            [self.scannerView setImage:[self applyFilterOnImage:self.previewScan.image]];
            
        }
        else if(identifier == MSSExtractorJobImageScan) {
            
            [self.scannedPages addObject:page];
            NSImage *currentImage = [self applyFilterOnImage:page.image];
            [self.scanPreview setImage:currentImage];
            
            if(self.asPdf.state == NSOffState) {
                
                [self.delegate saveContentElements:[NSArray arrayWithObject:currentImage] withProperties:[self exportPropForPage:page]];
            }
            
        }
    }
}

- (void)extractor:(MSSExtractor *)extractor didCompleteWithError:(NSError *)error {
    if(!error) {
        
        if(extractor.elements.count > 0) {
            if(self.asPdf.state == NSOnState) {
                
                if(!self.canvasController.hasSelectors) {
                    
                    NSAlert *yesNoAlert = [[NSAlert alloc] init];
                    [yesNoAlert setMessageText:SCW_TRANSLATE(@"QUESTION_ANOTHER_PAGE")];
                    [yesNoAlert addButtonWithTitle:SCW_TRANSLATE(@"ANSWER_NO")];
                    [yesNoAlert addButtonWithTitle:SCW_TRANSLATE(@"ANSWER_YES")];
                    [yesNoAlert beginSheetModalForWindow:self.window completionHandler:^(NSModalResponse returnCode) {
                        if(returnCode == NSAlertSecondButtonReturn) {
                            
                            NSAlert *readyAlert = [[NSAlert alloc] init];
                            [readyAlert setMessageText:SCW_TRANSLATE(@"QUESTION_NEXT_DOC_READY")];
                            [readyAlert addButtonWithTitle:SCW_TRANSLATE(@"ANSWER_START")];
                            [readyAlert addButtonWithTitle:SCW_TRANSLATE(@"ANSWER_CHANGED_MIND")];
                            [readyAlert beginSheetModalForWindow:self.window completionHandler:^(NSModalResponse returnCode) {
                                if(returnCode == NSAlertFirstButtonReturn) {
                                    
                                    [self startScan:nil];
                                    return;
                                } else {
                                    [self finishSheet];
                                }
                            }];
                        } else {
                            [self finishSheet];
                        }
                    }];
                    
                } else {
                    [self finishSheet];
                }
                
            }
        }
        
    } else {
        [[NSAlert alertWithError:error] runModal];
    }
}

- (void) forwardErrorToExtractorsDelegate:(NSError*)error {
    [self.canvasController.contentExtractor.delegate extractor:self.canvasController.contentExtractor didCompleteWithError:error];
}


#pragma mark - Actions

/**
 @abstract Scans one or more areas with a given scanner
 @param scanner The scanner to scan with
 @param index The index of the area (in self.scanAreas) which should be scanned
 @discussion This method is responsible for the actual scanning process.
 It can scan a whole page or a special frame defined in the scan area.
 */
- (void) scanArea:(ICScannerDevice*)scanner withIndex:(NSInteger)index {
    
    self.scanData = nil;
    
    if([self scannerIsReadyAndNotBusy:YES]) {
        
        NSRect scanArea = NSZeroRect;
        
        CGFloat x,y,w,h;
        ICScannerFunctionalUnit *funit = scanner.selectedFunctionalUnit;
        funit.measurementUnit = ICScannerMeasurementUnitInches;
        
        if(self.scanAreas.count > index) {
            
            scanArea = [(NSValue*)(self.scanAreas)[index] rectValue];
            
            if(!NSEqualRects(scanArea, NSZeroRect) && funit.type == ICScannerFunctionalUnitTypeFlatbed) {
                
                
                funit = (ICScannerFunctionalUnitFlatbed*)scanner.selectedFunctionalUnit;
                NSSize viewSize = self.canvasController.view.frame.size;
                CGFloat viewRatioX = ((funit.physicalSize.width * funit.scaleFactor) / viewSize.width);
                CGFloat viewRatioY = ((funit.physicalSize.height * funit.scaleFactor) / viewSize.height);
                w = ( scanArea.size.width * viewRatioX ) / funit.scaleFactor;
                h = ( scanArea.size.height * viewRatioY ) / funit.scaleFactor;
                x = ( scanArea.origin.x * viewRatioX ) / funit.scaleFactor;
                CGFloat Yflipped = ((scanArea.origin.y + scanArea.size.height - viewSize.height) * -1);
                y = (Yflipped * viewRatioY) / funit.scaleFactor;
                
            } else {
                
                NSDictionary *errorDesc = @{ NSLocalizedDescriptionKey : SCW_ERROR_DESC(SCWErrorScannerDeviceNotFlatbed) };
                NSError *error = [NSError errorWithDomain:SCWErrorDomain code:SCWErrorScannerDeviceNotFlatbed userInfo:errorDesc];
                [self.canvasController.contentExtractor didExtractElement:nil withError:error];
                
                return;
            }
            
        } else {
            
            
            NSSize s;
            if ( funit.type == ICScannerFunctionalUnitTypeFlatbed ) {
                s = ((ICScannerFunctionalUnitFlatbed*)funit).physicalSize;
                ((ICScannerFunctionalUnitFlatbed*)funit).documentType = ICScannerDocumentTypeA4;
            }
            
            else if ( funit.type == ICScannerFunctionalUnitTypePositiveTransparency ) {
                s = ((ICScannerFunctionalUnitPositiveTransparency*)funit).physicalSize;
                ((ICScannerFunctionalUnitPositiveTransparency*)funit).documentType = ICScannerDocumentTypeA4;
            }
            
            else if ( funit.type == ICScannerFunctionalUnitTypeNegativeTransparency ) {
                s = ((ICScannerFunctionalUnitNegativeTransparency*)funit).physicalSize;
                ((ICScannerFunctionalUnitNegativeTransparency*)funit).documentType = ICScannerDocumentTypeA4;
            }
            
            else if ( funit.type == ICScannerFunctionalUnitTypeDocumentFeeder ) {
                s = ((ICScannerFunctionalUnitDocumentFeeder*)funit).physicalSize;
                ((ICScannerFunctionalUnitDocumentFeeder*)funit).documentType = ICScannerDocumentTypeA4;
            } else {
                return;
            }
            
            
            x = 0; y = 0;
            w = s.width;
            h = s.height;
            
        }
        
        
        funit.resolution = [self scanResolution];
        funit.bitDepth = [funit.supportedBitDepths indexGreaterThanOrEqualToIndex:(self.scanColor > 0 ? 8 : 1)];
        funit.pixelDataType = self.scanColor;
        funit.scanArea = NSMakeRect(x, y, w, h);
        scanner.transferMode = ICScannerTransferModeMemoryBased;
        [scanner requestScan];
        
        
        
    }
    
}


/** Resets the scanning areas */
- (void) resetScanAreas {
    
    if(!self.scanAreas) {
        self.scanAreas = [NSMutableArray array];
    }
    [self.scanAreas removeAllObjects];
    self.currentArea = 0;
    self.scanData = nil;
    
    
}

- (IBAction) improveImageState:(id)sender {
    
    if(self.previewScan) {
        [self.scannerView setImage:[self applyFilterOnImage:self.previewScan.image]];
    }
    
    SCWScannedUnit *currentScan = [self.scannedPages lastObject];
    if(currentScan) {
        [self.scanPreview setImage:[self applyFilterOnImage:currentScan.image]];
    }
    
}

- (IBAction)startScan:(id)sender {
    
    if([self scannerIsReadyAndNotBusy:YES]) {

        [self.canvasController.contentExtractor prepareJob:MSSExtractorJobImageScan];
        [self.canvasController.contentExtractor.elements removeAllObjects];
        [self resetScanAreas];
        
        ICScannerFunctionalUnit *funit = self.selectedScanner.selectedFunctionalUnit;
        
        if ( funit.type == ICScannerFunctionalUnitTypeFlatbed && self.canvasController.hasSelectors) {
            
            NSArray *selectors = self.canvasController.selectors;
            for(MSSSelector* selector in selectors) {
                [self.scanAreas addObject:[NSValue valueWithRect:selector.frame]];
            }
            
        }
        [self scanArea:self.selectedScanner withIndex:0];
        
    }
    
}


- (IBAction)overviewScan:(id)sender {
    
    if([self scannerIsReadyAndNotBusy:YES]) {
        [self.canvasController.contentExtractor prepareJob:MSSExtractorJobOverviewScan];
        ICScannerFunctionalUnit *fu = self.selectedScanner.selectedFunctionalUnit;
        fu.overviewResolution = [fu.supportedResolutions indexGreaterThanOrEqualToIndex:72];
        [self.selectedScanner requestOverviewScan];
    }
}

- (IBAction)cancelScan:(id)sender {

    if([self scannerIsReadyAndNotBusy:NO] == NO) {
        [self.selectedScanner cancelScan];
    }

}

- (void)closeSheet:(id)sender {
    [[NSApp keyWindow] endSheet:self.window];
    [self close];
}

- (void)close {
    [self.setupWindow close];
    [super close];
    [self.selectedScanner requestCloseSession];
}

- (IBAction)openSetup:(id)sender {
    [self.setupWindow makeKeyAndOrderFront:nil];

}


#pragma mark - Helper

- (NSImage*) applyFilterOnImage:(NSImage*)image {
    
    if(self.improveResult.state == NSOnState) {
        return image.contrastEnhanced;
    } else {
        return image;
    }
}

- (NSDictionary*)exportPropForPage:(SCWScannedUnit*)page {
    NSDictionary* properties = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSNumber numberWithInteger:page.resolution], kCGImagePropertyDPIHeight,
                                [NSNumber numberWithInteger:page.resolution], kCGImagePropertyDPIWidth,
                                nil];
    return properties;

}

- (void) finishSheet {
    if(self.asPdf.state == NSOnState) {
        [self performSelectorInBackground:@selector(makePdfFromPages) withObject:nil];
    } else {
        [self closeSheet:nil];
    }
}

- (void) savePdfAndClose:(PDFDocument*)pdf {
    [self.delegate saveContentElements:[NSArray arrayWithObject:pdf] withProperties:nil];
    [self closeSheet:nil];
}

- (void) convertScanToImage {
    
    if (self.selectedScanner) {
        
        ICScannerFunctionalUnit *funit = self.selectedScanner.selectedFunctionalUnit;
        NSData *profile = [NSData dataWithContentsOfFile:self.lastBandData.colorSyncProfilePath];
        CFDataRef colorData             = CFDataCreate(NULL, profile.bytes, profile.length);
        NSUInteger width                = self.lastBandData.fullImageWidth;
        NSUInteger height               = self.lastBandData.fullImageHeight;
        size_t bitsPerComponent         = self.lastBandData.bitsPerComponent;
        size_t bitsPerPixel             = self.lastBandData.bitsPerPixel;
        size_t bytesPerRow              = funit.pixelDataType == ICScannerPixelDataTypeGray ? self.lastBandData.fullImageWidth : self.lastBandData.bytesPerRow;
        CGColorSpaceRef colorSpace      = CGColorSpaceCreateWithICCProfile(colorData);
        CGBitmapInfo bitmapInfo         = kCGBitmapByteOrderDefault|kCGImageAlphaNone;
        CFDataRef imageData             = CFDataCreate(kCFAllocatorDefault, self.scanData.bytes, self.scanData.length);
        CGDataProviderRef dataProvider  = CGDataProviderCreateWithCFData(imageData);
        CGColorRenderingIntent rIntent  = kCGRenderingIntentDefault;
        
        CGImageRef cgImage = CGImageCreate(width, height, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpace, bitmapInfo, dataProvider, NULL, true, rIntent);
        
        NSImage *image = [[NSImage alloc] initWithCGImage:cgImage size:NSMakeSize(width, height)];
        
        [self.canvasController.contentExtractor didExtractElement:image withError:nil];
        
        CFRelease(colorData);
        CFRelease(imageData);
        CGColorSpaceRelease(colorSpace);
        CGDataProviderRelease(dataProvider);
        CGImageRelease(cgImage);
    }
}

#pragma mark - PDF Creation

- (void) makePdfFromPages {

    PDFDocument *pdf = [[PDFDocument alloc] init];
    NSUInteger nextPageIndex = 0;
    for(SCWScannedUnit *page in self.scannedPages) {
        NSImage *pdfImage = [[self applyFilterOnImage:page.image] compressedCopyWithQuality:0.6 withOtherProperties:[self exportPropForPage:page]];
        PDFPage *page = [[PDFPage alloc] initWithImage:pdfImage];
        [pdf insertPage:page atIndex:(nextPageIndex++)];
    }
    [self performSelectorOnMainThread:@selector(savePdfAndClose:) withObject:pdf waitUntilDone:YES];
}


@end




