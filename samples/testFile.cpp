#include "file.h"
#include <vector>
#include <io.h>
#include <fstream>
#include <string>
using namespace JA::CPLUSPLUS;

int main()
{
	//std::cout << "please wait ...";
	//File::copyDir("C:\\Users\\Aaron\\Desktop\\ECAT-Vision-Team\\build\\src\\gui\\images", "D:\\calibFiles\\cowa_R1");

	/*int num = 0 ;
	File::countDirNum("D:\\CalibFiles\\WUHU", num);
	std::cout << num;*/

	string filePath = "G:\\JA\\OpenJA\\samples\\data\\img";
	vector<string> files;
	//save the info to allfiles.txt
	char * distAll = "AllFiles.txt";
	//File::getAllFiles(filePath, files);
	//读取所有格式为jpg的文件  

	/*string format = ".bmp";
	File::getAllFormatFiles(filePath, files, format);
	ofstream ofn(distAll);
	int size = files.size();
	ofn << size << endl;
	for (int i = 0; i < size; i++)
	{
		ofn << files[i] << endl;
		std::cout << files[i] << endl;
	}
	ofn.close();*/
	File::delAllFiles(filePath);
	return 0;
}