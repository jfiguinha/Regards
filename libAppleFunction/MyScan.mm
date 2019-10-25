#import <Foundation/Foundation.h>
#import "RegardsInclude.h"
#include "MyScanInterface.h"
#import <AppKit/AppKit.h>
#include "wx/osx/private.h"
#import "SCWBundle.h"

@interface SCWBundle ()
@property SPWindowBundle *scanBundle;
@end

@implementation SCWBundle


- (id)init {
    
    self = [super init];
    
    if (self) {
        id newBundleInst;
        Class bundlePrincipalClass = [SCWBundle principalClass];
        if (bundlePrincipalClass) {
            newBundleInst = [[bundlePrincipalClass alloc] init];
        }
        self.scanBundle = (SPWindowBundle *)newBundleInst;
    }
    
    return self;
}


- (BOOL) openWindow
 {
    
    if(!self.scannerCaptureWindow) {
        self.scannerCaptureWindow = [[SCWController alloc] init];
    }

    [self.scannerCaptureWindow showWindow:self];
    return YES;
}

@end

MyScannerImpl::MyScannerImpl( void )
{   
}

MyScannerImpl::~MyScannerImpl( void )
{
    
}


void MyScannerImpl::ScanDocument()
{
    SCWBundle * shareDelegate = [[SCWBundle alloc] init];
    [shareDelegate openWindow];
}  