#include "file.h"
#include <direct.h>
#include <io.h>
#include <tchar.h>
#include <stdio.h>

#define MAX_PATH 1024                         // �������·������
#define BUF_SIZE 256

using namespace JA::CPLUSPLUS;

int File::copyFile(const char * pSrc, const char *pDes)
{
	FILE *in_file, *out_file;
	char data[BUF_SIZE];
	size_t bytes_in, bytes_out;
	long len = 0;
	if ((in_file = fopen(pSrc, "rb")) == NULL)
	{
		perror(pSrc);
		return -2;
	}
	if ((out_file = fopen(pDes, "wb")) == NULL)
	{
		perror(pDes);
		return -3;
	}
	while ((bytes_in = fread(data, 1, BUF_SIZE, in_file)) > 0)
	{
		bytes_out = fwrite(data, 1, bytes_in, out_file);
		if (bytes_in != bytes_out)
		{
			perror("Fatal write error.\n");
			return -4;
		}
		len += bytes_out;
		printf("copying file .... %d bytes copy\n", len);
	}
	fclose(in_file);
	fclose(out_file);
	return 1;
}

FILE_API int File::copyDir(const char* pSrc, const char* pDes)
{
	if (NULL == pSrc || NULL == pDes)
		return -1;
	mkdir(pDes);

	char dir[MAX_PATH] = { 0 };
	char srcFileName[MAX_PATH] = { 0 };
	char dstFileName[MAX_PATH] = { 0 };

	char *str = "\\*.*";
	strcpy(dir, pSrc);
	strcat(dir, str);

	//���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(dir, &fileinfo)) != -1)
	{
		do
		{
			strcpy(srcFileName, pSrc);
			strcat(srcFileName, "\\");
			strcat(srcFileName, fileinfo.name);
			strcpy(dstFileName, pDes);
			strcat(dstFileName, "\\");
			strcat(dstFileName, fileinfo.name);
			//����ǲ���Ŀ¼
			//�������Ŀ¼,����д����ļ���������ļ�
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				copyFile(srcFileName, dstFileName);
			}
			else//����Ŀ¼���ݹ����
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					copyDir(srcFileName, dstFileName);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		return 1;
	}
	return -3;
}
//************************************
// Method:    countDirNum
// FullName:  JA::CPLUSPLUS::File::countDirNum
// Access:    public static 
// Returns:   FILE_API int
// Qualifier:
// Parameter: const char * pSrc
// Parameter: int & dirNum
//************************************
FILE_API int File::countDirNum(const char* pSrc, int& dirNum)
{
	char dir[MAX_PATH] = { 0 };
	char *str = "\\*.*";

	strcpy(dir, pSrc);
	strcat(dir, str);

	//���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(dir, &fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�������Ŀ¼,����д����ļ���������ļ�
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				continue;
			}
			else//����Ŀ¼���ݹ����
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					dirNum++;
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		return 1;
	}

	return dirNum;
}

FILE_API void File::getAllFiles(string pSrcPath, vector<string>& files)
{
	long hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(pSrcPath).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{	
			//������ļ��оͽ����ļ��У�����
			if ((fileinfo.attrib &  _A_SUBDIR))
			{		
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(pSrcPath).append("\\").append(fileinfo.name));
					getAllFiles(p.assign(pSrcPath).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(pSrcPath).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

}

//************************************
// Method:    delete the specified format files
// FullName:  JA::CPLUSPLUS::File::delAllFormatFiles
// Access:    public static 
// Returns:   FILE_API void
// Qualifier:
// Parameter: string path
//************************************
FILE_API void File::delAllFormatFiles(string path)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;
	// \\* ����Ҫ�������е�����
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		cout << "error no file!" << endl;
		return;
	}
	do
	{
		//�ж��ļ����������ļ��л����ļ�
		cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;
		//������ļ��оͽ����ļ��У�����
		if (fileInfo.attrib&_A_SUBDIR) {
			{//�����ļ�ϵͳʱ����"."��".."�ļ�
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
					string tmp;
					tmp = path + "\\" + fileInfo.name;
					delAllFormatFiles(tmp);
				}
			}

		}
		//���ļ��Ļ��Ͳ鿴�ļ��������ǡ�bmp����ɾ��
		else {
			//delete file
			if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
				std::string fileSuffix(fileInfo.name);
				fileSuffix = fileSuffix.substr(fileSuffix.find_last_of('.') + 1);		//��ȡ�ļ���׺  
				if (strcmp(fileSuffix.c_str(), "bmp")) {
					string delpath = path + "\\" + fileInfo.name;
					if (remove(delpath.c_str()) != 0)			//ɾ��ʧ�ܾͱ���
						perror("Error deleting file");
					else {
						cout << fileInfo.name << "deleted" << endl;
					}
				}
			}
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}

FILE_API void File::delAllFiles(string path)
{
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;
	// \\* ����Ҫ�������е�����
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		cout << "error no file!" << endl;
		return;
	}
	do
	{
		//�ж��ļ����������ļ��л����ļ�
		cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;
		//������ļ��оͽ����ļ��У�����
		if (fileInfo.attrib&_A_SUBDIR) {
			{//�����ļ�ϵͳʱ����"."��".."�ļ�
				if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
					string tmp;
					tmp = path + "\\" + fileInfo.name;
					delAllFiles(tmp);
				}
			}

		}
		else {
			//delete file
			if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0) {
			
				string delpath = path + "\\" + fileInfo.name;
				if (remove(delpath.c_str()) != 0)			//ɾ��ʧ�ܾͱ���
					perror("Error deleting file");
				else {
					cout << fileInfo.name << "deleted" << endl;
				}
			
			}
		}
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}
//************************************
// Method:    getAllFormatFiles
// FullName:  JA::CPLUSPLUS::File::getAllFormatFiles
// Access:    public static 
// Returns:   FILE_API void
// Qualifier:
// Parameter: string path
// Parameter: vector<string> & files
// Parameter: string format
// From: http://blog.csdn.net/adong76/article/details/39432467
//************************************
FILE_API void File::getAllFormatFiles(string path, vector<string>& files, string format)
{
	//�ļ����    
	long   hFile = 0;
	//�ļ���Ϣ    
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
					getAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}