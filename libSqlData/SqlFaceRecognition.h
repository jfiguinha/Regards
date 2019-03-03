#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFaceRecognition : public CSqlExecuteRequest
		{
		public:
			CSqlFaceRecognition();
			~CSqlFaceRecognition();
			bool InsertFaceRecognition(int numFace, int numFaceCompatible);
			bool UpdateFaceRecognition(int OldnumFaceCompatible, int NewnumFaceCompatible);
			bool MoveFaceRecognition(int numFace, int NewnumFaceCompatible);
			std::vector<int> GetUniqueFace();
			bool DeleteFaceRecognitionDatabase();
			bool DeleteFaceRecognitionDatabase(int numFace);
			int GetCompatibleFace(const int & numFace);
		private:

			int TraitementResult(CSqlResult * sqlResult);
			std::vector<int> listFace;
			int faceCompatible;
			int type;
		};
	}
}
