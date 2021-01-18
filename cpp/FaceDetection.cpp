#include <opencv2/imgproc.hpp>		// Gaussian Blur
#include <opencv2/core.hpp>         // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>		// OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace cv;

const string WindowName = "KAMERAWERK Face Detection";

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector
{
    public:
        CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector):
            IDetector(),
            Detector(detector)
        {
            CV_Assert(detector);
        }

        void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) CV_OVERRIDE
        {
            Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
        }

        virtual ~CascadeDetectorAdapter() CV_OVERRIDE
        {}

    private:
        CascadeDetectorAdapter();
        cv::Ptr<cv::CascadeClassifier> Detector;
 };

int main(int , char** )
{
	// open the left and right camera 
	double frameWidth = 2560;
	double threeqtFrameWidth = 1920;
	double halfFrameWidth = 1280;
	double quarterFrameWidth = 640;
	double frameHeight = 720;
	VideoCapture camera;
	camera.open(0, CAP_DSHOW);
	if (!camera.isOpened())
	{
		printf("Error: Cannot open video stream from camera\n");
		return 1;
	}
	camera.set(CAP_PROP_FRAME_WIDTH, frameWidth);       // width
	camera.set(CAP_PROP_FRAME_HEIGHT, frameHeight);     // height

    std::string cascadeFrontalfilename = "lbpcascade_frontalface.xml";
	
    cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error MainDetector: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error TrackingDetector: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    DetectionBasedTracker::Parameters params;
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

    if (!Detector.run())
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    Mat ReferenceFrame;
    Mat GrayFrame;
    vector<Rect> Faces;
	double FoV;
	Point CenterFace, CenterText;

    do
    {
        camera >> ReferenceFrame;
        cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
        Detector.process(GrayFrame);
        Detector.getObjects(Faces);

		// show all detected faces in green rectangle
        for (size_t i = 0; i < Faces.size(); i++)
        {
            rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0), 2);
           
            // center of face/rectangle
            CenterFace = 0.5 * (Faces[i].br() + Faces[i].tl());

            // center of text
            CenterText = Point(0,0);

            // face in lower half
            if (CenterFace.y >= frameHeight * 0.5) {
            
                CenterText.y = CenterFace.y - 0.5 * Faces[i].height;
            }
            // face in upper half
            if (CenterFace.y <= frameHeight * 0.5) {
				// shift the face center about +50 pixels along y-axis as the bottom of text string
                CenterText.y = CenterFace.y + 0.5 * Faces[i].height + 60;
            }
			// shift the face center about -50 pixels along x-axis as the left of text string
			CenterText.x = CenterFace.x - 60;
            
            //attach FOV calculation code here
			FoV = 0.0;
			// left frame, left half
			if (CenterFace.x >= 0 && CenterFace.x <= quarterFrameWidth) 
			{
				FoV = (CenterFace.x-quarterFrameWidth)/quarterFrameWidth*45;
			}
			// left frame, right half
			if (CenterFace.x > quarterFrameWidth && CenterFace.x <= halfFrameWidth) 
			{

				FoV = (CenterFace.x-quarterFrameWidth)/quarterFrameWidth*45;
			}
			// right frame, left half
			if (CenterFace.x > halfFrameWidth && CenterFace.x <= threeqtFrameWidth) 
			{

				FoV = (CenterFace.x-threeqtFrameWidth)/quarterFrameWidth*45;
			}
			// right frame, right half
			if (CenterFace.x > threeqtFrameWidth && CenterFace.x <= frameWidth) 
			{

				FoV = (CenterFace.x-threeqtFrameWidth)/quarterFrameWidth*45;
			}
			char FoVs[50];
			_itoa_s(FoV, FoVs, 10);
			// CenterText: Bottom-left corner of the text string 
            putText(ReferenceFrame, FoVs, CenterText, FONT_HERSHEY_PLAIN, 5, Scalar(0, 255, 0),3);
        }
		cv::namedWindow(WindowName, WINDOW_NORMAL);
		cv::resizeWindow(WindowName, frameWidth*0.5, frameHeight*0.5);
        cv::imshow(WindowName, ReferenceFrame);

    } while (waitKey(30) < 0);

    Detector.stop();

    return 0;
}