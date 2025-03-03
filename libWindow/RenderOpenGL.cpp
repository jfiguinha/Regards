// ReSharper disable All
#include <header.h>
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderOpenGL.h"
#include <OpenCLContext.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <FileUtility.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#if wxUSE_GLCANVAS_EGL == 1
#include <EGL/egl.h>
#endif
#endif

#include <utility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <GLCharacter.h>


#include <ft2build.h>
#include FT_FREETYPE_H
#define USE_GLUT

class CFreeTypeFace
{
public:
	CFreeTypeFace() {};
	FT_Face face;
};

extern string platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;
extern bool isOpenCLInitialized;
using namespace Regards::OpenGL;
using namespace Regards::OpenCL;

CRenderOpenGL::CRenderOpenGL(wxGLCanvas* canvas)
	: wxGLContext(canvas), base(0), myGLVersion(0), mouseUpdate(nullptr)
{
	width = 0;
	height = 0;
	this->openclOpenGLInterop = false;
	textureDisplay = nullptr;
}


bool CRenderOpenGL::IsInit()
{
	return isInit;
}

GLTexture* CRenderOpenGL::GetTextureDisplay()
{
	return textureDisplay;
}

bool CRenderOpenGL::GetOpenGLInterop()
{
	return openclOpenGLInterop;
}



void CRenderOpenGL::Init(wxGLCanvas* canvas)
{
	if (!isInit)
	{
		SetCurrent(*canvas);

		int epoxyversion = epoxy_gl_version();
		bool pboSupported = epoxy_has_gl_extension("GL_ARB_pixel_buffer_object");

        printf("CRenderOpenGL::Init \n");

		CRegardsConfigParam* regardsParam = CParamInit::getInstance();
		if (regardsParam != nullptr)
		{
             printf("CRenderOpenGL::Init 1 OpenCL Support : %d GetIsOpenCLOpenGLInteropSupport : %d \n",regardsParam->GetIsOpenCLSupport(), regardsParam->GetIsOpenCLOpenGLInteropSupport());
            
#ifdef USE_CUDA

			if (regardsParam->GetIsCudaSupport())
			{
				openclOpenGLInterop = false;
				platformName = "NVIDIA";
				COpenCLContext::AssociateToVulkan();
			}
			else if (regardsParam->GetIsOpenCLSupport())
#else
			 if (regardsParam->GetIsOpenCLSupport())
#endif
			{
				if (cv::ocl::haveOpenCL() && !isOpenCLInitialized && regardsParam->GetIsOpenCLOpenGLInteropSupport())
				{
                     printf("CRenderOpenGL::Init 2 \n");
					try
					{
						COpenCLContext::initializeContextFromGL();
						isOpenCLInitialized = true;
						openclOpenGLInterop = true;
						//cv::ocl::Device(clExecCtx.getContext().device(0));
					}
					catch (cv::Exception& e)
					{
                        openclOpenGLInterop = false;
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
						COpenCLContext::CreateDefaultOpenCLContext();
					}

				}



				if (!isOpenCLInitialized)
				{
					regardsParam->SetIsOpenCLSupport(false);
				}
				regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
			}
		}
		
		myGLVersion = 0;
		version = glGetString(GL_VERSION);
		sscanf(CConvertUtility::ConvertToUTF8(version), "%f", &myGLVersion);
		isInit = true;


		textureDisplay = new GLTexture();
        
#ifndef USE_GLUT
        LoadFont("Antonio-Bold.ttf");
#endif
	}
}

GLSLShader* CRenderOpenGL::CreateShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	auto m_pShader = new GLSLShader();
	m_pShader->CreateProgram(shaderName, glSlShaderType_i);
	return m_pShader;
}

GLSLShader* CRenderOpenGL::FindShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	for (COpenGLShader* shader : listShader)
	{
		if (shader->shaderName == shaderName)
			return shader->m_pShader;
	}

	auto openGLShader = new COpenGLShader();
	openGLShader->m_pShader = CreateShader(shaderName, glSlShaderType_i);
	openGLShader->shaderName = shaderName;

	listShader.push_back(openGLShader);

	return openGLShader->m_pShader;
}

CRenderOpenGL::~CRenderOpenGL()
{
	if (textureDisplay != nullptr)
		delete(textureDisplay);

	for (COpenGLShader* shader : listShader)
		delete shader;

	listShader.clear();
}

wxGLContext* CRenderOpenGL::GetGLContext()
{
	return this;
}

void CRenderOpenGL::Print(int x, int y, double scale_factor, const char* text)
{

#ifdef USE_GLUT	

    float font_height = 15;
    
    if(scale_factor > 1.0f)
        font_height = font_height * 2;

    //glPushMatrix();
	//glRasterPos2f(x, height - font_height);
    //glLoadIdentity();
	glWindowPos2i(x, height - font_height);
    
    
    //glColor4f(0.5, 0.8f, 0.2f, 1.0f);   
	//get the length of the string to display
	int len = static_cast<int>(strlen(text));
        
    //glScalef(scale_factor,scale_factor,scale_factor); 

	//loop to display character by character
	for (auto i = 0; i < len; i++)
	{
        if(scale_factor > 1.0f)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        else
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	}
    
    //glPopMatrix();
    //glColor4f(1,1,1,1);

#else

	RenderText(text, x, height - (heightFont * 0.3 * scale_factor), 0.3f * scale_factor, vec3f(0.5, 0.8f, 0.2f));

#endif
};

void CRenderOpenGL::PrintSubtitle(int x, int y, double scale_factor, float red, float green, float blue, wxString text)
{
    //RenderText(text, x, y, 1.0f, vec3f(0.5, 0.8f, 0.2f));
#ifdef USE_GLUT	
	float font_height = 15;
    void * font_choose = GLUT_BITMAP_TIMES_ROMAN_24;
	float font_width = glutBitmapWidth(font_choose, 'x');;
    int xPos = 0;
    glColor3f(red, green, blue); 
	std::vector<wxString> list = CConvertUtility::split(text, '\\');
	if (list.size() > 0)
	{
		wxString line = list[0];
        xPos = x - ((font_width * line.size()) / 2);
		glWindowPos2i(xPos, y);
		//get the length of the string to display
		int len = static_cast<int>(line.Length());

		//glScalef(scale_factor,scale_factor,scale_factor); 
        int xPosition = 0;
		//loop to display character by character
		for (auto i = 0; i < len; i++)
		{
			wxUniChar c = line[i];
			char letter;
			c.GetAsChar(&letter);
			glutBitmapCharacter(font_choose, c);
            xPosition += font_width;
		}

		for (int i = 1;i < list.size();i++)
		{
			wxUniChar c = list[i][0];
			if (c == 'N' || c == 'n')
			{
				//New Line
				wxString line = list[i];
				glWindowPos2i(x - ((font_width * line.size()) / 2), y - font_height * 2);
				//get the length of the string to display
				int len = static_cast<int>(line.Length());

				//glScalef(scale_factor,scale_factor,scale_factor); 

				//loop to display character by character
				for (auto i = 1; i < len; i++)
				{
					wxUniChar c = line[i];
					char letter;
					c.GetAsChar(&letter);
					glutBitmapCharacter(font_choose, c);
				}
			}
            else
            {
				wxString line = list[i];
				glWindowPos2i(xPos + xPosition + font_width, y - font_height * 2);
				//get the length of the string to display
				int len = static_cast<int>(line.Length());

				//glScalef(scale_factor,scale_factor,scale_factor); 

				//loop to display character by character
				for (auto i = 1; i < len; i++)
				{
					wxUniChar c = line[i];
					char letter;
					c.GetAsChar(&letter);
					glutBitmapCharacter(font_choose, c);
				}
            }
		}
	}
#else   
	int xPos = 0;
    
    cout << "Scale Factor : " << to_string(scale_factor) << endl;

	std::vector<wxString> list = CConvertUtility::split(text, '\\');
	if (list.size() > 0)
	{
		wxString line = list[0];
		xPos = x - ((widthFont * scale_factor * line.size()) / 2);
		//glWindowPos2i(xPos, y);
		//get the length of the string to display
		int len = static_cast<int>(line.Length());

		//glScalef(scale_factor,scale_factor,scale_factor); 
		int xPosition = 0;
        
        float fRed = red / 255.0f;
        float fGreen = green / 255.0f;
        float fBlue = blue / 255.0f;
        
		RenderText(line, xPos, y, scale_factor, vec3f(fRed, fGreen, fBlue));
		xPosition += widthFont * len * scale_factor;


		for (int i = 1; i < list.size(); i++)
		{
			wxUniChar c = list[i][0];
			if (c == 'N' || c == 'n')
			{
				//New Line
				wxString line = list[i];
                line = line.SubString(1,line.size() - 1);
				RenderText(line, x - ((widthFont * scale_factor * line.size()) / 2), y - heightFont * scale_factor, scale_factor, vec3f(fRed, fGreen, fBlue));

			}
			else
			{
				wxString line = list[i];
                line = line.SubString(1,line.size() - 1);
				RenderText(line, xPos + xPosition + widthFont * scale_factor, y - heightFont * scale_factor, scale_factor, vec3f(fRed, fGreen, fBlue));
			}
		}
	}
#endif
    /*

    */


};


GLvoid CRenderOpenGL::ReSizeGLScene(GLsizei width, GLsizei height) // Resize And Initialize The GL Window
{
	if (height == 0) // Prevent A Divide By Zero By
	{
		height = 1; // Making Height Equal One
	}

	glViewport(0, 0, width, height); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
}


bool CRenderOpenGL::SetData(Regards::Picture::CPictureArray& bitmap)
{
	return textureDisplay->SetData(bitmap);
}


GLTexture* CRenderOpenGL::GetDisplayTexture(const int& width, const int& height)
{
	return textureDisplay;
}

int CRenderOpenGL::GetWidth()
{
	return width;
}

int CRenderOpenGL::GetHeight()
{
	return height;
}

void CRenderOpenGL::CreateScreenRender(const int& width, const int& height, const CRgbaquad& color)
{
	if (this->width != width || this->height != height)
	{
		this->width = width;
		this->height = height;
		ReSizeGLScene(width, height);
        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	}

    glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);

}

void CRenderOpenGL::RenderQuad(GLTexture* texture, int left, int top, bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};
		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};
		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();
}


void CRenderOpenGL::RenderQuad(GLTexture* texture, const int& width, const int& height, const bool& flipH,
                               const bool& flipV, int left, int top, bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(height) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(height) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		if (flipV)
		{
			vertices[1] = 0;
			vertices[3] = 0;
			vertices[5] = 1;
			vertices[7] = 1;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};

		if (flipV)
		{
			vertices[1] = 1;
			vertices[3] = 1;
			vertices[5] = 0;
			vertices[7] = 0;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();
}


void CRenderOpenGL::RenderQuad(GLTexture* texture, const bool& flipH, const bool& flipV, int left, int top,
                               bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		if (flipV)
		{
			vertices[1] = 0;
			vertices[3] = 0;
			vertices[5] = 1;
			vertices[7] = 1;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};

		if (flipV)
		{
			vertices[1] = 1;
			vertices[3] = 1;
			vertices[5] = 0;
			vertices[7] = 0;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();
}

void CRenderOpenGL::RenderToScreen(IMouseUpdate* mousUpdate, CEffectParameter* effectParameter, const int& left,
                                   const int& top, const bool& inverted)
{
	bool renderPreview = false;
	textureDisplay->Enable();

	if (!renderPreview)
	{
		RenderQuad(textureDisplay, left, top, inverted);
	}

	textureDisplay->Disable();
}

GLTexture* CRenderOpenGL::GetGLTexture()
{
	return textureDisplay;
}

void CRenderOpenGL::LoadCharacter(unsigned char c, CFreeTypeFace& face)
{
	// Load character glyph 
	if (FT_Load_Char(face.face, c, FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		return;
	}
	// generate texture

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		face.face->glyph->bitmap.width,
		face.face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face.face->glyph->bitmap.buffer
	);
	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLTexture* glTexture = new GLTexture(texture, face.face->glyph->bitmap.width, face.face->glyph->bitmap.rows);

	// now store character for later use
	Character character = {
		glTexture,
		vec2d(face.face->glyph->bitmap.width, face.face->glyph->bitmap.rows),
		vec2d(face.face->glyph->bitmap_left, face.face->glyph->bitmap_top),
		static_cast<unsigned int>(face.face->glyph->advance.x)
	};
	Characters.insert(std::pair<char, Character>(c, character));

	widthFont = face.face->glyph->bitmap.width;
	heightFont = face.face->glyph->bitmap.rows;
}

int CRenderOpenGL::LoadFont(const wxString & fontName)
{
    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

	// find path to font
    wxString font_name = CFileUtility::GetResourcesFolderFontPathWithExt(fontName);
    //std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }
	
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 32; c < 127; c++)
        {
			CFreeTypeFace freetypeFace;
			freetypeFace.face = face;
			LoadCharacter(c, freetypeFace);
        }

		// load first 128 characters of ASCII set
		for (unsigned char c = 192; c < 255; c++)
		{
			CFreeTypeFace freetypeFace;
			freetypeFace.face = face;
			LoadCharacter(c, freetypeFace);
		}
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
	return 0;
}

void CRenderOpenGL::RenderQuad(GLTexture* texture, float left, float top, float scale, bool inverted)
{
 	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth() * scale) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth() * scale) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(texture->GetHeight() * scale) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight() * scale) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();   
}

void CRenderOpenGL::RenderCharacter(GLTexture* glTexture, const float & left, const float & top, const float & scale, const vec3f & color)
{

	printf("GLSLShader IDR_GLSL_COLOR \n ");
	GLSLShader* m_pShader = FindShader(L"IDR_GLSL_COLOR");
	if (m_pShader != nullptr)
	{
		m_pShader->EnableShader();
		if (!m_pShader->SetTexture("text", glTexture->GetTextureID()))
		{
			printf("SetTexture textureScreen failed \n ");
		}
		if (!m_pShader->SetVec3Param("textColor", color))
		{
			printf("SetParam intensity failed \n ");
		}
	}
	RenderQuad(glTexture, left, top, scale, true);
	if (m_pShader != nullptr)
		m_pShader->DisableShader();

	
	//glTexture->Disable();
}

// render line of text
// -------------------
void CRenderOpenGL::RenderText(wxString text, float x, float y, float scale, vec3f color)
{
	//glTexture->Enable();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, 1);
    // iterate through all characters
    wxString::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
    
        /*
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        */
        if(ch.glTexture != nullptr)
			RenderCharacter(ch.glTexture, xpos, ypos, scale, color);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

// render line of text
// -------------------
void CRenderOpenGL::RenderChar(char c, float x, float y, float scale, vec3f color)
{
    Character ch = Characters[c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
    
    RenderCharacter(ch.glTexture, xpos, ypos, scale, color);

}