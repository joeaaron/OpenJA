#include "calib.h"

using namespace JA::CV;

int main()
{
	Calib calib;
	calib.RunCalibrateCamera("in_VID5.xml", "out_camera_data.xml");
	return 0;
}