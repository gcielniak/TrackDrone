#include "Communication.h"

Communication::Communication(int port)
{
	this->port = port;

}

SOCKET Communication::createSocket(){
	
	sock = socket(AF_INET		//	domain : TCP/IP protocol
		     ,SOCK_DGRAM	//	service: UDP/IP protocol
		     ,0
		     );
	
	if( sock == -1 )
	{
		perror("Create Socket");
		exit(0);
	}

	return sock;

}

SOCKADDR_IN Communication::configureConnexion(char* ip, int p){

	SOCKADDR_IN sin;

	sin.sin_addr.s_addr = inet_addr(ip);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(p);

	return sin;
}

void Communication::bindSocket(){

	if(bind(sock,(SOCKADDR*)&addrComputer,sizeof(addrComputer))==SOCKET_ERROR)
	{
		perror("Bind Socket");
		exit(0);
	}

}

void Communication::closeSocket(){

	if(closesocket(sock)==SOCKET_ERROR)
	{
		perror("Close Socket");
		exit(0);
	}
}

void Communication::sendMessage(char* message){

	int length = strlen(message);

	if( sendto(sock, message, length, 0, (SOCKADDR*) &addrDrone, sizeof(addrDrone)) != length )
	{
		perror("Error Send Message\n");
		exit(0);
	}
}

SOCKADDR_IN Communication::receiveMessage(void* message, int* sizeMessage, int sizeMaximum){

	int sizeMess;
	SOCKADDR_IN from;
	socklen_t lengthFrom;

	lengthFrom = sizeof(from);

	sizeMess = recvfrom(sock, message, sizeMaximum, 0, (SOCKADDR*) &from , &lengthFrom);

	if (sizeMess < 0)
	{
		perror("Error Receive Message\n");
		exit(0);
	}

	*sizeMessage = sizeMess;

	return from;
}

void Communication::wakeUpPort()
{
	int one = 1;

	if( sendto(sock, (char*)&one, sizeof(one), 0, (struct sockaddr*)&addrDrone, sizeof(addrDrone)) < 0 )
	{
		printf("Error Wake Up\n");
		exit(0);
	}
}

void Communication::selectReceiveData(void* message, int* size, int sizeMaximum, int time_us)
{
	struct timeval tv;
	fd_set rfds;		//descriptor to know if a socket had receive data
	int ret;

	FD_ZERO(&rfds);		//delete all descriptor
	FD_SET(sock, &rfds);

	tv.tv_sec = 0;
	tv.tv_usec = time_us;

	ret = select(sock+1, &rfds, NULL, NULL, &tv);

	if (ret == -1)
		*size = -1;
	else if(ret)
		receiveMessage(message, size, sizeMaximum);
	else
		*size = 0;
}

SOCKET Communication::init()
{
	addrDrone = configureConnexion(IP_DRONE,port);
	addrComputer = configureConnexion(IP_COMPUTER, port + 50);
	sock = createSocket();
	bindSocket();

	return sock;
}


