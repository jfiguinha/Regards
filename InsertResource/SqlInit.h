#pragma once
#include "header.h"
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
				wstring filename = L"C:\\developpement\\Regards\\Regards\\Resources\\resource.db";
				//wstring filename = L"C:\\Regards\\Regards\\Resources\\resource.db";
				CSqlEngine::Initialize(filename,"ResourceDB",libResource);
			}


			static void KillSqlEngine()
			{
				CSqlEngine::kill("ResourceDB");
			}
		};
	}
}

