#ifndef CR_SHADERMANAGER
#define CR_SHADERMANAGER

#include "..\Inc\CRFileReader.h"

class ShaderProgram
{
private:
	GLuint programID;
	string vShaderName;
	GLuint vShaderID;
	string fShaderName;
	GLuint fShaderID;

	bool CompileShader(GLuint shaderID, const GLchar ** shaderSrc);

public:
	ShaderProgram();
	~ShaderProgram();
	void SetVertexShader(char* name);
	string GetVertexShaderName();

	void SetFragmentShader(char* name);
	string GetFragmentShaderName();

	void BuildShaderProgram();
	GLuint GetShaderProgram();
};

class ShaderManager
{
private:
	static ShaderManager* ins;
	unordered_map<unsigned int, ShaderProgram*> shaderPrograms;
	unsigned int newShaderID;

	ShaderManager();
	
public:
	~ShaderManager();
	static ShaderManager* Instance();
	unsigned int LoadShader(char* vShaderName, char* fShaderName);
	GLuint GetShaderProgram(unsigned int index);
	void RebuildShaders();
};

#endif
