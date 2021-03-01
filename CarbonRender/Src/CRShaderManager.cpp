#include "..\Inc\CRShaderManager.h"

bool ShaderProgram::CompileShader(GLuint shaderID, const GLchar ** shaderSrc)
{
	GLint isCompiled;

	glShaderSource(shaderID, 1, shaderSrc, NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		char* log;
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		log = (char*)malloc(logLength);
		glGetShaderInfoLog(shaderID, logLength, &logLength, log);
		cout << log << endl;
		free(log);
		
		return false;
	}

	return true;
}

ShaderProgram::ShaderProgram()
{
	programID = 0;
	vShaderID = 0;
	fShaderID = 0;
}

ShaderProgram::~ShaderProgram()
{
	if (programID != 0)
	{
		if (vShaderID != 0)
		{
			glDetachShader(programID, vShaderID);
			glDeleteShader(vShaderID);
		}
		if (fShaderID != 0)
		{
			glDetachShader(programID, fShaderID);
			glDeleteShader(fShaderID);
		}
		glDeleteProgram(programID);
	}
}

void ShaderProgram::SetVertexShader(char * name)
{
	vShaderName = name;
}

string ShaderProgram::GetVertexShaderName()
{
	return vShaderName;
}

void ShaderProgram::SetFragmentShader(char * name)
{
	fShaderName = name;
}

string ShaderProgram::GetFragmentShaderName()
{
	return fShaderName;
}

void ShaderProgram::BuildShaderProgram()
{
	GLint isLinked;

	char* dir = "Resources\\Shader\\";
	char* vShaderFilePath = FileReader::BindString(dir, (char*)vShaderName.c_str());
	char* fShaderFilePath = FileReader::BindString(dir, (char*)fShaderName.c_str());

	if (vShaderID == 0)
		vShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (fShaderID == 0)
		fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	TextFile vShaderSourceCode = FileReader::ReadTextFile(vShaderFilePath);
	TextFile fShaderSourceCode = FileReader::ReadTextFile(fShaderFilePath);

	if (!CompileShader(vShaderID, (const GLchar **)&vShaderSourceCode.data))
	{
		cout << "Compiling " << vShaderName << " failed" << endl;
		system("Pause");
	}
	if (!CompileShader(fShaderID, (const GLchar **)&fShaderSourceCode.data))
	{
		cout << "Compiling " << fShaderName << " failed" << endl;
		system("Pause");
	}

	free(vShaderSourceCode.data);
	free(fShaderSourceCode.data);

	if (programID == 0)
	{
		programID = glCreateProgram();
		glAttachShader(programID, vShaderID);
		glAttachShader(programID, fShaderID);
	}
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		GLint logLength;
		char* log;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(programID, logLength, &logLength, log);
		cout << log << endl;
		free(log);
		cout << "Linking program " << "[" << vShaderName << " " << fShaderName << "] failed" << endl;
		system("Pause");
	}
}

GLuint ShaderProgram::GetShaderProgram()
{
	return programID;
}


ShaderManager* ShaderManager::ins = nullptr;

ShaderManager::ShaderManager()
{
	newShaderID = 0;
}

ShaderManager::~ShaderManager()
{
	ins = nullptr;
}

ShaderManager * ShaderManager::Instance()
{
	if (ins == nullptr)
		ins = new ShaderManager();

	return ins;
}

unsigned int ShaderManager::LoadShader(char* vShaderName, char* fShaderName)
{
	ShaderProgram* newShaderProgram = new ShaderProgram();
	newShaderProgram->SetVertexShader(vShaderName);
	newShaderProgram->SetFragmentShader(fShaderName);
	newShaderProgram->BuildShaderProgram();

	unsigned int newID = newShaderID;
	newShaderID++;
	shaderPrograms[newID] = newShaderProgram;

	return newID;
}

GLuint ShaderManager::GetShaderProgram(unsigned int index)
{
	return shaderPrograms[index]->GetShaderProgram();
}

void ShaderManager::RebuildShaders()
{
	for (unordered_map<unsigned int, ShaderProgram*>::iterator i = shaderPrograms.begin(); i != shaderPrograms.end(); i++)
		i->second->BuildShaderProgram();
}