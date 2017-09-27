#include "file.h"

using namespace JA::CPLUSPLUS;

int main()
{
	std::cout << "please wait ...";
	File::copyDir("C:\\Users\\Aaron\\Desktop\\ECAT-Vision-Team\\build\\src\\gui\\images", "D:\\calibFiles\\cowa_R1");
	std::cout << "ok";
	return 0;
}