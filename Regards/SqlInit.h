#pragma once
#include <SqlEngine.h>
#include <SqlLibExplorer.h>
#include <LibResource.h>
using namespace Regards::Sqlite;

namespace Regards::Sqlite
{
	class CSqlInit
	{
	public:
		CSqlInit()
		{
		};

		~CSqlInit()
		{
		};


		static void InitializeSQLServerDatabase(const wxString& folder, const bool& load_inmemory)
		{
			wxString libelleNotGeo = CLibResource::LoadStringFromResource("LBLNOTGEO", 1);
			auto libExplorer = new CSqlLibExplorer(false, libelleNotGeo, load_inmemory);
			wxString filename = folder;

#ifdef WIN32
			filename.append("\\regards.db");
#else
                filename.append("/regards.db");
#endif

			CSqlEngine::Initialize(filename, L"RegardsDB", libExplorer);
		}


		static void KillSqlEngine()
		{
			CSqlEngine::kill(L"RegardsDB");
		}
	};
}
