#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <stdint.h>
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;
		class CSqlLib;

		class CSqlExecuteRequest
		{
		public:
			CSqlExecuteRequest(const wxString & databaseName);
			~CSqlExecuteRequest();
			int ExecuteRequestWithNoResult(const wxString &requestSQL);
			int ExecuteRequest(const wxString &requestSQL);
			bool ExecuteInsertBlobData(const wxString &requestSQL, const int &numCol, const void * zBlob, const int &nBlob);
			int64_t GetLastId();

		protected:

			void BeginTransaction();
			void CommitTransection();

			bool useTransaction;
			CSqlLib * _sqlLibTransaction;
			wxString databaseName;
		private:

			virtual int TraitementResult(CSqlResult * sqlResult) = 0;
		};
	}
}

