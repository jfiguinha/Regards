

#import <Foundation/Foundation.h>

@protocol ScannerCaptureWindowDelegate
- (void)saveContentElements:(NSArray *)elements withProperties:(NSDictionary*)properties;
@end


@interface SPWindowBundle : NSObject

- (BOOL) openWindowWithDelegate:(id<ScannerCaptureWindowDelegate>)delegate asSheet:(BOOL)sheet;

@end
