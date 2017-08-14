#include "..\Inc\CRShaderManager.h"

ShaderManager* ShaderManager::ins = nullptr;

ShaderManager::ShaderManager()
{
	logFile.open("ErrorLog.log", ios::out);
}

ShaderManager::~ShaderManager()
{
	logFile.close();
	ins = nullptr;
}

ShaderManager * ShaderManager::Instance()
{
	if (ins == nullptr)
		ins = new ShaderManager();

	return ins;
}

GLuint ShaderManager::BuildShader(char* shaderName, const GLchar ** shaderSrc, GLenum type)
{
	GLuint shaderIndex;
	GLint logLength, bComplied;
	char* log;

	shaderIndex = glCreateShader(type);
	glShaderSource(shaderIndex, 1, shaderSrc, NULL);
	glCompileShader(shaderIndex);
	glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &bComplied);
	if (!bComplied)
	{
		cout << shaderName << " compling fails, please check log" << endl;
		
		glGetShaderiv(shaderIndex, GL_INFO_LOG_LENGTH, &logLength);
		log = (GLchar*)malloc(logLength);
		glGetShaderInfoLog(shaderIndex, logLength, &logLength, log);
		logFile << log;
		free(log);
		system("Pause");
	}

	return shaderIndex;
}

GLuint ShaderManager::LoadShader(char* vShaderName, char* fShaderName)
{
	GLuint program;
	GLint logLength, bLinked;
	char* log;
	
	char* dir = "Resources\\Shader\\";
	char* realName = FileReader::BindString(dir, vShaderName);
	GLuint vShader = BuildShader(vShaderName, (const GLchar **)&FileReader::ReadTextFile(realName).data, GL_VERTEX_SHADER);
	realName = FileReader::BindString(dir, fShaderName);
	GLuint fShader = BuildShader(fShaderName, (const GLchar **)&FileReader::ReadTextFile(realName).data, GL_FRAGMENT_SHADER);
	
	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &bLinked);
	if (!bLinked)
	{
		cout << "[" << vShaderName << ", " << fShaderName << "] linking fails, please check error log" << endl;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(program, logLength, &logLength, log);
		logFile << log;
		free(log);
		system("Pause");
	}

	return program;
}

void ShaderManager::UseShader(GLuint shader)
{
	glUseProgram(shader);

	return;
}
