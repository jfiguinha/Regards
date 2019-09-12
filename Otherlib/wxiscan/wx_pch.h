00001 /***************************************************************
00002  * Name:      wx_pch.h
00003  * Purpose:   Header to create Pre-Compiled Header (PCH)
00004  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00005  * Created:   2008-05-04
00006  * Copyright: Daniel Nell (www.nellresearch.de)
00007  * License:   GPL
00008  **************************************************************/
00009 
00010 #ifndef WX_PCH_H_INCLUDED
00011 #define WX_PCH_H_INCLUDED
00012 
00013 // Basic wxWidgets headers.
00014 #include <wx/wxprec.h>
00015 
00016 #ifdef __BORLANDC__
00017 #   pragma hdrstop
00018 #endif
00019 
00020 #ifndef WX_PRECOMP
00021 #   include <wx/wx.h>
00022 #endif
00023 
00024 #ifdef WX_PRECOMP
00025 // Note Put here all your rarely-changing header files.
00026 #endif // WX_PRECOMP
00027 
00028 #endif // WX_PCH_H_INCLUDED

