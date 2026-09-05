#pragma once
#include <unordered_map>
#include <memory>
#include <mutex> // <-- Ajout du support de synchronisation
#include <wx/string.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlLib;

		class CSqlEngine
		{
		public:
			CSqlEngine() = delete;
			virtual ~CSqlEngine() = default;

			static CSqlLib* getInstance(const wxString& baseName);
			static bool Initialize(const wxString& filename, const wxString& baseName, CSqlLib* sqlLib);
			static void kill(const wxString& baseName);

		private:
			static std::unordered_map<wxString, std::unique_ptr<CSqlLib>> _bases;
			static std::mutex _engineMutex; // <-- Mutex global pour sécuriser la map
		};
	}
}