#include "calib.h"

using namespace JA::CV;
#define HAND_EYE_CALIBRATION_ROTATION_ANGLE  (-CV_PI/6)

int main()
{
	Calib calib;
	calib.RunCalibrateCamera("in_VID5.xml", "out_camera_data.xml");
	calib.RunCalibrateLaser("out_camera_data.xml", "_VID5.xml", "out_laser_camera.xml", "RawTransformationTable.bin");
	calib.RunStereoCalib("out_camera_data.xml", "config.xml", "out_stereo_data.xml");
	calib.RunHandEyesCalib("out_stereo_data.xml", HAND_EYE_CALIBRATION_ROTATION_ANGLE, "out_handeyes_data.xml");
	calib.RunTableChange("RawTransformationTable.bin", "out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin");
	return 0;
}