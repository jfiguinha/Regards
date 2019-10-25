/*
 
 ------------------------------------------------------------------------------------
 SCWError.h
 ------------------------------------------------------------------------------------
 
 The MIT License (MIT)
 
 Copyright (c) 2014 Sascha Paulus
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 */


#import <Foundation/Foundation.h>

#define SCW_ERROR_KEY(code) [NSString stringWithFormat:@"%d",code]
#define SCW_ERROR_DESC(code) NSLocalizedStringFromTableInBundle(SCW_ERROR_KEY(code), @"SCWError", [NSBundle bundleForClass:[self class]], nil)
#define SCW_TRANSLATE(key) NSLocalizedStringFromTableInBundle(key, @"Localizable", [NSBundle bundleForClass:[self class]], nil)


FOUNDATION_EXPORT NSString *const SCWErrorDomain;

enum {
    /** Error if no scanner is selected */
    SCWErrorNoScannerDeviceSelected = 1000,
    /** Error if selector is not type of flatbed */
    SCWErrorScannerDeviceNotFlatbed,
    /** Error if scanner is not ready or busy */
    SCWErrorScannerDeviceNotReadyOrBusy,
    /** Error if scanner not able to make overview scans */
    SCWErrorScannerDeviceNotOverviewScanReady,
    /** Error if scaning process was interrupted */
    SCWErrorScannerDeviceScanningInterrupted,
    /** Error if scanner device is not supported */
    SCWErrorScannerDeviceUnsupported,
};
