//
//  MyEmailImpl_linux.cpp
//  Regards
//

#ifdef __WXGTK__

#include "MyEmailInterface.h"
#include <wx/utils.h>
#include <wx/filename.h>

wxString EscapeSingleQuotes(const wxString& input)
{
    wxString output = input;
    output.Replace("'", "'\\''");
    return output;
}

void MyEmailImpl::SendEmail(const wxString& to, const wxString& subject, const wxString& body,const wxString& filePicture)
{
    wxString cmd = "xdg-email";

    if (!subject.IsEmpty())
        cmd += " --subject '" + EscapeSingleQuotes(subject) + "'";

    if (!body.IsEmpty())
        cmd += " --body '" + EscapeSingleQuotes(body) + "'";

    if (!filePicture.IsEmpty() && wxFileName::FileExists(filePicture))
        cmd += " --attach '" + EscapeSingleQuotes(filePicture) + "'";

    if (!to.IsEmpty())
        cmd += " '" + EscapeSingleQuotes(to) + "'";

    wxExecute(cmd, wxEXEC_ASYNC);
}

#endif