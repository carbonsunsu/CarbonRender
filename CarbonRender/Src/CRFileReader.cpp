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

	return data;
}