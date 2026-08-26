#include "header.h"
#include "SqlEngine.h"
#include "SqlLib.h"
#include <algorithm>

using namespace Regards::Sqlite;

// Initialisation du singleton à nullptr
std::unordered_map<wxString, std::unique_ptr<CSqlLib>> CSqlEngine::_bases;

CSqlLib* CSqlEngine::getInstance(const wxString& baseName)
{
	auto it = _bases.find(baseName);
	return (it != _bases.end()) ? it->second.get() : nullptr;
}


bool CSqlEngine::Initialize(const wxString& filename, const wxString& baseName, CSqlLib* sqlLib)
{
	auto i = _bases.find(baseName);
	if (i != _bases.end())
		return false;

	std::unique_ptr<CSqlLib> lib(sqlLib);

	if (lib->InitDatabase(filename))
	{
		_bases.emplace(baseName, std::move(lib));
		return true;
	}

	// Tentative de récupération
	if (!lib->RecoverDatabaseFile(filename))
		return false;

	if (!lib->OpenConnection(filename, false, false))
		return false;

	_bases.emplace(baseName, std::move(lib));
	return true;
}

void CSqlEngine::kill(const wxString& baseName)
{
	auto it = _bases.find(baseName);
	if (it == _bases.end())
		return;

	it->second->CloseConnection();
	_bases.erase(it); // unique_ptr détruit automatiquement l'objet
}
