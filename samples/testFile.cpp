#include "file.h"

using namespace JA::CPLUSPLUS;

int main()
{
	//std::cout << "please wait ...";
	//File::copyDir("C:\\Users\\Aaron\\Desktop\\ECAT-Vision-Team\\build\\src\\gui\\images", "D:\\calibFiles\\cowa_R1");
	int num = 0 ;
	File::countDirNum("D:\\CalibFiles\\WUHU", num);
	std::cout << num;
	return 0;
}