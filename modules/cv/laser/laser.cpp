#include "Laser.h"
#include <stdint.h>

using namespace JA::CV;

const int LASER_THRESHOLD = 250;
const int STANDARD_WIDTH = 9;
const int STANDARD_ANGLE = 10;
const int STANDARD_LENGTH = 160;

LASER_API bool Laser::LaserWidthDetect(cv::Mat img, bool& result, std::vector<cv::Point>& lPoints)
{
	cv::Mat grayImg, detectedEdges;
	///image preprocess
	cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
	threshold(grayImg, detectedEdges, 140, 255, cv::THRESH_BINARY);

	///find all widths of two laser contour
	std::vector<int> laserWidths;
	int x = 0;
	for (; x < detectedEdges.cols; ++x)
	{
		int y = 0;
		int yResutlts = 0;
		int maxLineWidth = 0;
		int yPreviousValidDot = 0;
		int currentLineWidth = 0;

		for (; y < detectedEdges.rows; ++y)
		{
			uint8_t *pData = detectedEdges.ptr<uint8_t>(y) +x;
			if (*pData > LASER_THRESHOLD)
			{
				if (yPreviousValidDot >= y - 2)              //if the two-point gap is more than 1, discard the valid dot!
				{
					currentLineWidth++;
				}
				else
				{
					currentLineWidth = 1;
				}
				yPreviousValidDot = y;

				if (currentLineWidth > maxLineWidth)
				{
					maxLineWidth = currentLineWidth;
					yResutlts = y;
				}
			}
		}

		if (yResutlts > 0.01)
		{
			yResutlts = yResutlts - maxLineWidth / 2;               //midpoint
			lPoints.push_back(cv::Point(x, yResutlts));
		}
		laserWidths.push_back(maxLineWidth);
	}

	///judge the width
	for (std::vector<int>::iterator laserWidth = laserWidths.begin(); laserWidth != laserWidths.end(); ++laserWidth)
	{
		if (abs(*laserWidth - STANDARD_WIDTH) > 2)
		{
			result = false;
			break;
		}
		else
			result = true;
	}
	return result;
}

LASER_API bool Laser::LaserAngleDetect(std::vector<cv::Point> points, bool& result)
{
	cv::Vec4f line;
	cv::fitLine(points, line, cv::DIST_L2, 0, 0.01, 0.01);

	double k = line[1] / line[0];       //straight slope
	double laserAngle = atan(k) / CV_PI * 180;

	///judge the angle
	if (laserAngle > STANDARD_ANGLE)
		result = false;
	else
		result = true;

	return result;
}

LASER_API bool Laser::LaserLengthDetect(std::vector<cv::Point> points, bool& result)
{
	int end = points.size();
	int x0 = points[0].x;
	int y0 = points[0].y;

	int x1 = points[end - 1].x;
	int y1 = points[end - 1].y;

	double laserLengthSquare = powf((x0, x1), 2) + powf((y0, y1), 2);
	double laserLength = sqrt(laserLengthSquare);

	///judge the length
	if (abs(laserLength - STANDARD_LENGTH) > 10)
		result = false;
	else
		result = true;

	return result;
}
