#pragma once

namespace Regards
{
	namespace Sqlite
	{
		class CSqlLib;

		class CSqlEngine
		{
		public:

			struct DataBase
			{
				CSqlLib * _singleton;
				wxString baseName;
			};

			CSqlEngine();
			virtual ~CSqlEngine();

			static CSqlLib * getInstance(const wxString &baseName);
			static void Initialize(const wxString &filename, const wxString &baseName, CSqlLib * sqlLib);
			static void kill(const wxString &baseName);

		private:
			
			static vector<DataBase> _listOfBase;
		};
	}
}

