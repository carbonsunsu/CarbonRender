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
	TextFile data;
	data.length = GetTextFileSize(fileName);
	
	file.open(fileName, ios::in);
	if (!file)
	{
		cout << "Fail to open file: " << fileName << endl;
		system("pause");
	}

	data.data = (GLchar*)malloc(data.length + 1);
	file.read(data.data, data.length);
	file.close();

	data.data[data.length] = '\0';

	return data;
}

void FileReader::WriteTextFile(TextFile fileData)
{
	fstream file;

	//rapidxml::print();
}

char * FileReader::BindString(char * str0, char * str1)
{
	char* bind = (char*)malloc(strlen(str0) + strlen(str1) + 1);
	memset(bind, 0, sizeof(bind));
	strcat_s(bind, strlen(str0) + 1, str0);
	strcat_s(bind, strlen(str0) + strlen(str1) + 1, str1);

	return bind;
}