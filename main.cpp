
#include<stdio.h>
#include<math.h>



#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<vector>

using namespace cv;
using namespace std;

int main()
{
	CascadeClassifier face_cascade, eye_cascade,nose_cascade,mouth_cascade;
	if(!face_cascade.load("c:\\haar\\haarcascade_frontalface_alt2.xml")) {
		printf("Error loading cascade file for face");
		return 1;
	}
	if(!eye_cascade.load("c:\\haar\\haarcascade_eye_tree_eyeglasses.xml")) {
		printf("Error loading cascade file for eye");
		return 1;
	}
	if(!nose_cascade.load("c:\\haar\\Nariz.xml")) {
		printf("Error loading cascade file for face");
		return 1;
	}
	if(!mouth_cascade.load("c:\\haar\\data_haarcascades_haarcascade_mcs_mouth.xml")){

        printf("Erro loading the cascade File of MOuth");
	}



		VideoCapture capture(0); //-1, 0, 1 device id
	if(!capture.isOpened())
	{
		printf("error to initialize camera");
		return 1;
	}


	Mat cap_img,gray_img;
	vector<Rect> faces, eyes,nose,head,mouth;
	while(1)
	{
		capture >> cap_img;
		waitKey(10);
				cvtColor(cap_img, gray_img, cv::COLOR_BGR2GRAY);
		cv::equalizeHist(gray_img,gray_img);

		face_cascade.detectMultiScale(gray_img, faces, 1.1, 10, CASCADE_SCALE_IMAGE

 | cv::CASCADE_DO_CANNY_PRUNING , cvSize(0,0), cvSize(300,300));
		for(int i=0; i < faces.size();i++)
		{
			Point pt1(faces[i].x+faces[i].width, faces[i].y+faces[i].height);
			Point pt2(faces[i].x,faces[i].y);
			Mat faceROI = gray_img(faces[i]);
			eye_cascade.detectMultiScale(faceROI, eyes, 1.1, 5, 0 | CASCADE_SCALE_IMAGE
, Size(30,30));
         nose_cascade.detectMultiScale(faceROI, nose, 1.1, 5, 0 | CASCADE_SCALE_IMAGE
, Size(30,30));
         mouth_cascade.detectMultiScale(faceROI, mouth, 1.1, 5, 0 | CASCADE_SCALE_IMAGE
, Size(30,30));

        bool is_full_detection = false;
        if( (!eye_cascade.empty()) && (!nose_cascade.empty()) && (!mouth_cascade.empty()) )
            is_full_detection = true;


			for(size_t j=0; j< eyes.size(); j++)
			{
				//Point center(faces[i].x+eyes[j].x+eyes[j].width*0.5, faces[i].y+eyes[j].y+eyes[j].height*0.5);
				Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.3 );
				int radius = cvRound((eyes[j].width+eyes[j].height)*0.20);
				circle(cap_img, center, radius, Scalar(255,0,0), 4, 4, 0);
			}

            double nose_center_height=0.0;
			for(size_t j=0; j< nose.size(); j++)
			{
				//Point center(faces[i].x+eyes[j].x+eyes[j].width*0.5, faces[i].y+eyes[j].y+eyes[j].height*0.5);
				Point center( faces[i].x + nose[j].x + nose[j].width*0.9, faces[i].y + nose[j].y + nose[j].height*0.1 );
				int radius = cvRound((nose[j].width+nose[j].height)*0.2);
				circle(cap_img, center, radius, Scalar(0,0,255), 2, 4, 0);
                nose_center_height = (nose[j].y + nose[j].height/2);
                printf("%d Nose :",&nose_center_height);
			}

            for(size_t k=0; k< mouth.size(); k++){

                        /*Point pt3(mouth[k].x+mouth[k].width, mouth[i].y+mouth[k].height);
			            Point pt4(mouth[k].x,mouth[k].y);
                        rectangle(cap_img, pt3, pt4, cvScalar(0,255,0), 2, 8, 0);
                        double mouth_center_height = (mouth[k].y +mouth[k].height/2);
*/

            double mouth_center_height = (mouth[k].y +mouth[k].height/2);
            Rect m=mouth[k];
                if( (is_full_detection)&&(mouth_center_height > nose_center_height))
                {
                    Point center( faces[i].x + mouth[k].x + mouth[k].width*0.9, faces[i].y + mouth[k].y + mouth[k].height*0.1 );
				int radius = cvRound((mouth[k].width+mouth[k].height)*0.3);
				circle(cap_img, center, radius, Scalar(0,0,0),2, 4, 0);
                double mouth_center_height = (mouth[k].y +mouth[k].height/2);


                                    }
               else if((is_full_detection) &&(mouth_center_height <= nose_center_height))
                {

                continue;
                }

			}

			rectangle(cap_img, pt1, pt2, cvScalar(0,255,0), 2, 8, 0);
     		}
	    	imshow("Result", cap_img);
		    waitKey(3);
		    char c = waitKey(3);
		    if(c == 27)
			break;
	}
	return 0;
}

