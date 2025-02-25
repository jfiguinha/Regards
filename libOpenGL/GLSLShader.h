#pragma once
#include "vec3.h"
#include "GLSLParameter.h"

namespace Regards
{
	namespace OpenGL
	{
		// Shader class handles creating of a Shader, and parameter setting to the 
		// GLSL Shader. This class uses GLExtension for calling opengl extension functions.
		class GLSLShader
		{
		public:
			GLSLShader(void);
			// nProgramID_i is the resource ID of Shader.
			// glSlShaderType_i should be GL_FRAGMENT_PROGRAM_ARB, GL_VERTEX_PROGRAM_ARB
			bool CreateProgram(const wxString& nProgramID_i, GLenum glSlShaderType_i);
			bool DeleteShader();
			bool EnableShader();
			bool DisableShader();
			// This function attach a texture to shader parameter.
			// pParamName_i is parameter name given in shader.
			bool SetTexture(const char* pParamName_i, int nTextureID_i);
			// This function passes a float value to Shader float parameter.
			bool SetParam(const char* pParamName_i, float fValue_i);
			bool SetFloatVectorParam(const char* pParamName_i, int size, float* tabVecs);
			bool SetIntVectorParam(const char* pParamName_i, int size, int* tabVecs);
			bool SetIntegerParam(const char* pParamName_i, int iValue_i);
            bool SetVec3Param(const char* pParamName_i, vec3f iValue_i);
			bool SetMatrixParam(const char* pParamName_i, float* tabVecs);

			GLuint GetProgramId()
			{
				return m_hProgramObject;
			}

			~GLSLShader(void);

		private:
			bool CreateComputeProgram(const wxString& nProgramID_i);
			bool CreateShaderProgram(const wxString& nProgramID_i);
			bool CreateVertexProgram(const wxString& nProgramID_i);
			bool check_shader_compile_status(GLuint obj);
			bool check_program_link_status(GLuint obj);

			GLuint m_hProgramObject;
			GLuint m_hShaderHandle;
			GLuint m_hVertexHandle;
			GLuint m_hComputeHandle;
		};
	}
}
