#pragma once
#include <string>
#include "LibInternet.h"
using namespace std;

namespace Regards
{
	namespace Internet
	{
		class CVerifNewVersion : public CInternet
		{
		public:
			CVerifNewVersion(){};
			~CVerifNewVersion(){};

			bool TestIfNewVersionAvailable(const wxString &szSiteServer, const wxString &szSiteAdress, const wxString &szVersion)
			{
				int error = 0;
				OpenHttpRequest(szSiteServer, error);
				wxString message = ExecuteHttpRequest(szSiteAdress, error);
				if (error == OK)
				{
					if (message > szVersion)
					{
						return true;
					}
				}

				CloseHttpRequest();
				return false;
			}

		};
	}
}


