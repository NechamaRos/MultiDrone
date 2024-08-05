#include "Server_function.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#ifndef _ws2tcpip
#define ws2tcpip
#include <ws2tcpip.h>
#endif // !_ws2tcpip

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
#define BACKLOG 10


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
int get_server_info(const char* hostname, struct addrinfo* hints, struct addrinfo** servinfo) {
    // Set up the address information
    memset(hints, 0, sizeof hints);
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = AI_PASSIVE; // use my IP

    //in erorr- print this
    int rv;
    if ((rv = getaddrinfo(NULL, DEFAULT_PORT, hints, servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        WSACleanup();
        return 1;
    }
    return 0;

}
//creat the socket by the servinfo- that contain the ip adress and protocol number.
int bind_to_first_available_socket(int& sockfd, struct addrinfo* servinfo) {
    struct addrinfo* p;
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

    return 0;

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
#pragma endregion



void add_to_activeConnectionPollfds(struct pollfd* activeConnectionPollfds[], int newfd, int* fd_count, int* fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *activeConnectionPollfds = (pollfd*)realloc(*activeConnectionPollfds, sizeof(**activeConnectionPollfds) * (*fd_size));
    }

    (*activeConnectionPollfds)[*fd_count].fd = newfd;
    (*activeConnectionPollfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}
// Remove an index from the set
void del_from_activeConnectionPollfds(struct pollfd activeConnectionPollfds[], int i, int* fd_count)
{
    // Copy the one from the end over this one
    activeConnectionPollfds[i] = activeConnectionPollfds[*fd_count - 1];

    (*fd_count)--;
}
//while(true)
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
int check_about_new_client_connection(int& ListenSocket, WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets) {
    // Check if there's a new connection
    if (fds[0].revents & POLLRDNORM) {
        int ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            //continue;
            return 1;
        }
        clientSockets.push_back(ClientSocket);
        printf("Client connected\n");
    }
    return 0;
}
void accept_message(std::vector<int>& clientSockets, int& i, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen) {
    int iResult = recv(clientSockets[i], recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        recvbuf[iResult] = '\0';
        printf("Received from client: %s\n", recvbuf);

        send_message_to_drone(clientSockets[i]);

    }
    else if (iResult == 0) {
        printf("Connection closing...\n");
        closesocket(clientSockets[i]);
        clientSockets.erase(clientSockets.begin() + i);
        i--; // Adjust index after erasing
    }
    else {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(clientSockets[i]);
        clientSockets.erase(clientSockets.begin() + i);
        i--; // Adjust index after erasing
    }

}

int send_message_to_drone(int clientSocket) {
    // Echo the received message back to the client
    char sendbuf[DEFAULT_BUFLEN];

    std::cout << "Enter echo to client: ";
    std::cin.getline(sendbuf, DEFAULT_BUFLEN);
    return send(clientSocket, sendbuf, (int)strlen(sendbuf), 0);
}

void checking_incoming_data_for_each_client(WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen) {
    for (int i = 0; i < clientSockets.size(); i++) {
        if (fds[i + 1].revents & POLLRDNORM) {
            //accept the message
            accept_message(clientSockets, i, recvbuf, recvbuflen);
        }
    }
}
void cleanup(int ListenSocket) {
    closesocket(ListenSocket);
    WSACleanup();
}
void send_message_to_all(int& sockfd, char* message) {}
