#include <header.h>
#include "SqlLibExplorer.h"
#include "SqlVersion.h"
#include <SqlFindPhotos.h>
#include <SqlThumbnail.h>
#include <libPicture.h>
#include <FiltreEffetCPU.h>
#include <FileUtility.h>
#include <wx/dir.h>
using namespace Regards::Picture;
using namespace Regards::Sqlite;

#define SQL_CREATE_VERSION_TABLE "CREATE TABLE VERSION (libelle NVARCHAR(255) PRIMARY KEY)"
#define SQL_DROP_VERSION	"DROP TABLE VERSION"

#define SQL_CREATE_LANGUE_TABLE	"CREATE TABLE LANGUE (NumLangue INTEGER PRIMARY KEY, LibelleLangue NVARCHAR(20))"
#define SQL_DROP_LANGUE	"DROP TABLE LANGUE"

#define SQL_CREATE_COUNTRY_TABLE "CREATE TABLE COUNTRY (NumCountry INTEGER PRIMARY KEY, CodeCountry NVARCHAR(4), LibelleContinent NVARCHAR(50), LibelleCountry NVARCHAR(50))"
#define SQL_DROP_COUNTRY "DROP TABLE COUNTRY"

#define SQL_CREATE_CATALOG_TABLE "CREATE TABLE CATALOG (NumCatalog INTEGER PRIMARY KEY AUTOINCREMENT, LibelleCatalog NVARCHAR(50))"
#define SQL_DROP_CATALOG "DROP TABLE CATALOG"

#define SQL_CREATE_FOLDERCATALOG_TABLE "CREATE TABLE FOLDERCATALOG (NumFolderCatalog INTEGER PRIMARY KEY AUTOINCREMENT, NumCatalog INT, FolderPath NVARCHAR(255))"
#define SQL_DROP_FOLDERCATALOG "DROP TABLE FOLDERCATALOG"

#define SQL_CREATE_CATEGORIE_TABLE "CREATE TABLE CATEGORIE (NumCategorie INT , NumLangue INT, Libelle NVARCHAR(20))"
#define SQL_DROP_CATEGORIE "DROP TABLE CATEGORIE"

#define SQL_CREATE_CRITERIA_TABLE "CREATE TABLE CRITERIA (NumCriteria INTEGER PRIMARY KEY AUTOINCREMENT, NumCategorie INT, NumCatalog INT, Libelle NVARCHAR(255))"
#define SQL_DROP_CRITERIA "DROP TABLE CRITERIA"

#define SQL_CREATE_PHOTOS_TABLE "CREATE TABLE PHOTOS (NumPhoto INTEGER PRIMARY KEY AUTOINCREMENT , NumFolderCatalog INT, FullPath NVARCHAR(255), CriteriaInsert INT, Process INT, ExtensionId INT, Multifiles INT)"
#define SQL_DROP_PHOTOS "DROP TABLE PHOTOS"

#define SQL_CREATE_PHOTOSCRITERIA_TABLE "CREATE TABLE PHOTOSCRITERIA (NumCriteria INT, NumPhoto INT)"
#define SQL_DROP_PHOTOSCRITERIA "DROP TABLE PHOTOSCRITERIA"

#define SQL_CREATE_PHOTOFOLDER_TABLE "CREATE TABLE PHOTOFOLDER (FullPath NVARCHAR(255))"
#define SQL_DROP_PHOTOFOLDER "DROP TABLE PHOTOFOLDER"

#define SQL_INDEX_PHOTOFOLDER "CREATE UNIQUE INDEX idx_PHOTOFOLDER_FullPath ON PHOTOFOLDER(FullPath)"

#define SQL_DROP_PHOTOSSEARCH "DROP TABLE PHOTOSSEARCHCRITERIA"

#define SQL_CREATE_PHOTOSTHUMBNAIL_TABLE "CREATE TABLE PHOTOSTHUMBNAIL (NumPhoto INT PRIMARY KEY, FullPath NVARCHAR(255), width INT, height INT, hash NVARCHAR(255))"
#define SQL_DROP_PHOTOSTHUMBNAIL "DROP TABLE PHOTOSTHUMBNAIL"

#define SQL_INDEX_PHOTOSTHUMBNAIL "CREATE UNIQUE INDEX idx_PHOTOSTHUMBNAIL_FullPath ON PHOTOSTHUMBNAIL(FullPath)"

#define SQL_CREATE_VIDEOTHUMBNAIL_TABLE "CREATE TABLE VIDEOTHUMBNAIL (NumPhoto INT, numVideo INT, FullPath NVARCHAR(255), rotation INT, percent INT, timePosition INT, width INT, height INT, PRIMARY KEY (NumPhoto, numVideo))"
#define SQL_DROP_VIDEOTHUMBNAIL "DROP TABLE VIDEOTHUMBNAIL"

#define SQL_CREATE_PHOTOGPS_TABLE "CREATE TABLE PHOTOGPS (id INTEGER PRIMARY KEY AUTOINCREMENT, FullPath NVARCHAR(255), latitude NVARCHAR(255), longitude NVARCHAR(255))"
#define SQL_DROP_PHOTOGPS "DROP TABLE PHOTOGPS"

#define SQL_INDEX_PHOTOGPS "CREATE UNIQUE INDEX idx_PHOTOGPS_FullPath ON PHOTOGPS(FullPath)"

#define SQL_CREATE_PHOTOSWIHOUTTHUMBNAIL_TABLE "CREATE TABLE PHOTOSWIHOUTTHUMBNAIL (FullPath NVARCHAR(255), Priority INT, ProcessStart INT)"
#define SQL_DROP_PHOTOSWIHOUTTHUMBNAIL "DROP TABLE PHOTOSWIHOUTTHUMBNAIL"

#define SQL_CREATE_PHOTO_EXIF_TABLE "CREATE TABLE PHOTO_EXIF (NumExifPhoto INTEGER PRIMARY KEY AUTOINCREMENT, FullPath NVARCHAR(255), Exif INT)"
#define SQL_DROP_PHOTO_EXIF "DROP TABLE PHOTO_EXIF"

#define SQL_INDEX_PHOTO_EXIF "CREATE UNIQUE INDEX idx_PHOTO_EXIF_FullPath ON PHOTO_EXIF(FullPath)"

#define SQL_CREATE_FACE_PHOTO_TABLE "CREATE TABLE FACEPHOTO (NumFace INTEGER PRIMARY KEY AUTOINCREMENT, FullPath NVARCHAR(255), Numberface INT, width INT, height INT, Pertinence REAL)"
#define SQL_DROP_FACE_PHOTO "DROP TABLE FACEPHOTO"

#define SQL_CREATE_FACE_VIDEO_TABLE "CREATE TABLE FACEVIDEO (NumFace INTEGER PRIMARY KEY, videoposition INT)"
#define SQL_DROP_FACE_VIDEO "DROP TABLE FACEVIDEO"

#define SQL_CREATE_FACE_RECOGNITION_TABLE "CREATE TABLE FACE_RECOGNITION (NumFace INT, NumFaceCompatible INT)"
#define SQL_DROP_FACE_RECOGNITION "DROP TABLE FACE_RECOGNITION"

#define SQL_CREATE_FACE_NAME_TABLE "CREATE TABLE FACE_NAME (NumFace INT, FaceName NVARCHAR(255), isSelectable INT)"
#define SQL_DROP_FACE_NAME "DROP TABLE FACE_NAME"

#define SQL_CREATE_FACE_PROCESSING_TABLE "CREATE TABLE FACE_PROCESSING (FullPath NVARCHAR(255))"
#define SQL_DROP_PROCESSING_NAME "DROP TABLE FACE_PROCESSING"

#define SQL_CREATE_PHOTO_CATEGORIE_USENET_PROCESSING_TABLE "CREATE TABLE PHOTO_CATEGORIE_USENET_PROCESSING (FullPath NVARCHAR(255))"
#define SQL_DROP_PHOTO_CATEGORIE_USENET_PROCESSING_NAME "DROP TABLE PHOTOCATEGORIE_PROCESSING"

#define SQL_CREATE_PHOTO_GPS_TABLE "CREATE TABLE PHOTOSGPS (NumPhoto INT, FullPath NVARCHAR(255), NumFolderId INT)"
#define SQL_DROP_PHOTO_GP_TABLE "DROP TABLE PHOTOSGPS"

#define SQL_CREATE_OPENCLKERNEL_TABLE "CREATE TABLE OPENCLKERNEL (numProgram NVARCHAR(255), platformName NVARCHAR(255), numDevice INT, typeData INT, openCLKernel BLOB)"
#define SQL_DROP_OPENCLKERNEL_TABLE "DROP TABLE OPENCLKERNEL"

#define  SQL_CREATE_SEARCH_VIEW_VIEW "CREATE VIEW SEARCH_VIEW AS SELECT NumPhoto, FullPath, CreateDate, GeoGPS, \
substr(CreateDate, 0, 5) as Year, substr(CreateDate, 6, 2) as Month, substr(CreateDate, 9, 2) as Day, \
strftime('%w', REPLACE(CreateDate, '.', '-')) as DayOfWeek FROM PHOTOSSEARCHCRITERIA Order By Year, Month, Day"

#define SQL_DROP_SEARCH_VIEW_VIEW "DROP TABLE PHOTOSGPS"

CSqlLibExplorer::CSqlLibExplorer(const bool& readOnly, const wxString& libelleNotGeo, const bool& load_inmemory)
	: CSqlLib()
{
	this->readonly = readOnly;
	this->load_inmemory = load_inmemory;
	this->libelleNotGeo = libelleNotGeo;
}


CSqlLibExplorer::~CSqlLibExplorer()
{
}


////////////////////////////////////////////////////////////////////////////////
// Function: InitDatabase()
//
// Description: Open a connection to database, 
//				then create a session object.
//
// Returns: NOERROR if succesfull
//
// Notes:
//
////////////////////////////////////////////////////////////////////////////////
bool CSqlLibExplorer::InitDatabase(const wxString& lpFilename)
{
	int hr; // Error code reporting

	if (!wxFileExists(lpFilename))
	{
		hr = CreateDatabase(lpFilename, load_inmemory);
	}
	else
	{
		OpenConnection(lpFilename, false, load_inmemory);
		ExecuteSQLWithNoResult("VACUUM;");
		hr = ExecuteSQLWithNoResult("PRAGMA auto_vacuum = FULL;");
	}

	return (hr != -1);
}

bool CSqlLibExplorer::CheckVersion(const wxString& lpFilename)
{
	int hr = -1;
	if (wxFileExists(lpFilename))
	{
		hr = ExecuteSQLWithNoResult("PRAGMA auto_vacuum = FULL");

		CSqlVersion sqlVersion;
		if (sqlVersion.GetVersion() == "2.0.0.2")
		{
			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOGPS_TABLE);
			//hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSSEARCH_TABLE);
			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSWIHOUTTHUMBNAIL_TABLE);
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.2.0.0");
		}

		if (sqlVersion.GetVersion() == "2.2.0.0")
		{
			hr = ExecuteSQLWithNoResult(SQL_DROP_PHOTOSSEARCH);
			//hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSSEARCH_TABLE);
			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSWIHOUTTHUMBNAIL_TABLE);
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.4.0.0");
		}

		if (sqlVersion.GetVersion() == "2.4.0.0")
		{
			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSWIHOUTTHUMBNAIL_TABLE);
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.5.0.0");
		}

		if (sqlVersion.GetVersion() == "2.5.0.0")
		{
			hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_PHOTO_TABLE);
			//hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_DESCRIPTOR_TABLE);
			hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_RECOGNITION_TABLE);
			hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_NAME_TABLE);
			hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_PROCESSING_TABLE);
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (4,2,'Personne');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (4,1,'Face Name');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (4,3,'Gente');");
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.11.0.0");
			//sqlVersion.UpdateVersion("2.5.0.0", "2.11.0.0");
		}

		if (sqlVersion.GetVersion() == "2.11.0.0")
		{
			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTO_GPS_TABLE);
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.13.0.0");
		}

		if (sqlVersion.GetVersion() == "2.13.0.0")
		{
			hr = ExecuteSQLWithNoResult("ALTER TABLE PHOTOS ADD COLUMN Process INT;");
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.14.0.0");
			hr = ExecuteSQLWithNoResult("UPDATE PHOTOS SET Process = 0");
			hr = ExecuteSQLWithNoResult("UPDATE PHOTOS SET Process = 1 where CriteriaInsert = 1");
		}

		if (sqlVersion.GetVersion() == "2.14.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.15.0.0");
			hr = ExecuteSQLWithNoResult(SQL_CREATE_VIDEOTHUMBNAIL_TABLE);
		}

		if (sqlVersion.GetVersion() == "2.15.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.15.1.0");
			hr = ExecuteSQLWithNoResult("ALTER TABLE PHOTOS ADD COLUMN ExtensionId INT;");
			//Update Photos Folder
			CSqlFindPhotos findPhotos;
			findPhotos.UpdatePhotosExtension();

			CSqlThumbnail sqlThumbnail;
			PhotosVector photosVector;
			findPhotos.GetAllVideo(&photosVector);
			CLibPicture libPicture;
			for (CPhotos photo : photosVector)
			{
				if (libPicture.TestIsVideo(photo.GetPath()) || libPicture.TestIsPDF(photo.GetPath()) || libPicture.
					TestIsAnimation(photo.GetPath()))
					sqlThumbnail.DeleteThumbnail(photo.GetId());
			}
		}

		if (sqlVersion.GetVersion() == "2.15.1.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.16.0.0");
			hr = ExecuteSQLWithNoResult(SQL_CREATE_OPENCLKERNEL_TABLE);
		}

		if (sqlVersion.GetVersion() == "2.16.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.17.0.0");
			hr = ExecuteSQLWithNoResult("DROP TABLE OPENCLKERNEL");
			hr = ExecuteSQLWithNoResult(SQL_CREATE_OPENCLKERNEL_TABLE);
		}

		if (sqlVersion.GetVersion() == "2.17.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.18.0.0");
			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTO_CATEGORIE_USENET_PROCESSING_TABLE);
		}

		if (sqlVersion.GetVersion() == "2.18.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.33.0.0");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (5,2,'Categorie');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (5,1,'Category');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (5,3,'Categoria');");
		}

		if (sqlVersion.GetVersion() == "2.33.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.60.0.0");
			hr = ExecuteSQLWithNoResult("UPDATE FACEPHOTO SET Pertinence = 0.7");
		}

		if (sqlVersion.GetVersion() == "2.60.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.62.0.0");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (6,2,'Etoiles');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (6,1,'Stars');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (6,3,'Estrella');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (7,2,'Mots Cles');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (7,1,'Keyword');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (7,3,'Palabra llave');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'1 Star');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'2 Star');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'3 Star');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'4 Star');");
			hr = ExecuteSQLWithNoResult(
				"INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'5 Star');");
		}

		if (sqlVersion.GetVersion() == "2.62.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.64.0.0");
			hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_VIDEO_TABLE);
		}

		if (sqlVersion.GetVersion() == "2.64.0.0")
		{
			hr = ExecuteSQLWithNoResult("ALTER TABLE PHOTOS ADD COLUMN Multifiles INT;");


			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.65.0.0");

			PhotosVector photosVector;
			CSqlFindPhotos findPhotos;
			findPhotos.GetAllVideo(&photosVector);
			CLibPicture libPicture;
			for (CPhotos photo : photosVector)
			{
				wxString fullpath = photo.GetPath();
				fullpath.Replace("'", "''");

				if (libPicture.TestIsVideo(photo.GetPath()) || libPicture.TestIsPDF(photo.GetPath()) || libPicture.
					TestIsAnimation(photo.GetPath()))
					ExecuteSQLWithNoResult("UPDATE PHOTOS SET Multifiles = 1 WHERE FullPath = '" + fullpath + "'");
				else
					ExecuteSQLWithNoResult("UPDATE PHOTOS SET Multifiles = 0 WHERE FullPath = '" + fullpath + "'");
			}

			hr = ExecuteSQLWithNoResult(SQL_DROP_PHOTOSTHUMBNAIL);
			hr = ExecuteSQLWithNoResult(SQL_DROP_VIDEOTHUMBNAIL);
			hr = ExecuteSQLWithNoResult(SQL_DROP_FACE_PHOTO);

			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSTHUMBNAIL_TABLE);
			hr = ExecuteSQLWithNoResult(SQL_CREATE_VIDEOTHUMBNAIL_TABLE);
			hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_PHOTO_TABLE);


			hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOFOLDER);
			//hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOSSEARCH);
			hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOSTHUMBNAIL);
			hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOGPS);
		}
		if (sqlVersion.GetVersion() == "2.65.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.66.0.0");
			hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTO_EXIF_TABLE);
			wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
			documentPath.append("\\Face");
#else
			documentPath.append("/Face");
#endif
			wxArrayString files;
			wxDir::GetAllFiles(documentPath, &files, wxEmptyString, wxDIR_FILES);
			CLibPicture libPicture;

			for (wxString file : files)
			{
				if (libPicture.TestImageFormat(file) != 0)
				{
					CFiltreEffetCPU::LoadAndRotate(file, 180);
				}
			}
		}
		if (sqlVersion.GetVersion() == "2.66.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.67.0.0");
			hr = ExecuteSQLWithNoResult(SQL_CREATE_SEARCH_VIEW_VIEW);
		}
		if (sqlVersion.GetVersion() == "2.67.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.68.0.0");
			hr = ExecuteSQLWithNoResult("DROP TABLE PHOTOSSEARCHCRITERIA");
		}
		if (sqlVersion.GetVersion() == "2.68.0.0")
		{
			sqlVersion.DeleteVersion();
			sqlVersion.InsertVersion("2.69.0.0");
			hr = ExecuteSQLWithNoResult("DROP TABLE FACEDESCRIPTOR");
		}
	}
	return hr;
}


////////////////////////////////////////////////////////////////////////////////
// Function: CreateDatabase
//
// Description:
//		Create Northwind Database through OLE DB
//		Create Employees table
//
// Returns: NOERROR if succesfull
//
////////////////////////////////////////////////////////////////////////////////
bool CSqlLibExplorer::CreateDatabase(const wxString& databasePath, const bool& load_inmemory)
{
	wxString libelle;
	wxString query;

	if (!OpenConnection(databasePath, false, load_inmemory))
		return false;

	int hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOFOLDER_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	// Create RESULT SEARCH table
	hr = ExecuteSQLWithNoResult(SQL_CREATE_VERSION_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	// Create RESULT SEARCH table
	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOGPS_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSTHUMBNAIL_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_VIDEOTHUMBNAIL_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	// Create RESULT SEARCH table
	//hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSSEARCH_TABLE);
	//if (hr == -1)
	//{
	//	goto Exit;
	//}

	// Create LANGUE table
	//
	hr = ExecuteSQLWithNoResult(SQL_CREATE_LANGUE_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	BeginTransaction();

	hr = ExecuteSQLWithNoResult("INSERT INTO VERSION (libelle) VALUES ('2.69.0.0');");
	if (hr == -1)
	{
		goto Exit;
	}


	hr = ExecuteSQLWithNoResult("INSERT INTO LANGUE (NumLangue, LibelleLangue) VALUES (1,'FRANCAIS');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO LANGUE (NumLangue, LibelleLangue) VALUES (2,'ENGLISH');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO LANGUE (NumLangue, LibelleLangue) VALUES (3,'SPANISH');");
	if (hr == -1)
	{
		goto Exit;
	}

	CommitTransection();

	// Create CATEGORIE table
	//
	hr = ExecuteSQLWithNoResult(SQL_CREATE_CATALOG_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}


	hr = ExecuteSQLWithNoResult("INSERT INTO CATALOG (LibelleCatalog) VALUES ('My Photo');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_FOLDERCATALOG_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	// Create CATEGORIE table
	//
	hr = ExecuteSQLWithNoResult(SQL_CREATE_CATEGORIE_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	BeginTransaction();

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (1,2,'Geographique');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (1,1,'Geographical');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (1,3,'Geografica');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (2,2,'Repertoire');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (2,1,'Folder');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (2,3,'Repertoria');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (3,2,'Date');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (3,1,'Date');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (3,3,'Fetcha');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (4,2,'Personne');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (4,1,'Face Name');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (4,3,'Gente');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (5,2,'Categorie');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (5,1,'Category');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (5,3,'Categoria');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (6,2,'Etoiles');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (6,1,'Stars');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (6,3,'Estrella');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (7,2,'Mots Cles');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (7,1,'Keyword');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO CATEGORIE (NumCategorie, NumLangue, Libelle) VALUES (7,3,'Palabra llave');");
	if (hr == -1)
	{
		goto Exit;
	}

	CommitTransection();


	// Create CATEGORIE table
	//
	hr = ExecuteSQLWithNoResult(SQL_CREATE_COUNTRY_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	BeginTransaction();

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (1,'A1','None','Anonymous Proxy');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (2,'A2','None','Satellite Provider');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (3,'AD','Europa','Andorra');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (4,'AE','Asia','United Arab Emirates');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (5,'AF','Asia','Afghanistan');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (6,'AG','America','Antigua and Barbuda');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (7,'AI','America','Anguilla');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (8,'AL','Europa','Albania');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (9,'AM','Asia','Armenia');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (10,'AN','Europa','Netherlands Antilles');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (11,'AO','Africa','Angola');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (12,'AP','Asia','Asia / Pacific Region');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (13,'AQ','Antarctica','Antarctica');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (14,'AR','America','Argentina');");
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (15,'AS','America','American Samoa');");
	if (hr == -1)
		goto Exit;
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (16,'AT','Europa','Austria');");
	if (hr == -1)
		goto Exit;
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (17,'AU','Oceania','Australia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (18,'AW','America','Aruba');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (19,'AX','Europa','Aland Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (20,'AZ','Asia','Azerbaijan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (21,'BA','Europa','Bosnia and Herzegovina');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (22,'BB','America','Barbados');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (23,'BD','Asia','Bangladesh');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (24,'BE','Europa','Belgium');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (25,'BF','Africa','Burkina Faso');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (26,'BG','Europa','Bulgaria');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (27,'BH','Asia','Bahrain');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (28,'BI','Africa','Burundi');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (29,'BJ','Africa','Benin');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (30,'BM','America','Bermuda');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (31,'BN','Asia','Brunei Darussalam');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (32,'BO','America','Bolivia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (33,'BR','America','Brazil');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (34,'BS','America','Bahamas');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (35,'BT','Asia','B hutan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (36,'BV','Africa','Bouvet Island');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (37,'BW','Africa','Botswana');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (38,'BY','Europa','Belarus');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (39,'BZ','America','Belize');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (40,'CA','America','Canada');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (41,'CC','Asia','Cocos(Keeling) Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (42,'CD','Africa','Congo, The Democratic Republic of the');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (43,'CF','Africa','Central African Republic');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (44,'CG','Africa','Congo');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (45,'CH','Europa','Switzerland');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (46,'CI','Africa','Cote d''Ivoire');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (47,'CK','Oceania','Cook Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (48,'CL','America','Chile');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (49,'CM','Africa','Cameroon');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (50,'CN','Asia','China');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (51,'CO','America','Colombia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (52,'CR','America','Costa Rica');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (53,'CU','America','Cuba');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (54,'CV','Africa','Cape Verde');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (55,'CX','Asia','Christmas Island');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (56,'CY','Europa','Cyprus');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (57,'CZ','Europa','Czech Republic');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (58,'DE','Europa','Germany');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (59,'DJ','Africa','Djibouti');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (60,'DK','Europa','Denmark');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (61,'DM','America','Dominica');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (62,'DO','America','Dominican Republic');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (63,'DZ','Africa','Algeria');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (64,'EC','America','Ecuador');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (65,'EE','Europa','Estonia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (66,'EG','Africa','Egypt');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (67,'EH','Africa','Western Sahara');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (68,'ER','Africa','Eritrea');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (69,'ES','Europa','Spain');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (70,'ET','Africa','Ethiopia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (71,'EU','Europa','Europa');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (72,'FI','Europa','Finland');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (73,'FJ','Oceania','Fiji');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (74,'FK','America','Falkland Islands(Malvinas)');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (75,'FM','Oceania','Micronesia, Federated States of');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (76,'FO','Europa','Faroe Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (77,'FR','Europa','France');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (78,'GA','Africa','Gabon');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (79,'GB','Europa','United Kingdom');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (80,'GD','Europa','Grenada');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (81,'GE','Europa','Georgia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (82,'GF','Africa','French Guiana');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (83,'GG','Europa','Guernsey');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (84,'GH','Africa','Ghana');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (85,'GI','Europa','Gibraltar');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (86,'GL','Europa','Greenland');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (87,'GM','Africa','Gambia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (88,'GN','Africa','Guinea');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (89,'GP','America','Guadeloupe');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (90,'GQ','Africa','Equatorial Guinea');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (91,'GR','Europa','Greece');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (92,'GS','America','South Georgia and the South Sandwich Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (93,'GT','America','Guatemala');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (94,'GU','Asia','Guam');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (95,'GW','Africa','Guinea - Bissau');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (96,'GY','America','Guyana');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (97,'HK','Asia','Hong Kong');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (98,'HM','Africa','Heard Island and McDonald Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (99,'HN','America','Honduras');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (100,'HR','Europa','Croatia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (101,'HT','America','Haiti');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (102,'HU','Europa','Hungary');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (103,'ID','Europa','Indonesia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (104,'IE','Europa','Ireland');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (105,'IL','Asia','Israel');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (106,'IM','Europa','Isle of Man');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (107,'IN','Asia','India');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (108,'IO','Asia','British Indian Ocean Territory');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (109,'IQ','Asia','Iraq');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (110,'IR','Asia','Iran, Islamic Republic of');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (111,'IS','Europa','Iceland');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (112,'IT','Europa','Italy');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (113,'JE','Europa','Jersey');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (114,'JM','America','Jamaica');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (115,'JO','Asia','Jordan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (116,'JP','Asia','Japan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (117,'KE','Africa','Kenya');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (118,'KG','Asia','Kyrgyzstan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (119,'KH','Asia','Cambodia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (120,'KI','Oceania','Kiribati');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (121,'KM','Africa','Comoros');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (122,'KN','America','Saint Kitts and Nevis');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (123,'KP','Asia','Korea, Democratic People''s Republic of');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (124,'KR','Asia','Korea, Republic of');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (125,'KW','Asia','Kuwait');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (126,'KY','America','Cayman Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (127,'KZ','Asia','Kazakhstan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (128,'LA','Asia','Lao People''s Democratic Republic');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (129,'LB','Asia','Lebanon');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (130,'LC','America','Saint Lucia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (131,'LI','Europa','Liechtenstein');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (132,'LK','Asia','Sri Lanka');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (133,'LR','Africa','Liberia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (134,'LS','Africa','Lesotho');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (135,'LT','Europa','Lithuania');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (136,'LU','Europa','Luxembourg');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (137,'LV','Europa','Latvia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (138,'LY','Asia','Libyan Arab Jamahiriya');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (139,'MA','Africa','Morocco');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (140,'MC','Europa','Monaco');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (141,'MD','Europa','Moldova, Republic of');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (142,'ME','Europa','Montenegro');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (143,'MG','Africa','Madagascar');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (144,'MH','Oceania','Marshall Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (145,'MK','Europa','Macedonia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (146,'ML','Africa','Mali');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (147,'MM','Asia','Myanmar');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (148,'MN','Asia','Mongolia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (149,'MO','Asia','Macao');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (150,'MP','Oceania','Northern Mariana Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (151,'MQ','America','Martinique');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (152,'MR','Africa','Mauritania');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (153,'MS','Europa','Montserrat');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (154,'MT','Europa','Malta');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (155,'MU','Africa','Mauritius');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (156,'MV','Asia','Maldives');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (157,'MW','Africa','Malawi');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (158,'MX','America','Mexico');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (159,'MY','Asia','Malaysia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (160,'MZ','Africa','Mozambique');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (161,'NA','Africa','Namibia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (162,'NC','Ocenaia','New Caledonia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (163,'NE','Africa','Niger');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (164,'NF','Europa','Norfolk Island');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (165,'NG','Africa','Nigeria');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (166,'NI','America','Nicaragua');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (167,'NL','Europa','Netherlands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (168,'NO','Europa','Norway');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (169,'NP','Asia','Nepal');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (170,'NR','Oceania','Nauru');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (171,'NU','Oceania','Niue');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (172,'NZ','Oceania','New Zealand');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (173,'OM','Asia','Oman');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (174,'PA','America','Panama');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (175,'PE','America','Peru');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (176,'PF','Oceania','French Polynesia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (177,'PG','Oceania','Papua New Guinea');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (178,'PH','Asia','Philippines');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (179,'PK','Asia','Pakistan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (180,'PL','Europa','Poland');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (181,'PM','America','Saint Pierre and Miquelon');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (182,'PN','Oceania','Pitcairn');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (183,'PR','America','Puerto Rico');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (184,'PS','Asia','Palestinian Territory');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (185,'PT','Europa','Portugal');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (186,'PW','Asia','Palau');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (187,'PY','America','Paraguay');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (188,'QA','Asia','Qatar');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (189,'RE','Africa','Reunion');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (190,'RO','Europa','Romania');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (191,'RS','Europa','Serbia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (192,'RU','Europa','Russian Federation');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (193,'RW','Africa','Rwanda');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (194,'SA','Asia','Saudi Arabia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (195,'SB','Asia','Solomon Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (196,'SC','Africa','Seychelles');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (197,'SD','Africa','Sudan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (198,'SE','Europa','Sweden');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (199,'SG','Asia','Singapore');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (200,'SH','Africa','Saint Helena');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (201,'SI','Europa','Slovenia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (202,'SJ','Europa','Svalbard and Jan Mayen');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (203,'SK','Europa','Slovakia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (204,'SL','Africa','Sierra Leone');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (205,'SM','Europa','San Marino');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (206,'SN','Africa','Senegal');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (207,'SO','Africa','Somalia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (208,'SR','America','Suriname');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (209,'ST','Africa','Sao Tome and Principe');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (210,'SV','America','El Salvador');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (211,'SY','Asia','Syrian Arab Republic');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (212,'SZ','Africa','Swaziland');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (213,'TC','America','Turks and Caicos Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (214,'TD','Africa','Chad');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (215,'TF','Asia','French Southern Territories');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (216,'TG','Africa','Togo');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (217,'TH','Asia','Thailand');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (218,'TJ','Asia','Tajikistan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (219,'TK','Oceania','Tokelau');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (220,'TL','Oceania','Timor - Leste');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (221,'TM','Asia','Turkmenistan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (222,'TN','Africa','Tunisia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (223,'TO','Oceania','Tonga');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (224,'TR','Europa','Turkey');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (225,'TT','Oceania','Trinidad and Tobago');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (226,'TV','Oceania','Tuvalu');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (227,'TW','Asia','Taiwan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (228,'TZ','Africa','Tanzania, United Republic of');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (229,'UA','Europa','Ukraine');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (230,'UG','Africa','Uganda');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (231,'UM','Oceania','United States Minor Outlying Islands');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (232,'US','America','United States');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (233,'UY','America','Uruguay');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (234,'UZ','Asia','Uzbekistan');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (235,'VA','Europa','Holy See(Vatican City State)');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (236,'VC','America','Saint Vincent and the Grenadines');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (237,'VE','America','Venezuela');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (238,'VG','Oceania','Virgin Islands, British');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (239,'VI','Oceania','Virgin Islands, U.S.');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (240,'VN','Asia','Vietnam');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (241,'VU','Oceania','Vanuatu');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (242,'WF','Oceania','Wallis and Futuna');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (243,'WS','Oceania','Samoa');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (244,'YE','Africa','Yemen');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (245,'YT','Africa','Mayotte');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (246,'ZA','Africa','South Africa');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (247,'ZM','Africa','Zambia');");
	hr = ExecuteSQLWithNoResult(
		"INSERT INTO COUNTRY (NumCountry, CodeCountry, LibelleContinent, LibelleCountry) VALUES (248,'ZW','Africa','Zimbabwe');");

	CommitTransection();

	// Create CATALOG ICONEFILE table
	//
	hr = ExecuteSQLWithNoResult(SQL_CREATE_CRITERIA_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult("INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'1 Star');");
	hr = ExecuteSQLWithNoResult("INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'2 Star');");
	hr = ExecuteSQLWithNoResult("INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'3 Star');");
	hr = ExecuteSQLWithNoResult("INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'4 Star');");
	hr = ExecuteSQLWithNoResult("INSERT INTO CRITERIA (NumCatalog,NumCategorie,Libelle) VALUES (1,6,'5 Star');");

	//Ajout de la valeur Non geolocalise
	libelle = libelleNotGeo; //CLibResource::LoadStringFromResource("LBLNOTGEO",1);
	query = "INSERT INTO CRITERIA (NumCatalog, NumCategorie, Libelle) VALUES (1,1,'";
	query.append(libelle.begin(), libelle.end());
	query.append("');");
	hr = ExecuteSQLWithNoResult(query);
	if (hr == -1)
	{
		goto Exit;
	}

	// Create ALBUM table
	//
	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOS_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	// Create Catalog table
	//
	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSCRITERIA_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTOSWIHOUTTHUMBNAIL_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_PHOTO_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_VIDEO_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_RECOGNITION_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_NAME_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_FACE_PROCESSING_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTO_GPS_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}


	hr = ExecuteSQLWithNoResult(SQL_CREATE_OPENCLKERNEL_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTO_CATEGORIE_USENET_PROCESSING_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_CREATE_PHOTO_EXIF_TABLE);
	if (hr == -1)
	{
		goto Exit;
	}

	hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTO_EXIF);
	hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOFOLDER);
	//hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOSSEARCH);
	hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOSTHUMBNAIL);
	hr = ExecuteSQLWithNoResult(SQL_INDEX_PHOTOGPS);

	hr = ExecuteSQLWithNoResult(SQL_CREATE_SEARCH_VIEW_VIEW);

Exit:


	return (hr != -1);
}
