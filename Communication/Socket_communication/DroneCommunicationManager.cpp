#include "DroneCommunicationManager.h"
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;


int DroneCommunicationManager::getSockfd() {
    return this->sockfd;
}

WSAPOLLFD* DroneCommunicationManager::getFds() {
    return this->fds;
}

#pragma region regular connection
int DroneCommunicationManager::initialize_winsock() {
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", iResult);
        return 1;
    }
    return 0;
}
int DroneCommunicationManager::setupAddressInfo(struct addrinfo** servinfo) {
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
int DroneCommunicationManager::bind_to_first_available_socket(struct addrinfo* servinfo) {
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

int DroneCommunicationManager::establishing_a_communication_infrastructure() {
    struct addrinfo* servinfo;
    if (initialize_winsock() == 0) {
        if (setupAddressInfo(&servinfo) == 0)
        {
            return bind_to_first_available_socket(servinfo);
        }
    }
    return 1;
}

int DroneCommunicationManager::start_listening() {
    if (listen(this->sockfd, BACKLOG) == -1) {
        perror("listen");
        closesocket(this->sockfd);
        WSACleanup();
        cout << "===error in start_listening===";
        return 1;
    }

    printf("server: waiting for connections...\n");
    return 0;
}

int DroneCommunicationManager::define_clients_sockets_and_poll() {
    int numFds = this->clientSockets.size() + 1;

    for (int i = 0; i < this->clientSockets.size(); i++) {
        fds[i + 1].fd = this->clientSockets[i];
        fds[i + 1].events = POLLRDNORM;
    }
    int pollResult = WSAPoll(fds, numFds, 1000); // 1 second timeout
    if (pollResult == SOCKET_ERROR) {
        printf("WSAPoll failed: %d\n", WSAGetLastError());
        return 1;
    }
    return 0;
}

int DroneCommunicationManager::check_about_new_client_connection() {
    // Check if there's a new connection
    if (fds[0].revents & POLLRDNORM) {
        int ClientSocket = accept(this->sockfd, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            return 1;
        }
        clientSockets.push_back(ClientSocket);
        printf("Client connected\n");
        send_message_to_drone(clientSockets.size() - 1, "you are connected to the master");
    }
    return 0;
}

int DroneCommunicationManager::accept_message(int& i, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen) {
    int iResult = recv(clientSockets[i], recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        recvbuf[iResult] = '\0';
        printf("Received from client: %s\n", recvbuf); 
        this->send_message_to_drone(i, "Successfully accepted");
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

int DroneCommunicationManager::send_message_to_drone(int num_drone, const char* message) {
    if(clientSockets.size()>=1){
    if (message == "") {
        // Echo the received message back to the client
        char sendbuf[DEFAULT_BUFLEN];
        std::cout << "Enter echo to client: ";
        std::cin.getline(sendbuf, DEFAULT_BUFLEN);
        int result = send(clientSockets[num_drone], message, (int)strlen(message), 0);
        return result;
    }

    int result = send(clientSockets[num_drone], message, (int)strlen(message), 0);
    return result;
    }
}

int DroneCommunicationManager::checking_incoming_data_for_each_client(char recvbuf[DEFAULT_BUFLEN], int& recvbuflen) {
    int num = 0;
    for (int i = 0; i < clientSockets.size(); i++) {
        if (fds[i + 1].revents & POLLRDNORM) {
            num += accept_message(i, recvbuf, recvbuflen);
        }
    }
    return num == 0;
}

void DroneCommunicationManager::infinite_checking_for_incoming_messages() {
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    fds[0].fd = this->sockfd;
    fds[0].events = POLLRDNORM;
    while (true)
    {
        int result = define_clients_sockets_and_poll();
        if (result == 1) break;
        result = check_about_new_client_connection();
        if (result == 1) continue;
        result = checking_incoming_data_for_each_client(recvbuf, recvbuflen);
    }

}

void DroneCommunicationManager::cleanup(int ListenSocket) {
    closesocket(ListenSocket);
    WSACleanup();
}

void DroneCommunicationManager::send_message_to_all(char* message) {
    if (message == "") {
        // Echo the received message back to the client
        char sendbuf[DEFAULT_BUFLEN];
        std::cout << "Enter echo to client: ";
        std::cin.getline(message, DEFAULT_BUFLEN);
    }
    for (int drone_socket : clientSockets) {
        int result = send(drone_socket, message, (int)strlen(message), 0);
    }
}

int DroneCommunicationManager::send_mataData_to_drone(int clientSocket, const Meta_Data& metaData){
    return 0;
}