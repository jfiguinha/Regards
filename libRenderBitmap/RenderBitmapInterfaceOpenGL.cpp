// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderBitmapInterfaceOpenGL.h"
#include <ConfigParam.h>
#include <GLSLShader.h>
#include <ParamInit.h>
#include <LoadingResource.h>
#include <FiltreEffet.h>
#include <RegardsConfigParam.h>
using namespace Regards::Render;



CRenderBitmapInterfaceOpenGL::CRenderBitmapInterfaceOpenGL()
{
	textureSource = nullptr;
	width = 0;
	height = 0;

}

CRenderBitmapInterfaceOpenGL::~CRenderBitmapInterfaceOpenGL()
{
	delete(textureSource);
}


float CRenderBitmapInterfaceOpenGL::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = (float)width / (float)(pictureWidth);
	else
		newRatio = (float)height / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > height)
	{
		newRatio = (float)height / (float)(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > width)
		{
			newRatio = (float)width / (float)(pictureWidth);
		}
	}

	return newRatio;
}

int CRenderBitmapInterfaceOpenGL::AddTextureBitmap(CRegardsBitmap * bitmap, const wxString &entry)
{
	bool find = false;
	for (vector<CTextureManager *>::const_iterator it = listTexture->begin(); it != listTexture->end(); it++)
	{
		COpenGLTextureManager * texture = (COpenGLTextureManager *)*it;
		if (texture->filename == entry)
		{
			texture->texture.Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
			find = true;
			break;
		}
	}

	if (!find)
	{
		COpenGLTextureManager * texture = new COpenGLTextureManager();
		texture->filename = entry;
		texture->texture.Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
		listTexture->push_back(texture);
	}


	return 0;
}

bool CRenderBitmapInterfaceOpenGL::TestIsTextureExist(const wxString &entry)
{
	bool find = false;
	for (vector<CTextureManager *>::const_iterator it = listTexture->begin(); it != listTexture->end(); it++)
	{
		CTextureManager * texture = *it;
		if (texture->filename == entry)
		{
			find = true;
			break;
		}
	}
	return find;
}


void CRenderBitmapInterfaceOpenGL::CreateScreenTexture(const int &width, const int &height)
{
	renderFromBitmap = 0;
	if (this->width != width || this->height != height)
	{

		if (textureSource != nullptr)
			delete(textureSource);

		textureSource = new GLTexture(width, height);
		this->width = width;
		this->height = height;
	}
}

void CRenderBitmapInterfaceOpenGL::CreateScreenRender(wxDC * deviceContext, const CRgbaquad &color)
{
	renderFromBitmap = 0;
	colorBackground = color;
    
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );   // Select the projection matrix
    glLoadIdentity();                // and reset it
    // Calculate the aspect ratio of the view
    gluPerspective( 45.0f, width / height, 0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix
    glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);

    
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_INIT");
    if(m_pShader == nullptr)
        return;
    
    //GLSLShader m_pShader;
    //m_pShader.CreateProgram(L"IDR_GLSL_SHADER_INIT", GL_FRAGMENT_PROGRAM_ARB);
    
    m_pShader->EnableShader();

    if (!m_pShader->SetParam("red", color.GetFRed()))
    {
        //TRACE( L"SetTexture failed" );
    }

    if (!m_pShader->SetParam("green", color.GetFGreen()))
    {
        //TRACE( L"SetTexture failed" );
    }
    
    if (!m_pShader->SetParam("blue", color.GetFBlue()))
    {
        //TRACE( L"SetTexture failed" );
    }

	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();
    
	RenderToTexture();
}



void CRenderBitmapInterfaceOpenGL::RenderQuad()
{
#if defined(__APPLE__)
    
    glPushMatrix();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat vertices[] = {
        0, 0,
        static_cast<GLfloat>(width), 0,
        static_cast<GLfloat>(width), static_cast<GLfloat>(height),
        0, static_cast<GLfloat>(height) };
    GLfloat texVertices[] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1 };
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
    
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glPopMatrix();
    
    glFlush();
    
#else
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		0, 0,
		static_cast<GLfloat>(width), 0,
		static_cast<GLfloat>(width), static_cast<GLfloat>(height),
		0, static_cast<GLfloat>(height) };
	GLfloat texVertices[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1 };

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

	glPopMatrix();
#endif
}

void CRenderBitmapInterfaceOpenGL::AddAlphaBlendingBitmap(const wxString &textureName, const int &left, const int &top)
{
	COpenGLTextureManager * m_pTexture = (COpenGLTextureManager *)FindTexture(textureName);
	//GLSLShader m_pShader;

	//m_pTexture->texture.Enable();
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_ADDALPHABLENDING_BITMAP");
    if(m_pShader == nullptr)
        return;

	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_ADDALPHABLENDING_BITMAP", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetTexture("textureToAdd", m_pTexture->texture.GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("textureWidth", m_pTexture->texture.GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("textureHeight", m_pTexture->texture.GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}

	RenderQuad();

	//m_pTexture->texture.Disable();

    m_pShader->DisableShader();
    
   // m_pShader.DeleteShader();

	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
    CRegardsBitmap * renderBitmap = GetRenderBitmap();
    int widthScale = renderBitmap->GetBitmapWidth() - (2*left);
    int heightScale = renderBitmap->GetBitmapHeight() - (2*top);
    CRegardsBitmap * bitmapScale =renderBitmap->CropBitmap(left, top,  widthScale, heightScale);
    
    int puissance = (int)((float)(bitmapScale->GetBitmapWidth() / 2) * ((float)sigma / 100.0f));
    CFiltreEffet * filtre = new CFiltreEffet(bitmapScale, colorBackground, config->GetEffectLibrary());
    filtre->MotionBlur(radius, puissance, angle);
    delete filtre;
    
    renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, left, top);
    
    UpdateRenderTexture(renderBitmap);
    
    delete renderBitmap;
    delete bitmapScale;
}


void CRenderBitmapInterfaceOpenGL::LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)
{
    //Get bitmap from Texture
    CRegardsBitmap * renderBitmap = GetRenderBitmap();
    int widthScale = renderBitmap->GetBitmapWidth() - (2*left);
    int heightScale = renderBitmap->GetBitmapHeight() - (2*top);
    CRegardsBitmap * bitmapScale =renderBitmap->CropBitmap(left, top,  widthScale, heightScale);
    
    CFiltreEffet * filtre = new CFiltreEffet(bitmapScale, colorBackground, config->GetEffectLibrary());
    int puissance = (int)((float)(bitmapScale->GetBitmapWidth() / 2) * ((float)iPuissance / 100.0f));
    int intensity = (int)((float)(bitmapScale->GetBitmapWidth() / 2) * ((float)iIntensity / 100.0f));
    
    filtre->LensFlare(iPosX - left, (bitmapScale->GetBitmapHeight() - iPosY) - top, puissance, iType, intensity, iColor, iColorIntensity);
    delete filtre;
    
    renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, left, top);
    
    UpdateRenderTexture(renderBitmap);
    
    delete renderBitmap;
    delete bitmapScale;
}

void CRenderBitmapInterfaceOpenGL::RenderToTexture()
{
	//textureSource->Enable();
	glBindTexture(GL_TEXTURE_2D, textureSource->GetTextureID());					// Bind To The Blur Texture
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
	//textureSource->Disable();
}

int CRenderBitmapInterfaceOpenGL::GetTextureID(const wxString &textureName)
{
	COpenGLTextureManager * m_pTexture = (COpenGLTextureManager *)FindTexture(textureName);
	if (m_pTexture != nullptr)
	{
		return m_pTexture->texture.GetTextureID();
	}

	return 0;
}

GLTexture * CRenderBitmapInterfaceOpenGL::GetTexture(const wxString &textureName)
{
	COpenGLTextureManager * m_pTexture = (COpenGLTextureManager *)FindTexture(textureName);
	if (m_pTexture != nullptr)
	{
		return &m_pTexture->texture;
	}

	return nullptr;
}

void CRenderBitmapInterfaceOpenGL::AddBitmap(const wxString &textureName, const int &left, const int &top)
{
	//GLSLShader m_pShader;
	COpenGLTextureManager * m_pTexture = (COpenGLTextureManager *)FindTexture(textureName);

	//m_pTexture->texture.Enable();
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_ADD_BITMAP");
    if(m_pShader == nullptr)
        return;

	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_ADD_BITMAP", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetTexture("textureToAdd", m_pTexture->texture.GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("textureWidth", m_pTexture->texture.GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("textureHeight", m_pTexture->texture.GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}

	RenderQuad();

	//m_pTexture->texture.Disable();
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();

}

void CRenderBitmapInterfaceOpenGL::AddBitmapToScale(const wxString &textureName, const int &widthScale, const int &heightScale, const int &left, const int &top)
{
	//GLSLShader m_pShader;
	COpenGLTextureManager * m_pTexture = (COpenGLTextureManager *)FindTexture(textureName);

    
	this->left = left;
	this->top = top;
    
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE");
    if(m_pShader == nullptr)
        return;

	//m_pTexture->texture.Enable();

	//m_pShader->CreateProgram(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetTexture("textureToAdd", m_pTexture->texture.GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("textureWidth", m_pTexture->texture.GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("textureHeight", m_pTexture->texture.GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("bitmapWidth", widthScale))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("bitmapHeight", heightScale))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}

	RenderQuad();

	//m_pTexture->texture.Disable();
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();
	
	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::AddBitmapToScalePosition(const wxString &textureName, const int &widthScale, const int &heightScale, const wxRect &rc, const int &left, const int &top)
{
	//GLSLShader m_pShader;
	COpenGLTextureManager * m_pTexture = (COpenGLTextureManager *)FindTexture(textureName);

	this->left = left;
	this->top = top;

	//m_pTexture->texture.Enable();
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE_POSITION");
    if(m_pShader == nullptr)
        return;

	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE_POSITION", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetTexture("textureToAdd", m_pTexture->texture.GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("textureWidth", m_pTexture->texture.GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("textureHeight", m_pTexture->texture.GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("bitmapWidth", rc.width))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("bitmapHeight", rc.height))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("pictureLeft", rc.x))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("pictureTop", rc.y))
	{
		//TRACE( L"SetTexture failed" );
	}

	RenderQuad();

	//m_pTexture->texture.Disable();
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

CRegardsBitmap * CRenderBitmapInterfaceOpenGL::GetRenderBitmap()
{
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    uint8_t * m_bData = textureSource->GetData();
    CRegardsBitmap * bitmap = new CRegardsBitmap();
    bitmap->SetBitmap(m_bData, textureSource->GetWidth(), textureSource->GetHeight());
    delete[] m_bData;
           
    return bitmap;
}

wxImage CRenderBitmapInterfaceOpenGL::GetwxImage()
{
	wxImage anImage;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    uint8_t * m_bData = textureSource->GetRGBData();
    //return wxImage(textureSource->GetWidth(), textureSource->GetHeight(), m_bData);
	int posData = 0;
	int widthSrcSize = textureSource->GetWidth() * 3;
	bool invertY = true;
	if(m_bData != nullptr)
	{
		for (int y = 0; y < textureSource->GetHeight(); y++)
		{
			if(invertY)
				posData = ((textureSource->GetHeight() - y) * widthSrcSize) - widthSrcSize;
			else
				posData = y * widthSrcSize;

			for (int x = 0; x < textureSource->GetWidth(); x++)
			{
				anImage.SetRGB(x, y, m_bData[posData], m_bData[posData + 1], m_bData[posData+2]);
				posData += 3;
			}
		}
		delete[] m_bData;
	}
	return anImage;
}


void CRenderBitmapInterfaceOpenGL::UpdateRenderTexture(CRegardsBitmap * image)
{
    GLuint m_nTextureVideoID = 0;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &m_nTextureVideoID);
    glBindTexture(GL_TEXTURE_2D, m_nTextureVideoID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, image->GetBitmapWidth(), image->GetBitmapHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image->GetPtBitmap());
    
    /*
    glBindTexture(GL_TEXTURE_2D, m_nTextureVideoID);
   
    RenderQuad();
    
    glUseProgram(0);
    
    glDeleteTextures(1, &m_nTextureVideoID);
    */
    
    GLuint m_hProgramObject = 0;
    GLuint m_hShaderHandle = 0;
    
    if (m_hProgramObject == 0)
        m_hProgramObject = glCreateProgram();
    
    m_hShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    
    if( 0 == m_hShaderHandle )
    {
        // Set Error messages.
        //AppError::SetError( CString(L"0 == m_hShaderHandle GL_FRAGMENT_PROGRAM_ARB shader creation failed") );
        return;
    }
    
    //wxString dataProgram = CLibResource::GetShaderProgram(nProgramID_i);
    
    wxString dataProgram = "uniform sampler2D tex;\nvoid main(){gl_FragColor =  texture2D(tex, gl_TexCoord[0].st);}";
    
    int length = (int)dataProgram.size() + 1;
    char * src = new char[length];
    strcpy(src, dataProgram.ToStdString().c_str());
    
    glShaderSource(m_hShaderHandle, 1, (const GLcharARB **)&src, &length);
    
    //glShaderSourceARB( m_hShaderHandle, strlen( pBufferResData ), (const GLcharARB**)&pbyShaderData, &nLENGTH );
    glCompileShader( m_hShaderHandle );
    
    //if (!check_shader_compile_status(m_hShaderHandle))
    //    return;
    
    glAttachShader(m_hProgramObject, m_hShaderHandle);
    delete[] src;
    
    glLinkProgram( m_hProgramObject );
    
    //if (!check_program_link_status(m_hProgramObject))
    //    return;
    
    glUseProgram(m_hProgramObject);
    
    GLint nParamObj = glGetUniformLocation(m_hProgramObject, "tex");
    if( -1 == nParamObj )
    {
        return;
    }
    
    glActiveTexture( GL_TEXTURE0 + m_nTextureVideoID);
    glBindTexture(GL_TEXTURE_2D, m_nTextureVideoID);
    glUniform1i(nParamObj, m_nTextureVideoID);
   
    RenderQuad();
    
    glUseProgram(0);
    
    RenderToTexture();
    
    if( m_hShaderHandle )
    {
        glDetachShader(m_hProgramObject, m_hShaderHandle);
        glDeleteShader(m_hShaderHandle);
        m_hShaderHandle = 0;
    }
    if( m_hProgramObject )
    {
        glDeleteProgram(m_hProgramObject);
        m_hProgramObject = 0;
    }
    
    glDeleteTextures(1, &m_nTextureVideoID);
}

void CRenderBitmapInterfaceOpenGL::RenderToScreen(wxDC * deviceContext)
{
    
	CLoadingResource loadingResource;
	if (renderFromBitmap)
	{
		wxRect rc = { 0, 0, width, height };
        wxColour color;
        color.Set(colorBackground.GetRed(), colorBackground.GetGreen(), colorBackground.GetBlue());
		wxBrush brush = wxBrush(color);
		deviceContext->SetBrush(brush);
		deviceContext->SetPen(wxPen(color, 1)); // 10-pixels-thick pink outline
		deviceContext->DrawRectangle(rc);
		deviceContext->SetPen(wxNullPen);
		deviceContext->SetBrush(wxNullBrush);
	}
	else
	{
		wxImage anImage = wxImage(width, height);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		uint8_t * m_bData = textureSource->GetRGBData();
		int posData = 0;
		int widthSrcSize = textureSource->GetWidth() * 3;
		bool invertY = true;
		if(m_bData != nullptr)
		{
			for (int y = 0; y < textureSource->GetHeight(); y++)
			{
				if(invertY)
					posData = ((textureSource->GetHeight() - y) * widthSrcSize) - widthSrcSize;
				else
					posData = y * widthSrcSize;

				for (int x = 0; x < textureSource->GetWidth(); x++)
				{
					anImage.SetRGB(x, y, m_bData[posData], m_bData[posData + 1], m_bData[posData+2]);
					posData += 3;
				}
			}
			delete[] m_bData;
		}
	    deviceContext->DrawBitmap(anImage,0,0);
	}
}

GLSLShader * CRenderBitmapInterfaceOpenGL::CreateShader(const wxString &shaderName)
{
    GLSLShader * m_pShader = new GLSLShader();
    m_pShader->CreateProgram(shaderName, GL_FRAGMENT_PROGRAM_ARB);
    return m_pShader;
}


GLSLShader * CRenderBitmapInterfaceOpenGL::FindShader(const wxString &shaderName)
{
    for(COpenGLShader * shader : listShader)
    {
        if(shader->shaderName == shaderName)
            return shader->m_pShader;
    }
    
    COpenGLShader * openGLShader = new COpenGLShader();
    openGLShader->m_pShader = CreateShader(shaderName);
    openGLShader->shaderName = shaderName;
    
    listShader.push_back(openGLShader);
    
    return openGLShader->m_pShader;
}

void CRenderBitmapInterfaceOpenGL::Transition(const wxString &textureName, const int &step)
{
	//Calcul des paramètres pour la transition
	COpenGLTextureManager * m_pTexture = (COpenGLTextureManager *)FindTexture(textureName);
	if (m_pTexture != nullptr)
	{
		float ratioSecond = CalculPictureRatio(m_pTexture->texture.GetWidth(), m_pTexture->texture.GetHeight());
		int widthScale = m_pTexture->texture.GetWidth() * ratioSecond;
		int heightScale = m_pTexture->texture.GetHeight() * ratioSecond;
		int left = (width - widthScale) / 2;
		int top = (height - heightScale) / 2;

		GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_TRANSITION");
        if(m_pShader == nullptr)
            return;

		//m_pTexture->texture.Enable();

		//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_TRANSITION", GL_FRAGMENT_PROGRAM_ARB);

		m_pShader->EnableShader();

		if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
		{
			//TRACE( L"SetTexture failed" );
		}
		if (!m_pShader->SetTexture("textureToAdd", m_pTexture->texture.GetTextureID()))
		{
			//TRACE( L"SetTexture failed" );
		}
		if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
		{
			//TRACE( L"SetTexture failed" );
		}

		if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
		{
			//TRACE( L"SetTexture failed" );
		}

		if (!m_pShader->SetParam("textureWidth", m_pTexture->texture.GetWidth()))
		{
			//TRACE( L"SetTexture failed" );
		}
		if (!m_pShader->SetParam("textureHeight", m_pTexture->texture.GetHeight()))
		{
			//TRACE( L"SetTexture failed" );
		}

		if (!m_pShader->SetParam("bitmapWidth", widthScale))
		{
			//TRACE( L"SetTexture failed" );
		}
		if (!m_pShader->SetParam("bitmapHeight", heightScale))
		{
			//TRACE( L"SetTexture failed" );
		}

		if (!m_pShader->SetParam("left", left))
		{
			//TRACE( L"SetTexture failed" );
		}
		if (!m_pShader->SetParam("top", top))
		{
			//TRACE( L"SetTexture failed" );
		}

		if (!m_pShader->SetParam("step", step))
		{
			//TRACE( L"SetTexture failed" );
		}

		if (!m_pShader->SetParam("red", colorBackground.GetFRed()))
		{
			//TRACE( L"SetTexture failed" );
		}

		if (!m_pShader->SetParam("green", colorBackground.GetFGreen()))
		{
			//TRACE( L"SetTexture failed" );
		}
		if (!m_pShader->SetParam("blue", colorBackground.GetFBlue()))
		{
			//TRACE( L"SetTexture failed" );
		}

		RenderQuad();

		//m_pTexture->texture.Disable();
        m_pShader->DisableShader();
        
        //m_pShader.DeleteShader();

		RenderToTexture();
	}
}

void CRenderBitmapInterfaceOpenGL::PhotoFiltre(const CRgbaquad &color, const int &intensity)
{
    
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_PHOTOFILTRE");
    if(m_pShader == nullptr)
        return;
    
	//GLSLShader m_pShader;
	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_PHOTOFILTRE", GL_FRAGMENT_PROGRAM_ARB);
    
    m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("red", color.GetFRed()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("green", color.GetFGreen()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("blue", color.GetFBlue()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("intensity", (float)intensity))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::RedEye(float &strength, const wxRect& rSelectionBox)
{

}

void CRenderBitmapInterfaceOpenGL::RGBFiltre(const int &red, const int &green, const int &blue)
{
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_RGB");
    if(m_pShader == nullptr)
        return;
    
	//GLSLShader m_pShader;
	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_RGB", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("red", (float)red))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("green", (float)green))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("blue", (float)blue))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::RotateFree(const int &angle)
{
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_ROTFREE");
    if(m_pShader == nullptr)
        return;
    
	//GLSLShader m_pShader;
	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_ROTFREE", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("texUnit", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("fAngle", (float)angle))
	{
		//TRACE( L"SetTexture failed" );
	}

	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::Swirl(const float &radius, const float &angle, const float &bitmapWidth, const float &bitmapHeight)
{
    
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_SWIRL");
    if(m_pShader == nullptr)
        return;
    
	//GLSLShader m_pShader;
	//m_pShader->CreateProgram(L"IDR_GLSL_SHADER_SWIRL", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthBitmap", bitmapWidth))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("heightBitmap", bitmapHeight))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("radius", radius))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("angleDegree", angle))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::BrightnessAndContrast(const int &lightness, const int &contrast)
{
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_BRIGHTANDCONTRAST");
    if(m_pShader == nullptr)
        return;
    
	//GLSLShader m_pShader;
	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_BRIGHTANDCONTRAST", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("brightness", (float)lightness))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("contrast", (float)contrast))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::Posterize(const float &level, const float &gamma)
{
	//GLSLShader m_pShader;
	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_POSTERIZE", GL_FRAGMENT_PROGRAM_ARB);
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_POSTERIZE");
    if(m_pShader == nullptr)
        return;

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("level", (float)level))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("gamma", (float)gamma))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

void CRenderBitmapInterfaceOpenGL::Solarize(const long &threshold)
{
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_SOLARIZE");
    if(m_pShader == nullptr)
        return;
    
	//GLSLShader m_pShader;
	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_SOLARIZE", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("threshold", (float)threshold))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();

}

void CRenderBitmapInterfaceOpenGL::Clouds(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave)
{
	GLint colorOne[4];
	GLint colorTwo[4];
	GLfloat tabAmplitude[8];
	GLfloat tabFrequence[8];

	colorOne[0] = color1.GetRed();
	colorOne[1] = color1.GetGreen();
	colorOne[2] = color1.GetBlue();
	colorTwo[0] = color2.GetRed();
	colorTwo[1] = color2.GetGreen();
	colorTwo[2] = color2.GetBlue();

	tabFrequence[0] = frequence;
	tabAmplitude[0] = 1;

	for (int k = 1; k<8; k++)
	{
		tabFrequence[k] = tabFrequence[k - 1] * 2;
		tabAmplitude[k] = tabAmplitude[k - 1] * amplitude;
	}
    
    GLSLShader * m_pShader = FindShader(L"IDR_GLSL_SHADER_CLOUDS");
    if(m_pShader == nullptr)
        return;

	//GLSLShader m_pShader;
	//m_pShader.CreateProgram(L"IDR_GLSL_SHADER_CLOUDS", GL_FRAGMENT_PROGRAM_ARB);

	m_pShader->EnableShader();

	if (!m_pShader->SetTexture("textureScreen", textureSource->GetTextureID()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("widthScreen", (float)textureSource->GetWidth()))
	{
		//TRACE( L"SetTexture failed" );
	}

	if (!m_pShader->SetParam("heightScreen", (float)textureSource->GetHeight()))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("left", left))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetParam("top", top))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetIntegerParam("octaves", 8))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetIntVectorParam("color1",4,colorOne))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetIntVectorParam("color2",4,colorTwo))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetFloatVectorParam("tabFrequence",8,tabFrequence))
	{
		//TRACE( L"SetTexture failed" );
	}
	if (!m_pShader->SetFloatVectorParam("tabAmplitude",8, tabAmplitude))
	{
		//TRACE( L"SetTexture failed" );
	}
	RenderQuad();
    
    m_pShader->DisableShader();
    
    //m_pShader.DeleteShader();

	RenderToTexture();
}

