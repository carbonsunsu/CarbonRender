#ifndef CR_GLOBLE
#define CR_GLOBLE

#define FBXSDK_SHARED
#define BUFFER_OFFSET(offset) ((void *)(offset))
#define CR_VERTATTRIPOS_POS 0
#define CR_VERTATTRIPOS_NOR 1
#define CR_VERTATTRIPOS_TAG 2
#define CR_VERTATTRIPOS_UVS 3
#define CR_VERTATTRIPOS_COL 4
#define CR_VERTATTRIPOS_BNL 5
#define FIXEDUPDATE_TIME 0.02f

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <unordered_map>

#include "..\Third\GL\include\glew.h"
#pragma comment(lib, "glew32.lib")
#include "..\Third\GL\include\glut.h"
#pragma comment(lib, "glut32.lib")
#include "..\Third\FBX\include\fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")
#include "..\Third\Simple OpenGL Image Library\src\SOIL.h"
#pragma comment(lib, "SOIL.lib")

using namespace std;

inline void SetGLArrayBuffer(GLuint bIndex, GLsizeiptr bSize, const GLvoid* bData, GLuint eSize, GLenum eType, GLuint aPos)
{
	glBindBuffer(GL_ARRAY_BUFFER, bIndex);
	glBufferData(GL_ARRAY_BUFFER, bSize, bData, GL_STATIC_DRAW);
	glVertexAttribPointer(aPos, eSize, eType, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

inline GLuint SetGLRenderTexture(GLsizei w, GLsizei h, GLint internalFormat, GLenum format, GLenum type, GLenum attach)
{
	GLuint rt;
	glGenTextures(1, &rt);
	glBindTexture(GL_TEXTURE_2D, rt);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, rt, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	return rt;
}

inline GLuint SetGLCubeRenderTexture(GLsizei size, GLint internalFormat, GLenum format, GLenum type)
{
	GLuint rt;
	glGenTextures(1, &rt);
	glBindTexture(GL_TEXTURE_CUBE_MAP, rt);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexStorage2D(GL_TEXTURE_CUBE_MAP, 10, GL_RGB, size, size);
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, size, size, 0, format, type, NULL);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return rt;
}

inline GLuint SetGLDepthBuffer(GLsizei w, GLsizei h)
{
	GLuint buffer;
	glGenRenderbuffers(1, &buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return buffer;
}

#endif