#include "image.h"

using namespace JA::CV;

int main()
{
	cv::Mat srcImg = cv::imread("x2.bmp");
	bool bImgCorrect = false;
	Image::ImgAvgGrayValue(srcImg, bImgCorrect);

	cout << bImgCorrect;
	return 0;
}