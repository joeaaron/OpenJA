#include "image.h"
#include <vector>

using namespace JA::CV;

int main()
{
	cv::Mat srcImg;
	char filename[100];
	std::vector<bool> result;
	std::vector<double> meanValue;
	//read imgs in cycle
	for (unsigned int i = 1; i < 16; i++)
	{
		sprintf(filename, "x%d.bmp", i);
		srcImg = cv::imread(filename, 0);

		bool bImgCorrect = false;
		double value;
		Image::ImgAvgGrayValue(srcImg, value, bImgCorrect);

		meanValue.push_back(value);
		result.push_back(bImgCorrect);
	}

	//display the value
	for (std::vector<double>::iterator valueIter = meanValue.begin();
		valueIter != meanValue.end(); ++valueIter)
	{
		std::cout << *valueIter << std::endl;
	}

	//display the result
	for (std::vector<bool>::iterator iter = result.begin();
		iter != result.end(); ++iter)
	{
		std::cout << *iter << std::endl;
	}
	
	system("pause");
	return 0;
}