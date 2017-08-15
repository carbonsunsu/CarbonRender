#ifndef CR_SHADERMANAGER
#define CR_SHADERMANAGER

#include "..\Inc\CRGloble.h"
#include "..\Inc\CRFileReader.h"

class ShaderManager
{
private:
	static ShaderManager* ins;

	ShaderManager();
	GLuint BuildShader(char* shaderName, const GLchar ** shaderSrc, GLenum type);
public:
	~ShaderManager();
	static ShaderManager* Instance();
	GLuint LoadShader(char* vShaderName, char* fShaderName);
	void UseShader(GLuint shader);
};

#endif
