// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-ORB Matching
******************************************************************************/

// main.cpp: This file contains the "main" function.
// This is where program execution begins and ends

#include "init.h"

#include "Header\calculation_feature_matching.h"

int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "D:\\GitHub\\KAMERAWERK\\Binocular-Stereo-Matching\\matlab\\Material";

	////left and right image name
	string name_image_a = "L13.jpg";
	string name_image_b = "R13.jpg";

	//string name_image_a = "L3.bmp";
	//string name_image_b = "R3.bmp";

	//Mat image_a = imread(folder_path + "\\" + name_image_a);
	//Mat image_b = imread(folder_path + "\\" + name_image_b);

	//if (!image_a.data || !image_b.data){

	//	cout << "==> ERROR: Images not found" << endl;
	//	return -1;
	//}
	////determine the order of duals
	//vector<Mat> vector_image = DualOrder(image_a, image_b);

	//Mat image_left = vector_image[0];
	//Mat image_right = vector_image[1];

	////calculate vertical difference between left and right images
	//int y_shift = CalculateVerticalDifference(image_left, image_right);

	//string fileName = "G:\\跑马山隧道3D视频素材\\网络相机3D视频素材\\Stiching20201114_134604.avi";
	string fileName = "D:\\工作\\工业相机3D视频素材\\20201113_143105.avi";
	VideoCapture capture(fileName);

	if (!capture.isOpened())

		cout << "fail to open!" << endl;

	int width = capture.get(CAP_PROP_FRAME_WIDTH);  //帧宽度
	int height = capture.get(CAP_PROP_FRAME_HEIGHT); //帧高度
	int frameRate = capture.get(CAP_PROP_FPS);  //帧率 x frames/s
	int totalFrames = capture.get(CAP_PROP_FRAME_COUNT); //总帧数

	cout << "==> 视频宽度:" << width << endl;
	cout << "==> 视频高度:" << height << endl;
	cout << "==> 视频总帧数:" << totalFrames << endl;
	cout << "==> 帧率:" << frameRate << endl;

	/*string leftFileName = "left.avi";
	string rightFileName = "right.avi";
	VideoWriter m_video1, m_video2;
	m_video1 = VideoWriter(leftFileName, VideoWriter::fourcc('X', 'V', 'I', 'D'), 20, Size(2048, 1088), true);
	m_video2 = VideoWriter(rightFileName, VideoWriter::fourcc('X', 'V', 'I', 'D'), 20, Size(2048, 1088), true);*/

	Mat merged_image, left_image, right_image;
	Mat newLeft, newRight;

	int cnt = totalFrames;

	//write y-shift into txt file
	ofstream out_file;
	out_file.open("y-shift.txt");

	int interval = 100;

	while (cnt--){

		if (!((totalFrames - cnt) % interval == 0)) {

			continue;
		}
		// 从视频中读取一帧
		capture >> merged_image;

		//         2048            1088
		cout << "==> frames remained: " << cnt << endl;

		// 获取左边图像 获取右边图像
		//工业相机
		left_image = merged_image(Rect(0, 0, 1024, 1088));
		right_image = merged_image(Rect(1024, 0, 1024, 1088));

		//网络相机
		//left_image= res(Rect(0, 0, 960, 1080));
		//right_image = res(Rect(960, 0, 960, 1080));

		resize(left_image, newLeft, Size(2048, 1088));
		resize(right_image, newRight, Size(2048, 1088));

		//imshow("left", left);
		//imshow("right", right);

		out_file << CalculateVerticalDifference(left_image, right_image) << endl;

		//  录像
		//m_video1 << newLeft;
		//m_video2 << newRight;

		//waitKey(30);
	}

	//m_video1.release();
	//m_video2.release();
	capture.release();
	return 0;
}