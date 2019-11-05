//
//  Test.h
//  Triangle
//
//  Created by Todd Vanderlin on 7/31/12.
//  Copyright (c) 2012 Interactive Design. All rights reserved.
//

#pragma once
//#include "ofMain.h"
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
using namespace std;

class ofxScannerDevice {

    public:
    string  name;
    wxImage icon;
    bool    isOpen, isBusy;
    bool    bOpenThenTakePicture;
    bool    bRemove;
    void *  device;
    
    ofxScannerDevice() {

        device  = NULL;
        isOpen  = false;
        bRemove = false;
        isBusy  = false;
        bOpenThenTakePicture = false;
    }
    
    void enableTethering();
    void disableTethering();
    void takePicture();
    void openDevice();
    void closeDevice();
    void removeAllImages();
    void StartScan();
    void StartOverviewScan();
};

class ofxPhotoDownloaded {
public:
	
	string		photoPath;
	string		photoName;
};


class ofxScannerKit {
    
public:

    //--------------------------------------------------------------
    // ofxScannerKit C++ Interface
    //--------------------------------------------------------------
    ofxScannerKit();

    void listScanners();
    void setDownloadFolder(string path);
    
    string downloadPath;
    vector  <ofxScannerDevice*> devices;
    wxWindow * parent;

};