MediaInfo.Dll - http://MediaArea.net/MediaInfo
Copyright (c) MediaArea.net SARL. All Rights Reserved

Use of this source code is governed by a BSD-style license that can be found in the License.html file in the root of the source tree.


For software developers
-----------------------
There are examples for:
- BCB: Borland C++ Builder 12 (aka 2009)
- Delphi: Borland Delphi 12 (aka 2009)
- MSCS: Microsoft C# 9 (aka 2008): normal binary and ASP.net web application
- MSJS: Microsoft J# 8 (aka 2005)
- MSVB: Microsoft Visual Basic 9 (aka 2008)
- MSVC: Microsoft Visual C++ 9 (aka 2008)
- Java: Netbeans (with JNA or JNative binding)
- PureBasic
- Python 2
- Python 3
Don't forget to put MediaInfo.Dll and Example.ogg in your executable folder.

Note: for Visual Studio 7 (aka 2002), 7.1 (aka 2003) or 8 (aka 2005), you can edit .sln and .xxproj to be compatible.
- .sln: "...Format Version 9.00" to "7.00"
- .xxproj: "Version="8.00" to "7.00"
This is not always tested, but examples files are only a template, you can easily adapt them to your compiler
Note: I can't handle all versions of all compilers, so I look for maintainers for examples.

Note: versioning method, for people who develop with LoadLibrary method
- if one of 2 first numbers change, there is no guaranties that the DLL is compatible with old one. You should verify with MediaInfo_Option("Version") if you are compatible
- if one of 2 last numbers change, there is a guaranty that the DLL is compatible with old one.
So you should test the version of the DLL, and if one of the 2 first numbers change, not load it.

Contributions (in the "Contrib" directory):
- MSVB5: Microsoft Visual Basic 5 and 6 example, from Ingo Brueckl
- ActiveX: ActiveX wrapper for MediaInfoDLL, with Internet Explorer and VB Script examples, from Ingo Brueckl
