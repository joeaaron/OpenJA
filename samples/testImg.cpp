#include "image.h"
#include <vector>

using namespace JA::CV;

int main()
{
	cv::Mat srcImg;
	char filename[100];
	std::vector<bool> result;
	//read imgs in cycle
	for (unsigned int i = 1; i < 13; i++)
	{
		sprintf(filename, "x%d.bmp", i);
		srcImg = cv::imread(filename, 0);

		bool bImgCorrect = false;
		Image::ImgAvgGrayValue(srcImg, bImgCorrect);

		result.push_back(bImgCorrect);
	}


	//display the result
	for (std::vector<bool>::iterator iter = result.begin();
		iter != result.end(); ++iter)
	{
		cout <<*iter;
	}
	
	system("pause");
	return 0;
}