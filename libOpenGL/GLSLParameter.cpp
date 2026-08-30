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

	glUniform1fv(nParamObj, size, value);
}

void CGLSLParameterTexture::Add(GLint progHandle)
{
    if (progHandle == 0 || nTextureID_i == 0)
        return;

    const GLint location =
        glGetUniformLocation(
            progHandle,
            libelle.c_str());

    if (location < 0)
        return;

    // Unité de texture 0
    constexpr GLint textureUnit = 0;

    glActiveTexture(GL_TEXTURE0 + textureUnit);

    // nTextureID_i = ID réel de la texture OpenGL
    glBindTexture(
        GL_TEXTURE_2D,
        static_cast<GLuint>(nTextureID_i));

    // Le sampler reçoit le numéro de l'unité,
    // pas l'ID de la texture.
    glUniform1i(
        location,
        textureUnit);
}

void CGLSLParameterTexture::Add(GLint progHandle, GLint textureUnit)
{
    const GLint location =
        glGetUniformLocation(
            progHandle,
            libelle.c_str());

    if (location < 0)
        return;

    glActiveTexture(GL_TEXTURE0 + textureUnit);

    glBindTexture(
        GL_TEXTURE_2D,
        static_cast<GLuint>(nTextureID_i));

    glUniform1i(
        location,
        textureUnit);
}