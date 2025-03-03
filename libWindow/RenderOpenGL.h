// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#pragma once
#include <GLTexture.h>
#include <GLSLShader.h>
#include "GLcontext.h"
#include <RGBAQuad.h>
#include <EffectParameter.h>
#include <MouseUpdate.h>
#include <vec3.h>
using namespace Regards::OpenGL;

//#define RENDEROPENGL WM_USER+1

class CRegardsBitmap;
class Character;
class CFreeTypeFace;

namespace Regards::OpenGL
{
	class COpenGLShader
	{
	public:
		virtual ~COpenGLShader()
		{
			if (m_pShader != nullptr)
				delete m_pShader;
		}


		wxString shaderName;
		GLSLShader* m_pShader = nullptr;
	};

	class CRenderOpenGL : public wxGLContext
	{
	public:
        
        
		CRenderOpenGL(wxGLCanvas* canvas);
		~CRenderOpenGL() override;
		GLTexture* GetGLTexture();
		wxGLContext* GetGLContext();
		bool GetOpenGLInterop();
		bool SetData(Regards::Picture::CPictureArray& bitmap);
		void CreateScreenRender(const int& width, const int& height, const CRgbaquad& color);
		GLTexture* GetDisplayTexture(const int& width, const int& height);
		void RenderToScreen(IMouseUpdate* mousUpdate, CEffectParameter* effectParameter, const int& left,
		                    const int& top, const bool& inverted = false);

		void Print(int x, int y, double scaleFactor, const char* string);
		void PrintSubtitle(int x, int y, double scale_factor, float red, float green, float blue, wxString text);
		void Init(wxGLCanvas* canvas);
		bool IsInit();
        int LoadFont(const wxString & fontName);
		int GetWidth();
		int GetHeight();
		GLSLShader* CreateShader(const wxString& shaderName, GLenum glSlShaderType_i = GL_FRAGMENT_PROGRAM_ARB);
		GLSLShader* FindShader(const wxString& shaderName, GLenum glSlShaderType_i = GL_FRAGMENT_PROGRAM_ARB);

		//void RenderToTexture();
        void RenderText(wxString text, float x, float y, float scale, vec3f color);


		GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
		void RenderQuad(GLTexture* texture, int left = 0, int top = 0, bool inverted = false);
        void RenderQuad(GLTexture* texture, float left = 0, float top = 0, float scale = 0, bool inverted = false);
		void RenderQuad(GLTexture* texture, const int& width, const int& height, const bool& flipH,
		                const bool& flipV, int left = 0, int top = 0, bool inverted = false);
		void RenderQuad(GLTexture* texture, const bool& flipH, const bool& flipV, int left = 0, int top = 0,
		                bool inverted = false);

		GLTexture* GetTextureDisplay();
        
        void RenderChar(char c, float x, float y, float scale, vec3f color);
        
	protected:
        
		void LoadCharacter(unsigned char c, CFreeTypeFace & face);
        void RenderCharacter(GLTexture* glTexture, const float & left, const float & top, const float & scale, const vec3f & color);
		void CreateDefaultOpenCLContext();
		void initializeContextFromGL();
		GLTexture* textureDisplay;
		int width;
		int height;
		GLuint base;
		wxString version;
		float myGLVersion;
		vector<COpenGLShader*> listShader;
		bool isInit = false;
		IMouseUpdate* mouseUpdate;
		bool openclOpenGLInterop = false;
		int widthFont = 0;
		int heightFont = 0;
        std::map<GLchar, Character> Characters;
	};
}
