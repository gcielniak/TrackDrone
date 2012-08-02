#ifndef HEADER_DISPLAY
#define HEADER_DISPLAY

#include <SDL/SDL.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define POS_X_LEVEL_BATTERY 170
#define POS_Y_LEVEL_BATTERY 160
#define POS_X_ALTITUDE 430
#define POS_Y_ALTITUDE 160
#define POS_X_VX 145
#define POS_Y_VX 213
#define POS_X_VY 403
#define POS_Y_VY 213
#define POS_X_PSI 220
#define POS_Y_PSI 261
#define POS_X_PHI 448
#define POS_Y_PHI 261
#define POS_X_THETA 250
#define POS_Y_THETA 308

#define POS_X_VERTICAL_SPEED 210
#define POS_Y_VERTICAL_SPEED 128
#define POS_X_ANGULAR_SPEED 210
#define POS_Y_ANGULAR_SPEED 171
#define POS_X_LEFT_RIGHT_TILT 205
#define POS_Y_LEFT_RIGHT_TILT 213
#define POS_X_FRONT_BACK_TILT 212
#define POS_Y_FRONT_BACK_TILT 259
#define POS_X_RECORD_SPEED 500
#define POS_Y_RECORD_SPEED 126
#define POS_X_TRACKING 500
#define POS_Y_TRACKING 172

class Display
{
	public:

	Display(SDL_Surface* s);
	void init();

	void video(bool lock,int numObject);
	void mainWindow();
	void navData();
	void parameters();
	void manualMode();
	void trackingMode();
	
	void saveFrame(const char* filename);

	void detectionOn();
	void detectionOff();
	void nextObject();
	void previousObject();
	void nextPerson();
	void previousPerson();
	void shutDown();

	int getObjectNumber();
	bool isDetection();
	bool getQuit();

	void setImage(IplImage* imageCamera);	
	void setRect(std::vector<cv::Rect> pFaceRectSeq);
	void setNavData(int levelBattery,int altitude,float psi, float phi, float theta, float vx, float vy);
	void setParameters(int verticalSpeed,int angularSpeed,int leftRightTilt, int frontBackTilt,int configuration, bool record, bool tracking);

	private:
	
	std::vector<cv::Rect> rect;

	IplImage* imageCamera;
	IplImage* imageNavData;
	IplImage* imageParameters;

	SDL_Surface* imageManualMode;
	SDL_Surface* imageTrackingMode;
	SDL_Surface* image;
	SDL_Surface* surface;
	SDL_Rect sdlRect;

	CvFont font;
	double hScale;
	double vScale;
	int    lineWidth;

	bool next, previous;
	bool quit;
	bool detection;

	int num;
	int numberObject;

	int levelBattery;
	int altitude;
	float psi;
	float phi;
	float theta;
	float vx;
	float vy;
	int verticalSpeed;
	int angularSpeed;
	int leftRightTilt;
	int frontBackTilt;

	bool record;
	bool tracking;
	int configuration;
};


#endif


