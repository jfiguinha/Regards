// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#include <GLTexture.h>
#include <GLSLShader.h>
#include <RGBAQuad.h>
#include <EffectParameter.h>
using namespace Regards::OpenGL;

#define RENDEROPENGL WM_USER+1

class CRegardsBitmap;

namespace Regards
{
	namespace OpenGL
	{
        class COpenGLShader
        {
        public:
            virtual ~COpenGLShader()
            {
                if(m_pShader != nullptr)
                    delete m_pShader;
            }
            wxString shaderName;
            GLSLShader * m_pShader = nullptr;
        };

		class CRenderOpenGL : public wxGLContext
		{
		public:

			CRenderOpenGL(wxGLCanvas *canvas);
			virtual ~CRenderOpenGL();
			wxGLContext * GetGLContext();
            GLTexture * GetDisplayTexture();
            GLTexture * GetDisplayTexture(const int &width, const int &height);
			void CreateScreenRender(const int &width, const int &height, const CRgbaquad &color);
			GLTexture * GetDisplayTexture(const int &width, const int &height, cl_context context);
			void RenderToScreen(const int &left, const int &top,const bool & inverted = false);
			cl_mem GetOpenCLTexturePt();
			GLTexture * GetCopyTextureBitmap(const int &width, const int &height, cl_context context);
			void Print(int x, int y, const char *string);
            void DeleteTexture();
            void Init(wxGLCanvas *canvas);
            bool IsInit();
		protected:
            GLSLShader * CreateShader(const wxString &shaderName);
            GLSLShader * FindShader(const wxString &shaderName);
			GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
			void RenderQuad(GLTexture * texture, int left = 0, int top = 0, bool inverted = false);
            void RenderQuad(GLTexture * texture, const bool & flipH,const bool & flipV, int left = 0, int top = 0, bool inverted = false);
			GLTexture * textureDisplay;
			int width;
			int height;
			cl_mem cl_textureDisplay;
            GLuint	base;
            wxString version;
            float myGLVersion;
            vector<COpenGLShader *> listShader;
            bool isInit = false;
		};
	}
}
