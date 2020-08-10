#pragma once
#include <WindowMain.h>
#include "FileGeolocation.h"
#include <window_id.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Internet
	{
		class CListOfWindowGeo
		{
		public:

			CListOfWindowGeo();

			void AddWindow(CWindowMain * windowMain);
			void SendMessageToWindow(const wxString &filename, const int &typeData);
				
		private:
			WindowMainVector listOfWinListener;
		};
		
		class CGpsEngine
		{
		public:

			CGpsEngine();
			virtual ~CGpsEngine();
			static CListOfWindowGeo * getInstance();
			static void Initialize();
			static void kill();

		private:

			static CListOfWindowGeo * file_geolocation_;
		};
	}
}
