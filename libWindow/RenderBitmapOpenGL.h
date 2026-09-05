// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#pragma once
#include <RenderOpenGL.h>
using namespace Regards::OpenGL;

//#define RENDEROPENGL WM_USER+1

;

namespace Regards::OpenGL
{
	class CRenderBitmapOpenGL
	{
	public:
		CRenderBitmapOpenGL(CRenderOpenGL* renderOpenGL);
		~CRenderBitmapOpenGL() = default;
		void LoadingResource(const double& scale_factor, wxColor& colorActifReplacement);
		void ShowArrowNext();
		void ShowArrowPrevious();

		void RenderTexture(GLTexture* textureTransition, const int& width, const int& height, const int& left, const int& top);
		void RenderTextureWithAlpha(GLTexture* textureTransition, const int& alpha, const int& width,
			const int& height, const int& left, const int& top);

		COpenGLShader * FindShader(const wxString& shaderName, GLenum glSlShaderType_i = GL_FRAGMENT_PROGRAM_ARB, const wxString& vertexName = "IDR_GLSL_VERTEX");
		float* GetProjectionMatrix();
		CRenderOpenGL* GetRenderOpengl()
		{
			return renderOpenGL;
		}
	private:
		std::unique_ptr<GLTexture> textureArrowRight;
		std::unique_ptr<GLTexture> textureArrowLeft;
		CRenderOpenGL* renderOpenGL = nullptr;
	};
}