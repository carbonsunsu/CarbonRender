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

TextFile FileReader::XML2Text(xml_node<>* xmlDoc)
{
	TextFile xmlData;

	string xmlStr;
	print(back_inserter(xmlStr), *xmlDoc, 0);
	xmlData.length = xmlStr.length();
	xmlData.data = (GLchar*)malloc(xmlData.length + 1);
	memcpy(xmlData.data, xmlStr.c_str(), xmlData.length);
	xmlData.data[xmlData.length] = '\0';

	return xmlData;
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

void FileReader::WriteTextFile(char* fileName, TextFile fileData)
{
	fstream file;

	file.open(fileName, ios::out);
	file.write(fileData.data, fileData.length);
	file.close();
}

char * FileReader::BindString(char * str0, char * str1)
{
	char* bind = (char*)malloc(strlen(str0) + strlen(str1) + 1);
	memset(bind, 0, sizeof(bind));
	strcat_s(bind, strlen(str0) + 1, str0);
	strcat_s(bind, strlen(str0) + strlen(str1) + 1, str1);

	return bind;
}