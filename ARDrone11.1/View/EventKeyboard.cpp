#include "EventKeyboard.h"

EventKeyboard::EventKeyboard()
{	
	mutexKeyData= SDL_CreateMutex();

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	surface = SDL_SetVideoMode(696, 456, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_FreeSurface(surface);
	
	int i;
	for(i=0;i<23;i++)
		key[i]=false;

	joystick = SDL_JoystickOpen(0);
	SDL_JoystickEventState(SDL_ENABLE);
}


bool EventKeyboard::getKey(int i){

	return key[i];
}

void EventKeyboard::setKey(bool val,int i){

	key[i]=val;
}

void EventKeyboard::mutexP(){

	SDL_mutexP(mutexKeyData);

}

void EventKeyboard::mutexV(){

	SDL_mutexV(mutexKeyData);

}

void EventKeyboard::destroyMutex(){

	SDL_DestroyMutex(mutexKeyData);

}

SDL_Surface* EventKeyboard::getSDLSurface(){return surface;}

void EventKeyboard::updateEvent(){

	while(!quit){

		mutexP();

		while(SDL_PollEvent(&event))
		{
			/* XBOX EVENT */

			if (event.type==SDL_JOYBUTTONDOWN){
				switch(event.jbutton.button){
						case 0:
							key[A_BUTTON] = true;
							break;
						case 1:
							break;
						case 2:
							key[X_BUTTON] = true;
							break;
						case 3:
							key[Y_BUTTON] = true;
							break;
						case 4:
							break;
						case 5:
							key[RIGHT_BUMPER] = true;
							break;
						case 6:
							key[BACK_BUTTON] = true;
							break;
						case 7:
							key[START_BUTTON] = true;
							break;
						case 8:
							key[XBOX_BUTTON] = true;
							break;
						case 9:
							break;
						case 10:
							break;

						default:
							break;
				}
			}

			if (event.type==SDL_JOYBUTTONUP){
				switch(event.jbutton.button){
						case 0:
							key[A_BUTTON] = false;
							break;
						case 1:
							break;
						case 2:
							key[X_BUTTON] = false;
							break;
						case 3:
							key[Y_BUTTON] = false;
							break;
						case 4:
							break;
						case 5:
							key[RIGHT_BUMPER] = false;
							break;
						case 6:
							key[BACK_BUTTON] = false;
							break;
						case 7:
							key[START_BUTTON] = false;
							break;
						case 8:
							key[XBOX_BUTTON] = false;
							break;
						case 9:
							break;
						case 10:
							break;

						default:
							break;
				}
			}

			if (event.type==SDL_JOYAXISMOTION){
				switch(event.jaxis.axis){
						case 0:
							if(event.jaxis.value < -2000)
								key[LEFT_STICK_LEFT] = true;
							else if(event.jaxis.value > 2000)
								key[LEFT_STICK_RIGHT] = true;
							else{	key[LEFT_STICK_LEFT] = false;
								key[LEFT_STICK_RIGHT] = false;
							    }

							break;
						case 1:
							if(event.jaxis.value > 2000)
								key[LEFT_STICK_DOWN] = true;
							else if(event.jaxis.value < -2000)
								key[LEFT_STICK_UP] = true;
							else{	key[LEFT_STICK_DOWN] = false;
								key[LEFT_STICK_UP] = false;
							    }
							break;
						case 2:
							
							if(event.jaxis.value == 32767){

								key[LEFT_TRIGGER] = true;
							}
							else if(event.jaxis.value == -32768){

								key[LEFT_TRIGGER] = false;

							}
							else{
								key[LEFT_TRIGGER] = false;
							}
							break;
						case 3:
							if(event.jaxis.value < -2000)
								key[RIGHT_STICK_LEFT] = true;
							else if(event.jaxis.value > 2000)
								key[RIGHT_STICK_RIGHT] = true;
							else{	key[RIGHT_STICK_LEFT] = false;
								key[RIGHT_STICK_RIGHT] = false;
							    }

							break;
						case 4:
							if(event.jaxis.value < -2000)
								key[RIGHT_STICK_UP] = true;
							else if(event.jaxis.value > 2000)
								key[RIGHT_STICK_DOWN] = true;
							else{	key[RIGHT_STICK_UP] = false;
								key[RIGHT_STICK_DOWN] = false;
							    }

							break;
						case 5:
							
							if(event.jaxis.value == 32767){
		
								key[RIGHT_TRIGGER] = true;
							}
							else if(event.jaxis.value == -32768){
								
								key[RIGHT_TRIGGER] = false;

							}
							else{	
								key[RIGHT_TRIGGER] = false;
							}
							break;
						case 6:
							break;

						default:
							break;
				}



			}

			if (event.type==SDL_JOYHATMOTION){
				if(event.jhat.hat==0){
					switch(event.jhat.value){
						case SDL_HAT_UP:
							key[DIRECTIONNAL_PAD_UP] = true;
							break;
						case SDL_HAT_DOWN:
							key[DIRECTIONNAL_PAD_RIGHT] = true;
							break;
						case SDL_HAT_RIGHT:
							key[DIRECTIONNAL_PAD_DOWN] = true;
							break;
						case SDL_HAT_LEFT:
							key[DIRECTIONNAL_PAD_LEFT] = true;
							break;
						case SDL_HAT_CENTERED:
							key[DIRECTIONNAL_PAD_UP] = false;
							key[DIRECTIONNAL_PAD_RIGHT] = false;
							key[DIRECTIONNAL_PAD_DOWN] = false;
							key[DIRECTIONNAL_PAD_LEFT] = false;
							break;

						default:
							break;
					}
				}
			}

			/* KEYBOARD EVENT */

			if (event.type==SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym){
					case SDLK_LEFT:
						key[LEFT_STICK_LEFT] = true;
						break;
					case SDLK_RIGHT:
						key[LEFT_STICK_RIGHT] = true;
						break;
					case SDLK_UP:
						key[LEFT_STICK_UP] = true;
						break;
					case SDLK_DOWN:
						key[LEFT_STICK_DOWN] = true;
						break;
					case SDLK_a:
						key[RIGHT_STICK_LEFT] = true;
						break;
					case SDLK_d:
						key[RIGHT_STICK_RIGHT] = true;
						break;
					case SDLK_l:
						key[BACK_BUTTON] = true;
						break;
					case SDLK_r:
						key[XBOX_BUTTON] = true;
						break;
					case SDLK_s:
						key[RIGHT_STICK_DOWN] = true;
						break;
					case SDLK_t:
						key[START_BUTTON] = true;
						break;
					case SDLK_w:
						key[RIGHT_STICK_UP] = true;
						break;
					case SDLK_m:
						key[A_BUTTON] = true;
						break;
					case SDLK_x:
						key[X_BUTTON] = true;
						break;
					case SDLK_1:
						key[DIRECTIONNAL_PAD_UP] = true;
						break;
					case SDLK_2:
						key[DIRECTIONNAL_PAD_RIGHT] = true;
						break;
					case SDLK_3:
						key[DIRECTIONNAL_PAD_DOWN] = true;
						break;
					case SDLK_4:
						key[DIRECTIONNAL_PAD_LEFT] = true;
						break;
					case SDLK_ESCAPE:
						key[Y_BUTTON] = true;
						break;
					default:
						break;
				}
			}
			else if (event.type==SDL_KEYUP){
				switch(event.key.keysym.sym){
					case SDLK_LEFT:
						key[LEFT_STICK_LEFT] = false;
						break;
					case SDLK_RIGHT:
						key[LEFT_STICK_RIGHT] = false;
						break;
					case SDLK_UP:
						key[LEFT_STICK_UP] = false;
						break;
					case SDLK_DOWN:
						key[LEFT_STICK_DOWN] = false;
						break;
					case SDLK_a:
						key[RIGHT_STICK_LEFT] = false;
						break;
					case SDLK_d:
						key[RIGHT_STICK_RIGHT] = false;
						break;
					case SDLK_l:
						key[BACK_BUTTON] = false;
						break;
					case SDLK_r:
						key[XBOX_BUTTON] = false;
						break;
					case SDLK_s:
						key[RIGHT_STICK_DOWN] = false;
						break;
					case SDLK_t:
						key[START_BUTTON] = false;
						break;
					case SDLK_w:
						key[RIGHT_STICK_UP] = false;
						break;
					case SDLK_m:
						key[A_BUTTON] = false;
						break;
					case SDLK_x:
						key[X_BUTTON] = false;
						break;
					case SDLK_1:
						key[DIRECTIONNAL_PAD_UP] = false;
						break;
					case SDLK_2:
						key[DIRECTIONNAL_PAD_RIGHT] = false;
						break;
					case SDLK_3:
						key[DIRECTIONNAL_PAD_DOWN] = false;
						break;
					case SDLK_4:
						key[DIRECTIONNAL_PAD_LEFT] = false;
						break;
					case SDLK_ESCAPE:
						key[Y_BUTTON] = false;
						break;
		
					default:
						break;
				}
			}
		}	
		mutexV();
	}
	SDL_JoystickClose(joystick);
}

bool EventKeyboard::getQuit(){return quit;}

void EventKeyboard::shutDown(){

	quit=true;
}

/******************** THREAD ****************************/
int threadEventKeyboard(void *data){

	EventKeyboard* eventKeyboard = (EventKeyboard*)data;
	
	eventKeyboard->updateEvent();	

	printf("END THREAD EVTKEYBOARD\n");

	return 0;
}
