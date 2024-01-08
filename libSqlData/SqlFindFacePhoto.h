#pragma once
#include "SqlExecuteRequest.h"
#include <FaceName.h>
#include <FaceFilePath.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlFindFacePhoto : public CSqlExecuteRequest
		{
		public:
			CSqlFindFacePhoto();
			~CSqlFindFacePhoto() override;
			std::vector<wxString> GetPhotoListNotProcess();
			std::vector<int> GetListFaceToRecognize();
			std::vector<CFaceName> GetListFaceName();
			std::vector<CFaceName> GetListFaceNum(const wxString& photopath);
			std::vector<CFaceName> GetListFaceName(const wxString& photoPath);
			std::vector<CFaceName> GetListFaceNameSelectable();
			std::vector<CFaceFilePath> GetListPhotoFace(const int& numFace, const double& pertinence = 0.0);
			int GetNbListFaceToRecognize();

		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			std::vector<wxString> listPhoto;
			std::vector<CFaceName> listFaceName;
			std::vector<CFaceFilePath> listFace;
			std::vector<int> listNumFace;
			int nbFaceList = 0;
			int type;
		};
	}
}
