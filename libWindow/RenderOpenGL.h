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
using namespace Regards::OpenGL;

//#define RENDEROPENGL WM_USER+1

class CRegardsBitmap;

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

		void Print(int x, int y, const char* string);

		void Init(wxGLCanvas* canvas);
		bool IsInit();
		int GetWidth();
		int GetHeight();
		GLSLShader* CreateShader(const wxString& shaderName, GLenum glSlShaderType_i = GL_FRAGMENT_PROGRAM_ARB);
		GLSLShader* FindShader(const wxString& shaderName, GLenum glSlShaderType_i = GL_FRAGMENT_PROGRAM_ARB);

		//void RenderToTexture();


		GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
		void RenderQuad(GLTexture* texture, int left = 0, int top = 0, bool inverted = false);
		void RenderQuad(GLTexture* texture, const int& width, const int& height, const bool& flipH,
		                const bool& flipV, int left = 0, int top = 0, bool inverted = false);
		void RenderQuad(GLTexture* texture, const bool& flipH, const bool& flipV, int left = 0, int top = 0,
		                bool inverted = false);

		GLTexture* GetTextureDisplay();

	protected:
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
	};
}
