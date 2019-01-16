//
//  mime.cpp
//  Regards.libControl
//
//  Created by figuinha on 11/06/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#include "mime.h"
#include <wx/base64.h>
#include <ConvertUtility.h>
/*
 * Purpose: wxWindows mime implementation
   * Author:  Frank Buß
   * Created: 2002
  */

// #include "email.h"
 //#include "Base64.h"


 #include "wx/arrimpl.cpp"
 WX_DEFINE_OBJARRAY(wxArrayMimePart);

 struct wxMimeTypeMapping
 {
         const wxChar* extension;
         const wxChar* mainType;
         const wxChar* subType;
     };

 // extension, main type, sub type
 // TODO: must be merged / integrated / substituted with wxFileType
 static const wxMimeTypeMapping g_mimeTypeMappings[] =
 {
         { _T("txt"), _T("text"), _T("plain") },
         { _T("htm"), _T("text"), _T("html") },
         { _T("html"), _T("text"), _T("html") },
         { _T("gif"), _T("image"), _T("gif") },
         { _T("png"), _T("image"), _T("png") },
         { _T("jpg"), _T("image"), _T("jpeg") },
         { _T("jpeg"), _T("image"), _T("jpeg") },
         { _T("pdf"), _T("application"), _T("pdf") },
         { _T("doc"), _T("application"), _T("msword") }
     };

 wxMimePart::wxMimePart(const wxFileName& fileName) :
     m_fileName(fileName)
 {
         // setup default encoding
         m_mainType = _T("application");
         m_subType = _T("octet-stream");
    
         // try to determine encoding by filename extension
         wxString ext = fileName.GetExt().Lower();
         for (auto i = 0; i < sizeof(g_mimeTypeMappings) / sizeof(wxMimeTypeMapping); i++) {
                 if (g_mimeTypeMappings[i].extension == ext) {
                         m_mainType = g_mimeTypeMappings[i].mainType;
                         m_subType = g_mimeTypeMappings[i].subType;
                         break;
                     }
             }
     }

 void wxMimePart::Encode(wxOutputStream& out)
 {
         // TODO: error handling for every return
         // TODO: encoding it on-the-fly without loading it all in memory
         wxFileInputStream in(m_fileName.GetFullPath());
         if (!in.Ok()) return;
    
     // TODO: perhaps this can be added to wxFileName for size()
         in.SeekI(0, wxFromEnd);
         size_t len = (size_t) in.TellI();
         in.SeekI(0);
         // that's possible for windows (#include <sys/types.h> and #include <sys/stat.h>):
         // struct _stat lenStat;
         // if (_stat(filename, &lenStat) == -1) return error;
         // size_t len = (size_t) lenStat.st_size;
    
         if (len == 0) return;
    
         // read file
         wxUint8* pData = new wxUint8[len];
         if (!pData) {
                 wxASSERT_MSG(pData != NULL, _T("out of memory"));
                 return;
             }
         in.Read(pData, len);
    
         // write encoded mime part to stream
         wxString cr("\x00d\x00a");
         wxString result;
         result << "Content-Type: " << m_mainType << "/" << m_subType << "; name=\"" << m_fileName.GetFullName() + "\"" + cr
                << "Content-Transfer-Encoding: base64" << cr
                << "Content-Disposition: attachment; filename=\"" << m_fileName.GetName() << "\"" << cr
                << cr
     << wxBase64Encode(pData, len) << cr;
         out.Write(CConvertUtility::ConvertToUTF8(result), result.Length());
     }
    