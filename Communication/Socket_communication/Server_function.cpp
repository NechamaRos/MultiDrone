#include "Server_function.h"
#include <winsock2.h>
//#include <stdio.h>
#include <iostream>
#include <vector>
#include "../Communication/Meta_Data.h"
#include <thread>
#ifndef _ws2tcpip
#define ws2tcpip
#include <ws2tcpip.h>
#endif // !_ws2tcpip

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
//#define DEFAULT_BUFLEN 512
#define BACKLOG 10

using namespace std;
#pragma region regular connection
int initialize_winsock() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", iResult);
        return 1;
    }
    return 0;
}
int setupAddressInfo(struct addrinfo** servinfo) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE; // use my IP

    int rv;
    if ((rv = getaddrinfo(NULL, DEFAULT_PORT, &hints, servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        WSACleanup();
        return 1;
    }
    return 0;
}
int bind_to_first_available_socket(struct addrinfo* servinfo) {
    struct addrinfo* p;
    int sockfd;
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        int yes = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)) == -1) {
            perror("setsockopt");
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            closesocket(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        WSACleanup();
        return 1;
    }

    return sockfd;

}
#pragma endregion

int establishing_a_communication_infrastructure() {
    struct addrinfo* servinfo;
    if (initialize_winsock() == 0) {
        if (setupAddressInfo(&servinfo) == 0)
        {
            return bind_to_first_available_socket(servinfo);
        }
    }
    return 1;
}

int start_listening(int& sockfd) {
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("server: waiting for connections...\n");
    return 0;
}


int define_clients_sockets_and_poll(std::vector<int>& clientSockets, WSAPOLLFD fds[FD_SETSIZE]) {
    int numFds = clientSockets.size() + 1;

    for (int i = 0; i < clientSockets.size(); i++) {
        fds[i + 1].fd = clientSockets[i];
        fds[i + 1].events = POLLRDNORM;
    }
    int pollResult = WSAPoll(fds, numFds, 1000); // 1 second timeout
    if (pollResult == SOCKET_ERROR) {
        printf("WSAPoll failed: %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}
int check_about_new_client_connection(int ListenSocket,const WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets) {
    // Check if there's a new connection
    if (fds[0].revents & POLLRDNORM) {
        int ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            return 1;
        }
        clientSockets.push_back(ClientSocket);
        printf("Client connected\n");
    }
    return 0;
}
int accept_message(std::vector<int>& clientSockets, int& i, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen) {
    int iResult = recv(clientSockets[i], recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        recvbuf[iResult] = '\0';
        printf("Received from client: %s\n", recvbuf); 
        thread echo_message_to_the_drone(send_message_to_drone,clientSockets[i],"Successfully accepted");
        echo_message_to_the_drone.join();
        return 0;
    }
    else if (iResult == 0) {
        printf("Connection closing...\n");
        closesocket(clientSockets[i]);
        clientSockets.erase(clientSockets.begin() + i);
        i--; // Adjust index after erasing
        return 1;
    }
    else {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(clientSockets[i]);
        clientSockets.erase(clientSockets.begin() + i);
        i--; // Adjust index after erasing
        return 1;
    }
    return 1;
}

int send_message_to_drone(int clientSocket, const char* message) {
    if (message == "") {
        // Echo the received message back to the client
        char sendbuf[DEFAULT_BUFLEN];
        std::cout << "Enter echo to client: ";
        std::cin.getline(sendbuf, DEFAULT_BUFLEN);
        int result = send(clientSocket, message, (int)strlen(message), 0);
        return result;
    }

    int result = send(clientSocket, message, (int)strlen(message), 0);
    return result;
}



int checking_incoming_data_for_each_client(const WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen) {
    int num = 0;
    for (int i = 0; i < clientSockets.size(); i++) {
        if (fds[i + 1].revents & POLLRDNORM) {
            num += accept_message(clientSockets, i, recvbuf, recvbuflen);
        }
    }
    return num == 0;
}

void infinite_checking_for_incoming_messages(int sockfd) {
    std::vector<int> clientSockets;
    WSAPOLLFD fds[FD_SETSIZE];
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    fds[0].fd = sockfd;
    fds[0].events = POLLRDNORM;
    while (true)
    {
        int result = define_clients_sockets_and_poll(clientSockets, fds);
        if (result == 1) break;
        result = check_about_new_client_connection(sockfd, fds, clientSockets);
        if (result == 1) continue;
        result = checking_incoming_data_for_each_client(fds, clientSockets, recvbuf, recvbuflen);
    }

}

void cleanup(int ListenSocket) {
    closesocket(ListenSocket);
    WSACleanup();
}
void send_message_to_all(const int& sockfd, const char* message) {}

int send_mataData_to_drone(int clientSocket, const Meta_Data& metaData){
    return 0;
}