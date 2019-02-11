#include "SqlResource.h"
#include <lz4.h>
#include <jpgd.h>
#include <jpge.h>
#include <lz4.h>
#include <RegardsBitmap.h>
#include <libPicture.h>
#include <fstream>
#include <ImageLoadingFormat.h>
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


void CSqlResource::InsertBitmap(const wstring &idName, const wstring &mimeType, const wstring &filename, const bool &flip)
{
	CLibPicture libPicture;
	CImageLoadingFormat * loadPicture = libPicture.LoadPicture(filename);
	if(loadPicture != nullptr && loadPicture->IsOk())
	{
		CRegardsBitmap * bitmap = loadPicture->GetRegardsBitmap();
		wchar_t _pwszRequeteSQL[512];
		swprintf(_pwszRequeteSQL, 512, L"INSERT INTO BitmapResource (idName, mimeType, width, height, depth, data) VALUES('%s', '%s', %d, %d, %d, ?)", idName.c_str(), mimeType.c_str(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetBitmapDepth());

		int size = bitmap->GetBitmapSize();
		if (flip)
			bitmap->VertFlipBuf();
		//uint8_t * dest = new uint8_t[size];
		//int outputsize = size;
		//bool result = jpge::compress_image_to_jpeg_file_in_memory(dest, outputsize, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), 4, bitmap->GetPtBitmap());
		ExecuteInsertBlobData(_pwszRequeteSQL, 5, bitmap->GetPtBitmap(), size);
		//delete[] dest;
		delete bitmap;
		delete loadPicture;
	}
}

string CSqlResource::readFileBytes(const string &name)
{
	std::ifstream t(name);
	std::string str;
	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	t.close();
	return str;
}

void CSqlResource::InsertText(const wstring &idName, const wstring &mimeType, const wstring &filename)
{
	string file;
	file.assign(filename.begin(), filename.end());
	string data = readFileBytes(file);
	char * out = new char[data.size()];
	int taille = LZ4_compress(data.c_str(), out, data.size());
	wchar_t _pwszRequeteSQL[512];
	swprintf(_pwszRequeteSQL, 512, L"INSERT INTO TextResource (idName, mimeType, size, data) VALUES('%s', '%s', %d, ?)", idName.c_str(), mimeType.c_str(), data.size());
	ExecuteInsertBlobData(_pwszRequeteSQL, 3, out, taille);
	
	delete[] out;
}

void CSqlResource::InsertVector(const wstring &idName, const wstring &filename)
{
	string file;
	file.assign(filename.begin(), filename.end());
	string data = readFileBytes(file);
	char * out = new char[data.size()];
	int taille = LZ4_compress(data.c_str(), out, data.size());
	wchar_t _pwszRequeteSQL[512];
	swprintf(_pwszRequeteSQL, 512, L"INSERT INTO VectorResource (idName, size, data) VALUES('%s', %d, ?)", idName.c_str(), data.size());
	ExecuteInsertBlobData(_pwszRequeteSQL, 2, out, taille);

	delete[] out;
}

void CSqlResource::InsertLibelle(const wstring &idName, const wstring &libelle, const int &lang)
{
	wchar_t _pwszRequeteSQL[512];
	swprintf(_pwszRequeteSQL, 512, L"Insert into StringResource (idName, language, libelle) VALUES ('%s', %d,'%s')", idName.c_str(), lang, libelle.c_str());
	ExecuteRequestWithNoResult(_pwszRequeteSQL);
}


CPictureData * CSqlResource::GetBitmap(const wxString &idName)
{
	typeResult = 1;
	ExecuteRequest("SELECT idName, mimeType, width, height, depth, data FROM BitmapResource WHERE idName = '" + idName + "'");
	return memFile;
}

wxString CSqlResource::GetText(const wxString &idName)
{
	typeResult = 1;
	ExecuteRequest("SELECT idName, mimeType, size, data FROM TextResource WHERE idName = '" + idName + "'");
    return text;
}

wxString  CSqlResource::GetVector(const wxString &idName)
{
    typeResult = 1;
    ExecuteRequest("SELECT idName, size, data FROM VectorResource WHERE idName = '" + idName + "'");
    return text;
}

int CSqlResource::GetExtensionId(const wxString &extension)
{
    typeResult = 4;
    ExecuteRequest("SELECT pictureId FROM FileExtension WHERE extension = '" + extension + "'");
    return result;
}

wxString CSqlResource::GetLibelle(const wxString &idName, const int &idLang)
{
	typeResult = 1;
	ExecuteRequest("SELECT idName, language, libelle FROM StringResource WHERE idName = '" + idName + "' and language = " + to_string(idLang));
	return libelle;
}

wxString CSqlResource::GetExifLibelle(const wxString &idName)
{
	typeResult = 1;
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
        if(typeResult == 4)
        {
            for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
            {
                switch (i)
                {
                    case 0:
                        result = sqlResult->ColumnDataInt(i);
                        break;
                }
            }
        }
        else
        {
            for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
                    result = sqlResult->ColumnDataInt(i);
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
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
        for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
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
