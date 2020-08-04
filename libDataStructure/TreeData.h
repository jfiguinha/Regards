#pragma once
#include "Tree.h"

namespace Regards
{
	namespace Window
	{
		class CTreeData
		{
		public:
			CTreeData(void);
			~CTreeData(void);

			void SetKey(const wxString & value);
			wxString GetKey();

			void SetValue(const wxString & value);
			wxString GetValue();

			void SetExifKey(const wxString & value);
			wxString GetExifKey();

			void SetActif(const bool & value);
			bool GetActif();

			void SetType(const int & type);
			int GetType();

			bool IsParent() { return parent; };
			void SetIsParent(const bool &value) { parent = value; };

			tree<CTreeData *>::iterator child;

		protected:
			wxString valeur;
			wxString exifKey;
			wxString key;
			bool parent;
			bool actif;
			int type;
		};
	}
}

