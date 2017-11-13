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

//************************************
// Method:    GetHash
// FullName:  JA::CV::Image::GetHash
// Access:    public static 
// Returns:   IMAGE_API bool
// Qualifier: Perceptual Hashing
// Parameter: cv::Mat src1
// Parameter: cv::Mat src2
// Parameter: bool & result
//************************************
IMAGE_API bool Image::GetHash(cv::Mat src1, cv::Mat src2, bool& result)
{

	cv::Mat matDst1, matDst2;

	cv::resize(src1, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(src2, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);
	cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);

	int iAvg1 = 0;
	int iAvg2 = 0;

	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = matDst1.ptr<uchar>(i);
		uchar* data2 = matDst2.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
		if (arr1[i] != arr2[i])
			++iDiffNum;

	std::cout << "iDiffNum = " << iDiffNum << std::endl;

	if (iDiffNum <= 5){
		std::cout << "two images ars very similar!" << std::endl;
		result = true;
	}
		
	else if (iDiffNum > 10){
		std::cout << "they are two different images!" << std::endl;
		result = false;
	}
		
	else{
		std::cout << "two image are somewhat similar!" << std::endl;
		result = true;
	}
		
	return result;
}
