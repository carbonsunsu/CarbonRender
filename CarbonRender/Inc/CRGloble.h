#ifndef CR_GLOBLE
#define CR_GLOBLE

#define FBXSDK_SHARED
#define BUFFER_OFFSET(offset) ((void *)(offset))
#define CR_VERTATTRIPOS_POS 0
#define CR_VERTATTRIPOS_NOR 1
#define CR_VERTATTRIPOS_TAG 2
#define CR_VERTATTRIPOS_UVS 3
#define CR_VERTATTRIPOS_COL 4
#define CR_VERTATTRIPOS_BNL 5
#define FIXEDUPDATE_TIME 0.01f//In seconds

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <ctime>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "..\Third\GL\include\glew.h"
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#define GLFW_EXPOSE_NATIVE_WIN32
#include "..\Third\GLFW\include\glfw3.h"
#include "..\Third\GLFW\include\glfw3native.h"
#pragma comment(lib, "glfw3.lib")

#include "..\Third\FBX\include\fbxsdk.h"
#pragma comment(lib, "libfbxsdk.lib")

#include "..\Third\Simple OpenGL Image Library\src\SOIL.h"
#pragma comment(lib, "SOIL.lib")

#include "..\Third\RapidXML\rapidxml.hpp"
#include "..\Third\RapidXML\rapidxml_print.hpp"

#include "..\Third\imgui\imgui.h"
#include "..\Third\imgui\imgui_impl_glfw.h"
#include "..\Third\imgui\imgui_impl_opengl3.h"

using namespace std;
using namespace rapidxml;
using namespace std::filesystem;

#endif