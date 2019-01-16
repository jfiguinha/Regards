/*
   * Purpose: wxWindows email implementation
   * Author:  Frank Buß
   * Created: 2002
   */

 #include "email.h"
#include <wx/base64.h>
#include <ConvertUtility.h>


 wxEmailMessage::wxEmailMessage(const wxString& subject, const wxString& text, const wxString& from) :
     m_subject(subject),
     m_text(text),
     m_from(from)
 {
     }

 const wxString& wxEmailMessage::GetFrom()
 {
         return m_from;
     }

 void wxEmailMessage::AddRecipient(const wxString& address)
 {
         m_rcptArray.Add(address);
     }

 void wxEmailMessage::AddTo(const wxString& address)
 {
         m_toArray.Add(address);
     }

 void wxEmailMessage::AddCc(const wxString& address)
 {
         m_ccArray.Add(address);
     }

 void wxEmailMessage::AddBcc(const wxString& address)
 {
         m_bccArray.Add(address);
     }

 void wxEmailMessage::AddFile(const wxFileName& fileName, const wxString mimeMainType, const wxString mimeSubType)
 {
         // add new entry
         m_mimeParts.Add(wxMimePart(fileName));
     }

 wxRecipientsIterator wxEmailMessage::GetRecipientsIterator()
 {
         return wxRecipientsIterator(*this);
     }

 void wxEmailMessage::Encode(wxOutputStream& out)
 {
         // TODO: use only MIME if neccessary
         wxString header;
         wxString cr("\x00d\x00a");
         // TODO: calculate a more random boundary
         wxString boundary("---_0123456789-boundary-9876543210_---");
         wxString boundarySep = "--" + boundary + cr;
         header << "From: " << m_from << cr;
         // TODO: add to, cc and bcc in the right format with line breaking, if too long
         header << "Bcc:" << cr
                << "Subject: " << m_subject << cr  // TODO: add Date:
                << "MIME-Version: 1.0" << cr
                << "Content-Type: multipart/mixed; boundary=\"" << boundary << "\"" << cr
                << cr
                << cr
                << "This is a multi-part message in MIME format" << cr
                << cr
                << boundarySep
                << "Content-Type: text/plain; charset=iso-8859-1" << cr
                << "Content-Transfer-Encoding: 8bit" << cr
                << cr
                << m_text << cr;  // TODO: is it possible in MIME message to have a single '.' on a line?
         out.Write(CConvertUtility::ConvertToUTF8(header), header.Length());
         for (size_t i = 0; i < m_mimeParts.GetCount(); i++) {
                 out.Write(CConvertUtility::ConvertToUTF8(boundarySep), boundarySep.Length());
                 m_mimeParts[i].Encode(out);
             }
         wxString footer = "--" + boundary + "--" + cr + "." + cr;  // TODO: perhaps moving the '.\r\n' sequence to another place
         out.Write(CConvertUtility::ConvertToUTF8(footer), footer.Length());
     }
    
     wxRecipientsIterator::wxRecipientsIterator(const wxEmailMessage& emailMessage) :
         m_emailMessage(emailMessage)
     {
             m_rcptCount = 0;
             m_toCount = 0;
             m_ccCount = 0;
             m_bccCount = 0;
         }
    
     bool wxRecipientsIterator::HasNext()
     {
             if (m_rcptCount < m_emailMessage.m_rcptArray.GetCount() ||
                               m_toCount < m_emailMessage.m_toArray.GetCount() ||
                               m_ccCount < m_emailMessage.m_ccArray.GetCount() ||
                               m_bccCount < m_emailMessage.m_bccArray.GetCount())
                 {
                         return true;
                     }
             return false;
         }
    
     wxString wxRecipientsIterator::GetNext()
     {
             if (m_rcptCount < m_emailMessage.m_rcptArray.GetCount()) {
                     return m_emailMessage.m_rcptArray[m_rcptCount++];
                 } else if (m_toCount < m_emailMessage.m_toArray.GetCount()) {
                         return m_emailMessage.m_toArray[m_toCount++];
                     } else if (m_ccCount < m_emailMessage.m_ccArray.GetCount()) {
                             return m_emailMessage.m_ccArray[m_ccCount++];
                         } else if (m_bccCount < m_emailMessage.m_bccArray.GetCount()) {
                                 return m_emailMessage.m_bccArray[m_bccCount++];
                             }
             return "";
         }
    