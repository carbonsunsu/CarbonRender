#ifndef CR_TEXMANAGER
#define CR_TEXMANAGER

#include "..\Inc\CRGloble.h"

class TextureManager
{
private:
	TextureManager();
	static TextureManager* ins;
	unordered_map<string, GLuint> texturesMap;

public:
	~TextureManager();
	static TextureManager* Instance();
	GLuint LoadTexture(string dir);
};

#endif