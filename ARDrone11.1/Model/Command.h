#ifndef HEADER_COMMAND
#define HEADER_COMMAND


#include <SDL/SDL.h>

#define SIZE_MSG		1024
#define DELAY_COMMAND		30

#include "Communication.h"

int threadCommand(void *data);
int threadWatchdog(void *data);

class Command
{
	public:

	Command();

	void turnOnUpdateEvent();
	void emergency();
	void land();
	void takeOff();
	void movement();
	void watchdog();
	void initializationCommunication();
	void disconnection();
	void initializationDrone();
	
	void changeCam(int cam);
	void shutDown();

	float getAltitude();
	float getPitch();
	float getYaw();
	float getRoll();
	char* getMsg();
	int getSeq();
	bool getKey(int i);
	bool isPriorityCommand();
	bool isOnGround();
	bool isConnected();
	bool getQuit();

	void setAltitude(float altitude);
	void setPitch(float pitch);
	void setYaw(float yaw);
	void setRoll(float roll);
	void setMsg(char* msg);
	void setOnGround(bool onGround);
	void setPriorityCommand(bool priorityCommand);
	void setKey(bool val,int i);
	
	void mutexP();
	void mutexV();
	void destroyMutex();


	private:

	SDL_mutex* mutexArg;
	Communication *communication;
	unsigned int seq;	// Current sequence number

	bool quit;
	bool connected;
	bool priorityCommand;

	float altitude;
	float pitch;
	float yaw;
	float roll;
	bool onGround;	
	char msg[SIZE_MSG];

};

#endif
