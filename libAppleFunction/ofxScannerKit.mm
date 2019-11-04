//
//  Test.cpp
//  Triangle
//
//  Created by Todd Vanderlin on 7/31/12.
//  Copyright (c) 2012 Interactive Design. All rights reserved.
//

#include "ofxScannerKit.h"
#include "event_scanner.h"
#import <Cocoa/Cocoa.h>
#import <ImageCaptureCore/ImageCaptureCore.h>
#import <ImageKit/IKScannerView.h>

static bool shouldRemoveDevice(ofxScannerDevice * d) {
    return d->bRemove;
}
//--------------------------------------------------------------
// Cocoa Interface
//--------------------------------------------------------------
@interface ScannerKit : NSObject <IKScannerDeviceViewDelegate, ICDeviceBrowserDelegate, ICScannerDeviceDelegate> {
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
    
    ICDeviceBrowser *mDeviceBrowser = [[ICDeviceBrowser alloc] init];
    mDeviceBrowser.delegate = self;
    mDeviceBrowser.browsedDeviceTypeMask = ICDeviceLocationTypeMaskLocal|ICDeviceLocationTypeMaskRemote|ICDeviceTypeMaskScanner;
    [mDeviceBrowser start];
}

//------------------------------------
// Load the icon image for the device and
// store it in the c++ ofImage object
//------------------------------------
-(void) loadIconImage:(ofxScannerDevice*)camera device:(ICDevice*)device {
	
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
        
        camera->icon = wxImage(width, height);
        int pointer = 0;
        
        unsigned char * alpha = nullptr;
        unsigned char *imgRGB = camera->icon.GetData();    // destination RGB buffer
        if(bytesPerPixel == 4)
        {
            alpha = new uint8_t(width * height);
            camera->icon.SetAlpha(alpha);
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
		free(pixels);
		
	}
}

// -------------------------------------------- 
// a device was found and added
// -------------------------------------------- 
- (void)deviceBrowser:(ICDeviceBrowser*)browser didAddDevice:(ICDevice*)addedDevice moreComing:(BOOL)moreComing {
    // NSLog( @"deviceBrowser:didAddDevice:moreComing: \n%@\n", addedDevice );
	
     if ( (addedDevice.type & ICDeviceTypeMaskScanner) == ICDeviceTypeScanner )
	 {
        
		 [scannerView setScannerDevice:(ICScannerDevice*)addedDevice];
    }
    
}

- (void)scannerDeviceDidBecomeAvailable:(ICScannerDevice*)scanner;
{
    [scanner requestOpenSession];
}

// -------------------------------------------- 
// Device was removed
// -------------------------------------------- 
- (void)deviceBrowser:(ICDeviceBrowser*)browser didRemoveDevice:(ICDevice*)device moreGoing:(BOOL)moreGoing; {
    
    if(camKitRef) {
        // try and find the camera in the c++ class
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
                //evt.eventType = ofxScannerKit::CK_CAMERA_REMOVED;
                //ofNotifyEvent(camKitRef->deviceEvents, evt, camKitRef);
                // printf("found the cam %p == %p\n", device, camKitRef->cameras[i].cam);	
            }
        }
        
        
        // update the list of devices
        camKitRef->devices.erase(camKitRef->devices.begin(), partition(camKitRef->devices.begin(), camKitRef->devices.end(), shouldRemoveDevice));
    }
    
}
-(void)didRemoveDevice:(ICDevice*)removedDevice
{
    [removedDevice requestCloseSession];
}

// -------------------------------------------- 
// Button Press
// -------------------------------------------- 
- (void)device:(ICDevice*)device didReceiveButtonPress:(NSString*)buttonType {
	NSLog(@"Device: %@\n", buttonType);	
}

// --------------------------------------------  
// Did open camera session
// and do we want to take a picture
// -------------------------------------------- 
- (void)device:(ICDevice*)device didOpenSessionWithError:(NSError*)error {
	
	// NSLog(@"Session Opened %@\n", [device name]);	
	
	// try and find the camera in the c++ class
	for(int i=0; i<camKitRef->devices.size(); i++) {
		
		
		if(camKitRef->devices[i]->device == device) {
			
            
			printf("--- Session opened: %s\n", camKitRef->devices[i]->name.c_str());
			
			camKitRef->devices[i]->isOpen = true;
			camKitRef->devices[i]->isBusy = false;
			
			if(camKitRef->devices[i]->bOpenThenTakePicture) {
				camKitRef->devices[i]->takePicture();
				camKitRef->devices[i]->bOpenThenTakePicture = false;
			}
			
			// printf("found the cam %p == %p\n", device, camKitRef->cameras[i].cam);	
		}
	}
	
}

// --------------------------------------------  
// The camera session is now closed
// --------------------------------------------  
- (void)device:(ICDevice*)device didCloseSessionWithError:(NSError*)error {
	
	ofxScannerDevice * cam = [self getScannerFromICDevice:device];
	cam->isOpen = false;
	printf("--- Device session closed: %s\n", cam->name.c_str());	
	
}

// -------------------------------------------- 
// Photo done download from camera
// -------------------------------------------- 
- (void)didDownloadFile:(ICScannerFile*)file error:(NSError*)error options:(NSDictionary*)options contextInfo:(void*)contextInfo {
    NSLog( @"didDownloadFile called with:\n" );
    NSLog( @"  file:        %@\n", file );
    NSLog( @"  error:       %@\n", error );
    NSLog( @"  options:     %@\n", options );
    NSLog( @"  contextInfo: %p\n", contextInfo );
    
    
    /*
     //[file.device requestDeleteFiles:file.device.mediaFiles];
     Scanner * cam = [self getScannerFromICDevice:file.device];
     cam->isBusy = false;
     printf("--- [%s] File Done Downloading ---\n", cam->name.c_str());
     
     //NSLog( @"  file:        %@\n", [file parentFolder]);
     //NSLog( @"  options:     %@\n", options );
     
     if(fileName != nil) {
     cam->photos.push_back([fileName cString]);
     }
     */
	NSString * fileName = file.name;
	NSString * path = [[NSString alloc] initWithFormat:@"%@/%@", downloadPath, fileName];
    
    ofxPhotoDownloaded * photoDown = new ofxPhotoDownloaded();
  	photoDown->photoPath = [path UTF8String];
	photoDown->photoName = [fileName UTF8String];  
    /*
	static ofxDeviceEvent evt;
	evt.eventType = ofxScannerKit::CK_PHOTO_DOWNLOADED;
	evt.photoPath = [path UTF8String];
	evt.photoName = [fileName UTF8String];
	ofNotifyEvent(camKitRef->deviceEvents, evt, camKitRef);
    */
    wxCommandEvent event(EVENT_CK_PHOTO_DOWNLOADED);
    if(camKitRef->parent != nullptr)
    {
        event.SetClientData(photoDown);
        wxPostEvent(camKitRef->parent, event);
    }
	
}


// -------------------------------------------- 
// Item added (like a photo)
// -------------------------------------------- 
- (void)cameraDevice:(ICScannerDevice*)camera didAddItem:(ICScannerItem*)item {
	
	if(downloadPath == NULL) return;
	if(camKitRef == NULL)	 return;
	//if(camKitRef->hackClose) {
	//	printf("need to press open first ** Hack **\n");
	//	return;
	//}
	
	
	//if([ICScannerFolder orientationOverridden] == NULL) return;
	printf("---------- :: Add Item :: ----------\n");
    
	ICScannerFile * file = (ICScannerFile*)item;	
    NSLog(@"UTI: %@", file.UTI);
    if([file.UTI isEqualToString:@"public.folder"] == NO) {
        NSMutableDictionary* options = [[NSMutableDictionary alloc] initWithObjectsAndKeys:[NSURL fileURLWithPath:[downloadPath stringByExpandingTildeInPath]], ICDownloadsDirectoryURL, nil];
        
        //NSMutableDictionary* options = [[NSMutableDictionary alloc] initWithObjectsAndKeys:[NSURL fileURLWithPath:[@"~/Desktop/temp/" stringByExpandingTildeInPath]], ICDownloadsDirectoryURL, nil];
        [file.device requestDownloadFile:file options:options downloadDelegate:self didDownloadSelector:@selector(didDownloadFile:error:options:contextInfo:) contextInfo:NULL];
        
    }
    //NSMutableDictionary* options = [[NSMutableDictionary alloc] initWithObjectsAndKeys:[NSURL fileURLWithPath:[downloadPath stringByExpandingTildeInPath]], ICDownloadsDirectoryURL, nil];
	//[options setObject:[NSNumber numberWithBool:YES] forKey:ICDeleteAfterSuccessfulDownload];
	//[options setObject:[NSNumber numberWithBool:NO] forKey:@"orientationOverridden"];
    
    //	
    //	[file.device requestDownloadFile:file
    //							 options:options 
    //					downloadDelegate:self 
    //				 didDownloadSelector:@selector(didDownloadFile:error:options:contextInfo:) 
    //						 contextInfo:NULL];
    //    
    //    [options release];
    //	
	
}

// -------------------------------------------- 
- (void)deviceDidBecomeReady:(ICDevice*)device {
	printf("---------- :: deviceDidBecomeReady :: ----------\n");
    if (device.type & ICDeviceTypeScanner ) {
        
        ICScannerDevice * camera = (ICScannerDevice*)device;
        NSLog(@"%@", camera);
        
        ofxScannerDevice * device = [self getScannerFromICDevice:camera];
        //static ofxDeviceEvent evt;
        //evt.eventType = ofxScannerKit::CK_CAMERA_READY;
        //evt.device    = [self getScannerFromICDevice:camera];
        //ofNotifyEvent(camKitRef->deviceEvents, evt, camKitRef);
        
        wxCommandEvent event(EVENT_CK_CAMERA_READY);
        if(camKitRef->parent != nullptr)
        {
            event.SetClientData(device);
            wxPostEvent(camKitRef->parent, event);
        }
        

        
        
        if([[camera mediaFiles] count] > 0) {
            
            //NSLog(@"%@", [camera mediaFiles]);
            printf("--- There are %i photos on the camera ---\n", [[camera mediaFiles] count]);
			// printf("--- deleting the photos ---\n");
            
            [camera requestDeleteFiles:[camera mediaFiles]];
            
            if(camKitRef != NULL) {
                //camKitRef->status = "Deleting Photos"; 
            }
            
        }
        
        
        
        
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
			printf("Download path set for camera kit\n");
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
    if(device != NULL) {
		ICScannerDevice * tempScanner = (ICScannerDevice*)device;
		if(tempScanner != NULL && ![tempScanner hasOpenSession]) {
			[tempScanner requestOpenSession];
		}
	}
}
void ofxScannerDevice::closeDevice() {
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
			printf("--- %s camera is busy\n", name.c_str());
			return;
		}
		if (isOpen == true && isBusy == false) {
			isBusy = true;
			printf("--- Photo taken ---\n");
			ICScannerDevice * tempScanner = (ICScannerDevice*)device;
			[tempScanner requestTakePicture];
		}
		else {
			printf("need to open camera frist\n");
			bOpenThenTakePicture = true;
			openDevice();
		}
	}
}
void ofxScannerDevice::enableTethering() {
    if(device != NULL) {
		ICScannerDevice * tempScanner = (ICScannerDevice*)device;
        [tempScanner requestEnableTethering];
    }
}
void ofxScannerDevice::disableTethering() {
    if(device != NULL) {
		ICScannerDevice * tempScanner = (ICScannerDevice*)device;
        [tempScanner requestDisableTethering];
    }
}








