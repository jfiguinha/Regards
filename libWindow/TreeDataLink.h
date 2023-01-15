#pragma once
#include "TreeData.h"

namespace Regards::Window
{
	class CTreeDataLink : public CTreeData
	{
	public:
		CTreeDataLink(void);
		~CTreeDataLink(void);

		void SetId(const int& id);
		int GetId();

		void SetLinkType(const int& type);
		int GetLinkType();

		void SetLinkPath(const wxString& value);
		wxString GetLinkPath();

	protected:
		wxString linkPath;
		int linktype;
		int id;
	};
}
