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

int main(){
	
	cout << "Built with OpenCV " << CV_VERSION << endl;

	string folder_path = "../Material";

	////left and right image name
	//string name_image_a = "L13.jpg";
	//string name_image_b = "R13.jpg";

	//string name_image_a = "L3.bmp";
	//string name_image_b = "R3.bmp";

    string name_image_a = "L14.png";
    string name_image_b = "R14.png";

	Mat image_a = imread(folder_path + "/" + name_image_a);
	Mat image_b = imread(folder_path + "/" + name_image_b);

	if (!image_a.data || !image_b.data){

		cout << "==> ERROR: Images not found" << endl;
		return -1;
	}
	//determine the order of duals
	vector<Mat> vector_image = DualCamerasOrder(image_a, image_b);

	Mat image_left = vector_image[0];
	Mat image_right = vector_image[1];

	//calculate vertical difference between left and right images
	double y_shift = CalculateVerticalDifference(image_left, image_right);

	//string fileName = "D:/工作/工业相机3D视频素材/20201113_143105.avi";

	//VideoStreaming(fileName);
}



int blockSize = 0, uniquenessRatio = 0, numDisparities = 0;
Ptr<StereoBM> bm = StereoBM::create(16, 9);
Mat xyz;              //三维坐标

Mat Q;//深度视差映射矩阵
Rect left_valid_roi, right_valid_roi;//图像校正之后，会对图像进行裁剪，这里的validRoi就是指裁剪之后的区域
Mat left_rectified_gray, right_rectified_gray;//左右摄像头校准后图像
Mat img;
Mat left_img, right_img;//左右摄像头原始图像
Mat left_rectified_img, right_rectified_img;//左右摄像头校准后图像

Vec3f  point3;
float d;
Point origin;         //鼠标按下的起始点
Rect selection;      //定义矩形选框
bool selectObject = false;    //是否选择对象
int mindisparity = 0;
int ndisparities = 64;
int SADWindowSize = 5;

void stereo_BM_match(int, void*)
{
    //不做标定和校正
    left_rectified_img = imread("../Material/L3.bmp");
    right_rectified_img = imread("../Material/R3.bmp");

    //立体匹配生成深度图需要用到灰度图
    cvtColor(left_rectified_img, left_rectified_gray, COLOR_BGR2GRAY);
    cvtColor(right_rectified_img, right_rectified_gray, COLOR_BGR2GRAY);

    bm->setBlockSize(2 * blockSize + 5);     //SAD窗口大小，5~21之间为宜
    //bm->setROI1(left_valid_roi);
    //bm->setROI2(right_valid_roi);
    bm->setPreFilterCap(31);
    bm->setMinDisparity(0);  //最小视差，默认值为0, 可以是负值，int型
    bm->setNumDisparities(numDisparities * 16 + 16);//视差窗口，即最大视差值与最小视差值之差,窗口大小必须是16的整数倍，int型
    bm->setTextureThreshold(10);
    bm->setUniquenessRatio(uniquenessRatio);//uniquenessRatio主要可以防止误匹配
    bm->setSpeckleWindowSize(100);
    bm->setSpeckleRange(32);
    bm->setDisp12MaxDiff(-1);
    Mat disp, disp8;
    bm->compute(left_rectified_gray, right_rectified_gray, disp);//输入图像必须为灰度图
    disp.convertTo(disp8, CV_8UC1, 255 / ((numDisparities * 16 + 16) * 16.));//计算出的视差是CV_16S格式
    reprojectImageTo3D(disp, xyz, Q, true); //在实际求距离时，ReprojectTo3D出来的X / W, Y / W, Z / W都要乘以16(也就是W除以16)，才能得到正确的三维坐标信息。
    xyz = xyz * 16;
    imshow("disparity_BM", disp8);
}


void stereo_SGBM_match(int, void*)
{
    Mat disp;

    //不做标定和校正
    left_rectified_img = imread("../Material/L3.bmp");
    right_rectified_img = imread("../Material/R3.bmp");

    //SGBM
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(mindisparity, ndisparities, SADWindowSize);
    int P1 = 8 * left_rectified_img.channels() * SADWindowSize * SADWindowSize;
    int P2 = 10 * left_rectified_img.channels() * SADWindowSize * SADWindowSize;
    sgbm->setP1(P1);
    sgbm->setP2(P2);
    sgbm->setPreFilterCap(15);
    sgbm->setUniquenessRatio(10);
    sgbm->setSpeckleRange(2);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setDisp12MaxDiff(1);
    //sgbm->setMode(cv::StereoSGBM::MODE_HH);
    sgbm->compute(left_rectified_img, right_rectified_img, disp);
    disp.convertTo(disp, CV_32F, 1.0 / 16);                //除以16得到真实视差值
    Mat disp8U = Mat(disp.rows, disp.cols, CV_8UC1);       //显示
    normalize(disp, disp8U, 0, 255, NORM_MINMAX, CV_8UC1);
    reprojectImageTo3D(disp8U, xyz, Q, true); //在实际求距离时，ReprojectTo3D出来的X / W, Y / W, Z / W都要乘以16(也就是W除以16)，才能得到正确的三维坐标信息。
    xyz = xyz * 16;
    imshow("disparity_SGBM", disp8U);
    waitKey(0);
}

/*void stereo_GC_match(int, void*)
{
    CvStereoGCState* state = cvCreateStereoGCState(16, 2);
    left_disp_ = cvCreateMat(left->height, left->width, CV_32F);
    right_disp_ = cvCreateMat(right->height, right->width, CV_32F);
    cvFindStereoCorrespondenceGC(left, right, left_disp_, right_disp_, state, 0);
    cvReleaseStereoGCState(&state);
    CvMat* disparity_left_visual = cvCreateMat(size.height, size.width, CV_8U);
    cvConvertScale(disparity_left, disparity_left_visual, -16);
}
*/

/*****描述：鼠标操作回调*****/
static void onMouse(int event, int x, int y, int, void*)
{
    if (selectObject)
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);
    }

    switch (event)
    {
    case EVENT_LBUTTONDOWN:   //鼠标左按钮按下的事件
        origin = Point(x, y);
        selection = Rect(x, y, 0, 0);
        selectObject = true;
        //cout << origin << "in world coordinate is: " << xyz.at<Vec3f>(origin) << endl;
        point3 = xyz.at<Vec3f>(origin);
        point3[0];
        //cout << "point3[0]:" << point3[0] << "point3[1]:" << point3[1] << "point3[2]:" << point3[2]<<endl;
        cout << "世界坐标：" << endl;
        cout << "x: " << point3[0] << "  y: " << point3[1] << "  z: " << point3[2] << endl;
        d = point3[0] * point3[0] + point3[1] * point3[1] + point3[2] * point3[2];
        d = sqrt(d);   //mm
       // cout << "距离是:" << d << "mm" << endl;

        d = d / 10.0;   //cm
        cout << "距离是:" << d << "cm" << endl;

        // d = d/1000.0;   //m
        // cout << "距离是:" << d << "m" << endl;

        break;
    case EVENT_LBUTTONUP:    //鼠标左按钮释放的事件
        selectObject = false;
        if (selection.width > 0 && selection.height > 0)
            break;
    }
}

int GlobalMatching()
{

    left_img = imread("../Material/L3.bmp");
    right_img = imread("../Material/R3.bmp");

    //VideoCapture cap(1);
    int FRAME_WIDTH = left_img.cols;
    int FRAME_HEIGHT = left_img.rows;

    Mat aligned_rectified_img(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);//校准+对齐后的图像
    //cout << "Resolution:<" << cap.get(CAP_PROP_FRAME_WIDTH) << "," << cap.get(CAP_PROP_FRAME_HEIGHT) << ">\n";
    //namedWindow("camera", WINDOW_AUTOSIZE);
    //    namedWindow("left_img",CV_WINDOW_NORMAL);
    //    namedWindow("right_img",CV_WINDOW_NORMAL);
    //    namedWindow("left_rectified_img",CV_WINDOW_NORMAL);
    //    namedWindow("right_rectified_img",CV_WINDOW_NORMAL);
    //    namedWindow("rectified_img",CV_WINDOW_NORMAL);
    //namedWindow("aligned_rectified_img", WINDOW_AUTOSIZE);
    //resizeWindow("camera", 1200, 600);
   // resizeWindow("aligned_rectified_img", 1200, 600);

    Mat left_cameraMatrix = Mat::eye(3, 3, CV_64F);//左相机内参矩阵
    left_cameraMatrix.at<double>(0, 0) = 2.762165790037839e+02;//Fx
    left_cameraMatrix.at<double>(0, 1) = 0;
    left_cameraMatrix.at<double>(0, 2) = 1.765880468329375e+02;//Cx
    left_cameraMatrix.at<double>(1, 1) = 2.762317738515432e+02;//Fy
    left_cameraMatrix.at<double>(1, 2) = 1.272320444598781e+02;//Cy

    Mat left_distCoeffs = Mat::zeros(5, 1, CV_64F);//畸变系数
    left_distCoeffs.at<double>(0, 0) = 0.065542106666972;//k1
    left_distCoeffs.at<double>(1, 0) = -0.099179821896270;//k2
    left_distCoeffs.at<double>(2, 0) = 0;//p1
    left_distCoeffs.at<double>(3, 0) = 0;//p2
    left_distCoeffs.at<double>(4, 0) = 0;

    Mat right_cameraMatrix = Mat::eye(3, 3, CV_64F);//右相机内参矩阵
    right_cameraMatrix.at<double>(0, 0) = 2.734695143541476e+02;//Fx
    right_cameraMatrix.at<double>(0, 1) = 0;
    right_cameraMatrix.at<double>(0, 2) = 1.724017536155269e+02;//Cx
    right_cameraMatrix.at<double>(1, 1) = 2.733548075965133e+02;//Fy
    right_cameraMatrix.at<double>(1, 2) = 1.255695004672240e+02;//Cy

    Mat right_distCoeffs = Mat::zeros(5, 1, CV_64F);//畸变系数
    right_distCoeffs.at<double>(0, 0) = 0.067619149443979;//k1
    right_distCoeffs.at<double>(1, 0) = -0.104286472771764;//k2
    right_distCoeffs.at<double>(2, 0) = 0;//p1
    right_distCoeffs.at<double>(3, 0) = 0;//p2
    right_distCoeffs.at<double>(4, 0) = 0;


    Mat rotation_matrix = Mat::zeros(3, 3, CV_64F);//旋转矩阵
    rotation_matrix.at<double>(0, 0) = 0.999997933684708;
    rotation_matrix.at<double>(0, 1) = 5.541735042905797e-04;
    rotation_matrix.at<double>(0, 2) = -0.001955893157243;
    rotation_matrix.at<double>(1, 0) = -5.560064711997943e-04;
    rotation_matrix.at<double>(1, 1) = 0.999999406695233;
    rotation_matrix.at<double>(1, 2) = -9.367315446314680e-04;
    rotation_matrix.at<double>(2, 0) = 0.001955372884999;
    rotation_matrix.at<double>(2, 1) = 9.378170983011573e-04;
    rotation_matrix.at<double>(2, 2) = 0.999997648505221;

    Mat rotation_vector;//旋转矩阵
    Rodrigues(rotation_matrix, rotation_vector);//旋转矩阵转化为旋转向量，罗德里格斯变换

    Mat translation_vector = Mat::zeros(3, 1, CV_64F);//平移向量
    translation_vector.at<double>(0, 0) = -74.303646210221200;
    translation_vector.at<double>(1, 0) = -0.208289299602746;
    translation_vector.at<double>(2, 0) = -1.203122420388863;

    Mat R1, R2;//左右相机的3x3矫正变换(旋转矩阵)
    Mat P1, P2;//左右相机新的坐标系统(矫正过的)输出 3x4 的投影矩阵
   // Mat Q;//深度视差映射矩阵

   // Rect left_valid_roi, right_valid_roi;//图像校正之后，会对图像进行裁剪，这里的validRoi就是指裁剪之后的区域
    Mat rmap[2][2];//映射表 必须为：CV_16SC2/CV_32FC1/CV_32FC2
    Size imageSize;
    imageSize = Size(FRAME_WIDTH >> 1, FRAME_HEIGHT);

    /*
        立体校正的时候需要两幅图像共面并且行对准 以使得立体匹配更加的可靠
        使得两幅图像共面的方法就是把两个摄像头的图像投影到一个公共成像面上，这样每幅图像从本图像平面投影到公共图像平面都需要一个旋转矩阵R
        stereoRectify 这个函数计算的就是从图像平面投影都公共成像平面的旋转矩阵R1,R2。 R1,R2即为左右相机平面行对准的校正旋转矩阵。
        左相机经过R1旋转，右相机经过R2旋转之后，两幅图像就已经共面并且行对准了。
        其中P1,P2为两个相机的投影矩阵，其作用是将3D点的坐标转换到图像的2D点的坐标:P*[X Y Z 1]' =[x y w]
        Q矩阵为重投影矩阵，即矩阵Q可以把2维平面(图像平面)上的点投影到3维空间的点:Q*[x y d 1] = [X Y Z W]。其中d为左右两幅图像的视差
    */
    //双目校准
    stereoRectify(left_cameraMatrix, left_distCoeffs,//左摄像头内参和畸变系数
        right_cameraMatrix, right_distCoeffs,//右摄像头内参和畸变系数
        imageSize, rotation_vector, translation_vector,//图像大小，右摄像头相对于左摄像头旋转矩阵，平移向量
        R1, R2, P1, P2, Q,//输出的参数
        CALIB_ZERO_DISPARITY, -1, imageSize, &left_valid_roi, &right_valid_roi);
    //Precompute maps for cv::remap().
    //用来计算畸变映射，输出的X / Y坐标重映射参数,remap把求得的映射应用到图像上。
    initUndistortRectifyMap(left_cameraMatrix, left_distCoeffs, R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(right_cameraMatrix, right_distCoeffs, R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    /*while (1)
    {*/
    //经过remap之后，左右相机的图像已经共面并且行对准了
    remap(left_img, left_rectified_img, rmap[0][0], rmap[0][1], INTER_LINEAR);
    remap(right_img, right_rectified_img, rmap[1][0], rmap[1][1], INTER_LINEAR);

    //立体匹配生成深度图需要用到灰度图
    cvtColor(left_rectified_img, left_rectified_gray, COLOR_BGR2GRAY);
    cvtColor(right_rectified_img, right_rectified_gray, COLOR_BGR2GRAY);

    //复制左相机校准图像到总图像上
    for (int i = 0; i < left_rectified_img.rows; i++)
    {
        for (int j = 0; j < left_rectified_img.cols; j++)
        {
            aligned_rectified_img.at<Vec3b>(i, j)[0] = left_rectified_img.at<Vec3b>(i, j)[0];
            aligned_rectified_img.at<Vec3b>(i, j)[1] = left_rectified_img.at<Vec3b>(i, j)[1];
            aligned_rectified_img.at<Vec3b>(i, j)[2] = left_rectified_img.at<Vec3b>(i, j)[2];
        }
    }
    //复制右相机校准图像到总图像上
    for (int i = 0; i < right_rectified_img.rows; i++)
    {
        for (int j = 0; j < right_rectified_img.cols; j++)
        {
            aligned_rectified_img.at<Vec3b>(i, j + (FRAME_WIDTH >> 1))[0] = right_rectified_img.at<Vec3b>(i, j)[0];
            aligned_rectified_img.at<Vec3b>(i, j + (FRAME_WIDTH >> 1))[1] = right_rectified_img.at<Vec3b>(i, j)[1];
            aligned_rectified_img.at<Vec3b>(i, j + (FRAME_WIDTH >> 1))[2] = right_rectified_img.at<Vec3b>(i, j)[2];
        }
    }

    //红色框出校正后的左右图像
    //rectangle(img, left_valid_roi, Scalar(0, 0, 255), 2, 8);
    //rectangle(img, Rect(right_valid_roi.x + (FRAME_WIDTH >> 1), right_valid_roi.y, right_valid_roi.width, right_valid_roi.height), Scalar(0, 0, 255), 2, 8);

    ////做绿色线增强对比
    //for (int i = 0; i < aligned_rectified_img.rows; i += 32)
    //{
    //    line(aligned_rectified_img, Point(0, i), Point(aligned_rectified_img.cols, i), Scalar(0, 255, 0), 1, LINE_8);
    //}


    //imshow("camera", img);
    imshow("left_img", left_img);
    imshow("right_img", right_img);
    /*imshow("left_rectified_img", left_rectified_img);
    imshow("right_rectified_img", right_rectified_img);
    imshow("aligned_rectified_img", aligned_rectified_img);*/
    waitKey(666);
    stereo_BM_match(0, 0);
    stereo_SGBM_match(0, 0);

    ////创建SAD窗口 Trackbar
    //createTrackbar("BlockSize:\n", "disparity_BM", &blockSize, 8, stereo_BM_match);
    //// 创建视差唯一性百分比窗口 Trackbar
    //createTrackbar("UniquenessRatio:\n", "disparity_BM", &uniquenessRatio, 50, stereo_BM_match);
    //// 创建视差窗口 Trackbar
    //createTrackbar("NumDisparities:\n", "disparity_BM", &numDisparities, 16, stereo_BM_match);

    //createTrackbar("SADWindowSize:\n", "disparity_SGBM", &SADWindowSize, 30, stereo_SGBM_match);
    //setMouseCallback("disparity_SGBM", onMouse, 0);
    //setMouseCallback("disparity_BM", onMouse, 0);

    //int key = waitKey(30);
    //if (key == 27)//按下Esc退出

    return 0;
    
}
