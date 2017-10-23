#include "image.h"
#include <opencv2/core//core.hpp>

using namespace JA::CV;

const double grayThreshold = 50;

IMAGE_API bool Image::ImgAvgGrayValue(cv::Mat img, bool& result)
{
	cv::Mat grayImg; 
	cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);

	cv::Mat roiImg = grayImg.clone();
	cv::Mat rowImg = roiImg.reshape(0, 1);
	cv::Mat convr, Mean;
	meanStdDev(rowImg, Mean, convr);

	double grayValue = Mean.at<double>(0, 0);

	if (grayValue > grayThreshold)
	{
		result = true;
		return result;
	}
	else
	{
		result = false;
		return result;
	}
}