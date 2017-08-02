#ifndef CR_FBXMANAGER
#define CR_FBXMANAGER

#include "..\Inc\CRGloble.h"

class FbxImportManager
{
private:
	static FbxImportManager* ins;
	FbxManager* fbxManager;
	FbxIOSettings* ioSettings;

	FbxImportManager();
	void Init();
public:
	~FbxImportManager();
	static FbxImportManager* instance();
	
};

#endif