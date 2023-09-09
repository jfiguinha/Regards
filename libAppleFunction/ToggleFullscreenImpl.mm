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

void ToggleScreenImpl::GetFullscreenSize(int & width, int & height)
{
  // NSWindow *mainWindow = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
  // NSSize size = mainWindow.maxFullScreenContentSize;

   width = _widthMax;
   height = _heightMax;
}

void ToggleScreenImpl::ToggleFullscreen(wxFrame * frame, int screenNumber)
{
    NSView* myView = (frame->GetHandle() );
    
    NSRect screenRect;
    NSArray *screenArray = [NSScreen screens];
    unsigned screenCount = [screenArray count];
    unsigned index  = screenNumber;
   // NSSize size = myView.maxFullScreenContentSize;
    NSScreen *screen = [screenArray objectAtIndex: index];
    screenRect = [screen visibleFrame];

    
    if (myView.inFullScreenMode) {
      [myView exitFullScreenModeWithOptions:nil];
    } else {
      NSApplicationPresentationOptions options =
          NSApplicationPresentationHideDock |       
          NSApplicationPresentationHideMenuBar;

      /*[myView enterFullScreenMode:[NSScreen mainScreen] withOptions:@{
             NSFullScreenModeApplicationPresentationOptions : @(options) }];
        */
        [myView enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
    }
    
    NSRect rectInScreen = [myView.window convertRectToScreen:myView.frame];
    _widthMax = rectInScreen.origin.x;
    _heightMax = rectInScreen.origin.y;
}

#endif