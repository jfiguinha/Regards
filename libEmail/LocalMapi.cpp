// Mapi.cpp: implementation of the CMapi class.
//
//////////////////////////////////////////////////////////////////////

#include "LocalMapi.h"
#include <MAPI.H>
//#include <MAPIUTIL.H>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapi::CMapi()
{

}

CMapi::~CMapi()
{

}
//////////////////////////////////////////////////////////////////////////////////////

HRESULT CMapi::SendEmail(const string &m_szBody, const vector<string> & attachment)
{
     MapiMessage message ;
	 MapiFileDesc * mapiAttachment = (MapiFileDesc*)malloc(sizeof(MapiFileDesc) * attachment.size());
     MAPISENDMAIL *lpfnMAPISendMail ;
     HMODULE hModule ;
	 int i = 0;


     hModule = LoadLibrary( L"mapi32.dll" ) ;
     if( hModule==NULL ) {
          printf("ERROR: Unable to load mapi32.dll\n" ) ;
          return false;
     }

     lpfnMAPISendMail = (MAPISENDMAIL *)GetProcAddress( hModule, "MAPISendMail" ) ;
     if( lpfnMAPISendMail==NULL ) {
          printf( "ERROR: Unable to locate MAPISendMail entry point\n" ) ;
          return false;
     }

	 ZeroMemory(mapiAttachment, sizeof(MapiFileDesc) * attachment.size());

	 for (vector<string>::const_iterator fit = attachment.begin(); fit != attachment.end(); fit++, i++)
	 {
		 mapiAttachment[i].nPosition = i;
		 mapiAttachment[i].lpszPathName = (char*)(*fit).c_str();
	 }

     
     memset( &message, 0, sizeof(message) ) ;
     message.lpszNoteText = (char *)m_szBody.c_str();
	 message.nFileCount = attachment.size();
	 message.lpFiles = mapiAttachment;

     lpfnMAPISendMail( 0, 0, &message, MAPI_DIALOG, 0 ) ;

	 FreeLibrary(hModule);

	 free(mapiAttachment);

	 return true;
}
