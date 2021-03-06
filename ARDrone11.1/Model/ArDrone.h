#ifndef HEADER_ARDRONE
#define HEADER_ARDRONE


#include "Command.h"
#include "Video.h"
#include "NavigationData.h"

#include <SDL/SDL.h>


#define SIZE_MAX_NAVDATA	2048

#define COMMAND		0
#define VIDEO		1
#define NAVIGATION_DATA	2

class ArDrone
{
	public:
	
	ArDrone();

	void runThread();
	void mutexP(int type);
	void mutexV(int type);
	void sendCommand();
	void emergency();
	void landing();
	void takeOff();
	void changeCam(int num);
	IplImage* getFrame(int type);
	void saveFrame(char* path,int type,bool color);
	void shutDown();
	int getLevelBattery();
	int getAltitude();
	float getPsi();
	float getPhi();
	float getTheta();
	float getVx();
	float getVy();
	void setAltitude(float altitude);
	void setPitch(float pitch);
	void setYaw(float yaw);
	void setRoll(float roll);
	void setMsg(char* msg);
	void setOnGround(bool onGround);
	void setPriorityCommand(bool priorityCommand);	

	private:

	Command *command;
	Video *video;
	NavigationData *navigationData;

	SDL_Thread* threadCmd;
	SDL_Thread* threadVd;
	SDL_Thread* threadNd;
	SDL_Thread* threadWd;
};


#endif
