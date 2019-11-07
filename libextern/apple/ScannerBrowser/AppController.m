/*
     File: AppController.m
 Abstract: Use the ImageCaptureCore framework to create a simple scanner application.
  Version: 1.1
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2012 Apple Inc. All Rights Reserved.
 
 */

#import "AppController.h"

//----------------------------------------------------------------------------------------------- CGImageRefToNSImageTransformer

@interface CGImageRefToNSImageTransformer: NSValueTransformer {}
@end

@implementation CGImageRefToNSImageTransformer

+ (Class)transformedValueClass { return [NSImage class]; }

+ (BOOL)allowsReverseTransformation { return NO; }

- (id)transformedValue:(id)item
{
    if ( item )
    {
        NSImage*  newImage  = nil;
        
        newImage = [[NSImage alloc] initWithCGImage:(CGImageRef)item size:NSZeroSize];
        
        return newImage;
    }
    else
        return nil;
}
@end

//------------------------------------------------------------------------------------------------------------- OpenControlTitle

@interface OpenControlTitle: NSValueTransformer {}
@end

@implementation OpenControlTitle

+ (Class)transformedValueClass { return [NSString class]; }

+ (BOOL)allowsReverseTransformation { return NO; }

- (id)transformedValue:(id)item
{
    NSString* out = @"Open";
    
    if ( item && [item isKindOfClass:[NSNumber class]] && [item intValue] )
        out = @"Close";

    return out;
}
@end

//--------------------------------------------------------------------------------------------------------- OpenStatusControlTitle

@interface OpenScanStatusValueTransformer : NSValueTransformer

@end

@implementation OpenScanStatusValueTransformer

+ (Class)transformedValueClass {
    return [NSNumber class];
}

+ (BOOL)allowsReverseTransformation {
    return NO;
}

- (id)transformedValue:(id)value {
    if (value == nil) return nil;
    if (![value isKindOfClass:[AppController class]]) return nil;

    AppController * controler = value;
    if (!controler.mOpenScanner) {
         return [NSNumber numberWithBool:NO];
    }
    return [NSNumber numberWithBool:YES];
}

@end

//--------------------------------------------------------------------------------------------------------- OpenStatusControlTitle

@interface OpenStatusValueTransformer : NSValueTransformer

@end

@implementation OpenStatusValueTransformer

+ (Class)transformedValueClass {
    return [NSNumber class];
}

+ (BOOL)allowsReverseTransformation {
    return NO;
}

- (id)transformedValue:(id)value {
    if (value == nil) return nil;
    if (![value isKindOfClass:[AppController class]]) return nil;

    AppController * controler = value;
    if (!controler.mOpenScanner) {
         return [NSNumber numberWithBool:NO];
    }
    return [NSNumber numberWithBool:YES];
}

@end


//--------------------------------------------------------------------------------------------------------- OverviewControlTitle

@interface OverviewControlTitle: NSValueTransformer {}
@end

@implementation OverviewControlTitle

+ (Class)transformedValueClass { return [NSString class]; }

+ (BOOL)allowsReverseTransformation { return NO; }

- (id)transformedValue:(id)item
{
    NSString* out = @"Overview";
    
    if ( item && [item isKindOfClass:[NSNumber class]] && [item intValue] )
        out = @"Cancel";
     
    return out;
}
@end

//------------------------------------------------------------------------------------------------------------- ScanControlTitle

@interface ScanControlTitle: NSValueTransformer {}
@end

@implementation ScanControlTitle

+ (Class)transformedValueClass { return [NSString class]; }

+ (BOOL)allowsReverseTransformation { return NO; }

- (id)transformedValue:(id)item
{
    NSString* out = @"Scan";
    
    if ( item && [item isKindOfClass:[NSNumber class]] && [item intValue] )
        out = @"Cancel";

    return out;
}
@end

//---------------------------------------------------------------------------------------------------------------- AppController

@implementation AppController

@synthesize scanColor;
@synthesize scanners = mScanners;

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
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

//------------------------------------------------------------------------------------------------------------------- initialize

+ (void)initialize
{
    CGImageRefToNSImageTransformer *imageTransformer = [[CGImageRefToNSImageTransformer alloc] init];
    [NSValueTransformer setValueTransformer:imageTransformer forName:@"NSImageFromCGImage"];
    
    [[NSRunningApplication currentApplication] activateWithOptions:(NSApplicationActivateAllWindows | NSApplicationActivateIgnoringOtherApps)];
}

//----------------------------------------------------------------------------------------------- applicationDidFinishLaunching:

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    mScanners = [[NSMutableArray alloc] initWithCapacity:0];
    [mScannersController setSelectsInsertedObjects:NO];

    mDeviceBrowser = [[ICDeviceBrowser alloc] init];
    mDeviceBrowser.delegate = self;
    mDeviceBrowser.browsedDeviceTypeMask = ICDeviceLocationTypeMaskLocal|ICDeviceLocationTypeMaskRemote|ICDeviceTypeMaskScanner;
    [mDeviceBrowser start];
    
    [mFunctionalUnitMenu removeAllItems];
    [mFunctionalUnitMenu setEnabled:NO];
}

//---------------------------------------------------------------------------------------------------- applicationWillTerminate:

- (void)applicationWillTerminate:(NSNotification*)notification
{
}

#pragma mark -
#pragma mark ICDeviceBrowser delegate methods
//------------------------------------------------------------------------------------------------------------------------------
// Please refer to the header files in ImageCaptureCore.framework for documentation about the following delegate methods.

//--------------------------------------------------------------------------------------- deviceBrowser:didAddDevice:moreComing:

- (void)deviceBrowser:(ICDeviceBrowser*)browser didAddDevice:(ICDevice*)addedDevice moreComing:(BOOL)moreComing
{
    NSLog( @"deviceBrowser:didAddDevice:moreComing: \n%@\n", addedDevice );
    
    if ( (addedDevice.type & ICDeviceTypeMaskScanner) == ICDeviceTypeScanner )
    {
        [self willChangeValueForKey:@"scanners"];
        [mScanners addObject:addedDevice];
        [self didChangeValueForKey:@"scanners"];
        addedDevice.delegate = self;
    }
}

//------------------------------------------------------------------------------------- deviceBrowser:didRemoveDevice:moreGoing:

- (void)deviceBrowser:(ICDeviceBrowser*)browser didRemoveDevice:(ICDevice*)removedDevice moreGoing:(BOOL)moreGoing;
{
    NSLog( @"deviceBrowser:didRemoveDevice: \n%@\n", removedDevice );
    [mScannersController removeObject:removedDevice];
}

//------------------------------------------------------------------------------------------- deviceBrowser:deviceDidChangeName:

- (void)deviceBrowser:(ICDeviceBrowser*)browser deviceDidChangeName:(ICDevice*)device;
{
    NSLog( @"deviceBrowser:\n%@\ndeviceDidChangeName: \n%@\n", browser, device );
}

//----------------------------------------------------------------------------------- deviceBrowser:deviceDidChangeSharingState:

- (void)deviceBrowser:(ICDeviceBrowser*)browser deviceDidChangeSharingState:(ICDevice*)device;
{
    NSLog( @"deviceBrowser:\n%@\ndeviceDidChangeSharingState: \n%@\n", browser, device );
}

//--------------------------------------------------------------------------------- deviceBrowser:didReceiveButtonPressOnDevice:

- (void)deviceBrowser:(ICDeviceBrowser*)browser requestsSelectDevice:(ICDevice*)device
{
    NSLog( @"deviceBrowser:\n%@\nrequestsSelectDevice: \n%@\n", browser, device );
}

#pragma mark -
#pragma mark ICDevice & ICScannerDevice delegate methods
//------------------------------------------------------------------------------------------------------------- didRemoveDevice:

- (void)didRemoveDevice:(ICDevice*)removedDevice
{
    NSLog( @"didRemoveDevice: \n%@\n", removedDevice );
    [mScannersController removeObject:removedDevice];
}

//---------------------------------------------------------------------------------------------- device:didOpenSessionWithError:

- (void)device:(ICDevice*)device didOpenSessionWithError:(NSError*)error
{
    NSLog( @"device:didOpenSessionWithError: \n" );
    NSLog( @"  device: %@\n", device );
    NSLog( @"  error : %@\n", error );
}

//-------------------------------------------------------------------------------------------------------- deviceDidBecomeReady:

- (void)deviceDidBecomeReady:(ICScannerDevice*)scanner
{
    NSArray*                    availabeTypes   = [scanner availableFunctionalUnitTypes];
    ICScannerFunctionalUnit*    functionalUnit  = scanner.selectedFunctionalUnit;
    _mOpenScanner = TRUE;
    NSLog( @"scannerDeviceDidBecomeReady: \n%@\n", scanner );
        
    [mFunctionalUnitMenu removeAllItems];
    [mFunctionalUnitMenu setEnabled:NO];
    
    if ( [availabeTypes count] )
    {
        NSMenu*     menu = [[NSMenu alloc] init];
        NSMenuItem* menuItem;
        
        [mFunctionalUnitMenu setEnabled:YES];
        for ( NSNumber* n in availabeTypes )
        {
            switch ( [n intValue] )
            {
                case ICScannerFunctionalUnitTypeFlatbed:
                    menuItem = [[NSMenuItem alloc] initWithTitle:@"Flatbed" action:@selector(selectFunctionalUnit:) keyEquivalent:@""];
                    [menuItem setTarget:self];
                    [menuItem setTag:ICScannerFunctionalUnitTypeFlatbed];
                    [menu addItem:menuItem];
                    break;
                case ICScannerFunctionalUnitTypePositiveTransparency:
                    menuItem = [[NSMenuItem alloc] initWithTitle:@"Postive Transparency" action:@selector(selectFunctionalUnit:) keyEquivalent:@""];
                    [menuItem setTarget:self];
                    [menuItem setTag:ICScannerFunctionalUnitTypePositiveTransparency];
                    [menu addItem:menuItem];
                    break;
                case ICScannerFunctionalUnitTypeNegativeTransparency:
                    menuItem = [[NSMenuItem alloc] initWithTitle:@"Negative Transparency" action:@selector(selectFunctionalUnit:) keyEquivalent:@""];
                    [menuItem setTarget:self];
                    [menuItem setTag:ICScannerFunctionalUnitTypeNegativeTransparency];
                    [menu addItem:menuItem];
                    break;
                case ICScannerFunctionalUnitTypeDocumentFeeder:
                    menuItem = [[NSMenuItem alloc] initWithTitle:@"Document Feeder" action:@selector(selectFunctionalUnit:) keyEquivalent:@""];
                    [menuItem setTarget:self];
                    [menuItem setTag:ICScannerFunctionalUnitTypeDocumentFeeder];
                    [menu addItem:menuItem];
                    break;
            }
        }
        
        NSMenu*     menuDPI = [[NSMenu alloc] init];
        
        _scanResolution = 75;
        [mScanResolutionMenu setEnabled:YES];
        
        //DPI Resolution
        NSMutableArray *resolutions = [[NSMutableArray alloc] init];
        if(self.selectedScanner) {
            [self.selectedScanner.selectedFunctionalUnit.preferredResolutions enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
                [resolutions addObject:[NSNumber numberWithInteger:idx]];
            }];
        }
        for ( NSNumber* n in resolutions )
        {
            NSMenuItem* menuItemDPI;
            NSString *myNumberInString = [n stringValue];
            menuItemDPI = [[NSMenuItem alloc] initWithTitle:myNumberInString action:@selector(selectResolutionDPI:) keyEquivalent:myNumberInString];
            [menuItemDPI setTarget:self];
            [menuItem setTag:n];
            [menuDPI addItem:menuItemDPI];
        }

        [mScanResolutionMenu setMenu:menuDPI];
        [mFunctionalUnitMenu setMenu:menu];
    }
    
    NSLog( @"observeValueForKeyPath - functionalUnit: %@\n", functionalUnit );
    
    if ( functionalUnit )
            [mFunctionalUnitMenu selectItemWithTag:functionalUnit.type];
    

    [mScanResolutionMenu selectItemWithTag:_scanResolution];
}

//--------------------------------------------------------------------------------------------- device:didCloseSessionWithError:

- (void)device:(ICDevice*)device didCloseSessionWithError:(NSError*)error
{
    NSLog( @"device:didCloseSessionWithError: \n" );
    NSLog( @"  device: %@\n", device );
    NSLog( @"  error : %@\n", error );
}

//--------------------------------------------------------------------------------------------------------- deviceDidChangeName:

- (void)deviceDidChangeName:(ICDevice*)device;
{
    NSLog( @"deviceDidChangeName: \n%@\n", device );
}

//------------------------------------------------------------------------------------------------- deviceDidChangeSharingState:

- (void)deviceDidChangeSharingState:(ICDevice*)device
{
    NSLog( @"deviceDidChangeSharingState: \n%@\n", device );
}

//------------------------------------------------------------------------------------------ device:didReceiveStatusInformation:

- (void)device:(ICDevice*)device didReceiveStatusInformation:(NSDictionary*)status
{
    NSLog( @"device: \n%@\ndidReceiveStatusInformation: \n%@\n", device, status );
    
    if ( [[status objectForKey:ICStatusNotificationKey] isEqualToString:ICScannerStatusWarmingUp] )
    {
        [mProgressIndicator setDisplayedWhenStopped:YES];
        [mProgressIndicator setIndeterminate:YES];
        [mProgressIndicator startAnimation:NULL];
        [mStatusText setStringValue:[status objectForKey:ICLocalizedStatusNotificationKey]];
    }
    else if ( [[status objectForKey:ICStatusNotificationKey] isEqualToString:ICScannerStatusWarmUpDone] )
    {
        [mStatusText setStringValue:@""];
        [mProgressIndicator stopAnimation:NULL];
        [mProgressIndicator setIndeterminate:NO];
        [mProgressIndicator setDisplayedWhenStopped:NO];
    }
}

//---------------------------------------------------------------------------------------------------- device:didEncounterError:

- (void)device:(ICDevice*)device didEncounterError:(NSError*)error
{
    NSLog( @"device: \n%@\ndidEncounterError: \n%@\n", device, error );
    
    NSBeginAlertSheet(
                  NULL,
                  @"OK", 
                  NULL, 
                  NULL, 
                  mWindow, 
                  NULL, 
                  NULL, 
                  NULL, 
                  NULL, 
                  [error localizedDescription],
                  NULL
              );
}

//----------------------------------------------------------------------------------------- scannerDevice:didReceiveButtonPress:

- (void)device:(ICDevice*)device didReceiveButtonPress:(NSString*)button
{
    NSLog( @"device: \n%@\ndidReceiveButtonPress: \n%@\n", device, button );
}

//--------------------------------------------------------------------------------------------- scannerDeviceDidBecomeAvailable:

- (void)scannerDeviceDidBecomeAvailable:(ICScannerDevice*)scanner;
{
    NSLog( @"scannerDeviceDidBecomeAvailable: \n%@\n", scanner );
    [scanner requestOpenSession];
}

//--------------------------------------------------------------------------------- scannerDevice:didSelectFunctionalUnit:error:

- (void)scannerDevice:(ICScannerDevice*)scanner didSelectFunctionalUnit:(ICScannerFunctionalUnit*)functionalUnit error:(NSError*)error
{
    NSLog( @"scannerDevice:didSelectFunctionalUnit:error:contextInfo:\n" );
    NSLog( @"  scanner:        %@:\n", scanner );
    NSLog( @"  functionalUnit: %@:\n", functionalUnit );
    NSLog( @"  error:          %@\n", error );
}

//--------------------------------------------------------------------------------------------- scannerDevice:didScanToURL:data:

- (void)scannerDevice:(ICScannerDevice*)scanner didScanToURL:(NSURL*)url data:(NSData*)data
{
    NSLog( @"scannerDevice:didScanToURL:data: \n" );
    NSLog( @"  scanner: %@", scanner );
    NSLog( @"  url:     %@", url );
    NSLog( @"  data:    %p\n", data );
}

//------------------------------------------------------------------------------ scannerDevice:didCompleteOverviewScanWithError:

- (void)scannerDevice:(ICScannerDevice*)scanner didCompleteOverviewScanWithError:(NSError*)error;
{
    NSLog( @"scannerDevice: \n%@\ndidCompleteOverviewScanWithError: \n%@\n", scanner, error );
    [mProgressIndicator setHidden:YES];
}

//-------------------------------------------------------------------------------------- scannerDevice:didCompleteScanWithError:

- (void)scannerDevice:(ICScannerDevice*)scanner didCompleteScanWithError:(NSError*)error;
{
    NSLog( @"scannerDevice: \n%@\ndidCompleteScanWithError: \n%@\n", scanner, error );
    [mProgressIndicator setHidden:YES];
    //Fin de l'application
    //Fermeture de la session
     [[self selectedScanner] requestCloseSession];
    [[NSApplication sharedApplication] terminate:nil];
}

#pragma mark -
//------------------------------------------------------------------------------------------------------------ openCloseSession:

- (IBAction)openCloseSession:(id)sender
{
    //[self.setupWindow makeKeyAndOrderFront:nil];
    [sender setTitle:@"Close"];
    _mOpenScanner = TRUE;
    if ( [self selectedScanner].hasOpenSession )
        [[self selectedScanner] requestCloseSession];
    else
        [[self selectedScanner] requestOpenSession];
}

- (IBAction)openSetup:(id)sender {
     [self.setupWindow makeKeyAndOrderFront:nil];

}

//-------------------------------------------------------------------------------------------------------- selectFunctionalUnit:

- (IBAction)selectFunctionalUnit:(id)sender
{
    NSString*                   titleOfSelectedItem = [sender title];
    ICScannerFunctionalUnitType selectedType        = [sender tag];
    ICScannerDevice*            scanner             = [self selectedScanner];
    
    NSLog( @"titleOfSelectedItem: %@, selectedType: %ld\n", titleOfSelectedItem, selectedType );
    
    if ( sender && ( selectedType != scanner.selectedFunctionalUnit.type ) )
        [scanner requestSelectFunctionalUnit:[sender tag]];
}

- (IBAction)selectResolutionDPI:(id)sender
{
    NSString*                   titleOfSelectedItem = [sender title];
    _scanResolution = [titleOfSelectedItem intValue];
    //ICScannerFunctionalUnitType selectedType        = [sender tag];
    //ICScannerDevice*            scanner             = [self selectedScanner];
    
   // NSLog( @"titleOfSelectedItem: %@, selectedType: %ld\n", titleOfSelectedItem, selectedType );
    
   // if ( sender && ( selectedType != scanner.selectedFunctionalUnit.type ) )
   //     [scanner requestSelectFunctionalUnit:[sender tag]];
}


//-------------------------------------------------------------------------------------------------------------- selectedScanner

- (ICScannerDevice*)selectedScanner
{
    ICScannerDevice*  device          = NULL;
    id                selectedObjects = [mScannersController selectedObjects];
    
    if ( [selectedObjects count] )
        device = [selectedObjects objectAtIndex:0];
        
    return device;
}

//------------------------------------------------------------------------------------------------------------ startOverviewScan

- (IBAction)startOverviewScan:(id)sender
{
    ICScannerDevice*          scanner = [self selectedScanner];
    ICScannerFunctionalUnit*  fu      = scanner.selectedFunctionalUnit;
    
    if ( fu.canPerformOverviewScan && ( fu.scanInProgress == NO ) && ( fu.overviewScanInProgress == NO ) )
    {
        fu.overviewResolution = [fu.supportedResolutions indexGreaterThanOrEqualToIndex:72];
        [scanner requestOverviewScan];
        [mProgressIndicator setHidden:NO];
    }
    else
        [scanner cancelScan];
}

//------------------------------------------------------------------------------------------------------------ startOverviewScan

- (IBAction)startScan:(id)sender
{
    ICScannerDevice*          scanner = [self selectedScanner];
    ICScannerFunctionalUnit*  fu      = scanner.selectedFunctionalUnit;
   
    if ( ( fu.scanInProgress == NO ) && ( fu.overviewScanInProgress == NO ) )
    {
        if ( fu.type == ICScannerFunctionalUnitTypeDocumentFeeder )
        {
            ICScannerFunctionalUnitDocumentFeeder* dfu = (ICScannerFunctionalUnitDocumentFeeder*)fu;
            
            dfu.documentType  = ICScannerDocumentTypeUSLetter;
        }
        else
        {
            NSSize s;
            
            fu.measurementUnit  = ICScannerMeasurementUnitInches;
            if ( fu.type == ICScannerFunctionalUnitTypeFlatbed )
                s = ((ICScannerFunctionalUnitFlatbed*)fu).physicalSize;
            else if ( fu.type == ICScannerFunctionalUnitTypePositiveTransparency )
                s = ((ICScannerFunctionalUnitPositiveTransparency*)fu).physicalSize;
            else
                s = ((ICScannerFunctionalUnitNegativeTransparency*)fu).physicalSize;
            fu.scanArea         = NSMakeRect( 0.0, 0.0, s.width, s.height );
        }
        
        //NSString * documentName = [NSString stringWithFormat:@"Scan_%@",
        //[@(_scanResolution) stringValue]];
        fu.resolution                   = _scanResolution;
        //[fu.supportedResolutions indexGreaterThanOrEqualToIndex:100];
        fu.bitDepth = [fu.supportedBitDepths indexGreaterThanOrEqualToIndex:(self.scanColor > 0 ? 8 : 1)];
        fu.pixelDataType                = self.scanColor;
        //fu.bitDepth                     = ICScannerBitDepth8Bits;
        //fu.pixelDataType                = ICScannerPixelDataTypeRGB;
        scanner.transferMode            = ICScannerTransferModeFileBased;
        scanner.downloadsDirectory      = [NSURL fileURLWithPath:[@"~/Documents/Regards/temp" stringByExpandingTildeInPath]];
        scanner.documentName            = @"Scan";
        scanner.documentUTI             = (id)kUTTypePDF;
        
        [scanner requestScan];
        [mProgressIndicator setHidden:NO];
    }
    else
        [scanner cancelScan];
}

//------------------------------------------------------------------------------------------------------------------------------

@end

//------------------------------------------------------------------------------------------------------------------------------

