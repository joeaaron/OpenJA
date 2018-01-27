#ifndef OPENJA_CV_LASER_H_
#define OPENJA_CV_LASER_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include "config.h"           //preprocessor
/// CV_CALIB_EXPORT
#ifdef CV_IMAGE_EXPORT
#define IMAGE_API __declspec(dllexport)
#else
#define IMAGE_API __declspec(dllimport)
#endif 

using namespace std;

namespace JA{
	namespace CV{
		class IMAGE_API Image
		{
		public:
			static bool ImgAvgGrayValue(cv::Mat img, double& meanValue, bool& result);
			static bool GetHash(cv::Mat src1, cv::Mat src2, bool& result);
			static float GetSterringAngle(const string inputCameraDataFile, float& angle);
		private:
			static void calcBoardCornerPositions(float squareSize, vector<cv::Point3f>& corners);
			static float rotationMatrixToEulerAngles(cv::Mat& R, float& angle);
		};
	}
}

#endif