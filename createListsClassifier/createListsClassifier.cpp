/*********************************************************/
/******
/* Folder negativeImage	: negative Image
/* Folder positiveImage	: positive Image
/* Command	: ./createListsClassifier
/*
/* Use positiveList.txt for createSample
/* Use negativeList.txt for trainCascade
/*********************************************************/

//#include "stdafx.h"
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>
#include <time.h>



/*main*/
int main(int argc, char* argv[])
{
	
	int i;
	char c;

	system("ls ./positiveImage > listPositiveImages.txt");
	FILE* file = fopen("./listPositiveImages.txt","r");
	FILE* file2 = fopen("./positiveImage.txt","w+");

	char line[64] = "";
	char nameImage[64] = "";
	char pathImage[64] = "";
	
	IplImage* frame = NULL;
	
 	//int i;
	if(file != NULL){
		while(fgets(line,64,file)!=NULL) {

			for(i=0;i<64;i++)
				nameImage[i] = '\0';
		
			i = 0;
			c = line[i];
			while(c!=10){
				nameImage[i] = line[i];
				c = line[++i];
			}
		
			printf("%s\n",nameImage);
			sprintf(pathImage,"./positiveImage/%s",nameImage);

			frame = cvLoadImage(pathImage);

			fprintf(file2,"./positiveImage/%s 1 0 0 %d %d\n",nameImage,frame->width,frame->height);
		
			//cvShowImage("image",frame);
			//cvWaitKey(0);
		}
	}

	fclose(file);
	fclose(file2);


	system("ls ./negativeImage > listNegativeImages.txt");
	file = fopen("./listNegativeImages.txt","r");
	file2 = fopen("./negativeImage.txt","w+");

	if(file != NULL){
		while(fgets(line,64,file)!=NULL) {

			for(i=0;i<64;i++)
				nameImage[i] = '\0';
		
			i = 0;
			c = line[i];
			while(c!=10){
				nameImage[i] = line[i];
				c = line[++i];
			}
		
			printf("%s\n",nameImage);
			sprintf(pathImage,"./negativeImage/%s",nameImage);

			frame = cvLoadImage(pathImage);

			fprintf(file2,"./negativeImage/%s\n",nameImage);
		
			//cvShowImage("image",frame);
			//cvWaitKey(0);
		}
	}

	fclose(file);

	fclose(file2);


	cvDestroyAllWindows();
	
	return 0;
}
	
