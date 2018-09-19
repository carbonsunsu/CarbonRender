#ifndef GL_HELPER
#define GL_HELPER

#include "..\Inc\CRGloble.h"

class GLHelper
{
public:
	static void SetGLArrayBuffer(GLuint bIndex, GLsizeiptr bSize, const GLvoid* bData, GLuint eSize, GLenum eType, GLuint aPos);
	static GLuint SetGLRenderTexture(GLsizei w, GLsizei h, GLint internalFormat, GLenum format, GLenum type, GLenum attach, bool mipmap);
	static void SetGLRenderTexture(GLuint rt, GLenum attach);
	static GLuint SetGLCubeRenderTexture(GLsizei size, GLint internalFormat, GLenum format, GLenum type);
	static GLuint SetGLDepthBuffer(GLsizei w, GLsizei h);
};

#endif
