//
//  MyObject.m
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//
#ifdef __APPLE__
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include "ToggleFullscreenImpl.h"
#include "wx/uri.h"

ToggleScreenImpl::ToggleScreenImpl( void )
{   }

ToggleScreenImpl::~ToggleScreenImpl( void )
{
}

void ToggleScreenImpl::GetFullscreenSize(int & width, int & height, int & top, int & left)
{
  // NSWindow *mainWindow = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
  // NSSize size = mainWindow.maxFullScreenContentSize;

   width = _widthMax;
   height = _heightMax;
   top = _top;
   left = _left;
}

void ToggleScreenImpl::ToggleFullscreen(wxFrame * frame, int screenNumber)
{
    NSView* myView = (frame->GetHandle() );
    
    NSRect f;
    //NSArray *screenArray = [NSScreen screens];
    //unsigned screenCount = [screenArray count];
    //unsigned index  = screenNumber;
   // NSSize size = myView.maxFullScreenContentSize;
    //NSScreen *screen = [screenArray objectAtIndex: index];
    f = [[NSScreen mainScreen] visibleFrame];

    
    if (myView.inFullScreenMode) {
      [myView exitFullScreenModeWithOptions:nil];
    } else {
        [myView.window toggleFullScreen:myView];
        /*
      NSApplicationPresentationOptions options =
          NSApplicationPresentationHideDock |       
          NSApplicationPresentationHideMenuBar;

      [myView enterFullScreenMode:[NSScreen mainScreen] withOptions:@{
             NSFullScreenModeApplicationPresentationOptions : @(options) }];

        //[myView enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];*/
    }
    
    NSSize rectInScreen = myView.window.maxFullScreenContentSize;
    printf("myView window : width : %f height : %f  \n",rectInScreen.width,rectInScreen.height );
    _widthMax = f.size.width;
    _heightMax = f.size.height ;
    _top = f.origin.x;
    _left = f.origin.y;
    printf("ToggleFullscreen : top : %f left : %f width : %f height : %f  \n", f.origin.x, f.origin.y, f.size.width,f.size.height );


}

#endif
