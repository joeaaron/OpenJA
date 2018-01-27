#include "image.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace JA::CV;

const float SQUARESIZE = 1.75;
const int BOARDSIZEWIDTH = 5;
const int BOARDSIZEHEIGHT = 4;

char filename[100];

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

//************************************
// Method:    GetSterringAngle
// FullName:  JA::CV::Image::GetSterringAngle
// Access:    public static 
// Returns:   IMAGE_API float
// Qualifier:
// Parameter: const string inputCameraDataFile
// Parameter: float & angle
//************************************
IMAGE_API float Image::GetSterringAngle(const string inputCameraDataFile, float& angle)
{
	cv::Mat cameraMatrix;
	cv::Mat distcofficients;

	//! [file_read]
	cv::FileStorage fs(inputCameraDataFile, cv::FileStorage::READ); // Read the settings
	if (!fs.isOpened())
	{
		std::cout << "Could not open the configuration file: \"" << inputCameraDataFile << "\"" << std::endl;
		return -1;
	}

	fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> distcofficients;

	///find image points
	cv::VideoCapture capture(0);
	if (!capture.isOpened())     //if the cam is opened
		return 1;
	bool stop(false);
	cv::Mat frame;
	cv::Mat dst;
	cv::namedWindow("capVideo");
	int i = 1;
	while (!stop)
	{
		vector<vector<cv::Point2f>> imagePoints;
		vector<cv::Point2f> pointBuf;
		vector<vector<cv::Point3f> > objectPoints(1);

		if (!capture.read(frame))
			break;
		cv::imshow("capVideo", frame);

		//Mat view = cv::imread("x0.bmp");
		cv::Mat view = frame;
		cv::Mat viewGray;
		cv::cvtColor(view, viewGray, cv::COLOR_BGR2GRAY);

		int chessBoardFlags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;
		cv::Size boardSize = cv::Size(BOARDSIZEWIDTH, BOARDSIZEHEIGHT);
		findChessboardCorners(view, boardSize, pointBuf, chessBoardFlags);   //pointBuf size : 117 = 13 * 9;
		cornerSubPix(viewGray, pointBuf, cv::Size(11, 11),
			cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
		//sortConnerPoints(pointBuf);        imagePoints counter to objectPoints
		imagePoints.push_back(pointBuf);

		///find object points
		calcBoardCornerPositions(SQUARESIZE, objectPoints[0]);

		///compute rotation vector
		cv::Mat rvecs, tvecs;
		solvePnP(objectPoints[0], imagePoints[0], cameraMatrix, distcofficients, rvecs, tvecs, false, CV_ITERATIVE);

		///rotation vector to rotation matrix
		cv::Mat R;
		cv::Rodrigues(rvecs, R);
		//R = getRotationMatix(cv::Mat(cv::Vec3d(0, 0, 1)), rvecs);         //2018-01-19 some problems needed to be verified later
		rotationMatrixToEulerAngles(R, angle);

		char c = cvWaitKey(33);

		//°´ENTER±£´æ
		if (13 == c)
		{
			sprintf(filename, "%s%d%s", "x", i++, ".bmp");
			cv::imwrite(filename, frame);
		}

		//Esc¼üÍ£Ö¹
		if (27 == c)
			break;
	}

	capture.release();
	return angle;
}

void Image::calcBoardCornerPositions(float squareSize, vector<cv::Point3f>& corners)
{
	corners.clear();

	for (int i = 0; i < BOARDSIZEHEIGHT; ++i)
		for (int j = 0; j < BOARDSIZEWIDTH; ++j)
			corners.push_back(cv::Point3f(j*squareSize, i*squareSize, 0));

}

//Checks if a matrix is a valid rotation matrix
bool isRotationMatrix(cv::Mat& R)
{
	cv::Mat Rt;
	transpose(R, Rt);
	cv::Mat shouldBeIdentity = Rt * R;
	cv::Mat I = cv::Mat::eye(3, 3, shouldBeIdentity.type());

	return norm(I, shouldBeIdentity) < 1e-6;
}

///pitch angle is the angle we need
void eulerAnglesToOrdinaryAngles(vector<float> rads, vector<float>& angles)
{

	for (vector<float>::iterator it = rads.begin(); it != rads.end(); it++)
	{
		float angle = *it / CV_PI * 180;     //rad to angle
		//cout << angle << endl;
		angles.push_back(angle);
	}

}

float Image::rotationMatrixToEulerAngles(cv::Mat& R, float& angle)
{
	vector<float> rads;
	vector<float> angles;

	assert(isRotationMatrix(R));

	float sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));
	bool singular = sy < 1e-6;		//If singular is true
	float x, y, z;

	if (!singular)
	{
		x = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
		y = atan2(-R.at<double>(2, 0), sy);
		z = atan2(R.at<double>(1, 0), R.at<double>(0, 0));
	}
	else
	{
		x = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
		y = atan2(-R.at<double>(2, 0), sy);
		z = 0;
	}


	rads.push_back(x);
	rads.push_back(y);
	rads.push_back(z);
	eulerAnglesToOrdinaryAngles(rads, angles);

	float pitch = angles[0];        //ROTATIONAXIS:X
	float roll = angles[1];			//ROTATIONAXIS:Y
	float yaw = angles[2];			//ROTATIONAXIS:Z

	cout << yaw << "\t" << roll << "\t" << pitch << endl;

	angle = pitch;
	return angle;
}
