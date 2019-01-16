//
//  mime.h
//  Regards.libControl
//
//  Created by figuinha on 11/06/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

/*
 * Purpose: wxWindows mime implementation
 * Author:  Frank Buß
 * Created: 2002
 */

#ifndef MIME_H
#define MIME_H

#include <wx/filename.h>
#include <wx/wfstream.h>

class wxMimePart
{
    public:
    
         wxMimePart(const wxFileName& fileName);
         void Encode(wxOutputStream& out);
    
     private:
         wxFileName m_fileName;
         wxString m_mainType;
         wxString m_subType;
};

WX_DECLARE_OBJARRAY(wxMimePart, wxArrayMimePart);

#endif
