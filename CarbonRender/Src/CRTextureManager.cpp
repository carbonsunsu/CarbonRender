#include "..\Inc\CRTextureManager.h"

TextureManager* TextureManager::ins = nullptr;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	texturesMap.clear();
	ins = nullptr;
}

TextureManager * TextureManager::Instance()
{
	if (ins == nullptr)
		ins = new TextureManager();

	return ins;
}

GLuint TextureManager::LoadTexture(string dir)
{
	if (texturesMap.find(dir) != texturesMap.end())
		return texturesMap[dir];

	GLuint tex = SOIL_load_OGL_texture(dir.c_str(),
		SOIL_LOAD_AUTO, 
		SOIL_CREATE_NEW_ID, 
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

	if (tex == 0)
		cout << "SOIL loading error: " << SOIL_last_result() << endl;
	else
		texturesMap[dir] = tex;

	return tex;
}
