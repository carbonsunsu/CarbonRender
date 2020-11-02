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
	GLuint LoadTexture(char* filePath);
	GLuint LoadDefaultD();
	GLuint LoadDefaultN();
	GLuint LoadDefaultS();
	GLuint* LoadDefaultTexs();
	GLuint GetNullTex();
};

#endif