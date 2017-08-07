#include "..\Inc\CRFileReader.h"

int FileReader::GetTextFileSize(char* fileName)
{
	fstream file;
	int size = 0;
	file.open(fileName, ios::in);
	while (file.get() != EOF)
	{
		size++;
	}

	file.close();
	return size;
}

TextFile FileReader::ReadTextFile(char* fileName)
{
	fstream file;
	int fileSize = GetTextFileSize(fileName);
	GLchar* fileData;
	
	file.open(fileName, ios::in);
	if (!file)
	{
		cout << "Fail to open file" << endl;
		system("pause");
	}

	fileData = (GLchar*)malloc(fileSize);
	file.read(fileData, fileSize);
	file.close();

	TextFile data;
	data.data = fileData;
	data.length = fileSize;
	data.data[data.length] = 0;

	return data;
}

char * FileReader::BindString(char * str0, char * str1)
{
	char* bind = (char*)malloc(strlen(str0) + strlen(str1) + 1);
	memset(bind, 0, sizeof(bind));
	strcat_s(bind, strlen(str0) + 1, str0);
	strcat_s(bind, strlen(str0) + strlen(str1) + 1, str1);

	return bind;
}
