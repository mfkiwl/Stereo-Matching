#include <opencv2/imgproc.hpp>		// Gaussian Blur
#include <opencv2/core.hpp>         // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>		// OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

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

    do
    {
        camera >> ReferenceFrame;
        cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
        Detector.process(GrayFrame);
        Detector.getObjects(Faces);

		// show all detected faces in green rectangle
        for (size_t i = 0; i < Faces.size(); i++)
        {
            rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));
           
            //center of face
            Point CenterFace = 0.5 * (Faces[i].br() + Faces[i].tl());

            //center of text
            Point CenterText;

            //up
            if (CenterFace.y >= frameHeight * 0.5) {
            
                CenterText.y = CenterFace.y - 0.5 * Faces[i].height;
            }
            //left
            if (CenterFace.y <= frameHeight * 0.5) {

                CenterText.y = CenterFace.y + 0.5 * Faces[i].height + 66;
            }
            CenterText.x = CenterFace.x - 66;
            
            //attach FOV calculation code here
            putText(ReferenceFrame, "FOV", CenterText, FONT_HERSHEY_PLAIN, 6.6, Scalar(0, 255, 0));
        }
		namedWindow(WindowName, WINDOW_NORMAL);
		resizeWindow(WindowName, frameWidth*0.5, frameHeight*0.5);
        imshow(WindowName, ReferenceFrame);

    } while (waitKey(30) < 0);

    Detector.stop();

    return 0;
}