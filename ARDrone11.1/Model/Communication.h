#ifndef HEADER_COMMUNICATION
#define HEADER_COMMUNICATION

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(param) close(param)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IP_DRONE		"192.168.1.1"
#define IP_COMPUTER		"192.168.1.2"

#define AT_PORT				5556
#define VIDEO_PORT			5555
#define NAVDATA_PORT			5554


class Communication
{
	public:

	Communication(int port);
	SOCKET init();

	SOCKET createSocket();
	void bindSocket();
	void closeSocket();

	SOCKADDR_IN configureConnexion(char* ip, int port);
	SOCKADDR_IN receiveMessage(void* message, int* sizeMessage, int sizeMaximum);
	void sendMessage(char* message);
	void selectReceiveData(void* message, int* size, int sizeMaximum, int time_us);
	void wakeUpPort();

	private:

	int port;
	SOCKADDR_IN addrDrone, addrComputer;
	SOCKET sock;

};

#endif
