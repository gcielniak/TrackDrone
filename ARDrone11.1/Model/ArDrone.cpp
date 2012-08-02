#include "ArDrone.h"

ArDrone::ArDrone(){

	command = new Command();
	video = new Video();
	navigationData = new NavigationData();

}

void ArDrone::runThread(){

	threadVd = SDL_CreateThread(threadVideo,video);
	threadCmd = SDL_CreateThread(threadCommand,command);
	threadNd = SDL_CreateThread(threadNavData,navigationData);
	threadWd = SDL_CreateThread(threadWatchdog,command);
}

void ArDrone::mutexP(int type){

	if(type == VIDEO)
		video->mutexP();
	else if(type == COMMAND)
		command->mutexP();
	else if(type == NAVIGATION_DATA)
		navigationData->mutexP();
}

void ArDrone::mutexV(int type){

	if(type == VIDEO)
		video->mutexV();
	else if(type == COMMAND)
		command->mutexV();
	else if(type == NAVIGATION_DATA)
		navigationData->mutexV();
}

void ArDrone::shutDown(){

	command->shutDown();
	video->shutDown();
	navigationData->shutDown();

	SDL_WaitThread(threadCmd, NULL);
	SDL_WaitThread(threadVd, NULL);
	SDL_WaitThread(threadNd, NULL);
	SDL_WaitThread(threadWd, NULL);

	video->destroyMutex();
	command->destroyMutex();
	navigationData->destroyMutex();
}

/*************************** COMMAND *******************************/

void ArDrone::setAltitude(float altitude){
	command->setAltitude(altitude);
}

void ArDrone::setPitch(float pitch){
	command->setPitch(pitch);
}

void ArDrone::setYaw(float yaw){
	command->setYaw(yaw);
}

void ArDrone::setRoll(float roll){
	command->setRoll(roll);
}

void ArDrone::setPriorityCommand(bool priorityCommand){
	command->setPriorityCommand(priorityCommand);
}

void ArDrone::emergency(){
	command->emergency();
	shutDown();
}

void ArDrone::landing(){

	command->mutexP();
	if(!command->isOnGround()){
		command->land();
	}
	command->mutexV();
}

void ArDrone::takeOff(){

	command->mutexP();
	if(command->isOnGround()){
		command->takeOff();
	}
	command->mutexV();
}

void ArDrone::changeCam(int num){
	
	command->mutexP();
	command->changeCam(num);	
	command->mutexV();
}

/********************************** VIDEO **********************************/
IplImage* ArDrone::getFrame(int type){

	return video->getFrame(type);
}

void ArDrone::saveFrame(char* path,int type,bool color){

	video->saveFrame(path,type,color);
}

/***************************** NAVIGATION DATA *******************************/

int ArDrone::getLevelBattery(){
	return navigationData->getLevelBattery();
}

int ArDrone::getAltitude(){
	return navigationData->getAltitude();
}

float ArDrone::getPsi(){
	return navigationData->getPsi();
}

float ArDrone::getPhi(){
	return navigationData->getPhi();
}

float ArDrone::getTheta(){
	return navigationData->getTheta();
}

float ArDrone::getVx(){
	return navigationData->getVx();
}

float ArDrone::getVy(){
	return navigationData->getVy();
}

