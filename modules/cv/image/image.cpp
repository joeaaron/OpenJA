#include "image.h"
#include <opencv2/core//core.hpp>

using namespace JA::CV;


IMAGE_API bool Image::ImgAvgGrayValue(cv::Mat img, double& meanValue, bool& result)
{
	if (1 != img.channels())
		cvtColor(img, img, cv::COLOR_BGR2GRAY);
	const double grayThreshold = 33;

	cv::Mat roiImg = img.clone();
	cv::Mat rowImg = roiImg.reshape(0, 1);
	cv::Mat convr, Mean;
	meanStdDev(rowImg, Mean, convr);

	meanValue = Mean.at<double>(0, 0);
 	double grayConvrValue = convr.at<double>(0, 0);

	if (meanValue > grayThreshold)
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