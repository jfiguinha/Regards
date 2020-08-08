#include <header.h>
#include "SqlResult.h"
#include "SqlFaceDescriptor.h"
#include <FaceDescriptor.h>
using namespace Regards::Sqlite;


CSqlFaceDescriptor::CSqlFaceDescriptor()
	: CSqlExecuteRequest(L"RegardsDB")
{
	faceDescriptor = nullptr;
}


CSqlFaceDescriptor::~CSqlFaceDescriptor()
{
}

//--------------------------------------------------------
//Chargement de toutes les données d'un album
//--------------------------------------------------------
int CSqlFaceDescriptor::InsertFaceDescriptor(const int& numFace, const char* zBlob, const int& nBlob)
{
	ExecuteInsertBlobData("INSERT INTO FACEDESCRIPTOR (NumFace, FaceDescriptor) VALUES(" + to_string(numFace) + ", ? )",
	                      2, zBlob, nBlob);
	return 0;
}

CFaceDescriptor* CSqlFaceDescriptor::GetFaceDescriptor(const int& numFace)
{
	ExecuteRequest("SELECT NumFace, FaceDescriptor FROM FACEDESCRIPTOR WHERE NumFace = " + to_string(numFace));
	return faceDescriptor;
}

bool CSqlFaceDescriptor::DeleteFaceDescriptorDatabase()
{
	return (ExecuteRequestWithNoResult("DELETE FROM FACEDESCRIPTOR") != -1) ? true : false;
}

bool CSqlFaceDescriptor::DeleteFaceDescriptorDatabase(const int& numFace)
{
	return (ExecuteRequestWithNoResult("DELETE FROM FACEDESCRIPTOR WHERE NumFace = " + to_string(numFace)) != -1)
		       ? true
		       : false;
}

int CSqlFaceDescriptor::TraitementResult(CSqlResult* sqlResult)
{
	int nbResult = 0;
	while (sqlResult->Next())
	{
		for (auto i = 0; i < sqlResult->GetColumnCount(); i++)
		{
			switch (i)
			{
			case 0:
				numFace = sqlResult->ColumnDataInt(i);
				break;
			case 1:
				{
					int size = sqlResult->ColumnDataBlobSize(i);
					if (size > 0)
					{
						faceDescriptor = new CFaceDescriptor();
						faceDescriptor->descriptor = new char[size];
						faceDescriptor->size = size;
						faceDescriptor->numFace = numFace;
						sqlResult->ColumnDataBlob(i, (void* &)faceDescriptor->descriptor, size);
					}
				}
				break;
			}
		}
		nbResult++;
	}
	return nbResult;
}
