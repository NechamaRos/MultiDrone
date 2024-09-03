#pragma once
#include <vector>
#include <ws2tcpip.h>
#include "../Communication/Meta_Data.h"
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define BACKLOG 10
using namespace std;

class DroneCommunicationManager
{
public:
	DroneCommunicationManager()
	{
		this->sockfd = -1;
		cout << "in DroneCommunicationManager ctor";
		this->sockfd=this->establishing_a_communication_infrastructure();
		if(this->sockfd!=-1){
		int result=this->start_listening();
		cout << sockfd<<"finish"<<endl;
		}
	}

	int getSockfd();
	WSAPOLLFD* getFds();
#pragma region regular connection
	int establishing_a_communication_infrastructure();
	int initialize_winsock();
	int setupAddressInfo(struct addrinfo** servinfo);
	int bind_to_first_available_socket(struct addrinfo* servinfo);
	int start_listening();
#pragma endregion
	int define_clients_sockets_and_poll();
	int check_about_new_client_connection();
	int accept_message(int& i, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
	int checking_incoming_data_for_each_client(char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
	void infinite_checking_for_incoming_messages();
	void cleanup(int ListenSocket);
	void send_message_to_all(char* message);
	int send_message_to_drone(int clientSocket, const char* message);
	int send_mataData_to_drone(int clientSocket, const Meta_Data& metaData);

private:
	int sockfd;
	WSAPOLLFD fds[FD_SETSIZE];
	vector<int> clientSockets;
};


