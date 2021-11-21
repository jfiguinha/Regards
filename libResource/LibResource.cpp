// DllResource.cpp : définit les fonctions exportées pour l'application DLL.
//
#include "header.h"
#include "LibResource.h"
#include <SqlResource.h>
#include <SqlLibResource.h>
#include <SqlEngine.h>
#include <ConvertUtility.h>
#include <FileUtility.h>
#define NANOSVG_IMPLEMENTATION
#include <nanosvg.h>
#define NANOSVGRAST_IMPLEMENTATION
#include <nanosvgrast.h>
#include <wx/sstream.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::Sqlite;

wxString CLibResource::GetPhotoCancel()
{
#ifdef WIN32
	return CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
	return CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif
}

void CLibResource::InitializeSQLServerDatabase(const wxString& folder)
{
	auto libResource = new CSqlLibResource(true, true);
	wxString filename = folder;
#ifdef WIN32
	filename.append(L"\\resource.db");
#else
    filename.append(L"/resource.db");
#endif
	printf("ResourceDB %s \n", CConvertUtility::ConvertToUTF8(filename));
	CSqlEngine::Initialize(filename, L"ResourceDB", libResource);
}

void CLibResource::KillSqlEngine()
{
	CSqlEngine::kill(L"ResourceDB");
}

wxImage CLibResource::CreatePictureFromSVGFilename(const wxString& filename, const int& buttonWidth, const int& buttonHeight)
{
    wxImage img;
    bool isError = false;
    NSVGimage *image = NULL;
    NSVGrasterizer *rast = NULL;
    int w, h;
    uint8_t * data = nullptr;
    image = nsvgParseFromFile(filename.ToStdString().c_str(), "px", 96.0f, buttonWidth, buttonHeight);
    if (image == NULL) 
    {
        isError = true;
        printf("Could not open SVG image.\n");
    }
    
    if(!isError)
    {
        w = (int)image->width;
        h = (int)image->height;
        rast = nsvgCreateRasterizer();        
    }

    if (rast == NULL) {
        isError = true;
        printf("Could not init rasterizer.\n");
    }

    if(!isError)
    {
        data = new uint8_t[w*h*4];
    }
        
    if (data == NULL) {
        printf("Could not alloc image buffer.\n");
        isError = true;
    }

    if(!isError)
        nsvgRasterize(rast, image, 0,0,1, data, w, h, w*4);

    nsvgDeleteRasterizer(rast);
    nsvgDelete(image);
    
    bool flip = false;
    
    if(!isError)
    {
        const int width = w;
        const int height = h;
        const int widthSrcSize = width * 4;
       
        wxImage anImage(width, height, true);
        anImage.InitAlpha();

        unsigned char* dataOut = anImage.GetData();
        unsigned char* dataAlpha = anImage.GetAlpha();

        if (data != nullptr)
        {
            int pos_data;
            for (auto y = 0; y < height; y++)
            {
                if (flip)
                    pos_data = y * widthSrcSize;
                else
                    pos_data = ((height - y) * widthSrcSize) - widthSrcSize;
                int posDataOut = y * (width * 3);
                int posAlpha = y * width;
                for (auto x = 0; x < width; x++)
                {
                    dataOut[posDataOut] = data[pos_data + 2];
                    dataOut[posDataOut + 1] = data[pos_data + 1];
                    dataOut[posDataOut + 2] = data[pos_data];
                    dataAlpha[posAlpha++] = data[pos_data + 3];
                    pos_data += 4;
                    posDataOut += 3;
                }
            }
        }
        delete[] data;
        return anImage;
    }
	return img;
}

wxImage CLibResource::CreatePictureFromSVG(const wxString& idName, const int& buttonWidth, const int& buttonHeight)
{
    wxString filename = GetVector(idName);
    return CreatePictureFromSVGFilename(filename, buttonWidth, buttonHeight);
}

wxString CLibResource::LoadExifNameFromResource(const wxString& id)
{
	CSqlResource sqlResource;
	wxString libelle = sqlResource.GetExifLibelle(id);
	return libelle;
}

CPictureData* CLibResource::LoadBitmapFromResource(const wxString& idName)
{
	CSqlResource sqlResource;
	return sqlResource.GetBitmap(idName);
}

wxString CLibResource::LoadStringFromResource(const wxString& idName, const int& idLang)
{
	CSqlResource sqlResource;
	int numLanguage = idLang;

	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		numLanguage = config->GetNumLanguage();

	wxString libelle = sqlResource.GetLibelle(idName, numLanguage);
	return libelle;
}

wxString CLibResource::GetVector(const wxString& idName)
{
	CSqlResource sqlResource;
	wxString libelle = sqlResource.GetVectorFromFile(idName);
	return libelle;
}

int CLibResource::GetExtensionId(const wxString& extension)
{
	CSqlResource sqlResource;
	int id = sqlResource.GetExtensionId(extension);
	return id;
}

wxString CLibResource::GetOpenGLShaderProgram(const wxString& idName)
{
	CSqlResource sqlResource;
	wxString program = sqlResource.GetOpenGLFromFile(idName);
	//printf("Program : %s \n",CConvertUtility::ConvertToUTF8(program));
	return program;
}

wxString CLibResource::GetOpenCLFloatProgram(const wxString& idName)
{
	CSqlResource sqlResource;
	wxString program = sqlResource.GetOpenCLFloatFromFile(idName);
	//printf("Program : %s \n",CConvertUtility::ConvertToUTF8(program));
	return program;
}

wxString CLibResource::GetOpenCLUcharProgram(const wxString& idName)
{
	CSqlResource sqlResource;
	wxString program = sqlResource.GetOpenCLUcharFromFile(idName);
	//printf("Program : %s \n",CConvertUtility::ConvertToUTF8(program));
	return program;
}
