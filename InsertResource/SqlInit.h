#pragma once
#include <SqlEngine.h>
#include <SqlLibResource.h>
#include <FileUtility.h>
using namespace Regards::Sqlite;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlInit
		{
		public:

			CSqlInit(){};
			~CSqlInit(){};

			static void InitializeSQLServerDatabase()
			{
				CSqlLibResource * libResource = new CSqlLibResource(false, false);
				wstring filename = L"c:\\Regards\\Regards\\resources\\resource.db";
				CSqlEngine::Initialize(filename,"ResourceDB",libResource);
			}


			static void KillSqlEngine()
			{
				CSqlEngine::kill("ResourceDB");
			}
		};
	}
}

