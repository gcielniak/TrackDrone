/*********************************************************/
/******
/* Folder XML	: classifier to evaluate
/* Folder Image : image test
/* Command	: ./evaluateClassifier nameOfclassifier
/*			without .xml
/*********************************************************/


#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[])
{
	if(argc!=2){
		printf("error parameters\n");	
		exit(1);
	}

	int i;
	char c;

	system("ls ./Image > listImages.txt");
	FILE* file = fopen("./listImages.txt","r");

	char line[64] = "";
	char nameImage[64] = "";
	char pathImage[64] = "";
	char pathXML[64] = "";

	IplImage* frame = cvCreateImage( cvSize(176, 144 ), IPL_DEPTH_8U, 1 );

	sprintf(pathXML,"./XML/%s.xml",argv[1]);

	cv::CascadeClassifier *classifier;
	classifier = new cv::CascadeClassifier();
	classifier->load(pathXML);
	std::vector<cv::Rect> rect;

	if(file != NULL){
		while(fgets(line,64,file) != NULL) {
			
			for(i=0;i<64;i++)
				nameImage[i] = '\0';
		
			i = 0;
			c = line[i];
			while(c!=10){ 				//end of line
				nameImage[i] = line[i];
				c = line[++i];
			}		
		
			sprintf(pathImage,"./Image/%s",nameImage);

			frame = cvLoadImage(pathImage);
		
			classifier->detectMultiScale(frame,rect,1.1,3,1);

			for(i = 0; i<rect.size(); i++){
				CvPoint pt1 = { rect[i].x, rect[i].y };
				CvPoint pt2 = { rect[i].x + rect[i].width, rect[i].y + rect[i].height };
				cvRectangle(frame, pt1, pt2, CV_RGB(255,0,0), 1, 1, 0);
			}
			cvShowImage("image",frame);
		
			c = cvWaitKey(0);
			}
		}

		cvDestroyAllWindows();
	
	return 0;
}
	
