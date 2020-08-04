#pragma once
#include "SqlExecuteRequest.h"
//#include "SqlResult.h"
//#include <FaceDescriptor.h>

class CFaceDescriptor;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;
		

		class CSqlFaceDescriptor : public CSqlExecuteRequest
		{
		public:
			CSqlFaceDescriptor();
			~CSqlFaceDescriptor();
			int InsertFaceDescriptor(const int & numFace, const char *zBlob, const int &nBlob);
			CFaceDescriptor * GetFaceDescriptor(const int & numFace);
			bool DeleteFaceDescriptorDatabase();
			bool DeleteFaceDescriptorDatabase(const int & numFace);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			CFaceDescriptor * faceDescriptor;
			int64_t numFace;
			wxString filename;
		};
	}
}
