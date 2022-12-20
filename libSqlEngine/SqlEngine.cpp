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

CSqlLib * CSqlEngine::getInstance(const wxString &baseName)
{
	vector<CSqlEngine::DataBase>::iterator i = std::find_if(_listOfBase.begin(),
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



void CSqlEngine::Initialize(const wxString & filename, const wxString &baseName, CSqlLib * sqlLib)
{
	vector<CSqlEngine::DataBase>::iterator i = std::find_if(_listOfBase.begin(),
		_listOfBase.end(),
		[&](const auto& val) { return val.baseName == baseName; });
	if (i != _listOfBase.end())
		return;

	//for (DataBase db : _listOfBase)
	//	if (db.baseName == baseName)
	//		return;

	DataBase db;
	db._singleton = sqlLib;
	db.baseName = baseName;
	bool hr = db._singleton->InitDatabase(filename);
	if (!(hr))
	{
		delete db._singleton;
		db._singleton = nullptr;
	}
	else
	{
		_listOfBase.push_back(db);
	}
    
    db._singleton->CheckVersion(filename);

}

void CSqlEngine::kill(const wxString &baseName)
{
	vector<CSqlEngine::DataBase>::iterator i = std::find_if(_listOfBase.begin(),
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
