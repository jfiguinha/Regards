#pragma once
#include <string>
#include <SqlLib.h>
#include "SqlResult.h"
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlLibExplorer : public CSqlLib
		{
		public:
			CSqlLibExplorer();
			~CSqlLibExplorer();

			bool InitDatabase(const wxString &lpFilename);
            bool CheckVersion(const wxString &lpFilename);
		private:

			bool CreateDatabase(const wxString &databasePath);
		};
	}
}

