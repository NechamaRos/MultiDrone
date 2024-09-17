#include "SlaveDroneCommunicationManager.h"
#include <iostream>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#include <winsock2.h>
#define POLL WSAPoll
#define CLOSE_SOCKET closesocket
#define PRINT_ERROR(name) (std::cerr << name << " failed: " << WSAGetLastError() << std::endl)
#define CLEANUP WSACleanup()
#define INVALID__SOCKET INVALID_SOCKET
#define SOCKET__ERROR SOCKET_ERROR
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define POLL poll
#define CLOSE_SOCKET close
#define PRINT_ERROR(name) (perror(name))
#define CLEANUP 
#define INVALID__SOCKET -1
#define SOCKET__ERROR -1
#endif

using namespace std;

#ifdef _WIN32
int SlaveDroneCommunicationManager::initialize_winsock() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", iResult);
        return 1;
    }
    return 0;
}
#endif
int SlaveDroneCommunicationManager::setupAddressInfo(const char* hostname, struct addrinfo* hints, struct addrinfo** servinfo) {
    memset(hints, 0, sizeof(*hints));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;

    int rv;
    if ((rv = getaddrinfo(hostname, DEFAULT_PORT, hints, servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        CLEANUP;
        return 1;
    }
    return 0;

}

int SlaveDroneCommunicationManager::connect_to_server(int& sockfd, struct addrinfo* servinfo) {

    struct addrinfo* ptr = NULL;
    for (ptr = servinfo; ptr != NULL; ptr = ptr->ai_next) {
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sockfd == -1) {
            PRINT_ERROR("socket");
            CLEANUP;
            return 1;
        }

        int iResult = connect(sockfd, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == -1) {
            CLOSE_SOCKET(sockfd);
            sockfd = -1;
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);

    if (sockfd == -1) {
        printf("Unable to connect to server!\n");
        CLEANUP;
        return 1;
    }
    return sockfd;

}

int SlaveDroneCommunicationManager::establishing_a_communication_infrastructure() {
    int sockfd;
    struct addrinfo hints, * servinfo;
    int initialize_winsock_result;
#ifdef _WIN32
    initialize_winsock_result = initialize_winsock();
#else    
    initialize_winsock_result = 0;
#endif
    if (initialize_winsock_result == 0) {
        if (setupAddressInfo("10.100.102.4", &hints, &servinfo) == 0)
        {
            sockfd = connect_to_server(sockfd, servinfo);
            return sockfd;
        }
    }
    return 1;
}


int SlaveDroneCommunicationManager::checking_about_incoming_data_from_server(int& sockfd, POLLFD fds[FD_SETSIZE], char recvbuf[DEFAULT_BUFLEN], int recvbuflen) {
    if (fds[0].revents & POLLIN) {
        int iResult = recv(sockfd, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::cout << "in the inifinity loop we finf out that one message received from server: " << std::string(recvbuf, iResult) << std::endl;
            return 0;
        }
        else if (iResult == 0) {
            std::cout << "Connection closed by server." << std::endl;
            return 1;
        }
        else {
            PRINT_ERROR("recv");
            return 1;
        }
    }
}

void SlaveDroneCommunicationManager::infinite_checking_for_incoming_messages(int sockfd) {

    POLLFD fds[2];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN; 
    fds[1].fd = fileno(stdin);
    fds[1].events = POLLIN;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    std::string userInput;
    char sendbuf[DEFAULT_BUFLEN];

    while (true)
    {
        int pollCount = POLL(fds, 2, -1);
        if (pollCount == -1) {
            PRINT_ERROR("poll");
            break;
        }
        if (checking_about_incoming_data_from_server(sockfd, fds, recvbuf, recvbuflen) == 1) break;
    }

}

void SlaveDroneCommunicationManager::loop_of_sending_messages_to_server(int sockfd) {
    int i = 0;
    while (true)
    {
        int result = send_message_to_server(sockfd);
        if (result == 1) break;
    }
}

int SlaveDroneCommunicationManager::send_message_to_server(int& sockfd) {
    char sendbuf[DEFAULT_BUFLEN];

    std::cout << "Enter message to send: ";
    std::cin.getline(sendbuf, DEFAULT_BUFLEN);

    if (sendbuf == "exit") return 1;
    int iResult = send(sockfd, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == -1) {
        PRINT_ERROR("send");
        CLOSE_SOCKET(sockfd);
        CLEANUP;
        return 1;
    }
    return 0;
}

void SlaveDroneCommunicationManager::cleanup(int& sockfd) {
    CLOSE_SOCKET(sockfd);
    CLEANUP;
}