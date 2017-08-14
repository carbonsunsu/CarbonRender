#ifndef CR_GLOBLE
#define CR_GLOBLE

#define FBXSDK_SHARED
#define BUFFER_OFFSET(offset) ((void *)(offset))
#define CR_VERTARRTIPOS_POS 0
#define CR_VERTARRTIPOS_NOR 1
#define CR_VERTARRTIPOS_TAG 2
#define CR_VERTARRTIPOS_UVS 3
#define CR_VERTARRTIPOS_COL 4


#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "..\Third\GL\include\glew.h"
#pragma comment(lib, "glew32.lib")
#include "..\Third\GL\include\glut.h"
#pragma comment(lib, "glut32.lib")
#include "..\Third\FBX\include\fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")

using namespace std;

#endif