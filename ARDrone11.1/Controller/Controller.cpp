#include "Controller.h"
#include "time.h"

Controller::Controller()
{
	arDrone = new ArDrone();
	eventKeyboard = new EventKeyboard();
	display = new Display(eventKeyboard->getSDLSurface());
	classifier = new cv::CascadeClassifier();

	classifier->load("./XML/trainCascadestg10-50.xml");

	tracking = false;
	quit=false;
	record = false;
	cam = HORIZONTAL_CAM;

	int i;
	for(i=0;i<23;i++)
		pressed[i]=false;

	levelBattery=0;
	altitude=0;
	psi=0;
	phi=0;
	theta=0;
	vx=0;
	vy=0;

	verticalSpeed = 0;
	angularSpeed = 0;
	leftRightTilt = 0;
	frontBackTilt = 0;
	priorityCommand = false;	

	verticalSpeedPercentage = 50;
	angularSpeedPercentage = 50;
	leftRightTiltPercentage = 5;
	frontBackTiltPercentage = 5;

	redisplayParameters = true;
	trackingMode = false;
	manualMode = true;

	configuration=0;

	iplBottomImage = cvCreateImage( cvSize(176, 144 ), IPL_DEPTH_8U, 3 );
	iplFrontImage = cvCreateImage( cvSize(320, 240 ), IPL_DEPTH_8U, 3 );
	iplGreyBottomImage = cvCreateImage( cvSize(176, 144 ), IPL_DEPTH_8U, 1 );
}	


void Controller::run(){

	display->init();

	arDrone->runThread();
	threadEK = SDL_CreateThread(threadEventKeyboard,eventKeyboard);
	
	int i;

	clock_t start, end;

	int nbIm = 0;

	CvRect saveR;

	std::vector<cv::Rect> r;
	cv::Rect r2;
	cv::Rect ri;

	bool lock = false;

	float f1, f2;


	while(!quit)
	{
		updateKey();

		if(key[XBOX_BUTTON]==true){
			arDrone->emergency();		
			exit();
		}

		char c = cvWaitKey(33);

		updateNavData();
		updateFrame();
	
		recordImage();

		/* Detection */	
	
		if(trackingMode){

			cvCvtColor(iplBottomImage,iplGreyBottomImage,CV_RGB2GRAY);
			classifier->detectMultiScale(iplGreyBottomImage,r,1.1,3,1);
		}

		/* Track */

		bool b=false;
		if(tracking){
			if(!lock){
				objectNumber = display->getObjectNumber();
				printf("%d\n",objectNumber);
				r2 = r[objectNumber];


				lock = true;

			}
			else{
				
				for(int j(0); j<r.size(); ++j){
					//r intermediaire

						f1 = sqrt( pow(abs(r[j].x - r2.x),2) + pow(abs(r[j].y - r2.y ),2));

						if( (r[j].height > r2.height - 20) && 
							(r[j].height < r2.height + 20) &&
							(r[j].width > r2.width - 20) &&
							(r[j].width < r2.width + 20 ) &&
							( f1 < 40 )
						){
							//printf("hypothenuse en pixel %f\n",f1);

							if(b){
								f2 = sqrt( pow(abs(ri.x - r2.x),2) + pow(abs(ri.y - r2.y ),2));
								if(f1<f2){
									ri = r[j];
									printf("hypothenuse11 en pixel %f\n",f1);
									printf("hypothenuse22 en pixel %f\n",f2);
									objectNumber = j;
								}
							}else{
								b = true;
								ri = r[j];
								objectNumber = j;
							}
						}
				}	
				if(b){
					b = false;
					r2 = ri;

				float f3 = sqrt( pow(r2.x + r2.width/2 - 88,2) + pow(r2.y + r2.height/2 - 72,2) );

				if(f3<20){
					system("clear");
					printf("not control, it's on top\n");
				}
				else{
					system("clear");
					if( (r2.x + r2.height/2 - 88)>0 ){
						printf("go right\n");
						leftRightTilt = leftRightTiltPercentage/100.0;

					}
					else if( (r2.x + r2.height/2 - 88) < 0 ){
						printf("go left\n");
						leftRightTilt = -leftRightTiltPercentage/100.0;

					}

					if( (r2.y + r2.width/2 - 77)<0 ){
						printf("go front\n");
						frontBackTilt = -frontBackTiltPercentage/100.0;
					}
					else if( (r2.y + r2.width/2 - 77) > 0 ){

						printf("go back\n");
						frontBackTilt = frontBackTiltPercentage/100.0;

					}	

				}

				}
				else objectNumber = -1;
				
			
					


			}
		}

	
				


	/* Display */

	if(redisplayParameters)
		display->setParameters(verticalSpeedPercentage
				,angularSpeedPercentage
				,leftRightTiltPercentage
				,frontBackTiltPercentage
				,configuration
				,record
				,tracking);

	
	display->setNavData(levelBattery,altitude,psi,phi,theta,vx,vy);


	if(cam==VERTICAL_CAM || cam==LARGE_VERTICAL_CAM){
		
		if(trackingMode){
			display->setImage(iplBottomImage);
			display->setRect(r);
			display->detectionOn();
		}
		else{
			display->setImage(iplBottomImage);
			display->detectionOff();
		}
	}else{	
		display->setImage(iplFrontImage);
		display->detectionOff();
	}

	display->video(lock,objectNumber);

	display->navData();
	
	if(redisplayParameters){	
		display->parameters();
		redisplayParameters=false;
	}
	


		/* Control */

		arDrone->mutexP(COMMAND);

		arDrone->setYaw(angularSpeed);
		arDrone->setRoll(frontBackTilt);
		arDrone->setPitch(leftRightTilt);
		arDrone->setAltitude(verticalSpeed);
		
		if(priorityCommand)
			arDrone->setPriorityCommand(true);
		else	arDrone->setPriorityCommand(false);

		arDrone->mutexV(COMMAND);

		verticalSpeed = 0;
		angularSpeed = 0;
		leftRightTilt = 0;
		frontBackTilt = 0;
		priorityCommand = false;

		rightStickLeft();
		rightStickRight();
	 	rightStickDown();
		rightStickUp();
		leftStickLeft();
		leftStickRight();
		leftStickDown();
		leftStickUp();

		directionnalPadLeft();
		directionnalPadDown();
		directionnalPadRight();
		directionnalPadUp();
		startButton();
		backButton();
	
		aButton();
		xButton();
		rightBumper();
		rightTrigger();
		leftTrigger();
		yButton();

	}

	
	printf("EXIIIIIIIIIIIIIIIT\n");

}



void Controller::updateFrame(){

	arDrone->mutexP(VIDEO);
	
	if(cam==HORIZONTAL_CAM || cam==LARGE_HORIZONTAL_CAM){
		iplFrontImage = arDrone->getFrame(FRONTCAMERA);
	}else{
		iplBottomImage = arDrone->getFrame(BOTTOMCAMERA);
	}

	arDrone->mutexV(VIDEO);
}

void Controller::startStopRecord(){

	redisplayParameters=true;
	
	if(!record){

		time_t timestamp= time(NULL);
		struct tm * t = localtime(&timestamp);

		imageNumber = 1;
		sprintf(folderRecord,"./Images/%02u%02u%04u_%02u%02u%02u",t->tm_mon,t->tm_mday,1900+t->tm_year,t->tm_hour,t->tm_min,t->tm_sec);
		sprintf(folderRecord2,"./ImagesDetection/%02u%02u%04u_%02u%02u%02u",t->tm_mon,t->tm_mday,1900+t->tm_year,t->tm_hour,t->tm_min,t->tm_sec);
		sprintf(unixCommand,"mkdir %s",folderRecord);
		system(unixCommand);
		sprintf(unixCommand,"mkdir %s",folderRecord2);
		system(unixCommand);

		record=true;
	}
	else{
		record=false;
	}
}

void Controller::recordImage(){

	if(record){
			
		sprintf(nameImage,"%s/image%05u.png",folderRecord,imageNumber);

		if(cam==HORIZONTAL_CAM || cam==LARGE_HORIZONTAL_CAM)
			arDrone->saveFrame(nameImage,FRONTCAMERA,false);
		else	arDrone->saveFrame(nameImage,BOTTOMCAMERA,false);

		if(tracking){
			sprintf(nameImage2,"%s/image%05u.png",folderRecord2,imageNumber);
			display->saveFrame(nameImage2);
		}

		imageNumber++;
	}
}

void Controller::yButton(){

	if(key[Y_BUTTON])
		pressed[Y_BUTTON]=true;
	
	if(!key[Y_BUTTON] && pressed[Y_BUTTON]){
			pressed[Y_BUTTON]=false;
		if(manualMode){
			exit();
		}else if(trackingMode){
				
		}	
	}
}

void Controller::aButton(){

	if(key[A_BUTTON])
		pressed[A_BUTTON]=true;
	
	if(!key[A_BUTTON] && pressed[A_BUTTON]){
			pressed[A_BUTTON]=false;
			
		if(manualMode){
			startStopRecord();
		}else if(trackingMode){
			startStopRecord();	
		}	
	}
}

void Controller::xButton(){

	if(key[X_BUTTON])
		pressed[X_BUTTON]=true;

	if(!key[X_BUTTON] && pressed[X_BUTTON]){
			pressed[X_BUTTON]=false;
			
			if(manualMode){
				manualMode2trackingMode();
			}else if(trackingMode){
				trackingMode2manualMode();	
				}	
		}
}

void Controller::manualMode2trackingMode(){

	trackingMode = true;
	manualMode = false;
	redisplayParameters=true;
	verticalSpeedPercentage = 50;
	angularSpeedPercentage = 50;
	leftRightTiltPercentage = 5;
	frontBackTiltPercentage = 5;
	cam = VERTICAL_CAM;
	arDrone->changeCam(VERTICAL_CAM);

	display->trackingMode();
	display->mainWindow();

}

void Controller::trackingMode2manualMode(){

	trackingMode = false;
	tracking = false;
	manualMode = true;
	redisplayParameters=true;
	display->manualMode();
	display->mainWindow();
}

void Controller::rightBumper(){

	if(key[RIGHT_BUMPER])
		pressed[RIGHT_BUMPER]=true;

	if(!key[RIGHT_BUMPER] && pressed[RIGHT_BUMPER]){
		pressed[RIGHT_BUMPER]=false;
		redisplayParameters=true;
		if(manualMode){
			changeParameter();
		}
		else if(trackingMode){
			if(tracking)
				tracking = false;
			else
				tracking = true;
		}
	}
}

void Controller::changeParameter(){
	
	if(configuration==3)
		configuration=0;
	else	configuration++;
}

void Controller::rightTrigger(){
	
	if(key[RIGHT_TRIGGER])
		pressed[RIGHT_TRIGGER]=true;

	if(!key[RIGHT_TRIGGER] && pressed[RIGHT_TRIGGER]){
		pressed[RIGHT_TRIGGER]=false;

		if(manualMode){
			increaseParameter();
		}
		else if(trackingMode){
	
			display->nextPerson();
		}
	}

}

void Controller::leftTrigger(){

	if(key[LEFT_TRIGGER])
		pressed[LEFT_TRIGGER]=true;

	if(!key[LEFT_TRIGGER] && pressed[LEFT_TRIGGER]){
		pressed[LEFT_TRIGGER]=false;

		if(manualMode){
			decreaseParameter();
		}
		else if(trackingMode){
	
			display->previousPerson();
		}
	}
}

void Controller::increaseParameter(){

	redisplayParameters=true;

	switch(configuration){

		case 0 : if(verticalSpeedPercentage < MAX_VERTICAL_SPEED)
				verticalSpeedPercentage += 5;
		break;
		case 1 : if(angularSpeedPercentage < MAX_ANGULAR_SPEED)
				angularSpeedPercentage += 5;
		break;
		case 2 : if(leftRightTiltPercentage < MAX_LEFT_RIGHT_TILT)
				leftRightTiltPercentage += 5;
		break;
		case 3 : if(frontBackTiltPercentage < MAX_FRONT_BACK_TILT)
				frontBackTiltPercentage += 5;
		break;
	}
}

void Controller::decreaseParameter(){

	redisplayParameters=true;

	switch(configuration){

		case 0 : if(verticalSpeedPercentage > MIN_VERTICAL_SPEED)
				verticalSpeedPercentage -= 5;
		break;
		case 1 : if(angularSpeedPercentage > MIN_ANGULAR_SPEED)
				angularSpeedPercentage -= 5;
		break;
		case 2 : if(leftRightTiltPercentage > MIN_LEFT_RIGHT_TILT)
				leftRightTiltPercentage -= 5;
		break;
		case 3 : if(frontBackTiltPercentage > MIN_FRONT_BACK_TILT)
				frontBackTiltPercentage -= 5;
		break;
	}
}

void Controller::directionnalPadUp(){

	if(key[DIRECTIONNAL_PAD_UP])
		pressed[DIRECTIONNAL_PAD_UP]=true;

	if(!key[DIRECTIONNAL_PAD_UP] && pressed[DIRECTIONNAL_PAD_UP]){
		pressed[DIRECTIONNAL_PAD_UP]=false;

		if(manualMode){
			cam = HORIZONTAL_CAM;
			priorityCommand = true;
			arDrone->changeCam(HORIZONTAL_CAM);
		}
		else if(trackingMode){
	
		}
	}
}

void Controller::directionnalPadRight(){

	if(key[DIRECTIONNAL_PAD_RIGHT])
		pressed[DIRECTIONNAL_PAD_RIGHT]=true;

	if(!key[DIRECTIONNAL_PAD_RIGHT] && pressed[DIRECTIONNAL_PAD_RIGHT]){
		pressed[DIRECTIONNAL_PAD_RIGHT]=false;

		if(manualMode){
			cam = VERTICAL_CAM;
			priorityCommand = true;
			arDrone->changeCam(VERTICAL_CAM);
		}
		else if(trackingMode){
	
		}
	}
}

void Controller::directionnalPadDown(){

	if(key[DIRECTIONNAL_PAD_DOWN])
		pressed[DIRECTIONNAL_PAD_DOWN]=true;

	if(!key[DIRECTIONNAL_PAD_DOWN] && pressed[DIRECTIONNAL_PAD_DOWN]){
		pressed[DIRECTIONNAL_PAD_DOWN]=false;
		
		if(manualMode){
			cam = LARGE_HORIZONTAL_CAM;
			priorityCommand = true;
			arDrone->changeCam(LARGE_HORIZONTAL_CAM);
		}
		else if(trackingMode){
	
		}
	}

}

void Controller::directionnalPadLeft(){

	if(key[DIRECTIONNAL_PAD_LEFT])
		pressed[DIRECTIONNAL_PAD_LEFT]=true;

	if(!key[DIRECTIONNAL_PAD_LEFT] && pressed[DIRECTIONNAL_PAD_LEFT]){
		pressed[DIRECTIONNAL_PAD_LEFT]=false;
		

		if(manualMode){
			cam = LARGE_VERTICAL_CAM;
			priorityCommand = true;
			arDrone->changeCam(LARGE_VERTICAL_CAM);
		}
		else if(trackingMode){
	
		}
	}

}

void Controller::startButton(){

	if(key[START_BUTTON])
		pressed[START_BUTTON]=true;

	if(!key[START_BUTTON] && pressed[START_BUTTON]){
		pressed[START_BUTTON]=false;

		if(manualMode){
			priorityCommand = true;
			arDrone->takeOff();
		}
		else if(trackingMode){
			priorityCommand = true;
			arDrone->takeOff();
		}
	}
}


void Controller::backButton(){

	if(key[BACK_BUTTON])
		pressed[BACK_BUTTON]=true;

	if(!key[BACK_BUTTON] && pressed[BACK_BUTTON]){
		pressed[BACK_BUTTON]=false;

		if(manualMode){
			priorityCommand = true;
			arDrone->landing();
		}
		else if(trackingMode){
			priorityCommand = true;
			arDrone->landing();
		}
	}
}

void Controller::leftStickUp(){

	if(key[LEFT_STICK_UP]){

		if(manualMode){
			frontBackTilt = -frontBackTiltPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				tracking = false;
				redisplayParameters = true;
			}
			else{
			frontBackTilt = -frontBackTiltPercentage/100.0;

			}
		}
	}
}

void Controller::leftStickDown(){

	if(key[LEFT_STICK_DOWN]){

		if(manualMode){
			frontBackTilt = frontBackTiltPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				tracking = false;
				redisplayParameters = true;
			}
			else{
			frontBackTilt = frontBackTiltPercentage/100.0;
			
			}
		}
	}
}

void Controller::leftStickRight(){

	if(key[LEFT_STICK_RIGHT]){
	
		if(manualMode){
			leftRightTilt = leftRightTiltPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				tracking = false;
				redisplayParameters = true;
			}
			else{
			leftRightTilt = leftRightTiltPercentage/100.0;

			}
		}
	}
}

void Controller::leftStickLeft(){

	if(key[LEFT_STICK_LEFT]){

		if(manualMode){
			leftRightTilt = -leftRightTiltPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				tracking = false;
				redisplayParameters = true;
			}
			else{
			leftRightTilt = -leftRightTiltPercentage/100.0;
			

			}
		}
	}
}

void Controller::rightStickUp(){

	if(key[RIGHT_STICK_UP]){

		if(manualMode){
			verticalSpeed = verticalSpeedPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				verticalSpeed = verticalSpeedPercentage/100.0;

			}
			else{
			verticalSpeed = verticalSpeedPercentage/100.0;

			}
		}
	}
}

void Controller::rightStickDown(){

	if(key[RIGHT_STICK_DOWN]){

		if(manualMode){
			verticalSpeed = -verticalSpeedPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				verticalSpeed = -verticalSpeedPercentage/100.0;
			}
			else{
			verticalSpeed = -verticalSpeedPercentage/100.0;
			}
		}
	}
}

void Controller::rightStickRight(){

	if(key[RIGHT_STICK_RIGHT]){

		if(manualMode){
			angularSpeed = -angularSpeedPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				//tracking = false;
			}
			else{
				angularSpeed = -angularSpeedPercentage/100.0;

			}
		}
	}
}

void Controller::rightStickLeft(){

	if(key[RIGHT_STICK_LEFT]){

		if(manualMode){
			angularSpeed = angularSpeedPercentage/100.0;
		}
		else if(trackingMode){
			if(tracking){
				//tracking = false;
			}
			else{
				angularSpeed = angularSpeedPercentage/100.0;

			}
		}
	}
}

void Controller::updateKey(){

	int i;
	eventKeyboard->mutexP();

	for(i=0;i<23;i++)
		key[i]=eventKeyboard->getKey(i);
	eventKeyboard->mutexV();
}

void Controller::updateNavData(){

	arDrone->mutexP(NAVIGATION_DATA);

	levelBattery = arDrone->getLevelBattery();
	altitude = arDrone->getAltitude();
	psi = arDrone->getPsi();
	phi = arDrone->getPhi();
	theta = arDrone->getTheta();
	vx = arDrone->getVx();
	vy = arDrone->getVy();

	arDrone->mutexV(NAVIGATION_DATA);

}

void Controller::exit(){

	quit=true;
	display->shutDown();
	eventKeyboard->shutDown();
	arDrone->shutDown();

	SDL_WaitThread(threadEK, NULL);

	cvDestroyAllWindows();

	eventKeyboard->destroyMutex();

	SDL_Quit();
	
	printf("end exit\n");
}

