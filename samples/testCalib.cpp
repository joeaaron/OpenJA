#include "calib.h"
#include <windows.h>
using namespace JA::CV;
#define HAND_EYE_CALIBRATION_ROTATION_ANGLE  (-CV_PI/6)

int main()
{
	char buf[1000];
	GetCurrentDirectory(1000, buf);
	std::cout << buf << std::endl;

	std::string path = buf;
	std::string dirPath = path + "\\images\\";
	std::string xmlDir = path + "\\xml\\";
	std::string xmlBottomDir = path + "\\xmlbottom\\";
	std::string backup = "D:\\calibFiles\\";

	std::string mkDir1 = "md " + dirPath + "cowa_cam_config";
	std::string mkDir2 = "md " + dirPath + "cowa_cam_config\\" + "unaligned";
	std::string mkDir3 = "md " + dirPath + "cowa_cam_config\\" + "aligned";
	std::string mkDir4 = "md " + backup + "cowa_R1";

	system(mkDir1.c_str());
	system(mkDir2.c_str());
	system(mkDir3.c_str());
	system(mkDir4.c_str());

	const char* dstPath = "D:\\calibFiles\\cowa_R1";

	for (int i = 8; i <= 8; i++)
	{
		if (3 == i || 7 == i)
			continue;

		std::string currentPath;
		if (i < 5)
		{
			currentPath = dirPath + "top\\" + std::to_string(i);
			//copy the xml files to \\1 \\2 \\3 \\4
			std::string copyXML = "copy " + xmlDir + "* " + currentPath;
			system(copyXML.c_str());
		}
		else
		{
			currentPath = dirPath + "bottom\\" + std::to_string(i - 4);
			//copy the xml files to \\1 \\2 \\3 \\4
			std::string copyXML = "copy " + xmlBottomDir + "* " + currentPath;
			system(copyXML.c_str());
		}
		SetCurrentDirectory(currentPath.c_str());
		std::string fullDirPath = currentPath;
		int indexofCurrentDir = fullDirPath.find_last_of("\\");
		std::string currentDirName = fullDirPath.substr(indexofCurrentDir, fullDirPath.size()); // e.g. \\1 \\2 \\3 \\4
		std::string upperDirPath = fullDirPath.substr(0, indexofCurrentDir);

		int indexofUpperDir = upperDirPath.find_last_of("\\");
		std::string upperDirName = upperDirPath.substr(indexofUpperDir, upperDirPath.size());
		std::string logDirPath = fullDirPath.substr(0, indexofUpperDir);


		if (upperDirName == "\\top") // calibrate top laser
		{
			Calib::RunCalibrateCamera("in_VID5.xml", "out_camera_data.xml");
			Calib::RunCalibrateLaser("out_camera_data.xml", "_VID5.xml", "out_laser_camera.xml", "RawTransformationTable.bin");

			if (currentDirName == "\\5")
			{
				Calib::RunTableChange("RawTransformationTable.bin", upperDirPath + "\\..\\top\\1\\out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin");
			}
			else if (currentDirName == "\\6")
			{
				Calib::RunTableChange("RawTransformationTable.bin", upperDirPath + "\\..\\top\\3\\out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin");
			}
			else
			{
				Calib::RunStereoCalib("out_camera_data.xml", "config.xml", "out_stereo_data.xml");
				Calib::RunHandEyesCalib("out_stereo_data.xml", HAND_EYE_CALIBRATION_ROTATION_ANGLE, "out_handeyes_data.xml");
				Calib::RunTableChange("RawTransformationTable.bin", "out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin");
			}
		}
		else if (upperDirName == "\\bottom") // calibrate bottom laser
		{
			std::string outCameraDataFilePath = upperDirPath + "\\..\\top" + currentDirName + "\\out_camera_data.xml";
			cv::FileStorage fs(outCameraDataFilePath, cv::FileStorage::READ); // Read the settings
			cv::Mat cameraMatrix, distcofficients;
			fs["camera_matrix"] >> cameraMatrix;
			fs["distortion_coefficients"] >> distcofficients;

			Calib::RunCalibrateCamera("in_VID5.xml", "out_camera_data.xml", &cameraMatrix, &distcofficients);
			Calib::RunCalibrateLaser("out_camera_data.xml", "_VID5.xml", "out_laser_camera.xml", "RawTransformationTable.bin");

			if (currentDirName == "\\5")
			{
				Calib::RunTableChange("RawTransformationTable.bin", upperDirPath + "\\..\\top\\1\\out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin");
			}
			else if (currentDirName == "\\6")
			{
				Calib::RunTableChange("RawTransformationTable.bin", upperDirPath + "\\..\\top\\3\\out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin");
			}
			else
			{
				if (currentDirName == "\\2")
					Calib::RunTableChange("RawTransformationTable.bin", upperDirPath + "\\..\\top" + currentDirName + "\\out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin", 0);
				else
					Calib::RunTableChange("RawTransformationTable.bin", upperDirPath + "\\..\\top" + currentDirName + "\\out_handeyes_data.xml", "out_laser_camera.xml", -HAND_EYE_CALIBRATION_ROTATION_ANGLE / 2, 0, "transformationTable.bin");
			}
		}
		else
		{
			CV_Assert(false);
		}

		// copy *.bin file to output folder
		char buffer[256];
		std::string outBinFileName;
		if (upperDirName == "\\top")
		{
			outBinFileName = "transformationTable" + currentDirName.substr(1, 2) + ".bin";
		}
		else if (upperDirName == "\\bottom")
		{
			outBinFileName = "transformationTableBottom" + currentDirName.substr(1, 2) + ".bin";
		}
		else
		{
			CV_Assert(false);
		}

		std::string outDir = upperDirPath + "\\..\\cowa_cam_config\\aligned";
		std::ifstream in(fullDirPath + "\\transformationTable.bin", std::ios_base::in | std::ios_base::binary);
		std::ofstream out(outDir + "\\" + outBinFileName, std::ios_base::out | std::ios_base::binary);
		std::ofstream out2(outDir + "\\..\\" + outBinFileName, std::ios_base::out | std::ios_base::binary);
		while (!in.eof())
		{
			in.read(buffer, 256);       //从文件中读取256个字节的数据到缓存区  
			int n = in.gcount();             //由于最后一行不知读取了多少字节的数据，所以用函数计算一下。  
			out.write(buffer, n);       //写入那个字节的数据  
			out2.write(buffer, n);
		}
		in.close();
		out.close();
		out2.close();

		outDir = upperDirPath + "\\..\\cowa_cam_config\\unaligned";
		in.open(fullDirPath + "\\RawTransformationTable.bin", std::ios_base::in | std::ios_base::binary);
		out.open(outDir + "\\" + outBinFileName, std::ios_base::out | std::ios_base::binary);
		while (!in.eof())
		{
			in.read(buffer, 256);       //从文件中读取256个字节的数据到缓存区  
			int n = in.gcount();             //由于最后一行不知读取了多少字节的数据，所以用函数计算一下。  
			out.write(buffer, n);       //写入那个字节的数据  
		}
		in.close();
		out.close();

		cv::destroyAllWindows();
	}

	std::cout << "Press any key to continue!" << std::endl;
	getchar();

	return 0;
}