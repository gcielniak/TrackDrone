#include "NavigationData.h"

NavigationData::NavigationData()
{
	mutexNavData= SDL_CreateMutex();
	quit = false;	
}

void NavigationData::initializationCommunication(){

	communication = new Communication(NAVDATA_PORT);
	communication->init();
}

void NavigationData::disconnection(){

	communication->closeSocket();
}

void NavigationData::update()
{
	if( length >= 60 && nav.header == 0x55667788 )
	{
		nav.theta = (nav.theta / 900) + 2.8;
		nav.phi = (nav.phi / 900) + 6.4;
		nav.psi = nav.psi / 900;
	}
}

bool NavigationData::getQuit(){
	return quit;
}

int NavigationData::getLevelBattery(){
	return nav.bat;
}

int NavigationData::getAltitude(){
	return nav.altitude;
}

float NavigationData::getPsi(){
	return nav.psi;
}

float NavigationData::getPhi(){
	return nav.phi;
}

float NavigationData::getTheta(){
	return nav.theta;
}

float NavigationData::getVx(){
	return nav.vx;
}

float NavigationData::getVy(){
	return nav.vy;
}

void NavigationData::mutexP(){

	SDL_mutexP(mutexNavData);

}

void NavigationData::mutexV(){

	SDL_mutexV(mutexNavData);
}

void NavigationData::destroyMutex(){

	SDL_DestroyMutex(mutexNavData);
}

void NavigationData::shutDown(){
	quit=true;
}

/********************************** THREAD ****************************/
int threadNavData(void *data){

	NavigationData* navigationData = (NavigationData*)data;

	navigationData->initializationCommunication();
	
	navigationData->communication->wakeUpPort();

	while(!navigationData->getQuit())
	{
		navigationData->communication->selectReceiveData(&navigationData->nav, &navigationData->length, SIZE_MAX_NAVDATA, TIMER);
		
		if( navigationData->length < 1 )
		navigationData->communication->wakeUpPort();
		else
		navigationData->update();
		
	}
	
	printf("END THREAD NAV DATA\n");

	return 0;
}
