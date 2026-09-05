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
#include <unordered_map>
#include <OpenCLContext.h>
using namespace Regards::OpenGL;
using namespace Regards::OpenCL;
//#define RENDEROPENGL WM_USER+1

;
class Character;
class CFreeTypeFace;

namespace Regards::OpenGL
{
	class COpenGLShader
	{
	public:

		bool EnableShader(const float* projectionMatrix)
		{
			bool isOk = true;

			if (m_pShader)
				isOk = m_pShader->EnableShader();

			// Envoi automatique de la projection si elle est fournie
			if (isOk && m_pShader && projectionMatrix != nullptr)
			{
				m_pShader->SetMatrixParam("projection", projectionMatrix);
			}

			return isOk;
		}

		bool DisableShader()
		{
			bool isOk = true;

			if (m_pShader)
				isOk = m_pShader->DisableShader();

			return isOk;
		}
		wxString shaderName;
		std::unique_ptr<GLSLShader> m_pShader = nullptr;

	};

	class CRenderOpenGL : public wxGLContext
	{
	public:
        
        
		CRenderOpenGL(wxGLCanvas* canvas);
		~CRenderOpenGL() override;
		GLTexture* GetGLTexture();
		wxGLContext* GetGLContext();
		bool GetOpenGLInterop();
		bool SetData(Regards::Picture::CPictureArray& bitmap, const bool& deleteOldData = false);
		bool CreateScreenRender(const int& width, const int& height, const CRgbaquad& color);
		GLTexture* GetDisplayTexture(const int& width, const int& height);
		void RenderToScreen(IMouseUpdate* mousUpdate, CEffectParameter* effectParameter, const int& left,
		                    const int& top, const bool& inverted = false);

		void Print(int x, int y, double scaleFactor, const char* string);
        void PrintSubtitle(int x, int y, double scale_factor, wxString text);
		void PrintSubtitle(int x, int y, double scale_factor, float red, float green, float blue, wxString text);
		void Init(wxGLCanvas* canvas);
		bool IsInit();
        int LoadFont(const wxString & fontName);
		int GetWidth();
		int GetHeight();
		COpenGLShader * FindShader(const wxString& shaderName, GLenum glSlShaderType_i = GL_FRAGMENT_PROGRAM_ARB, const wxString& vertexName = "IDR_GLSL_VERTEX");

		//void RenderToTexture();
        void RenderText(wxString text, float x, float y, float scale, vec3f color);


		GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
		void RenderQuad(int width, int height, int left, int top, bool inverted);
		void RenderQuad(GLTexture* texture, int left = 0, int top = 0, bool inverted = false);
        void RenderQuad(GLTexture* texture, float left = 0, float top = 0, float scale = 0, bool inverted = false);
		void RenderQuad(GLTexture* texture, const int& width, const int& height, const bool& flipH,
		                const bool& flipV, int left = 0, int top = 0, bool inverted = false);
		void RenderQuad(GLTexture* texture, const bool& flipH, const bool& flipV, int left = 0, int top = 0,
		                bool inverted = false);

		GLTexture* GetTextureDisplay();
		COpenCLContext* GetOpenCLContext(){ return openCLContext.get(); };

		// ============================================================
		// MATRICES
		//
		// Matrice orthographique utilisée par les shaders modernes.
		// ============================================================

		float projectionMatrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
									  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
       
	protected:
        
		void UpdateProjectionMatrix();
		void LoadCharacter(unsigned char c, CFreeTypeFace & face);
        void RenderCharacter(GLSLShader* m_pShader, GLTexture* glTexture, const float & left, const float & top, const float & scale, const vec3f & color);

        void RenderQuadInternal(float width,
                                       float height,
                                       int left,
                                       int top,
                                       bool inverted,
                                       bool flipH,
                                       bool flipV);
		std::unique_ptr<GLTexture> textureDisplay;
		int width;
		int height;
		GLuint base;
		wxString version;
		float myGLVersion;
		std::unordered_map<wxString, std::unique_ptr<COpenGLShader>> shaderMap;
		bool isInit = false;
		IMouseUpdate* mouseUpdate;


		//OpenCL interop
		std::unique_ptr<COpenCLContext> openCLContext = nullptr;
		int widthFont = 0;
		int heightFont = 0;
        std::map<GLchar, Character> Characters;

		// À insérer dans la section protected ou private de CRenderOpenGL dans RenderOpenGL.h
	protected:

		float CalculateTextWidth(const wxString& text, float scale);

		struct TextVertex {
			float x, y;   // Position (Attribut 0)
			float u, v;   // Coordonnées de texture (Attribut 1)
		};

		GLuint textVAO = 0;
		GLuint textVBO = 0;
		GLuint textEBO = 0;

		void InitTextBuffers();
	};
}
