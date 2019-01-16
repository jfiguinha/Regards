#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <FaceDescriptor.h>
#include <FaceName.h>
#include <FaceFilePath.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFindFacePhoto : public CSqlExecuteRequest
		{
		public:
			CSqlFindFacePhoto();
			~CSqlFindFacePhoto();
			std::vector<wxString> GetPhotoListNotProcess();
			std::vector<CFaceName> GetListFaceName();
			std::vector<CFaceName> GetListFaceName(const wxString &photoPath);
			std::vector<CFaceName> GetListFaceNameSelectable();
			std::vector<CFaceFilePath> GetListPhotoFace(const int &numFace, const double &pertinence = 0.0);
			std::vector<CFaceDescriptor *> GetUniqueFaceDescriptor(const int &numFace);
		private:
			int TraitementResult(CSqlResult * sqlResult);
			std::vector<wxString> listPhoto;
			std::vector<CFaceDescriptor *> listFaceDescriptor;
			std::vector<CFaceName> listFaceName;
			std::vector<CFaceFilePath> listFace;
			int type;
		};
	}
}
