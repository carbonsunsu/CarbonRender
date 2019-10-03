#ifndef CR_GLHELPER
#define CR_GLHELPER

#include "..\Inc\CRGloble.h"

class GLHelper
{
public:
	static void SetGLArrayBuffer(GLuint bIndex, GLsizeiptr bSize, const GLvoid* bData, GLuint eSize, GLenum eType, GLuint aPos);
	static GLuint SetGLRenderTexture(GLsizei w, GLsizei h, GLint internalFormat, GLenum format, GLenum type, GLint filter, GLenum attach, bool mipmap);
	static void SetGLRenderTexture(GLuint rt, GLenum attach);
	static GLuint SetGLCubeRenderTexture(GLsizei size, GLint internalFormat, GLenum format, GLenum type);
	static GLuint SetGLDepthBuffer(GLsizei w, GLsizei h);
};

#endif
