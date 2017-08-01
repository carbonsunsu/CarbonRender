#ifndef CR_FILEREADER
#define CR_FILEREADER

#include <fstream>
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

private :
	static int GetTextFileSize(char* fileName);
};

#endif