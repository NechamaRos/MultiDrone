#include "DroneCommunicationManager.h"
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


int DroneCommunicationManager::getSockfd() {
    return this->sockfd;
}

POLLFD* DroneCommunicationManager::getFds() {
    return this->fds;
}

#pragma region regular connection
#ifdef _WIN32
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
#endif

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
        CLEANUP;
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
            CLOSE_SOCKET(sockfd);
            CLEANUP;
            return 1;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            CLOSE_SOCKET(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        CLEANUP;
        return 1;
    }

    return sockfd;

}
#pragma endregion

int DroneCommunicationManager::establishing_a_communication_infrastructure() {
    struct addrinfo* servinfo;
    int initialize_winsock_result;
    #ifdef _WIN32
        initialize_winsock_result = initialize_winsock();
    #else    
        initialize_winsock_result = 0;
    #endif
    if (initialize_winsock_result == 0) {
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
        CLOSE_SOCKET(this->sockfd);
        CLEANUP;
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
        fds[i + 1].events = POLLIN;
    }
    int pollResult = POLL(fds, numFds, 1000); // 1 second timeout
    if (pollResult == SOCKET__ERROR) {
        //printf("WSAPoll failed: %d\n", WSAGetLastError());
        PRINT_ERROR("poll");
        return 1;
    }
    return 0;
}

int DroneCommunicationManager::check_about_new_client_connection() {
    // Check if there's a new connection
    if (fds[0].revents & POLLIN) {
        int ClientSocket = accept(this->sockfd, NULL, NULL);
        if (ClientSocket == INVALID__SOCKET) {
            //printf("accept failed: %d\n", WSAGetLastError());
            PRINT_ERROR("accept");
            return 1;
        }
        clientSockets.push_back(ClientSocket);
        send_message_to_drone(clientSockets.size() - 1, "you are connected to the master");
        printf("Client connected\n");
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
        CLOSE_SOCKET(clientSockets[i]);
        clientSockets.erase(clientSockets.begin() + i);
        i--; // Adjust index after erasing
        return 1;
    }
    else {
        //printf("recv failed: %d\n", WSAGetLastError());
        PRINT_ERROR("recv");
        CLOSE_SOCKET(clientSockets[i]);
        clientSockets.erase(clientSockets.begin() + i);
        i--; // Adjust index after erasing
        return 1;
    }
    return 1;
}

int DroneCommunicationManager::send_message_to_drone(int num_drone, const char* message) {
    if (clientSockets.size() >= 1) {
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
    else return 0;
}

int DroneCommunicationManager::checking_incoming_data_for_each_client(char recvbuf[DEFAULT_BUFLEN], int& recvbuflen) {
    int num = 0;
    for (int i = 0; i < clientSockets.size(); i++) {
        if (fds[i + 1].revents & POLLIN) {
            num += accept_message(i, recvbuf, recvbuflen);
        }
    }
    return num == 0;
}

void DroneCommunicationManager::infinite_checking_for_incoming_messages() {
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    fds[0].fd = this->sockfd;
    fds[0].events = POLLIN;
    int count = 0;
    while (true)
    {
        //cout << count++ <<endl;
        int result = define_clients_sockets_and_poll();
        if (result == 1) break;
        result = check_about_new_client_connection();
        if (result == 1) continue;
        result = checking_incoming_data_for_each_client(recvbuf, recvbuflen);
    }
}

void DroneCommunicationManager::cleanup(int ListenSocket) {
    CLOSE_SOCKET(ListenSocket);
    CLEANUP;
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
