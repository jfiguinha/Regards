//
//  RegardsInclude.h
//  Regards.libPicture
//
//  Created by figuinha jacques on 11/08/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

inline NSString* wxNSStringWithWxString(const wxString &wxstring)
{
#if wxUSE_UNICODE
    return [NSString stringWithUTF8String: wxstring.mb_str(wxConvUTF8)];
#else
    return [NSString stringWithCString: wxstring.c_str() length:wxstring.Len()];
#endif // wxUSE_UNICODE
}

inline wxString wxStringWithNSString(NSString *nsstring)
{
#if wxUSE_UNICODE
    return wxString([nsstring UTF8String], wxConvUTF8);
#else
    return wxString([nsstring lossyCString]);
#endif // wxUSE_UNICODE
}