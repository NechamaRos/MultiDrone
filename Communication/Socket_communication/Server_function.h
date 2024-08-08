#pragma once
#pragma once
#include <vector>
#include <ws2tcpip.h>
#include "../Communication/Meta_Data.h"
#define DEFAULT_BUFLEN 512
//using namespace std;
#pragma region regular connection
int establishing_a_communication_infrastructure();
int initialize_winsock();
int setupAddressInfo(struct addrinfo** servinfo);
int bind_to_first_available_socket(struct addrinfo* servinfo);
int start_listening(int& sockfd);
#pragma endregion


int define_clients_sockets_and_poll(std::vector<int>& clientSockets, WSAPOLLFD fds[FD_SETSIZE]);
int check_about_new_client_connection(int ListenSocket,const WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets);
int accept_message(std::vector<int>& clientSockets, int& i, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
int checking_incoming_data_for_each_client(const WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
void infinite_checking_for_incoming_messages(int sockfd);
void cleanup(int ListenSocket);

void send_message_to_all(const int& sockfd, const char* message);
int send_message_to_drone(int clientSocket, const char* message);
int send_mataData_to_drone(int clientSocket, const Meta_Data& metaData);
void try_sending();