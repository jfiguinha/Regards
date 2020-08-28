///////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/private/available.h
// Purpose:     Helper for checking API availability under macOS.
// Author:      Vadim Zeitlin
// Created:     2019-04-17
// Copyright:   (c) 2019 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_OSX_PRIVATE_AVAILABLE_H_
#define _WX_OSX_PRIVATE_AVAILABLE_H_

// Xcode 9 adds new @available keyword and the corresponding __builtin_available
// builtin which should be used instead of manual checks for API availability
// as using this builtin suppresses the compiler -Wunguarded-availability
// warnings, so use it if possible for the implementation of our own macro.

    #include "wx/platinfo.h"

    #define WX_IS_MACOS_AVAILABLE(major, minor) \
        wxPlatformInfo::Get().CheckOSVersion(major, minor)

#ifdef wxOSX_USE_IPHONE
    #define WX_IS_MACOS_OR_IOS_AVAILABLE(macmajor, macminor, imajor, iminor) \
        wxPlatformInfo::Get().CheckOSVersion(imajor, iminor)
#else
    #define WX_IS_MACOS_OR_IOS_AVAILABLE(macmajor, macminor, imajor, iminor) \
        wxPlatformInfo::Get().CheckOSVersion(macmajor, macminor)
#endif

    #define WX_IS_MACOS_AVAILABLE_FULL(major, minor, micro) \
        wxPlatformInfo::Get().CheckOSVersion(major, minor, micro)

    #define WX_API_AVAILABLE_MACOS(major, minor)


#endif // _WX_OSX_PRIVATE_AVAILABLE_H_
