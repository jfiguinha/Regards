

#import "AppDelegate.h"

@interface AppDelegate ()
@property (nonatomic,strong) SPWindowBundle *scanBundle;
@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

- (void) openScanSheet:(id)sender {
    
    NSURL *pluginUrl = [[[NSBundle mainBundle] builtInPlugInsURL] URLByAppendingPathComponent:@"SPScannerCaptureWindow.bundle"];
    
    NSBundle *scannerBundle = [NSBundle bundleWithURL:pluginUrl];
    
    id newBundleInst;
    Class bundlePrincipalClass = [scannerBundle principalClass];
    if (bundlePrincipalClass) {
        newBundleInst = [[bundlePrincipalClass alloc] init];
    }
    
    
    if(newBundleInst) {
        self.scanBundle = (SPWindowBundle *)newBundleInst;
        [self.scanBundle openWindowWithDelegate:self asSheet:YES];
    }
}

- (void)saveContentElements:(NSArray *)elements withProperties:(NSDictionary *)properties {
    
    NSLog(@"saveContentElements: Receive the scanned elements here!");
    
}


@end
