#include "SqlResource.h"
#include <lz4.h>
#include <jpgd.h>
#include <jpge.h>
#include <lz4.h>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
using namespace Regards::Sqlite;

CSqlResource::CSqlResource(CSqlLib * _sqlLibTransaction, const bool &useTransaction)
	: CSqlExecuteRequest(L"ResourceDB")
{
	this->_sqlLibTransaction = _sqlLibTransaction;
	this->useTransaction = useTransaction;
}


CSqlResource::~CSqlResource()
{
}

CPictureData * CSqlResource::GetBitmap(const wxString &idName)
{
	typeResult = 1;
	ExecuteRequest("SELECT idName, mimeType, width, height, depth, data FROM BitmapResource WHERE idName = '" + idName + "'");
	return memFile;
}

wxString CSqlResource::GetText(const wxString &idName)
{
	typeResult = 2;
	ExecuteRequest("SELECT idName, mimeType, size, data FROM TextResource WHERE idName = '" + idName + "'");
    return text;
}

wxString  CSqlResource::GetVector(const wxString &idName)
{
    typeResult = 4;
    ExecuteRequest("SELECT idName, size, data FROM VectorResource WHERE idName = '" + idName + "'");
    return text;
}

wxString CSqlResource::GetLibelle(const wxString &idName, const int &idLang)
{
	typeResult = 3;
	ExecuteRequest("SELECT idName, language, libelle FROM StringResource WHERE idName = '" + idName + "' and language = " + to_string(idLang));
	return libelle;
}

wxString CSqlResource::GetExifLibelle(const wxString &idName)
{
	typeResult = 5;
	ExecuteRequest("SELECT Name FROM exif WHERE id = '" + idName + "'");
	return libelle;
}


int CSqlResource::TraitementResultBitmap(CSqlResult * sqlResult)
{
	int nbResult = 0;
	wxString idName;
	wxString mimeType;
	//int depth;
	memFile = new CPictureData();

	while (sqlResult->Next())
	{
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			//id, mimeType, width, height, size, data
			switch (i)
			{
			case 0:
				idName = sqlResult->ColumnDataText(i);
				break;

			case 1:
				mimeType = sqlResult->ColumnDataText(i);
				break;

			case 2:
				memFile->width = sqlResult->ColumnDataInt(i);
				break;

			case 3:
				memFile->height = sqlResult->ColumnDataInt(i);
				break;

			case 4:
				//depth = sqlResult->ColumnDataInt(i);
				break;

			case 5:
				{
					memFile->size = sqlResult->ColumnDataBlobSize(i);
					if (memFile->size > 0)
					{
						
						//const int req_comps = 4;
						//int actual_comps = 4;
						int sizeBuffer = memFile->width * memFile->height * 4;
						if(sizeBuffer != memFile->size)
							assert("error");
						memFile->data = new uint8_t[sizeBuffer];
						sqlResult->ColumnDataBlob(i, (void * &)memFile->data, sizeBuffer);
					}
					break;
				}
			}
		}
		nbResult++;
	}
	return nbResult;
}

int CSqlResource::TraitementResultText(CSqlResult * sqlResult)
{
	int nbResult = 0;
	int size_file = 0;
	wxString idName;
	wxString mimeType;
	while (sqlResult->Next())
	{
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				idName = sqlResult->ColumnDataText(i);
				break;

			case 1:
				mimeType = sqlResult->ColumnDataText(i);
				break;

			case 2:
				size_file = sqlResult->ColumnDataInt(i);
				break;

			case 3:
				int size = sqlResult->ColumnDataBlobSize(i);
				if (size > 0)
				{
					int k = 0;
					char * out = new char[size_file+1];
					const char * data = (const char *)sqlResult->ColumnDataBlob(i);
					LZ4_decompress_safe(data, out, size, size_file);
					out[size_file] = '\0';
					for(k = 0;k < size_file;k++)
					{
						if(out[k] >= 0)
							text.append(out[k]);
					}
					delete[] out;
				}
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
}


int CSqlResource::TraitementResultVector(CSqlResult * sqlResult)
{
    int nbResult = 0;
    int size_file = 0;
    wxString idName;
    wxString mimeType;
    while (sqlResult->Next())
    {
        for (int i = 0; i < sqlResult->GetColumnCount(); i++)
        {
            switch (i)
            {
                case 0:
                    idName = sqlResult->ColumnDataText(i);
                    break;
                    
                case 1:
                    size_file = sqlResult->ColumnDataInt(i);
                    break;
                    
                case 2:
                    int size = sqlResult->ColumnDataBlobSize(i);
                    if (size > 0)
                    {
							int k = 0;
							char * out = new char[size_file+1];
							const char * data = (const char *)sqlResult->ColumnDataBlob(i);
							LZ4_decompress_safe(data, out, size, size_file);
							out[size_file] = '\0';
							for(k = 0;k < size_file;k++)
							{
								if(out[k] >= 0)
									text.append(out[k]);
							}
							delete[] out;
                    }
                    break;
            }
        }
        nbResult++;
    }
    return nbResult;
}

int CSqlResource::TraitementResultLibelle(CSqlResult * sqlResult)
{
	int nbResult = 0;
	wxString idName;
	int langid;
	while (sqlResult->Next())
	{
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				idName = sqlResult->ColumnDataText(i);
				break;
			case 1:
				langid = sqlResult->ColumnDataInt(i);
				break;
			case 2:
				libelle = sqlResult->ColumnDataText(i);
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
}

int CSqlResource::TraitementResultExif(CSqlResult * sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		for (int i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				libelle = sqlResult->ColumnDataText(i);
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
}

int CSqlResource::TraitementResult(CSqlResult * sqlResult)
{
	int nbResult = 0;
	switch (typeResult)
	{
	case 1:
		nbResult = TraitementResultBitmap(sqlResult);
		break;

	case 2:
		nbResult = TraitementResultText(sqlResult);
		break;

	case 3:
		nbResult = TraitementResultLibelle(sqlResult);
		break;
            
    case 4:
        nbResult = TraitementResultVector(sqlResult);
        break;

	case 5:
		nbResult = TraitementResultExif(sqlResult);
		break;

	}

	return nbResult;
}