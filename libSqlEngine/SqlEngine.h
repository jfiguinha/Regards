#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

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
				CSqlLib * _singleton = nullptr;
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

