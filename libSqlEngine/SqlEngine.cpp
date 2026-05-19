#include "header.h"
#include "SqlEngine.h"
#include "SqlLib.h"
#include <algorithm>

using namespace Regards::Sqlite;

// Initialisation du singleton à nullptr
vector<CSqlEngine::DataBase> CSqlEngine::_listOfBase;

CSqlEngine::CSqlEngine()
{
}


CSqlEngine::~CSqlEngine()
{
}


CSqlLib* CSqlEngine::getInstance(const wxString& baseName)
{
	auto i = std::find_if(_listOfBase.begin(),
	                      _listOfBase.end(),
	                      [&](const auto& val) { return val.baseName == baseName; });

	if (i != _listOfBase.end())
	{
		return (*i)._singleton;
	}
	//for (DataBase db : _listOfBase)
	//	if (db.baseName == baseName)
	//		return db._singleton;
	return nullptr;
}


bool CSqlEngine::Initialize(const wxString& filename, const wxString& baseName, CSqlLib* sqlLib)
{
	auto i = std::find_if(_listOfBase.begin(),
	                      _listOfBase.end(),
	                      [&](const auto& val) { return val.baseName == baseName; });
	if (i != _listOfBase.end())
		return false;

	//for (DataBase db : _listOfBase)
	//	if (db.baseName == baseName)
	//		return;

	DataBase db;
	db._singleton = sqlLib;
	db.baseName = baseName;
	bool hr = db._singleton->InitDatabase(filename);
	if (!(hr))
	{
		// Tenter la récupération sur le fichier
		bool ok = db._singleton->RecoverDatabaseFile(filename);

		// Reouvrir la connexion (non readonly et sans load_inmemory ici)
		if (ok)
		{
			ok = db._singleton->OpenConnection(filename, false, false);
			if (ok)
				db._singleton->CheckVersion(filename);
		}
		else
			return false;
	}
	else
	{
		_listOfBase.push_back(db);
		db._singleton->CheckVersion(filename);
	}

	return true;
}

void CSqlEngine::kill(const wxString& baseName)
{
	auto i = std::find_if(_listOfBase.begin(),
	                      _listOfBase.end(),
	                      [&](const auto& val) { return val.baseName == baseName; });

	if (i != _listOfBase.end())
	{
		DataBase db = *i;
		db._singleton->CloseConnection();
		delete db._singleton;
		db._singleton = nullptr;
	}

	/*
	for (DataBase db : _listOfBase)
	{
		if (db.baseName == baseName)
		{
			if (nullptr != db._singleton)
			{
				db._singleton->CloseConnection();

				delete db._singleton;
				db._singleton = nullptr;
			}
			break;
		}
	}
	*/
}
