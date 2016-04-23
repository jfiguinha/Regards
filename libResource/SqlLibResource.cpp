#include "SqlLibResource.h"
#include <wx/filefn.h> 
using namespace Regards::Sqlite;

CSqlLibResource::CSqlLibResource()
{

}


CSqlLibResource::~CSqlLibResource()
{

}

////////////////////////////////////////////////////////////////////////////////
// Function: InitDatabase()
//
// Description: Open a connection to database, 
//				then create a session object.
//
// Returns: NOERROR if succesfull
//
// Notes:
//
////////////////////////////////////////////////////////////////////////////////
bool CSqlLibResource::InitDatabase(const wxString &lpFilename)
{
	bool			   	hr = false;	// Error code reporting

	if (wxFileExists(lpFilename))
	{
		return OpenConnection(lpFilename, true);
	}
	return hr;
}

