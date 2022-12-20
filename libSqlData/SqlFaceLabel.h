#pragma once
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlFaceLabel : public CSqlExecuteRequest
		{
		public:
			CSqlFaceLabel();
			~CSqlFaceLabel();
			bool InsertFaceLabel(const int &numFace, const wxString &faceName, const int &isSelectable);
			bool UpdateFaceLabel(const int &numFace, const wxString &faceName);
			bool UpdateNumFaceLabel(const int &numFace, const int &NewNumName);
			bool UpdateFaceLabel(const int &numFace, const int &isSelectable);
			wxString GetFaceName(int numFace);
			int GetFaceNumLabel(int numFace);
			int GetNumFace(const wxString &faceName);
			bool DeleteFaceLabelDatabase(int numFace);
			bool DeleteFaceLabelDatabase();
			vector<int> GetFaceLabelAlone();
		private:

			int type;
			int TraitementResult(CSqlResult * sqlResult);
			wxString faceName;
			vector<int> listOfFace;
			int numFace;
		};
	}
}
