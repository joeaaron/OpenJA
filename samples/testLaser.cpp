#include "laser.h"

using namespace JA::CV;

int main()
{
	cv::Mat srcImg = cv::imread("laser.bmp");
	if (srcImg.empty())
	{
		std::cout << "An invalid input!";
		return -1;
	}

	bool widthResult = true;
	bool lengthResult = true;
	bool angleResult = true;
	std::vector<cv::Point> lPoints;

	//////////////////////////////////width judge////////////////////////////////////////
	Laser::LaserWidthDetect(srcImg, widthResult, lPoints);
	if (!widthResult)
		std::cout << "The width of the laser is unqualified!" << std::endl;
	else
		std::cout << "The width of the laser is qualified!" << std::endl;

	//////////////////////////////////angle judge////////////////////////////////////////
	Laser::LaserAngleDetect(lPoints, angleResult);
	if (!angleResult)
		std::cout << "The angle of the laser is unqualified!" << std::endl;
	else
		std::cout << "The angle of the laser is qualified!" << std::endl;
	//CV_Assert(widthResult);

	//////////////////////////////////length judge////////////////////////////////////////
	Laser::LaserLengthDetect(lPoints, lengthResult);
	if (!lengthResult)
		std::cout << "The length of the laser is unqualified!" << std::endl;
	else
		std::cout << "The length of the laser is qualified!" << std::endl;

	//cv::waitKey(0);						//only works if there is at least one HighGUI window created and the window is active
	system("pause");
	return 0;
}