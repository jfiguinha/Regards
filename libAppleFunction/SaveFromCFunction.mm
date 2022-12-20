//
//  SaveFromCFunction.cpp
//  TestDialog
//
//  Created by Philip Schneider on 1/16/15.
//  Copyright (c) 2015 Code From Above, LLC. All rights reserved.
//

#include "SaveFromCFunction.h"
#import <AppKit/NSSavePanel.h>
#import <AppKit/NSPopUpButton.h>
#import <AppKit/NSTextField.h>


std::string saveFileProgrammaticVersion(std::string fileextension)
{
    NSString * extension = [NSString stringWithCString:fileextension.c_str() encoding:[NSString defaultCStringEncoding]];
    NSArray *fileTypesArray = [NSArray arrayWithObjects:extension, nil];

    NSSavePanel * savePanel = [NSSavePanel savePanel];
    
    [savePanel setAllowedFileTypes:fileTypesArray];
    [savePanel setTitle:@"Save Image"];
    [savePanel setAllowsOtherFileTypes:NO];
    [savePanel setExtensionHidden:YES];
    [savePanel setCanCreateDirectories:YES];
    
    if ([savePanel runModal] == NSFileHandlingPanelOKButton)
    {
        NSURL *URL = [savePanel URL];
        if (URL)
        {
            NSString *path = [URL path];
            return std::string([path UTF8String]);
        }
    }
    
    return std::string();
}

