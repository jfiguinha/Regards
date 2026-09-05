#include <header.h>
#include "GLSLShader.h"
#include <LibResource.h>
#include <ConvertUtility.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace Regards::OpenGL;

GLSLShader::GLSLShader(void) : m_hVertexHandle(0), m_hComputeHandle(0)
{
	m_hProgramObject = 0;
	m_hShaderHandle = 0;
}

GLSLShader::~GLSLShader(void)
{
	DeleteShader();
}

bool GLSLShader::IsOk()
{
	return isOk;
}

// Helper pour vérifier et afficher les erreurs du compilateur de shader
bool GLSLShader::check_shader_compile_status(GLuint obj)
{
	GLint status;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			std::vector<GLchar> errorLog(length);
			glGetShaderInfoLog(obj, length, &length, errorLog.data());
			std::cout << errorLog.data() << std::endl;
		}

		return false;
	}
	return true;
}


// Helper pour vérifier et afficher les erreurs du linker de programme
bool GLSLShader::check_program_link_status(GLuint obj)
{
	GLint status;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			std::vector<GLchar> errorLog(length);
			glGetProgramInfoLog(obj, length, &length, errorLog.data());
			std::cout << errorLog.data() << std::endl;
		}

		return false;
	}
	return true;
}

bool GLSLShader::CreateProgram(const wxString& nProgramID_i, GLenum glSlShaderType_i)
{
	if (m_hProgramObject == 0)
		m_hProgramObject = glCreateProgram();

	if (glSlShaderType_i == GL_VERTEX_SHADER)
	{
		isOk = CreateVertexProgram(nProgramID_i);
		return isOk;
	}
	if (glSlShaderType_i == GL_COMPUTE_SHADER)
	{
		isOk = CreateComputeProgram(nProgramID_i);
		return isOk;
	}

	isOk = CreateShaderProgram(nProgramID_i);
	return isOk;
}

bool GLSLShader::CreateShaderProgram(const wxString& nProgramID_i)
{
	m_hShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	if (0 == m_hShaderHandle)
	{
		return false;
	}

	wxString dataProgram = CLibResource::GetOpenGLShaderProgram(nProgramID_i);
	const char* src = dataProgram.c_str();

	glShaderSource(m_hShaderHandle, 1, &src, nullptr);
	glCompileShader(m_hShaderHandle);

	if (!check_shader_compile_status(m_hShaderHandle))
		return false;

	glAttachShader(m_hProgramObject, m_hShaderHandle);
	return true;
}

bool GLSLShader::CreateComputeProgram(const wxString& nProgramID_i)
{
	m_hComputeHandle = glCreateShader(GL_COMPUTE_SHADER);

	if (0 == m_hComputeHandle)
	{
		return false;
	}

	wxString dataProgram = CLibResource::GetOpenGLShaderProgram(nProgramID_i);
	const char* src = dataProgram.c_str();

	glShaderSource(m_hComputeHandle, 1, &src, nullptr);
	glCompileShader(m_hComputeHandle);

	if (!check_shader_compile_status(m_hComputeHandle))
		return false;

	glAttachShader(m_hProgramObject, m_hComputeHandle);
	return true;
}


bool GLSLShader::CreateVertexProgram(const wxString& nProgramID_i)
{
	m_hVertexHandle = glCreateShader(GL_VERTEX_SHADER);

	if (0 == m_hVertexHandle)
	{
		return false;
	}

	wxString kernelSource = CLibResource::GetOpenGLShaderProgram(nProgramID_i);
	const char* src = kernelSource.c_str();

	// NETTOYAGE Core Profile : Laisse OpenGL déduire la taille via la sentinelle de chaîne \0
	glShaderSource(m_hVertexHandle, 1, &src, nullptr);
	glCompileShader(m_hVertexHandle);

	if (!check_shader_compile_status(m_hVertexHandle))
		return false;

	glAttachShader(m_hProgramObject, m_hVertexHandle);
	return true;
}

bool GLSLShader::DeleteShader()
{
	if (m_hVertexHandle)
	{
		glDetachShader(m_hProgramObject, m_hVertexHandle);
		glDeleteShader(m_hVertexHandle);
		m_hVertexHandle = 0;
	}
	if (m_hComputeHandle)
	{
		glDetachShader(m_hProgramObject, m_hComputeHandle);
		glDeleteShader(m_hComputeHandle);
		m_hComputeHandle = 0;
	}
	if (m_hShaderHandle)
	{
		glDetachShader(m_hProgramObject, m_hShaderHandle);
		glDeleteShader(m_hShaderHandle);
		m_hShaderHandle = 0;
	}
	if (m_hProgramObject)
	{
		glDeleteProgram(m_hProgramObject);
		m_hProgramObject = 0;
	}
	return true;
}

bool GLSLShader::EnableShader()
{
	if (!isLink)
	{
		glLinkProgram(m_hProgramObject);

		if (!check_program_link_status(m_hProgramObject))
			return false;

		isLink = true;
	}

	glUseProgram(m_hProgramObject);
	return true;
}

bool GLSLShader::DisableShader()
{
	// NETTOYAGE : glActiveTexture et glBindTexture n'ont rien à faire ici en Core Profile.
	glUseProgram(0);
	return true;
}

bool GLSLShader::SetTexture(const char* name, GLuint textureID, GLuint textureUnit)
{
	if (m_hProgramObject == 0)
		return false;

	const GLint location = glGetUniformLocation(m_hProgramObject, name);
	if (location < 0)
		return false;

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(location, static_cast<GLint>(textureUnit));

	return glGetError() == GL_NO_ERROR;
}

bool GLSLShader::SetParam(const char* pParamName_i, const float fValue_i)
{
	GLint nParamObj = glGetUniformLocation(m_hProgramObject, pParamName_i);
	if (-1 == nParamObj)
	{
		return false;
	}

	glUniform1f(nParamObj, fValue_i);

	// NETTOYAGE : Remplacement de gluErrorString obsolète
	GLenum glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		std::string errStr;
		switch (glErr) {
		case GL_INVALID_ENUM:      errStr = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE:     errStr = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: errStr = "GL_INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:     errStr = "GL_OUT_OF_MEMORY"; break;
		default:                   errStr = "UNKNOWN_ERROR"; break;
		}
		std::cerr << "SetParam OpenGL error (" << pParamName_i << ") : " << errStr << "\n";
	}
	return (GL_NO_ERROR == glErr);
}

bool GLSLShader::SetIntegerParam(const char* pParamName_i, const int iValue_i)
{
	GLint nParamObj = glGetUniformLocation(m_hProgramObject, pParamName_i);
	if (-1 == nParamObj)
	{
		return false;
	}

	glUniform1i(nParamObj, iValue_i);
	return (GL_NO_ERROR == glGetError());
}

bool GLSLShader::SetFloatVectorParam(const char* pParamName_i, const int size, float* tabVecs)
{
	GLint nParamObj = glGetUniformLocation(m_hProgramObject, pParamName_i);
	if (-1 == nParamObj)
	{
		return false;
	}

	if (tabVecs != nullptr)
		glUniform1fv(nParamObj, size, tabVecs);

	return (GL_NO_ERROR == glGetError());
}

bool GLSLShader::SetIntVectorParam(const char* pParamName_i, const int size, int* tabVecs)
{
	GLint nParamObj = glGetUniformLocation(m_hProgramObject, pParamName_i);
	if (-1 == nParamObj)
	{
		return false;
	}

	if (tabVecs != nullptr)
		glUniform1iv(nParamObj, size, tabVecs);

	return (GL_NO_ERROR == glGetError());
}

bool GLSLShader::SetVec3Param(const char* pParamName_i, vec3f iValue_i)
{
	GLint nParamObj = glGetUniformLocation(m_hProgramObject, pParamName_i);
	if (-1 == nParamObj)
	{
		return false;
	}

	glUniform3f(nParamObj, iValue_i.x, iValue_i.y, iValue_i.z);

	return (GL_NO_ERROR == glGetError());
}

bool GLSLShader::SetMatrixParam(const char* pParamName_i, const float* tabVecs)
{
	if (m_hProgramObject == 0 || tabVecs == nullptr)
		return false;

	const GLint location = glGetUniformLocation(m_hProgramObject, pParamName_i);
	if (location < 0)
		return false;

	glUniformMatrix4fv(location, 1, GL_FALSE, tabVecs);

	return glGetError() == GL_NO_ERROR;
}