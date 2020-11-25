// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title: Source-Operation on video
******************************************************************************/

#include "..\Header\operation_video.h"

#include "..\Header\calculation_feature_matching.h"

int VideoStreaming(string fileName) {

	VideoCapture capture(fileName);

	if (!capture.isOpened())

		cout << "fail to open!" << endl;

	double width = capture.get(CAP_PROP_FRAME_WIDTH); 
	double height = capture.get(CAP_PROP_FRAME_HEIGHT); 
	double frameRate = capture.get(CAP_PROP_FPS);
	double totalFrames = capture.get(CAP_PROP_FRAME_COUNT); 

	cout << "==> width: " << int(width) << endl;
	cout << "==> height: " << int(height) << endl;
	cout << "==> amount of frames: " << int(totalFrames) << endl;
	cout << "==> frame rate: " << int(frameRate) << endl;

	/*string leftFileName = "left.avi";
	string rightFileName = "right.avi";
	VideoWriter m_video1, m_video2;
	m_video1 = VideoWriter(leftFileName, VideoWriter::fourcc('X', 'V', 'I', 'D'), 20, Size(2048, 1088), true);
	m_video2 = VideoWriter(rightFileName, VideoWriter::fourcc('X', 'V', 'I', 'D'), 20, Size(2048, 1088), true);*/

	Mat merged_image, left_image, right_image;
	Mat new_left_image, new_right_image;

	int cnt = int(totalFrames);

	//write y-shift into txt file
	ofstream out_file;
	out_file.open("y-shift.txt");

	int interval = 1;

	while (cnt--) {

		if (!((int(totalFrames) - cnt) % interval == 0)) {

			continue;
		}
		//read frames from video one by one
		capture >> merged_image;

		cout << "==> frames remained: " << cnt << endl;

		//obtain left and right images
		//industurial camera 2048*1088
		left_image = merged_image(Rect(0, 0, 1024, 1088));
		right_image = merged_image(Rect(1024, 0, 1024, 1088));

		//network camera 1920*1080
		//left_image= res(Rect(0, 0, 960, 1080));
		//right_image = res(Rect(960, 0, 960, 1080));

		resize(left_image, new_left_image, Size(2048, 1088));
		resize(right_image, new_right_image, Size(2048, 1088));

		//imshow("left", left);
		//imshow("right", right);

		out_file << CalculateVerticalDifference(left_image, right_image) << endl;

		//recording video
		//m_video1 << newLeft;
		//m_video2 << newRight;

		//waitKey(30);
	}
	//m_video1.release();
	//m_video2.release();
	capture.release();
	return 0;
}