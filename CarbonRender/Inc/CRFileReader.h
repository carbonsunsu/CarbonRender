#ifndef CR_FILEREADER
#define CR_FILEREADER

#include "..\Inc\CRGloble.h"

struct  TextFile
{
public :
	char* data;
	int length;
};

class FileReader
{
public :
	static TextFile XML2Text(xml_node<>* xmlDoc);
	static TextFile ReadTextFile(char* fileName);
	static void WriteTextFile(char* fileName, TextFile fileData);
	static char* BindString(char* str0, char* str1);
private :
	static int GetTextFileSize(char* fileName);
};

#endif