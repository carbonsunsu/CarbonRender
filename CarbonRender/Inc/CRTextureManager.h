#ifndef CR_TEXMANAGER
#define CR_TEXMANAGER

#include "..\Inc\CRFileReader.h"

class TextureManager
{
private:
	TextureManager();
	static TextureManager* ins;
	unordered_map<string, GLuint> texturesMap;
	GLuint defaultTex[3];
	GLuint nullTex;

public:
	~TextureManager();
	static TextureManager* Instance();
	GLuint LoadTexture(string dir);
	GLuint LoadDefaultD();
	GLuint LoadDefaultN();
	GLuint LoadDefaultS();
	GLuint GetNullTex();
};

#endif