// -*- coding: utf-8 -*-
/******************************************************************************
Created on Mon Nov 11 14:51:37 2020

@author: Wei Huajing
@company: KAMERAWERK
@e-mail: wei@kamerawerk.cn

@title：script-Veritical Difference Calculation

******************************************************************************/

// main.cpp: This file contains the "main" function.
// This is where program execution begins and ends

#include "init.h"

#include "Header\operation_video.h"

#include "Header\calculation_feature_matching.h"
#include "Header\configuration_global_variable.h"

Mat CenterROI(Mat& image_gray) {

	Point_<int> center_ROI;
	center_ROI.x = int(image_gray.cols / 2);
	center_ROI.y = int(image_gray.rows / 2);

	//ROI factor
	double zoom_factor = 0.9;

	//define ROI size
	int width_ROI = int(image_gray.cols * zoom_factor);
	int height_ROI = int(image_gray.rows * zoom_factor);

	//define ROI parameter
	Rect rect(center_ROI.x - int(width_ROI / 2),
		center_ROI.y - int(height_ROI / 2),
		width_ROI,
		height_ROI);

	return image_gray(rect);
}
Mat DisparityMapBM(Mat& image_left, Mat& image_right) {

	//gray level to calculate disparity
	Mat image_gray_left;
	Mat image_gray_right;

	cvtColor(image_left, image_gray_left, COLOR_BGR2GRAY);
	cvtColor(image_right, image_gray_right, COLOR_BGR2GRAY);
	
	image_gray_left = image_gray_left;
	image_gray_right = image_gray_right;

	//Mat of disparity
	Mat disparity, disparity_8U;

	int block_size = 9;
	int num_disparities = 16 * 8;
	int uniqueness_ratio = 15;

	Ptr<StereoBM> BM = StereoBM::create(16, 9);

	//BM->setROI1(ROI);
	//BM->setROI2(ROI);
	BM->setPreFilterCap(31);
	BM->setBlockSize(block_size);
	BM->setMinDisparity(-16);
	BM->setNumDisparities(num_disparities);
	BM->setTextureThreshold(10);
	BM->setUniquenessRatio(uniqueness_ratio);
	BM->setSpeckleWindowSize(100);
	BM->setSpeckleRange(32);
	BM->setDisp12MaxDiff(1);
	BM->compute(CenterROI(image_gray_left), CenterROI(image_gray_right), disparity);

	disparity.convertTo(disparity_8U, CV_8UC1, 255 / (num_disparities));
    
	/*namedWindow("Disparity (BM)", 1);
	imshow("Disparity (BM)", disparity_8U);
	waitKey(0);*/

	return disparity;
}
Mat DisparityMapSGBM(Mat& image_left, Mat& image_right) {

	//gray level to calculate disparity
	Mat image_gray_left;
	Mat image_gray_right;

	cvtColor(image_left, image_gray_left, COLOR_BGR2GRAY);
	cvtColor(image_right, image_gray_right, COLOR_BGR2GRAY);

	image_gray_left = image_gray_left;
	image_gray_right = image_gray_right;

	//Mat of disparity
	Mat disparity, disparity_8U;
	int block_size = 9;
	int num_disparities = 16 * 8;
	int uniqueness_ratio = 15;
	int P1 = image_left.channels() * block_size * block_size * 8;
	int P2 = image_left.channels() * block_size * block_size * 32;

	Ptr<StereoSGBM> SGBM = StereoSGBM::create(16, 9);

	//BM->setROI1(ROI);
	//BM->setROI2(ROI);
	SGBM->setPreFilterCap(31);
	SGBM->setBlockSize(block_size);
	SGBM->setMinDisparity(-16);
	SGBM->setNumDisparities(num_disparities);
	SGBM->setUniquenessRatio(uniqueness_ratio);
	SGBM->setSpeckleWindowSize(100);
	SGBM->setSpeckleRange(32);
	SGBM->setDisp12MaxDiff(1);
	SGBM->setP1(P1);
	SGBM->setP2(P2); 
	SGBM->setMode(StereoSGBM::MODE_SGBM_3WAY); //MODE_HH MODE_SGBM MODE_SGBM_3WAY
	SGBM->compute(CenterROI(image_gray_left), CenterROI(image_gray_right), disparity);

	disparity.convertTo(disparity_8U, CV_8UC1, 255 / (num_disparities));

	namedWindow("Disparity (SGBM)", 1);
	imshow("Disparity (SGBM)", disparity_8U);
	waitKey(0);

	return disparity;
}

void StereoMapInit() {

	cout << endl << "-- Stereo Map Init" << endl;

	//所有矩阵都需要转置
	//左相机内参矩阵
	Mat intrinsic_matrix_a = Mat(3, 3, CV_64F);
	intrinsic_matrix_a.at<double>(0, 0) = 2.025302732582580e+03;
	intrinsic_matrix_a.at<double>(0, 1) = 0;
	intrinsic_matrix_a.at<double>(0, 2) = 0;
	intrinsic_matrix_a.at<double>(1, 0) = 0;
	intrinsic_matrix_a.at<double>(1, 1) = 2.017205378516199e+03;
	intrinsic_matrix_a.at<double>(1, 2) = 0;
	intrinsic_matrix_a.at<double>(2, 0) = 9.041139186082133e+02;
	intrinsic_matrix_a.at<double>(2, 1) = 4.923717435392825e+02;
	intrinsic_matrix_a.at<double>(2, 2) = 1;

	//左相机的畸变参数(k1,k2,p1,p2,k3)
	Mat distortion_coefficient_a(1, 5, CV_64F);
	distortion_coefficient_a.at<double>(0, 0) = -0.168900512358313;
	distortion_coefficient_a.at<double>(0, 1) = -0.225738897303396;
	distortion_coefficient_a.at<double>(0, 2) = -0.004945481275994;
	distortion_coefficient_a.at<double>(0, 3) = -0.003308759407141;
	distortion_coefficient_a.at<double>(0, 4) = 0;

	//右相机内参矩阵
	Mat intrinsic_matrix_b(3, 3, CV_64F);
	intrinsic_matrix_b.at<double>(0, 0) = 2.028101198985155e+03;
	intrinsic_matrix_b.at<double>(0, 1) = 0;
	intrinsic_matrix_b.at<double>(0, 2) = 0;
	intrinsic_matrix_b.at<double>(1, 0) = 0;
	intrinsic_matrix_b.at<double>(1, 1) = 2.019747675060556e+03;
	intrinsic_matrix_b.at<double>(1, 2) = 0;
	intrinsic_matrix_b.at<double>(2, 0) = 9.170004607229216e+02;
	intrinsic_matrix_b.at<double>(2, 1) = 5.082350278821574e+02;
	intrinsic_matrix_b.at<double>(2, 2) = 1;

	//右相机的畸变参数(k1,k2,p1,p2,k3)
	Mat distortion_coefficient_b(1, 5, CV_64F);
	distortion_coefficient_b.at<double>(0, 0) = -0.171025662304250;
	distortion_coefficient_b.at<double>(0, 1) = -0.231978044444149;
	distortion_coefficient_b.at<double>(0, 2) = -0.005470034522850;
	distortion_coefficient_b.at<double>(0, 3) = -0.004569382288119;
	distortion_coefficient_b.at<double>(0, 4) = 0;

	//旋转矩阵
	Mat rotation_matrix(3, 3, CV_64F);
	rotation_matrix.at<double>(0, 0) = 0.999896935428937;
	rotation_matrix.at<double>(0, 1) = -0.002671235636848;
	rotation_matrix.at<double>(0, 2) = -0.014106134126420;
	rotation_matrix.at<double>(1, 0) = 0.002664753042799;
	rotation_matrix.at<double>(1, 1) = 0.999996335137106;
	rotation_matrix.at<double>(1, 2) = -4.783341704535389e-04;
	rotation_matrix.at<double>(2, 0) = 0.014107360172655;
	rotation_matrix.at<double>(2, 1) = 4.406955073119460e-04;
	rotation_matrix.at<double>(2, 2) = 0.999900389127052;

	//转换成角度
	//平移向量												  
	Mat translation_vector(1, 3, CV_64F);
	translation_vector.at<double>(0, 0) = -88.310241092249840;
	translation_vector.at<double>(0, 1) = -0.128020177295975;
	translation_vector.at<double>(0, 2) = 3.751702383354567;

	//transpose
	intrinsic_matrix_a = intrinsic_matrix_a.t();
	intrinsic_matrix_b = intrinsic_matrix_b.t();
	distortion_coefficient_a = distortion_coefficient_a.t();
	distortion_coefficient_b = distortion_coefficient_b.t();
	rotation_matrix = rotation_matrix.t();
	translation_vector = translation_vector.t();

	//旋转矩阵
	Mat rotation_vector;

	//旋转矩阵转化为旋转向量，罗德里格斯变换
	Rodrigues(rotation_matrix, rotation_vector);

	//图像尺寸
	Size image_size = Size(WIDTH, HEIGHT);

	//左右相机的3x3矫正变换(旋转矩阵)
	Mat rotation_matrix_a;
	Mat rotation_matrix_b;

	//左右相机新的坐标系统(矫正过的)输出 3x4 的投影矩阵
	Mat projection_matrix_a;
	Mat projection_matrix_b;

	//深度视差映射矩阵4*4
	Mat disparity_mapping_matrix;

	//图像校正之后，会对图像进行裁剪，这里的validRoi就是指裁剪之后的区域
	Rect valid_ROI_a;
	Rect valid_ROI_b;

	//alpha-拉伸参数。如果设置为负或忽略，将不进行拉伸。
	//如果设置为0，那么校正后图像只有有效的部分会被显示（没有黑色的部分）,
	//如果设置为1，那么就会显示整个图像。设置为0~1之间的某个值，其效果也居于两者之间。

	//立体校正
	stereoRectify(intrinsic_matrix_a,
		distortion_coefficient_a,
		intrinsic_matrix_b,
		distortion_coefficient_b,
		image_size,
		rotation_vector,
		translation_vector,
		rotation_matrix_a,
		rotation_matrix_b,
		projection_matrix_a,
		projection_matrix_b,
		disparity_mapping_matrix,
		CALIB_ZERO_DISPARITY,
		0,
		image_size,
		&valid_ROI_a,
		&valid_ROI_b);

	//rectification mapping matrix
	Mat mapping_x_a;
	Mat mapping_y_a;
	Mat mapping_x_b;
	Mat mapping_y_b;

	//用来计算畸变映射，输出的X / Y坐标重映射参数, remap把求得的映射应用到图像上。
	initUndistortRectifyMap(intrinsic_matrix_a,
		distortion_coefficient_a,
		rotation_matrix_a,
		projection_matrix_a,
		image_size,
		CV_32FC1,
		mapping_x_a,
		mapping_y_a);

	initUndistortRectifyMap(intrinsic_matrix_b,
		distortion_coefficient_b,
		rotation_matrix_b,
		projection_matrix_b,
		image_size,
		CV_32FC1,
		mapping_x_b,
		mapping_y_b);

	//determine order based on x-shift
	if (translation_vector.at<double>(0, 0) > 0) {

		mapping_x_left = mapping_x_a;
		mapping_y_left = mapping_y_a;
		mapping_x_right = mapping_x_b;
		mapping_y_right = mapping_y_b;
	}
	else {

		mapping_x_left = mapping_x_b;
		mapping_y_left = mapping_y_b;
		mapping_x_right = mapping_x_a;
		mapping_y_right = mapping_y_a;
	}
}
int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "../Material/20210309";

	//left and right image name
	string name_image_a = "L13.jpg";
	string name_image_b = "R13.jpg";
	
	StereoMapInit();

	for (int k = 1; k <= 20; ++k) {

		name_image_a = "Left/" + to_string(k) + ".jpg";
		name_image_b = "Right/" + to_string(k) + ".jpg";

		cout << name_image_a << endl;

		//string name_image_a = "L3.bmp";
		//string name_image_b = "R3.bmp";

		//string name_image_a = "L14.png";
		//string name_image_b = "R14.png";

		Mat image_a = imread(folder_path + "/" + name_image_a);
		Mat image_b = imread(folder_path + "/" + name_image_b);

		cout << folder_path + "/" + name_image_a << endl;


		if (!image_a.data || !image_b.data) {

			cout << "==> ERROR: Images not found" << endl;
			return -1;
		}
		//determine the order of duals
		vector<Mat> vector_image = DualCamerasOrder(image_a, image_b, MATCH_OPERATOR);

		Mat image_left = vector_image[0];
		Mat image_right = vector_image[1];

		Mat image_rectified_left;
		Mat image_rectified_right;

		remap(image_left, image_rectified_left, mapping_x_left, mapping_y_left, INTER_LINEAR);
		remap(image_right, image_rectified_right, mapping_x_right, mapping_y_right, INTER_LINEAR);

		imshow("original left", image_left);
		imshow("original right", image_right);
		imshow("rectified left", image_rectified_left);
		imshow("rectified right", image_rectified_right);

		string folder_name_left = "/Rectified ";
		string folder_name_right = "/Rectified ";

		waitKey(0);

		//save left and right image
		imwrite(folder_path + folder_name_left + name_image_a, image_rectified_left);
		imwrite(folder_path + folder_name_right + name_image_b, image_rectified_left);
	}

	//putText(image_right, "PutText", Point(500, 500), FONT_HERSHEY_PLAIN, 6.6, Scalar(0, 255, 0));
	//
	//namedWindow("PutText", WINDOW_NORMAL);
	//resizeWindow("PutText", WIDTH, HEIGHT);
	//imshow("PutText", image_right);
	//waitKey(0);

	//for (int i = 0; i < 10; i++) {

	//	double t1 = clock();

	//	//DisparityMapSGBM(image_a, image_b);
	//	//calculate vertical difference between left and right images
	//	double y_shift = CalculateVerticalDifference(image_left, image_right, MATCH_OPERATOR).second;
	//	
	//	double t2 = clock();

	//	cout << "==> time consumed: " << (t2 - t1) / CLOCKS_PER_SEC * 1000 << endl;
	//}
	
	//calculate vertical difference between left and right images
	//double y_shift = CalculateVerticalDifference(image_left, image_right, "SURF");

	//string fileName = "D:/工作/工业相机3D视频素材/20201113_143105.avi";

	//VideoStreaming(fileName);
}
