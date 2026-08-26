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
				CSqlLib* _singleton;
				wxString baseName;
			};

			CSqlEngine() = delete;
			virtual ~CSqlEngine() = default;

			static CSqlLib* getInstance(const wxString& baseName);
			static bool Initialize(const wxString& filename, const wxString& baseName, CSqlLib* sqlLib);
			static void kill(const wxString& baseName);


		private:
			static std::unordered_map<wxString, std::unique_ptr<CSqlLib>> _bases;
		};
	}
}
