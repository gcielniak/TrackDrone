#include "Command.h"

Command::Command()
{
	seq = 1;
	onGround = 1;
	altitude=0;
	pitch=0;
	yaw=0;
	roll=0;

	connected=false;	
	quit=false;
	priorityCommand = false;

	mutexArg = SDL_CreateMutex();
}

void Command::mutexP(){

	SDL_mutexP(mutexArg);
}

void Command::mutexV(){

	SDL_mutexV(mutexArg);
}

void Command::destroyMutex(){

	SDL_DestroyMutex(mutexArg);
}

void Command::emergency(){

	sprintf(msg, "AT*REF=%i,290717952\r", seq);
	communication->sendMessage(msg);
	seq++;
	onGround = true;
}

void Command::land(){

	sprintf(msg, "AT*REF=%i,290717696\r", seq);
	communication->sendMessage(msg);
	seq++;
	onGround = true;
}

void Command::takeOff(){

	sprintf(msg, "AT*CONFIG=%d,\"general:navdata_demo\",\"TRUE\"\r", seq);
        seq++;
        communication->sendMessage(msg);
	SDL_Delay(DELAY_COMMAND);

        sprintf(msg, "AT*CTRL=0\r");
        //seq++;
        communication->sendMessage(msg);
        SDL_Delay(DELAY_COMMAND);
	
	sprintf(msg, "AT*FTRIM=%i\r", seq);
	communication->sendMessage(msg);
	seq ++;
	SDL_Delay(DELAY_COMMAND);
	sprintf(msg, "AT*REF=%i,290718208\r", seq);
	communication->sendMessage(msg);
	seq++;
	onGround = false;

}

void Command::changeCam(int cam){

	sprintf(msg, "AT*CONFIG=%i,\"video:video_channel\",\"%i\"\r", seq, cam);
	communication->sendMessage(msg);
	seq++;
}

void Command::watchdog(){

	sprintf(msg, "AT*COMWDG=%d\r", seq);
	communication->sendMessage(msg);
	seq++;

}

void Command::movement(){

	if( pitch || roll || altitude || yaw )
		sprintf(msg, "AT*PCMD=%d,1,%d,%d,%d,%d\r", seq, *(int*)(&pitch), *(int*)(&roll), *(int*)(&altitude), *(int*)(&yaw));
	else
		sprintf(msg, "AT*PCMD=%d,0,%d,%d,%d,%d\r", seq, *(int*)(&pitch), *(int*)(&roll), *(int*)(&altitude), *(int*)(&yaw));

	communication->sendMessage(msg);
	seq++;
}

void Command::initializationCommunication(){

	communication = new Communication(AT_PORT);
	communication->init();
	connected = true;
}

void Command::initializationDrone()
{
	sprintf(msg, "AT*CONFIG=%d,\"general:navdata_demo\",\"TRUE\"\r", seq);
	seq++;
	communication->sendMessage(msg);
	SDL_Delay(DELAY_COMMAND);

	sprintf(msg, "AT*CONFIG=1,\"video:video_channel\",\"0\"\r");
	communication->sendMessage(msg);
	SDL_Delay(DELAY_COMMAND);

	sprintf(msg, "AT*CONFIG=%d,\"control:altitude_max\",\"%d\"\r", seq, 10000);//10000 == unlimited
	seq ++; 
	communication->sendMessage(msg);
	SDL_Delay(DELAY_COMMAND);

}

bool Command::isConnected(){
	return connected;
}

bool Command::isOnGround(){
	return onGround;
}

float Command::getAltitude(){
	return altitude;
}

float Command::getPitch(){
	return pitch;
}

float Command::getYaw(){
	return yaw;
}

float Command::getRoll(){
	return roll;
}

int Command::getSeq(){
	return seq;
}

bool Command::isPriorityCommand(){
	return priorityCommand;
}

void Command::setOnGround(bool onGround){
	this->onGround=onGround;
}

void Command::setAltitude(float altitude){
	this->altitude=altitude;
}

void Command::setPitch(float pitch){
	this->pitch=pitch;
}

void Command::setYaw(float yaw){
	this->yaw=yaw;
}

void Command::setRoll(float roll){
	this->roll=roll;
}

void Command::setPriorityCommand(bool priorityCommand){
	this->priorityCommand=priorityCommand;
}

void Command::disconnection(){
	communication->closeSocket();
}

bool Command::getQuit(){
	return quit;
}

void Command::shutDown(){
	quit=true;
}

/***************************** THREAD *******************************/
int threadCommand(void *data){

	Command* command = (Command*)data;

	command->initializationCommunication();
	command->initializationDrone();

	while(!command->getQuit()){

		if(!command->isPriorityCommand() && !command->isOnGround()){

			command->mutexP();
			command->movement();
			command->mutexV();
		}
	
	SDL_Delay(DELAY_COMMAND);
	}

	printf("END THREAD CMD\n");	
	
	return 0;
}

int threadWatchdog(void *data){

	Command* command = (Command*)data;

	while(!command->getQuit()){

		if(command->isConnected()){

			command->mutexP();	
			command->watchdog();
			command->mutexV();
			command->setPriorityCommand(true);
			SDL_Delay(1000); //1 seconde
		}
	}

	printf("END THREAD WATCHDOG\n");	
	
	return 0;
}


