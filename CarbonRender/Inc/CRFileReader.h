#ifndef CR_FILEREADER
#define CR_FILEREADER

#include "..\Inc\CRGloble.h"

struct  TextFile
{
public :
	GLchar* data;
	int length;
};

class FileReader
{
public :
	static TextFile ReadTextFile(char* fileName);
	static char* BindString(char* str0, char* str1);
private :
	static int GetTextFileSize(char* fileName);
};

#endif