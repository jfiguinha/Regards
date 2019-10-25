

#import "AppDelegate.h"
#import "CaptureWindow.h"

@interface AppDelegate ()

@end


@implementation AppDelegate

@synthesize window;

- (void)awakeFromNib {
    
    
}


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    if (!window) {
        window = [[CaptureWindow alloc] initWithWindowNibName:@"CaptureWindow"];
    }
    [window showWindow:nil];
    
}



- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
