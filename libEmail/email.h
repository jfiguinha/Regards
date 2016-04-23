/*
   * Purpose: wxWindows email implementation
   * Author:  Frank Buß
   * Created: 2002
   */

 #ifndef EMAIL_H
 #define EMAIL_H

 #include <wx/wx.h>
 #include <wx/filename.h>
 #include <wx/wfstream.h>

 #include "mime.h"

 // forward declaration
 class wxRecipientsIterator;

 class wxEmailMessage
 {
     public:
         wxEmailMessage(const wxString& subject, const wxString& text, const wxString& from);
    
         const wxString& GetFrom();
    
         void AddRecipient(const wxString& address);
    
         void AddTo(const wxString& address);
    
         void AddCc(const wxString& address);
    
         void AddBcc(const wxString& address);
    
         // loads the file in memory and adds it to the message.
    
         void AddFile(const wxFileName& fileName, const wxString mimeMainType = "", const wxString mimeSubType = "");
    
         wxRecipientsIterator GetRecipientsIterator();
    
         //
         void Encode(wxOutputStream& out);
    
    private:
         wxString m_subject;
         wxString m_text;
         wxString m_from;
         wxArrayString m_rcptArray;
         wxArrayString m_toArray;
         wxArrayString m_ccArray;
         wxArrayString m_bccArray;
         wxArrayMimePart m_mimeParts;
    
         friend class wxRecipientsIterator;
     };


 class wxRecipientsIterator
 {
     public:
         wxRecipientsIterator(const wxEmailMessage& emailMessage);
    
         bool HasNext();
    
         wxString GetNext();
    
     private:
         const wxEmailMessage& m_emailMessage;
         size_t m_rcptCount;
         size_t m_toCount;
         size_t m_ccCount;
         size_t m_bccCount;
    
     };


 #endif