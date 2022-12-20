#include <header.h>
#include "GLSLParameter.h"
using namespace Regards::OpenGL;

void CGLSLParameter::SetLibelle(const string& libelle)
{
	this->libelle = libelle;
}

void CGLSLParameterInt::Add(GLint progHandle)
{
	GLint nParamObj = glGetUniformLocation(progHandle, libelle.c_str());
	if (-1 == nParamObj)
	{
		return;
	}

	glUniform1i(nParamObj, value);
}

void CGLSLParameterUInt::Add(GLint progHandle)
{
	GLint nParamObj = glGetUniformLocation(progHandle, libelle.c_str());
	if (-1 == nParamObj)
	{
		return;
	}

	glUniform1ui(nParamObj, value);
}

void CGLSLParameterFloat::Add(GLint progHandle)
{
	GLint nParamObj = glGetUniformLocation(progHandle, libelle.c_str());
	if (-1 == nParamObj)
	{
		return;
	}

	glUniform1f(nParamObj, value);
}

void CGLSLParameterTabFloat::Add(GLint progHandle)
{
	GLint nParamObj = glGetUniformLocation(progHandle, libelle.c_str());
	if (-1 == nParamObj)
	{
		return;
	}

	if (nParamObj != -1)
		glUniform1fv(nParamObj, size, value);
}

void CGLSLParameterTabInt::Add(GLint progHandle)
{
	GLint nParamObj = glGetUniformLocation(progHandle, libelle.c_str());
	if (-1 == nParamObj)
	{
		return;
	}

	if (nParamObj != -1)
		glUniform1iv(nParamObj, size, value);
}

void CGLSLParameterTexture::Add(GLint progHandle)
{
	GLint nParamObj = glGetUniformLocation(progHandle, libelle.c_str());
	if (-1 == nParamObj)
	{
		return;
	}

	glActiveTexture(GL_TEXTURE0 + nTextureID_i);
	glBindTexture(GL_TEXTURE_2D, nTextureID_i);
	glUniform1i(nParamObj, nTextureID_i);

	//bool error = (GL_NO_ERROR == glGetError());
}
