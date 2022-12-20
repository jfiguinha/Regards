#pragma once
#include "SqlExecuteRequest.h"
#include <Criteria.h>


//Exif Value
//Angle HFlip VFlip 

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlPhotos : public CSqlExecuteRequest
		{
		public:
			CSqlPhotos(CSqlLib * _sqlLibTransaction = nullptr, const bool &useTransaction = 0);
			~CSqlPhotos();
			bool InsertPhoto(const wxString & filepath, const int64_t &idFolder);
			int64_t GetPhotoId(const wxString & filepath, const int64_t &idFolder);
            int64_t GetPhotoId(const wxString & filepath);
			int64_t GetOrInsertPhoto(const wxString & filepath, const int64_t &idFolder);

			int64_t GetPhotoExif(const wxString& filepath);
			bool InsertPhotoExif(const wxString& filepath, const int64_t& exif);
			bool UpdatePhotoExif(const wxString& filepath, const int64_t& exif);
			wxString GetPhotoPath(const int64_t &numPhoto);

			void GetAngleAndFlip(const int64_t& exif, int& angle, int& flipH, int& flipV);
			int GetExifFromAngleAndFlip(const int& angle, const int& flipH, const int& flipV);

			bool DeletePhoto(const int64_t &numPhoto);
			void DeletePhotoSearch();
			bool DeletePhotoFolder(const int64_t &idFolder);
			bool DeletePhotoCatalog(const int64_t &idCatalog);
			bool UpdatePhotoCriteria(const int64_t &numPhoto);
			int GetCriteriaInsert(const wxString & filepath);
			void GetPhotoCriteria(CriteriaVector * criteriaVector, const wxString & filepath);
            void GetPhotoCriteriaByCategorie(CriteriaVector * criteriaVector, const wxString & filepath, const int &numCategorie);
			vector<wxString> GetPhotoFromFolder(const int64_t &idFolder);
			void DeletePhotoExif(const wxString& filepath);
		private:

			int TraitementResult(CSqlResult * sqlResult);
			int64_t photoId;
			int64_t exif;
			int typeResult;
			CriteriaVector * criteriaVector;
			vector<wxString> listPhoto;
			wxString photoPath;
		};
	}
}

