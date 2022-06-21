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
    int w = 0, h = 0;
    int width = buttonWidth;
    int height = buttonHeight;

    if (width != height)
    {
        if (width > height)
        {
            width = height;
        }
        else
        {
            height = width;
        }
    }

    //Calcul Ratio picture
    bool isError = false;

    {
        NSVGimage* image = NULL;
        image = nsvgParseFromFile(CConvertUtility::ConvertToStdString(filename).c_str(), "px", 96.0f, 0, 0);
        if (image == NULL)
        {
            isError = true;
            printf("Could not open SVG image.\n");
        }

        if (!isError)
        {
            w = (int)image->width;
            h = (int)image->height;

        }

        nsvgDelete(image);
    }

    float ratio = 1.0f;
    if (w != h)
    {
        if (w > h)
        {
            ratio = (float)h / (float)w;
            height = width * ratio;
        }
        else
        {
            ratio = (float)w / (float)h;
            width = height * ratio;
        }
    }

    wxImage img;
    NSVGimage *image = NULL;
    NSVGrasterizer *rast = NULL;
    uint8_t * data = nullptr;
    image = nsvgParseFromFile(CConvertUtility::ConvertToStdString(filename).c_str(), "px", 96.0f, width, height);
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
        if (data == NULL) {
            printf("Could not alloc image buffer.\n");
            isError = true;
        }
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

        {
            if (data != nullptr)
            {

                tbb::parallel_for(0, height, 1, [=](int y) { //changed line
                    int pos_data = y * widthSrcSize;
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
                    }); //added ); at the end

            }
            delete[] data;
        }


        {
            int posX = (buttonWidth - w) / 2;
            int posY = (buttonHeight - h) / 2;
            wxImage final(buttonWidth, buttonHeight, true);
            final.InitAlpha();
            unsigned char* dataAlpha = final.GetAlpha();
            if (dataAlpha != nullptr)
            {
                tbb::parallel_for(0, buttonHeight, 1, [=](int y)
                {
                    int posAlpha = y * buttonWidth;
                    for (auto x = 0; x < buttonWidth; x++)
                        dataAlpha[posAlpha++] = 0;
                });


            }
            final.Paste(anImage, posX, posY);
            return final;
        }

    }
	return img;
}

wxImage CLibResource::CreatePictureFromSVG(const wxString& idName, const int& buttonWidth, const int& buttonHeight)
{
    CSqlResource sqlResource;
    wxString filename = sqlResource.GetFilepath(idName);
    return CreatePictureFromSVGFilename(filename, buttonWidth, buttonHeight);
}

wxString CLibResource::LoadExifNameFromResource(const wxString& id)
{
	CSqlResource sqlResource;
	wxString libelle = sqlResource.GetExifLibelle(id);
	return libelle;
}

wxString CLibResource::LoadBitmapFromResource(const wxString& idName)
{
	CSqlResource sqlResource;
	return sqlResource.GetBitmapResourcePath(idName);
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
