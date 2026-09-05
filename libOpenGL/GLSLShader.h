#pragma once
#include "vec3.h"
#include "GLSLParameter.h"

namespace Regards
{
	namespace OpenGL
	{
		class GLSLShader
		{
		public:
			GLSLShader(void);
			~GLSLShader(void);

			bool CreateProgram(const wxString& nProgramID_i, GLenum glSlShaderType_i);
			bool DeleteShader();
			bool EnableShader();
			bool DisableShader();
			bool IsOk();

			bool SetTexture(const char* name, GLuint textureID, GLuint textureUnit);
			bool SetParam(const char* pParamName_i, float fValue_i);
			bool SetFloatVectorParam(const char* pParamName_i, int size, float* tabVecs);
			bool SetIntVectorParam(const char* pParamName_i, int size, int* tabVecs);
			bool SetIntegerParam(const char* pParamName_i, int iValue_i);
			bool SetVec3Param(const char* pParamName_i, vec3f iValue_i);
			bool SetMatrixParam(const char* pParamName_i, const float* tabVecs);

			GLuint GetProgramId()
			{
				return m_hProgramObject;
			}

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

			bool isOk = false;
			bool isLink = false;
		};
	}
}