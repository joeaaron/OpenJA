#ifndef OPENJA_CV_CALIB_H_
#define OPENJA_CV_CALIB_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include "config.h"           //preprocessor
/// CV_CALIB_EXPORT
#ifdef CV_CALIB_EXPORT
#define  CALIB_API __declspec(dllexport)
#else
#define CALIB_API __declspec(dllimport)
#endif 

using namespace std;

namespace JA{
	namespace CV{
		class CALIB_API Calib
		{
		public:
			int RunCalibrateCamera(const std::string inputSettingsFile, const std::string outCameraDataFilePath, cv::Mat* prefCameraMatrix = NULL, cv::Mat* prefDistCoefficent = NULL);
		private:
			void sortConnerPoints(std::vector<cv::Point2f>& corners);
		};
	}
}

#endif