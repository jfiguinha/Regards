//
//  Test.cpp
//  Triangle
//
//  Created by Todd Vanderlin on 7/31/12.
//  Copyright (c) 2012 Interactive Design. All rights reserved.
//

#include "ofxScannerKit.h"
#include "event_scanner.h"
//#import <Quartz/Quartz.h>
#import <Cocoa/Cocoa.h>
#import <ImageCaptureCore/ImageCaptureCore.h>


static bool shouldRemoveDevice(ofxScannerDevice * d) {
    return d->bRemove;
}
//--------------------------------------------------------------
// Cocoa Interface
//--------------------------------------------------------------
@interface ScannerKit : NSObject <ICScannerDeviceDelegate, ICDeviceBrowserDelegate> {
    ICDeviceBrowser *   mDeviceBrowser;
    NSMutableArray  *   mScanners;
	NSString		*	downloadPath;
    ofxScannerKit    *   camKitRef;
}
- (void)                lookForScanners;
- (ofxScannerDevice*)	getScannerFromICDevice:(ICDevice*)device;

@property(retain) NSString * downloadPath;

@end;




@implementation ScannerKit 
@synthesize downloadPath;

//------------------------------------
- (id)init {
	self = [super init];
    if (self != nil) {
		camKitRef = NULL;
    }
    return self;
}

//------------------------------------
-(void) dealloc {
    [mScanners release];
    [mDeviceBrowser release];
    [downloadPath release];
    [super dealloc];    
}

//------------------------------------
-(void) setScannerKitRef:(ofxScannerKit*)camKit {
	if(camKit != NULL) {
		camKitRef = camKit;	
		//camKitRef->hackClose = true;
	}
}

// -------------------------------------------- 
// Match pointers and return the ref
// -------------------------------------------- 
-(ofxScannerDevice*) getScannerFromICDevice:(ICDevice*)device {
	if(camKitRef != NULL) {
		for(int i=0; i<camKitRef->devices.size(); i++) {
			if(camKitRef->devices[i]->device == device) {
				return camKitRef->devices[i]; 
			}
		}
	}
	else {
		return NULL;
	}
	
}

//------------------------------------
-(void) lookForScanners {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];  
	mScanners = [[NSMutableArray alloc] initWithCapacity:0];
	
    // Get an instance of ICDeviceBrowser
    mDeviceBrowser = [[ICDeviceBrowser alloc] init];
    
    // Assign a delegate
    mDeviceBrowser.delegate = self;
    
    mDeviceBrowser.browsedDeviceTypeMask = ICDeviceTypeMask(ICDeviceLocationTypeMaskLocal|ICDeviceLocationTypeMaskRemote|ICDeviceTypeMaskScanner);

	// set the download path
	NSString * resourcePath = [[NSBundle mainBundle] bundlePath];
	if(camKitRef != NULL) {
        NSString * folder = [[NSString alloc] initWithCString:camKitRef->downloadPath.c_str()];
        downloadPath = [[NSString alloc] initWithFormat:@"%@/../data/%@/", resourcePath, folder];
        [folder release];
    }
    else {
        downloadPath = [[NSString alloc] initWithFormat:@"%@/../data/", resourcePath];
    }
    
	
	printf("lookForScanners \n");
	// Start browsing for Scanners
	[mDeviceBrowser start];
    
	
	// clean up 
	[pool release];
	// [resourcePath release];
}

- (void)scannerDeviceDidBecomeAvailable:(ICScannerDevice*)scanner;
{
    [scanner requestOpenSession];
}

//------------------------------------
// Load the icon image for the device and
// store it in the c++ ofImage object
//------------------------------------
-(void) loadIconImage:(ofxScannerDevice*)Scanner device:(ICDevice*)device {
	
	//												
	// thanks memo for this help
	//
	
	CGContextRef spriteContext;
	CGImageRef	 cgImage = device.icon;
	
	int bytesPerPixel	= CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	int width			= CGImageGetWidth(cgImage);
	int height			= CGImageGetHeight(cgImage);
	
	// Allocated memory needed for the bitmap context
	GLubyte *pixels		= (GLubyte *) malloc(width * height * bytesPerPixel);
	
	// Uses the bitmatp creation function provided by the Core Graphics framework. 
	spriteContext = CGBitmapContextCreate(pixels, width, height, CGImageGetBitsPerComponent(cgImage), width * bytesPerPixel, CGImageGetColorSpace(cgImage), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	
	if(spriteContext == NULL) {
		free(pixels);
	}
	else {
		
		// After you create the context, you can draw the sprite image to the context.
		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), cgImage);
		
		// You don't need the context at this point, so you need to release it to avoid memory leaks.
		CGContextRelease(spriteContext);
        
        Scanner->icon = wxImage(width, height);
        int pointer = 0;
        
        unsigned char * alpha = nullptr;
        unsigned char *imgRGB = Scanner->icon.GetData();    // destination RGB buffer
        if(bytesPerPixel == 4)
        {
            alpha = new uint8_t(width * height);
            Scanner->icon.SetAlpha(alpha);
        }
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                 switch(bytesPerPixel) 
                 {
                    case 1:
                    {
                        uint8_t value = *pixels++;
                        imgRGB[pointer++] = value;
                        imgRGB[pointer++] = value;
                        imgRGB[pointer++] = value;   // B
                    }
                    break;
                    case 3:
                    {
                        imgRGB[pointer++] = *pixels++;
                        imgRGB[pointer++] = *pixels++;
                        imgRGB[pointer++] = *pixels++;
                    }
                    break;
                    default: //RGBA
                    {
                        imgRGB[pointer++] = *pixels++;
                        imgRGB[pointer++] = *pixels++;
                        imgRGB[pointer++] = *pixels++;  
                        *alpha++ = *pixels++;// B
                    }
                    break;
                }
            }
        }
		
		// vertically flip
		//	GLubyte *pixelsFlipped = (GLubyte *) malloc(width * height * bytesPerPixel);
		//	int numBytesPerRow = width * bytesPerPixel;
		//	for(int y=0; y<height; y++) {
		//		memcpy(pixelsFlipped + (numBytesPerRow * y), pixels + (numBytesPerRow * (height - 1 - y)), numBytesPerRow);
		//	}
		//	outImage.setFromPixels(pixelsFlipped, width, height, OF_IMAGE_COLOR_ALPHA, true);
		//	free(pixelsFlipped);
		
        //Copy image data to do
        
        /*
		int ofImageMode;
        
		switch(bytesPerPixel) {
			case 1:
				ofImageMode = OF_IMAGE_GRAYSCALE;
				break;
			case 3: 
				ofImageMode = OF_IMAGE_COLOR;
				break;
			case 4: 
			default:
				ofImageMode = OF_IMAGE_COLOR_ALPHA;
            break;
		}
		
		Scanner->icon.setFromPixels(pixels, width, height, ofImageMode, true);
		*/
		free(pixels);
		
	}
}



// -------------------------------------------- 
// a device was found and added
// -------------------------------------------- 
- (void)deviceBrowser:(ICDeviceBrowser*)browser didAddDevice:(ICDevice*)addedDevice moreComing:(BOOL)moreComing {
    // NSLog( @"deviceBrowser:didAddDevice:moreComing: \n%@\n", addedDevice );
	printf("deviceBrowser:didAddDevice:moreComing: \n");
    if ( (addedDevice.type & ICDeviceTypeMaskScanner) == ICDeviceTypeScanner ) {
        
        addedDevice.delegate = self;
		
		// make a new Scanner
        ofxScannerDevice * c = new ofxScannerDevice();
		
		// load the icon image
		// I think that there is a leak here...
		//[self loadIconImage:c device:addedDevice];
		
		// set the name and ICDevice pointer
		c->name   = [[addedDevice name] UTF8String];
		c->device = addedDevice; 
        
		// add to the Scanner kit vector
		camKitRef->devices.push_back(c);
		
		// update the num of Scanners
		//camKitRef->nScanners = camKitRef->Scanners.size();
		printf("Scanner added: %s\n", c->name.c_str());
		
		
		if(moreComing == false) 
        {
            wxCommandEvent event(EVENT_CK_ALL_CAMERAS_FOUND);
            if(camKitRef->parent != nullptr)
            {
                event.SetClientData(camKitRef);
                wxPostEvent(camKitRef->parent, event);
            }
                
			// printf("all Scanners found tell the app about it!\n");
            //static ofxDeviceEvent evt;
            //evt.eventType = ofxScannerKit::CK_ALL_ScannerS_FOUND;
			//ofNotifyEvent(camKitRef->deviceEvents, evt, camKitRef);
            
		}
		
		// This triggers KVO messages to AppController
		// to add the new Scanner object to the Scanners array.
		//[[self mutableArrayValueForKey:@"Scanners"] addObject:addedDevice];
    }
    
}

// -------------------------------------------- 
// Device was removed
// -------------------------------------------- 
- (void)deviceBrowser:(ICDeviceBrowser*)browser didRemoveDevice:(ICDevice*)device moreGoing:(BOOL)moreGoing; {
    
    if(camKitRef) {
        // try and find the Scanner in the c++ class
        for(int i=0; i<camKitRef->devices.size(); i++) {
            if(camKitRef->devices[i]->device == device) {
                
                printf("--- device removed: %s\n", camKitRef->devices[i]->name.c_str());
                
                camKitRef->devices[i]->isOpen  = false;
                camKitRef->devices[i]->isBusy  = false;
                camKitRef->devices[i]->bRemove = true;
                
                wxCommandEvent event(EVENT_CK_CAMERA_REMOVED);
                if(camKitRef->parent != nullptr)
                {
                    event.SetClientData(camKitRef);
                    wxPostEvent(camKitRef->parent, event);
                }
                
               // static ofxDeviceEvent evt;
                //evt.eventType = ofxScannerKit::CK_Scanner_REMOVED;
                //ofNotifyEvent(camKitRef->deviceEvents, evt, camKitRef);
                // printf("found the cam %p == %p\n", device, camKitRef->Scanners[i].cam);	
            }
        }
        
        
        // update the list of devices
        camKitRef->devices.erase(camKitRef->devices.begin(), partition(camKitRef->devices.begin(), camKitRef->devices.end(), shouldRemoveDevice));
    }
    
}
- (void)didRemoveDevice:(ICDevice*)removedDevice {
}

// -------------------------------------------- 
// Button Press
// -------------------------------------------- 
- (void)device:(ICDevice*)device didReceiveButtonPress:(NSString*)buttonType {
	NSLog(@"Device: %@\n", buttonType);	
}

// --------------------------------------------  
// Did open Scanner session
// and do we want to take a picture
// -------------------------------------------- 
- (void)device:(ICDevice*)device didOpenSessionWithError:(NSError*)error {
	
	// NSLog(@"Session Opened %@\n", [device name]);	
	
	// try and find the Scanner in the c++ class
	for(int i=0; i<camKitRef->devices.size(); i++) {
		
		
		if(camKitRef->devices[i]->device == device) {
			
            
			printf("--- Session opened: %s\n", camKitRef->devices[i]->name.c_str());
			
			camKitRef->devices[i]->isOpen = true;
			camKitRef->devices[i]->isBusy = false;
			
			if(camKitRef->devices[i]->bOpenThenTakePicture) {
				camKitRef->devices[i]->takePicture();
				camKitRef->devices[i]->bOpenThenTakePicture = false;
			}
			
			// printf("found the cam %p == %p\n", device, camKitRef->Scanners[i].cam);	
		}
	}
	
}

// --------------------------------------------  
// The Scanner session is now closed
// --------------------------------------------  
- (void)device:(ICDevice*)device didCloseSessionWithError:(NSError*)error {
	
	ofxScannerDevice * cam = [self getScannerFromICDevice:device];
	cam->isOpen = false;
	printf("--- Device session closed: %s\n", cam->name.c_str());	
	
}


// -------------------------------------------- 
- (void)deviceDidBecomeReady:(ICDevice*)device {
	printf("---------- :: deviceDidBecomeReady :: ----------\n");
    if (device.type & ICDeviceTypeScanner ) {
        
        ICScannerDevice * Scanner = (ICScannerDevice*)device;
        NSLog(@"%@", Scanner);
        
        ofxScannerDevice * device = [self getScannerFromICDevice:Scanner];
        //static ofxDeviceEvent evt;
        //evt.eventType = ofxScannerKit::CK_Scanner_READY;
        //evt.device    = [self getScannerFromICDevice:Scanner];
        //ofNotifyEvent(camKitRef->deviceEvents, evt, camKitRef);
        
        wxCommandEvent event(EVENT_CK_CAMERA_READY);
        if(camKitRef->parent != nullptr)
        {
            event.SetClientData(device);
            wxPostEvent(camKitRef->parent, event);
        }
        

/*
        
        if([[Scanner mediaFiles] count] > 0) {
            
            //NSLog(@"%@", [Scanner mediaFiles]);
            printf("--- There are %i photos on the Scanner ---\n", [[Scanner mediaFiles] count]);
			// printf("--- deleting the photos ---\n");
            
            [Scanner requestDeleteFiles:[Scanner mediaFiles]];
            
            if(camKitRef != NULL) {
                //camKitRef->status = "Deleting Photos"; 
            }
            
        }
*/
        
        
        
    }
}


@end


ScannerKit * camKit = NULL;
//--------------------------------------------------------------
// ofxScannerKit C++ Interface
//--------------------------------------------------------------
ofxScannerKit::ofxScannerKit() {
    camKit = NULL;   
}


void ofxScannerKit::listScanners() {
    
    printf("ofxScannerKit::listScanners \n");
    
    if(camKit == NULL) {
        camKit = [[ScannerKit alloc] init];
        [camKit setScannerKitRef:this];
        [camKit lookForScanners];
    }
}

void ofxScannerKit::setDownloadFolder(string path) {
	downloadPath = path;
	if(camKit != NULL) {
		if (camKit.downloadPath != nil) {
			[[camKit downloadPath] release];
			camKit.downloadPath = [[NSString alloc] initWithCString:downloadPath.c_str()];
			printf("Download path set for Scanner kit\n");
		}
	}
}


#pragma mark ofxScannerDevice
//--------------------------------------------------------------
// ofxScannerDevice C++ Interface
//--------------------------------------------------------------
void ofxScannerDevice::removeAllImages() {
    if(device != NULL) {
        ICScannerDevice * tempScanner = (ICScannerDevice*)device;
		if(tempScanner != NULL) {
            NSLog(@"does not work?!");
        }
    }
}

void ofxScannerDevice::openDevice() {
    printf("ofxScannerDevice::openDevice\n");
    if(device != NULL) {
		ICScannerDevice * tempScanner = (ICScannerDevice*)device;
		if(tempScanner != NULL && ![tempScanner hasOpenSession]) {
			[tempScanner requestOpenSession];
		}
	}
}
void ofxScannerDevice::closeDevice() {
    printf("ofxScannerDevice::closeDevice\n");
    if(device != NULL) {
		ICScannerDevice * tempScanner = (ICScannerDevice*)device;
		[tempScanner requestCloseSession];
	}
}
void ofxScannerDevice::takePicture() {
    //printf("--- Photo taken ---\n");
    ICScannerDevice * tempScanner = (ICScannerDevice*)device;
    [tempScanner requestTakePicture];
	
    
    if(device != NULL) {
		if (isBusy) {
			printf("--- %s Scanner is busy\n", name.c_str());
			return;
		}
		if (isOpen == true && isBusy == false) {
			isBusy = true;
			printf("--- Photo taken ---\n");
			ICScannerDevice * tempScanner = (ICScannerDevice*)device;
			[tempScanner requestTakePicture];
		}
		else {
			printf("need to open Scanner first\n");
			bOpenThenTakePicture = true;
			openDevice();
		}
	}
}

void ofxScannerDevice::enableTethering() {
    if(device != NULL) {
		//ICScannerDevice * tempScanner = (ICScannerDevice*)device;
        //[tempScanner requestEnableTethering];
    }
}
void ofxScannerDevice::disableTethering() {
    if(device != NULL) {
		//ICScannerDevice * tempScanner = (ICScannerDevice*)device;
        //[tempScanner requestDisableTethering];
    }
}

void ofxScannerDevice::StartOverviewScan() 
{
    printf("ofxScannerDevice::StartOverviewScan\n");
    if(device != NULL) 
    {
        ICScannerDevice*          scanner = (ICScannerDevice*)device;
        ICScannerFunctionalUnit*  fu      = scanner.selectedFunctionalUnit;
        
        if ( fu.canPerformOverviewScan && ( fu.scanInProgress == NO ) && ( fu.overviewScanInProgress == NO ) )
        {
            fu.overviewResolution = [fu.supportedResolutions indexGreaterThanOrEqualToIndex:72];
            [scanner requestOverviewScan];
            //[mProgressIndicator setHidden:NO];
        }
        else
            [scanner cancelScan];
    }
}


//------------------------------------------------------------------------------------------------------------ startOverviewScan

void ofxScannerDevice::StartScan() 
{
    printf("ofxScannerDevice::StartScan\n");
    if(device != NULL) 
    {
        ICScannerDevice*          scanner = (ICScannerDevice*)device;
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
            
            fu.resolution                   = [fu.supportedResolutions indexGreaterThanOrEqualToIndex:100];
            fu.bitDepth                     = ICScannerBitDepth8Bits;
            fu.pixelDataType                = ICScannerPixelDataTypeRGB;
            
            scanner.transferMode            = ICScannerTransferModeFileBased;
            //scanner.downloadsDirectory      = [NSURL fileURLWithPath:[@"~/Pictures" stringByExpandingTildeInPath]];
            scanner.downloadsDirectory      = [NSURL fileURLWithPath:[camKit.downloadPath stringByExpandingTildeInPath]];
            scanner.documentName            = @"Scan";
            scanner.documentUTI             = (id)kUTTypeJPEG;
            
            [scanner requestScan];
            //[mProgressIndicator setHidden:NO];
        }
        else
            [scanner cancelScan];
    }

}







