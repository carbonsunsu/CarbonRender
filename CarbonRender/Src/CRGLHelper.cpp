#include "..\Inc\CRGLHelper.h"

GLuint GLHelper::sharedRT = -1;

void GLHelper::SetGLArrayBuffer(GLuint bIndex, GLsizeiptr bSize, const GLvoid* bData, GLuint eSize, GLenum eType, GLuint aPos)
{
	glBindBuffer(GL_ARRAY_BUFFER, bIndex);
	glBufferData(GL_ARRAY_BUFFER, bSize, bData, GL_STATIC_DRAW);
	glVertexAttribPointer(aPos, eSize, eType, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

GLuint GLHelper::SetGLRenderTexture(GLsizei w, GLsizei h, GLint internalFormat, GLenum format, GLenum type, GLint filter, GLenum attach)
{
	GLuint rt;
	glGenTextures(1, &rt);
	glBindTexture(GL_TEXTURE_2D, rt);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, rt, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return rt;
}

void GLHelper::SetGLRenderTexture(GLuint rt, GLenum attach)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, rt, 0);
}

GLuint GLHelper::SetGLCubeRenderTexture(GLsizei size, GLint internalFormat, GLenum format, GLenum type)
{
	GLuint rt;
	glGenTextures(1, &rt);
	glBindTexture(GL_TEXTURE_CUBE_MAP, rt);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 10, GL_RGB, size, size);
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, size, size, 0, format, type, NULL);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return rt;
}

GLuint GLHelper::SetGLDepthBuffer(GLsizei w, GLsizei h)
{
	GLuint buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return buffer;
}

void GLHelper::InitSharedRT(GLsizei w, GLsizei h, GLint internalFormat, GLenum format, GLenum type, GLint filter)
{
	glGenTextures(1, &sharedRT);
	glBindTexture(GL_TEXTURE_2D, sharedRT);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint GLHelper::GetSharedRT()
{
	return sharedRT;
}
