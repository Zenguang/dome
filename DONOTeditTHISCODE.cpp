/*一个普通的相机采集代码	
//--------------------------------------------不要修改这个代码---------------------------------
//--------------------------------------------不要修改这个代码---------------------------------
//--------------------------------------------不要修改这个代码---------------------------------
//--------------------------------------------不要修改这个代码---------------------------------
//--------------------------------------------不要修改这个代码---------------------------------
//——————————————————————若使用其他代码采集图片，请先将这个cpp文件从工程中移除，不是删除————————————————————————————
//——————————————————————若使用其他代码采集图片，请先将这个cpp文件从工程中移除，不是删除————————————————————————————
//——————————————————————若使用其他代码采集图片，请先将这个cpp文件从工程中移除，不是删除————————————————————————————
//——————————————————————若使用其他代码采集图片，请先将这个cpp文件从工程中移除，不是删除————————————————————————————
//——————————————————————祝你全家身体健康—————————————————
//author：zenguang
*/



#include <stdio.h>
#include <filesystem>
#include <direct.h>
#include <tchar.h>
#include <conio.h>
#include <windows.h>
#include <direct.h>
#include <io.h>
#include "opencv2/opencv.hpp"
#include "VirtualFG30.h"
#include <boost/filesystem.hpp>
#pragma comment (lib, "VirtualFG30.lib")


using namespace std;
using namespace cv;


// HERE A SERIAL NUM LIST

//#define	SERIAL_NUM_CAMERA_ORDER_01     
//#define	SERIAL_NUM_CAMERA_ORDER_02     
//#define	SERIAL_NUM_CAMERA_ORDER_03     
#define	SERIAL_NUM_CAMERA_ORDER_04     "221215A0010"
//#define	SERIAL_NUM_CAMERA_ORDER_05     
#define	SERIAL_NUM_CAMERA_ORDER_06     "221216C0006"
#define	SERIAL_NUM_CAMERA_ORDER_07     "221216C0001"
#define	SERIAL_NUM_CAMERA_ORDER_08     "221215A0011"
#define	SERIAL_NUM_CAMERA_ORDER_09     "221213B9003"
#define	SERIAL_NUM_CAMERA_ORDER_10     "221216C0004"
#define	SERIAL_NUM_CAMERA_ORDER_11     "221216C0002"
#define	SERIAL_NUM_CAMERA_ORDER_12     "221216C0003"
#define	SERIAL_NUM_CAMERA_ORDER_13     "221216C0008"
#define	SERIAL_NUM_CAMERA_ORDER_14     "221215A0012"
#define	SERIAL_NUM_CAMERA_ORDER_15     "221216C0005"
#define	SERIAL_NUM_CAMERA_ORDER_16     "221216C0007"
// #define	SERIAL_NUM_CAMERA_ORDER_17     "22121369008"
#define	SERIAL_NUM_CAMERA_ORDER_17     "22121660002"

//parameter list here

#define GAIN_RAW  312
#define BALANCE_RATIO  1.6f
#define EXPOSURE  15000  // 10*exp(-6) s 
#define CAM_ACQUISITIONF_RAMERATE 15
#define ROOTPATH "D:\\zengData\\"

char* prefix0 = "";
char* prefix1 = "";
char* prefix2 = "";
char* prefix3 = "";
char* prefix4 = "";

char* flieName0 = "videoname0.avi";//保存的视频文件名
char* flieName1 = "videoname1.avi";
char* flieName2 = "videoname2.avi";
char* flieName3 = "videoname3.avi";
char* flieName4 = "videoname4.avi";

//define frame count per scene  
int Num = 1;
// # of scene
int WIDTH = 1296, HEIGHT = 966;
int endFrame = 0;
char fileName[50];
unsigned char* g_pImage = NULL;
const int Thr = 60;
Mat img_src;
// for video output use
int isColor = 1;//图片是否为彩色
double fps = 10; //帧率 按照需求设置
int startFrame = 0;//
char cur_fn[255];//
char* ext = ".bmp";//序列图片后缀名

int ordernum = -1;


int Frames2Video(int ENDFRAME, const char *PREFIX, const char *FILENAME)
{
	startFrame = 0;
	CvSize size = cvSize(WIDTH, HEIGHT);
	VideoWriter output_src(FILENAME, CV_FOURCC('D', 'I', 'V', 'X'), fps, size, 1);
	//VideoWriter output_src(FILENAME, -1, fps, size, 1);
	while (startFrame <= endFrame)
	{
		strcpy(cur_fn, "");
		sprintf(cur_fn, "%s%03d%s", PREFIX, startFrame, ext);
		img_src = imread(cur_fn);
		//cout << img_src.channels();
		if (img_src.empty())
		{
			printf("cannot load images check prefix/path\n");
			printf("any key to exit\n");
			getchar();
			return 0;

		}

		std::cout << "Write frame " << startFrame << std::endl;
		//output_src<<img_src;
		output_src.write(img_src);
		startFrame++;
	}
	return 0;
}

Mat CreateAndConvertMat(Mat& mat, const uchar*p, const int &width, const int &height)
{
	Mat rgbImage(height, width, CV_8UC3);
	memcpy((uchar*)mat.data, p, sizeof(uchar)*height*width * 1);
	cvtColor(mat, rgbImage, CV_BayerRG2RGB);
	return rgbImage;
}

//intput serial number 
int getOrderNum(char DeviceID[])
{
	string temp = DeviceID;
	int orderNum = -1;
	if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_06))
		orderNum = 6;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_07))
		orderNum = 7;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_08))
		orderNum = 8;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_09))
		orderNum = 9;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_10))
		orderNum = 10;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_11))
		orderNum = 11;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_12))
		orderNum = 12;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_13))
		orderNum = 13;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_14))
		orderNum = 14;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_15))
		orderNum = 15;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_16))
		orderNum = 16;
	else if (!temp.compare(SERIAL_NUM_CAMERA_ORDER_17))
		orderNum = 17;
	else
	{
		printf("cannot find corresponding camera,check again\n"); return -1;
	}

	return orderNum;
}

int GetDeviceInformation(int hDevice)
{
	unsigned __int32 size;
	char szBuffer[256];
	printf("**************************\n");
	// Device ID
	size = 256;
	ST_GetStrReg(hDevice, MCAM_DEVICE_ID, szBuffer, &size);
	ordernum = getOrderNum(szBuffer);
	printf("This is camera%d\nDevice ID : %s\n", ordernum, szBuffer);


	// Device Model Name
	size = 256;
	ST_GetStrReg(hDevice, MCAM_DEVICE_MODEL_NAME, szBuffer, &size);
	printf("Device Model Name : %s\n", szBuffer);

	// Device Version
	size = 256;
	ST_GetStrReg(hDevice, MCAM_DEVICE_VERSION, szBuffer, &size);
	printf("Device Version : %s\n", szBuffer);

	//// Device Scan Type	
	//size = 256;
	//ST_GetEnumReg(hDevice, MCAM_DEVICE_SCANTYPE, szBuffer, &size);
	//printf("Device Scan Type : %s\n", szBuffer);

	//// Device Vendor Name
	//size = 256;
	//ST_GetStrReg(hDevice, MCAM_DEVICE_VENDOR_NAME, szBuffer, &size);
	//printf("Device Vendor Name : %s\n", szBuffer);

	//// Device Manufacture Info
	//size = 256;
	//ST_GetStrReg(hDevice, MCAM_DEVICE_MANUFACTURER_INFO, szBuffer, &size);
	//printf("Device Manufacture Info : %s\n", szBuffer);
	printf("**************************\n\n");
	return ordernum;
}

void SetFeature(int hDevice)
{
	__int32 status = MCAM_ERR_SUCCESS;


	status = ST_SetEnumReg(hDevice, MCAM_ACQUISITION_FRAMERATE_ENABLE, GAIN_AUTO_OFF);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	//// analog control
	status = ST_SetEnumReg(hDevice, MCAM_GAIN_AUTO, GAIN_AUTO_OFF);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	status = ST_SetEnumReg(hDevice, MCAM_GAIN_SELECTOR, GAIN_SELECTOR_ALL);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	//参数修改1
	status = ST_SetIntReg(hDevice, MCAM_GAIN_RAW, GAIN_RAW);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}



	// ENABLE BLACKLEVEL
	status = ST_SetEnumReg(hDevice, MCAM_BLACK_LEVEL_SELECTOR, BLACK_LEVEL_SELECTOR_ALL);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	status = ST_SetIntReg(hDevice, MCAM_BLACK_LEVEL_RAW, 0);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}



	status = ST_SetEnumReg(hDevice, MCAM_BALANCE_RATIO_SELECTOR, BALANCE_RATIO_SELECTOR_RED);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	//参数修改2
	status = ST_SetFloatReg(hDevice, MCAM_BALANCE_RATIO, BALANCE_RATIO);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}



	status = ST_SetEnumReg(hDevice, MCAM_BALANCE_WHITE_AUTO, BALANCE_WHITE_AUTO_OFF);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}


	// ANALOG CONTROL END


	// Set Trigger Mode : off
	status = ST_SetEnumReg(hDevice, MCAM_TRIGGER_MODE, TRIGGER_MODE_ON);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	//	status = ST_SetEnumReg(hDevice, MCAM_ACQUISITION_MODE, ACQUISITION_MODE_CONTINUOUS);
	//	if (status != MCAM_ERR_SUCCESS)
	//	{
	//		printf("Write Register failed : %d\n", status);
	//	}

	status = ST_SetEnumReg(hDevice, MCAM_ACQUISITION_FRAMERATE_ENABLE, ACQUISITION_FRAMERATE_ENABLE_ON);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}


	status = ST_SetFloatReg(hDevice, MCAM_ACQUISITIONF_RAMERATE, CAM_ACQUISITIONF_RAMERATE);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	status = ST_SetEnumReg(hDevice, MCAM_ACQUISITION_MODE, ACQUISITION_MODE_CONTINUOUS);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}



	//	//TriggerSource => soft
	//	status = ST_SetEnumReg(hDevice, MCAM_TRIGGER_SOURCE, TRIGGER_SOURCE_SOFTWARE);
	//	if (status != MCAM_ERR_SUCCESS)
	//	{
	//		printf("Write Register failed : %d\n", status);
	//	}

	printf("-----------------------------\n");
	printf("-----------------------------\n");
	printf("line trigger mode @ falling edge.\n");
	printf("-----------------------------\n");
	printf("-----------------------------\n\n");
	//TriggerSource => Line1

	status = ST_SetEnumReg(hDevice, MCAM_TRIGGER_SOURCE, TRIGGER_SOURCE_LINE1);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}



	//TriggerActivation => TRIGGER_ACTIVATION_RISING_EDGE
	status = ST_SetEnumReg(hDevice, MCAM_TRIGGER_ACTIVATION, TRIGGER_ACTIVATION_FALLING_EDGE);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}


	//TriggerDelay => 1us
	status = ST_SetFloatReg(hDevice, MCAM_TRIGGER_DELAY, 1.0f);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	// manual exposure

	status = ST_SetEnumReg(hDevice, MCAM_EXPOSURE_MODE, EXPOSURE_MODE_TIMED);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}


	//MCAM_EXPOSURE_AUTO => off
	status = ST_SetEnumReg(hDevice, MCAM_EXPOSURE_AUTO, EXPOSURE_AUTO_OFF);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}
	//参数修改3
	//MCAM_EXPOSURE_TIME =>  () us
	//status = ST_SetFloatReg(hDevice, MCAM_EXPOSURE_TIME, EXPOSURE_TIME*TIMED);
	status = ST_SetFloatReg(hDevice, MCAM_EXPOSURE_TIME, EXPOSURE);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}
	//edit end


	// Set PixelFormat : PIXEL_FORMAT_BAYERBG8
	status = ST_SetEnumReg(hDevice, MCAM_PIXEL_FORMAT, PIXEL_FORMAT_BAYERRG8);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	// Set GrabTimeout to 5 sec
	status = ST_SetGrabTimeout(hDevice, 5000);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	// FilterDriver : Must be "OFF" when debugging. Otherwise, the BSOD may occur.
	status = ST_SetEnumReg(hDevice, MCAM_DEVICE_FILTER_DRIVER_MODE, DEVICE_FILTER_DRIVER_MODE_OFF);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

	//Heartbeat Disable : Must be "true" when debugging. Otherwise, the device will be disconnected.
	status = ST_SetBoolReg(hDevice, GEV_GVCP_HEARTBEAT_DISABLE, true);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Write Register failed : %d\n", status);
	}

}

int DeleteYourFile(char* prefix)
{
	if (_access(prefix, 0) == 0)
	{
		//SetFileAttributes(prefix0, FILE_ATTRIBUTE_NORMAL);     //设定为一般 (取消前四种属性)
		// 0.using boost library
		if (boost::filesystem::remove_all(prefix) == 1)
		{
			printf("now delete exsiting data in %s \npress anykey to continue...\n ", prefix);
			getchar();
			printf("Removed %s.\n", prefix);
		}
		else
		{
			perror("remove_all");
		}
		printf("create a new one for you in %s...\n", prefix);
		mkdir(prefix);
	}
	else
	{
		printf("file in %s does not exsit...\ncreate a new one for you...\n", prefix);
		mkdir(prefix);
	}
	return 0;
}

int CameraNum1()
{
	printf("now 1 camera connected\n");
	__int32 status = MCAM_ERR_SUCCESS;
	__int32 hDevice0, ordernum0;
	__int32 width = 0, height = 0, buffferSize0 = 0;
	unsigned char* pImage0 = NULL;

	// system open
	ST_InitSystem();
	// Update Device List
	status = ST_UpdateDevice();
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Update Device list failed : %d\n", status);
		ST_FreeSystem();
		return 0;
	}

	// camera open
	status = ST_OpenDevice(0, &hDevice0);

	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Open device failed : %d\n", status);
		ST_FreeSystem();
		return 0;
	}
	ordernum0 = GetDeviceInformation(hDevice0);
	// create saving path after delete exsiting ones

	char prefix0[100], temp[100];
	strcpy(temp, "");
	sprintf(temp, "v%d\\", ordernum0);
	sprintf(prefix0, "%s%s", ROOTPATH, temp);



	// 怎么删除文件夹?
	DeleteYourFile(prefix0);
	SetFeature(hDevice0);
	//
	try
	{
		//Get Width
		status = ST_GetIntReg(hDevice0, MCAM_WIDTH, &width);
		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Read Register failed : ");
			throw status;
		}

		//Get Height
		status = ST_GetIntReg(hDevice0, MCAM_HEIGHT, &height);
		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Read Register failed : ");
			throw status;
		}

		Mat rgbFrame0(height, width, CV_8UC3);
		Mat bayerRG8Frame0(height, width, CV_8UC1);

		// Image Buffer Allocation
		buffferSize0 = width * height;
		pImage0 = (unsigned char*)malloc(buffferSize0);
		memset((void*)pImage0, 0, buffferSize0);

		printf("Press <Enter> to start.\n");
		_getch();

		// Acquisition Start
		status = ST_AcqStart(hDevice0);

		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Acquisition start failed : ");
			throw status;
		}

		ST_GrabStartAsync(hDevice0, -1);

		//	CvSize size = cvSize(WIDTH, HEIGHT);

		while (1)
		{


			//Grab Image
			status = ST_GrabImageAsync(hDevice0, pImage0, buffferSize0, -1);

			if (status == MCAM_ERR_TIMEOUT)
			{
				printf("GrabImage timeout : %d\n", status);
				if (_kbhit())
				{
					break;
				}

				continue;
			}
			else if (status != MCAM_ERR_SUCCESS)
			{
				printf("GrabImage failed : ");
				throw status;
			}
			//printf("Image(0,0) = %d\n", pImage[0]);
			//////////////////////////////////////////////////////////////////////////
			rgbFrame0 = CreateAndConvertMat(bayerRG8Frame0, pImage0, width, height);
			printf("now %d frame EXPOSURE_TIME %d\n", endFrame, EXPOSURE);
			
			imshow(prefix0, rgbFrame0);
			if (waitKey(30))
				//if (waitKey(300) && endFrame<50)
			{
				strcpy(fileName, "");
				sprintf_s(fileName, "%s%03d%s", prefix0, endFrame, ext);
				imwrite(fileName, rgbFrame0);
                printf("@%s\n", fileName);
				++endFrame;
			}


			if (_kbhit())
			{
				break;
			}

		}

		// Acquisition Stop
		status = ST_AcqStop(hDevice0);
		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Acquisition stop failed : ");
			throw status;
		}
	}
	catch (__int32 err)
	{
		printf("%d\n", err);
		ST_CloseDevice(hDevice0);


		ST_FreeSystem();
	}

	// Close Device
	ST_CloseDevice(hDevice0);


	// Free system
	ST_FreeSystem();

	printf("now video is processing\n");
	printf(".......................\n");
	Frames2Video(endFrame, prefix0, flieName0);
	printf("1 video saved\n");

	return 0;
}

int CameraNum2()
{
	printf("now 2 cameras connected\n");
	__int32 status = MCAM_ERR_SUCCESS;
	__int32 hDevice0, hDevice1, ordernum0, ordernum1;
	__int32 width = 0, height = 0,
		buffferSize0 = 0, buffferSize1 = 0;
	unsigned char* pImage0 = NULL;
	unsigned char* pImage1 = NULL;
	// system open
	ST_InitSystem();

	// Update Device List
	status = ST_UpdateDevice();
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Update Device list failed : %d\n", status);
		ST_FreeSystem();
		return 0;
	}


	// camera open
	status = ST_OpenDevice(0, &hDevice0);
	status = ST_OpenDevice(1, &hDevice1);
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Open device failed : %d\n", status);
		ST_FreeSystem();
		return 0;
	}

	ordernum0 = GetDeviceInformation(hDevice0);
	ordernum1 = GetDeviceInformation(hDevice1);
	// create saving path after delete exsiting ones

	char prefix0[100], prefix1[100], temp[100];
	//1
	strcpy(temp, "");
	sprintf(temp, "v%d\\", ordernum0);
	sprintf(prefix0, "%s%s", ROOTPATH, temp);
	DeleteYourFile(prefix0);
	//2
	strcpy(temp, "");
	sprintf(temp, "v%d\\", ordernum1);
	sprintf(prefix1, "%s%s", ROOTPATH, temp);
	// 删除文件夹
	DeleteYourFile(prefix1);
	SetFeature(hDevice0);
	SetFeature(hDevice1);
	try
	{
		//Get Width
		status = ST_GetIntReg(hDevice0, MCAM_WIDTH, &width);
		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Read Register failed : ");
			throw status;
		}

		//Get Height
		status = ST_GetIntReg(hDevice0, MCAM_HEIGHT, &height);
		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Read Register failed : ");
			throw status;
		}

		Mat rgbFrame0(height, width, CV_8UC3);
		Mat rgbFrame1(height, width, CV_8UC3);

		Mat bayerRG8Frame0(height, width, CV_8UC1);
		Mat bayerRG8Frame1(height, width, CV_8UC1);


		// Image Buffer Allocation
		buffferSize0 = width * height;
		pImage0 = (unsigned char*)malloc(buffferSize0);
		memset((void*)pImage0, 0, buffferSize0);

		buffferSize1 = width * height;
		pImage1 = (unsigned char*)malloc(buffferSize1);
		memset((void*)pImage1, 0, buffferSize1);


		printf("line trigger mode\n");
		printf("-----------------------------\n\n");
		printf("Press <Enter> to start.\n");
		_getch();

		// Acquisition Start
		status = ST_AcqStart(hDevice0);
		status = ST_AcqStart(hDevice1);

		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Acquisition start failed : ");
			throw status;
		}

		ST_GrabStartAsync(hDevice0, -1);
		ST_GrabStartAsync(hDevice1, -1);


		//	CvSize size = cvSize(WIDTH, HEIGHT);

		//	VideoWriter output_src0(flieName0, -1, fps, size, 1);
		//	VideoWriter output_src1(flieName1, -1, fps, size, 1);

		while (1)
		{
			//Grab Image
			status = ST_GrabImageAsync(hDevice0, pImage0, buffferSize0, -1);
			status = ST_GrabImageAsync(hDevice1, pImage1, buffferSize1, -1);

			if (status == MCAM_ERR_TIMEOUT)
			{
				printf("GrabImage timeout : %d\n", status);
				if (_kbhit())
				{
					break;
				}

				continue;
			}
			else if (status != MCAM_ERR_SUCCESS)
			{
				printf("GrabImage failed : ");
				throw status;
			}
			//printf("Image(0,0) = %d\n", pImage[0]);
			//////////////////////////////////////////////////////////////////////////
			rgbFrame0 = CreateAndConvertMat(bayerRG8Frame0, pImage0, width, height);
			rgbFrame1 = CreateAndConvertMat(bayerRG8Frame1, pImage1, width, height);

			printf("now %d frame EXPOSURE_TIME %d\n", endFrame, EXPOSURE);

			imshow(prefix0, rgbFrame0);
			imshow(prefix1, rgbFrame1);

			if (waitKey(30))
				//if (waitKey(300) && endFrame<50)
			{
				strcpy(fileName, "");
				sprintf_s(fileName, "%s%03d%s", prefix0, endFrame, ext);
				imwrite(fileName, rgbFrame0);
				printf("@%s\n", fileName);
				sprintf_s(fileName, "%s%03d%s", prefix1, endFrame, ext);
				imwrite(fileName, rgbFrame1);
				printf("@%s\n", fileName);
				++endFrame;
			}

			if (_kbhit())
			{
				break;
			}

		}

		// Acquisition Stop
		status = ST_AcqStop(hDevice0);
		status = ST_AcqStop(hDevice1);

		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Acquisition stop failed : ");
			throw status;
		}
	}
	catch (__int32 err)
	{
		printf("%d\n", err);
		ST_CloseDevice(hDevice0);
		ST_CloseDevice(hDevice1);

		ST_FreeSystem();
	}

	// Close Device
	ST_CloseDevice(hDevice0);
	ST_CloseDevice(hDevice1);

	// Free system
	ST_FreeSystem();

	printf("now video is processing\n");
	printf(".......................\n");
	Frames2Video(endFrame, prefix0, flieName0);
	Frames2Video(endFrame, prefix1, flieName1);
	printf("2 videos saved\n");

	return 0;

}

int CameraNum3()
{
	printf("now 3 cameras connected\n");
	__int32 status = MCAM_ERR_SUCCESS;
	__int32 hDevice0, hDevice1, hDevice2, ordernum0, ordernum1, ordernum2;
	__int32 width = 0, height = 0,
		buffferSize0 = 0, buffferSize1 = 0, buffferSize2 = 0;
	unsigned char* pImage0 = NULL;
	unsigned char* pImage1 = NULL;
	unsigned char* pImage2 = NULL;

	// system open
	ST_InitSystem();

	// Update Device List
	status = ST_UpdateDevice();
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Update Device list failed : %d\n", status);
		ST_FreeSystem();
		return 0;
	}


	// camera open
	status = ST_OpenDevice(0, &hDevice0);
	status = ST_OpenDevice(1, &hDevice1);
	status = ST_OpenDevice(2, &hDevice2);

	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Open device failed : %d\n", status);
		ST_FreeSystem();
		return 0;
	}

	ordernum0 = GetDeviceInformation(hDevice0);
	ordernum1 = GetDeviceInformation(hDevice1);
	ordernum2 = GetDeviceInformation(hDevice2);
	// create saving path after delete exsiting ones

	char prefix0[100], prefix1[100], prefix2[100], temp[100];
	//1
	strcpy(temp, "");
	sprintf(temp, "v%d\\", ordernum0);
	sprintf(prefix0, "%s%s", ROOTPATH, temp);
	DeleteYourFile(prefix0);
	//2
	strcpy(temp, "");
	sprintf(temp, "v%d\\", ordernum1);
	sprintf(prefix1, "%s%s", ROOTPATH, temp);
	DeleteYourFile(prefix1);
	//3
	strcpy(temp, "");
	sprintf(temp, "v%d\\", ordernum2);
	sprintf(prefix2, "%s%s", ROOTPATH, temp);
	DeleteYourFile(prefix2);

	SetFeature(hDevice0);
	SetFeature(hDevice1);
	SetFeature(hDevice2);

	try
	{
		//Get Width
		status = ST_GetIntReg(hDevice0, MCAM_WIDTH, &width);
		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Read Register failed : ");
			throw status;
		}

		//Get Height
		status = ST_GetIntReg(hDevice0, MCAM_HEIGHT, &height);
		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Read Register failed : ");
			throw status;
		}

		Mat rgbFrame0(height, width, CV_8UC3);
		Mat rgbFrame1(height, width, CV_8UC3);
		Mat rgbFrame2(height, width, CV_8UC3);

		Mat bayerRG8Frame0(height, width, CV_8UC1);
		Mat bayerRG8Frame1(height, width, CV_8UC1);
		Mat bayerRG8Frame2(height, width, CV_8UC1);



		// Image Buffer Allocation
		buffferSize0 = width * height;
		pImage0 = (unsigned char*)malloc(buffferSize0);
		memset((void*)pImage0, 0, buffferSize0);

		buffferSize1 = width * height;
		pImage1 = (unsigned char*)malloc(buffferSize1);
		memset((void*)pImage1, 0, buffferSize1);

		buffferSize2 = width * height;
		pImage2 = (unsigned char*)malloc(buffferSize2);
		memset((void*)pImage2, 0, buffferSize2);


		printf("line trigger mode\n");
		printf("-----------------------------\n\n");
		printf("Press <Enter> to start.\n");
		_getch();

		// Acquisition Start
		status = ST_AcqStart(hDevice0);
		status = ST_AcqStart(hDevice1);
		status = ST_AcqStart(hDevice2);


		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Acquisition start failed : ");
			throw status;
		}

		ST_GrabStartAsync(hDevice0, -1);
		ST_GrabStartAsync(hDevice1, -1);
		ST_GrabStartAsync(hDevice2, -1);


		//	CvSize size = cvSize(WIDTH, HEIGHT);

		//	VideoWriter output_src0(flieName0, -1, fps, size, 1);
		//	VideoWriter output_src1(flieName1, -1, fps, size, 1);
		//	VideoWriter output_src2(flieName2, -1, fps, size, 1);


		while (1)
		{
			//Software trigger command
			status = ST_SetCmdReg(hDevice0, MCAM_TRIGGER_SOFTWARE);
			status = ST_SetCmdReg(hDevice1, MCAM_TRIGGER_SOFTWARE);
			status = ST_SetCmdReg(hDevice2, MCAM_TRIGGER_SOFTWARE);


			if (status != MCAM_ERR_SUCCESS)
			{
				printf("Software trigger command failed : ");
				throw status;
			}

			//Grab Image
			status = ST_GrabImageAsync(hDevice0, pImage0, buffferSize0, -1);
			status = ST_GrabImageAsync(hDevice1, pImage1, buffferSize1, -1);
			status = ST_GrabImageAsync(hDevice2, pImage2, buffferSize2, -1);


			if (status == MCAM_ERR_TIMEOUT)
			{
				printf("GrabImage timeout : %d\n", status);
				if (_kbhit())
				{
					break;
				}

				continue;
			}
			else if (status != MCAM_ERR_SUCCESS)
			{
				printf("GrabImage failed : ");
				throw status;
			}
			//printf("Image(0,0) = %d\n", pImage[0]);
			//////////////////////////////////////////////////////////////////////////
			rgbFrame0 = CreateAndConvertMat(bayerRG8Frame0, pImage0, width, height);
			rgbFrame1 = CreateAndConvertMat(bayerRG8Frame1, pImage1, width, height);
			rgbFrame2 = CreateAndConvertMat(bayerRG8Frame2, pImage2, width, height);
			printf("now %d frame EXPOSURE_TIME %f\n", endFrame, EXPOSURE);

			imshow(prefix0, rgbFrame0);
			imshow(prefix1, rgbFrame1);
			imshow(prefix2, rgbFrame2);

			if (waitKey(30))
				//if (waitKey(300) && endFrame<50)
			{
				strcpy(fileName, "");
				sprintf_s(fileName, "%s%03d%s", prefix0, endFrame, ext);
				imwrite(fileName, rgbFrame0);
				printf("@%s\n", fileName);
				sprintf_s(fileName, "%s%03d%s", prefix1, endFrame, ext);
				imwrite(fileName, rgbFrame1);
				printf("@%s\n", fileName);
				sprintf_s(fileName, "%s%03d%s", prefix2, endFrame, ext);
				imwrite(fileName, rgbFrame2);
				printf("@%s\n", fileName);
				++endFrame;
			}


			if (_kbhit())
			{
				break;
			}

		}

		// Acquisition Stop
		status = ST_AcqStop(hDevice0);
		status = ST_AcqStop(hDevice1);
		status = ST_AcqStop(hDevice2);


		if (status != MCAM_ERR_SUCCESS)
		{
			printf("Acquisition stop failed : ");
			throw status;
		}
	}
	catch (__int32 err)
	{
		printf("%d\n", err);
		ST_CloseDevice(hDevice0);
		ST_CloseDevice(hDevice1);
		ST_CloseDevice(hDevice2);

		ST_FreeSystem();
	}

	// Close Device
	ST_CloseDevice(hDevice0);
	ST_CloseDevice(hDevice1);
	ST_CloseDevice(hDevice2);

	// Free system
	ST_FreeSystem();

	printf("now video is processing\n");
	printf(".......................\n");
	Frames2Video(endFrame, prefix0, flieName0);
	Frames2Video(endFrame, prefix1, flieName1);
	Frames2Video(endFrame, prefix2, flieName2);
	printf("3 videos saved\n");

	return 0;
}

int  _tmain(int argc, _TCHAR* argv[])
{
	__int32 status = MCAM_ERR_SUCCESS;
	// system open
	ST_InitSystem();

	// Update Device List
	status = ST_UpdateDevice();
	if (status != MCAM_ERR_SUCCESS)
	{
		printf("Update Device list failed : %d\n", status);
		ST_FreeSystem();
		return 0;
	}

	unsigned __int32 CamNum;
	ST_GetAvailableCameraNum(&CamNum);
	if (CamNum <= 0)
	{
		printf("No device!\n press Esc to exit\n");
		_getch();
		ST_FreeSystem();
		return 0;
	}
	if (1 == CamNum)
	{
		CameraNum1();
	}
	if (2 == CamNum)
	{
		CameraNum2();
	}
	if (3 == CamNum)
	{
		CameraNum3();
	}
	if (3 < CamNum)
	{
		printf("more than 3 devices!\n press Esc to exit\n");
		_getch();
		ST_FreeSystem();
		return 0;
	}


	return 0;

}