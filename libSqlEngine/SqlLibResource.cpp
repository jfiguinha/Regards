#include "header.h"
#include "SqlLibResource.h"
using namespace Regards::Sqlite;

CSqlLibResource::CSqlLibResource(const bool& readOnly, const bool& load_inmemory)
{
	this->m_readonly = readOnly;
	this->m_loadInMemory = load_inmemory;
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
bool CSqlLibResource::InitDatabase(const wxString& lpFilename)
{
	bool hr = false; // Error code reporting

	if (wxFileExists(lpFilename))
	{
		return OpenConnection(lpFilename, m_readonly, m_loadInMemory);
	}
	return hr;
}
