#ifndef OPENJA_CV_LASER_H_
#define OPENJA_CV_LASER_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include "config.h"           //preprocessor
/// CV_CALIB_EXPORT
#ifdef CV_LASER_EXPORT
#define  LASER_API __declspec(dllexport)
#else
#define LASER_API __declspec(dllimport)
#endif 

using namespace std;

namespace JA{
	namespace CV{
		class LASER_API Laser
		{
		public:
			static bool LaserWidthDetect(cv::Mat img, bool& result, std::vector<cv::Point>& lPoints);
			static bool LaserAngleDetect(std::vector<cv::Point> points, bool& result);
			static bool LaserLengthDetect(std::vector<cv::Point> points, bool& result);

		};
	}
}

#endif