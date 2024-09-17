#pragma once

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#include <Ws2tcpip.h>
#define POLLFD WSAPOLLFD
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#define POLLFD struct pollfd
#endif

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define BACKLOG 10

class SlaveDroneCommunicationManager
{
public:
#ifdef _WIN32
    int initialize_winsock();
#endif
    int setupAddressInfo(const char* hostname, struct addrinfo* hints, struct addrinfo** servinfo);

    int connect_to_server(int& sockfd, struct addrinfo* servinfo);

    int establishing_a_communication_infrastructure();


    int checking_about_incoming_data_from_server(int& sockfd, POLLFD fds[FD_SETSIZE], char recvbuf[DEFAULT_BUFLEN], int recvbuflen);

    void infinite_checking_for_incoming_messages(int sockfd);

    void loop_of_sending_messages_to_server(int sockfd);

    int send_message_to_server(int& sockfd);

    void cleanup(int& sockfd);
};

