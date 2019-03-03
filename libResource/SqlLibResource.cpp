#include "header.h"
#include "SqlLibResource.h"
//#include <wx/filefn.h> 
using namespace Regards::Sqlite;

CSqlLibResource::CSqlLibResource(const bool &readOnly, const bool &load_inmemory)
{
	this->readonly = readOnly;
	this->load_inmemory = load_inmemory;
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
	bool hr = false;	// Error code reporting

	if (wxFileExists(lpFilename))
	{
		return OpenConnection(lpFilename, readonly, load_inmemory);
	}
	return hr;
}

