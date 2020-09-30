#pragma once
#include <WindowMain.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Window
	{
		class CListOfWindow
		{
		public:

			CListOfWindow();

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
			static CListOfWindow * getInstance();
			static void Initialize();
			static void kill();

		private:

			static CListOfWindow * listofWindow;
		};
	}
}
