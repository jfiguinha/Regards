#pragma once

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;
		class CSqlLib;

		class CSqlExecuteRequest
		{
		public:
			CSqlExecuteRequest(const wxString& databaseName);
			virtual ~CSqlExecuteRequest();
			int ExecuteRequestWithNoResult(const wxString& requestSQL);
			int ExecuteRequest(const wxString& requestSQL);
			bool ExecuteInsertBlobData(const wxString& requestSQL, const int& numCol, const void* zBlob,
			                           const int& nBlob);
			//virtual int64_t GetLastId();

			static void BeginTransaction();
			static void CommitTransection();

		protected:
			bool useTransaction;
			CSqlLib* _sqlLibTransaction;
			wxString databaseName;

		private:
			virtual int TraitementResult(CSqlResult* sqlResult) = 0;
		};
	}
}
