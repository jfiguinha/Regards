#include "header.h"
#include "CheckVersion.h"
#include <ConvertUtility.h>
#include <wx/url.h>

#include <wx/sstream.h>
using namespace Regards::Internet;

CCheckVersion::CCheckVersion(const wxString &server)
{
	serverHttp = server;
}


CCheckVersion::~CCheckVersion()
{

}

wxString CCheckVersion::GetLastVersion()
{
	printf("CCheckVersion::GetLastVersion \n");

	//int error = 0;
	bool returnValue = true;
	wxString xml = L"";
	wxString httpAdress = serverHttp;

	try
	{
		printf("http address : %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
		wxURL url(httpAdress);
		if (url.GetError() == wxURL_NOERR)
		{
			wxInputStream* in_stream = url.GetInputStream();
			if (in_stream != nullptr)
			{
				if (in_stream && in_stream->IsOk()) {
					wxStringOutputStream html_stream(&xml);
					in_stream->Read(html_stream);
				}
			}

			if (in_stream == nullptr)
			{
				return "";
			}
			else
				delete in_stream;
		}
		else
		{
			printf("URL ERROR %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
			return "";
		}
	}
	catch (...)
	{
		printf("wxURL ERROR CRASH %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
	}

	return xml;
}
