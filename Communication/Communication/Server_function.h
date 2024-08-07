#pragma once
#include <winsock2.h>
#include <vector>
#define DEFAULT_BUFLEN 512

#pragma region regular connection
int initialize_winsock();
int get_server_info(const char* hostname, struct addrinfo* hints, struct addrinfo** servinfo);
int bind_to_first_available_socket(int& sockfd, struct addrinfo* servinfo);
int start_listening(int& sockfd);
#pragma endregion
// Add a new file descriptor to the set
void add_to_activeConnectionPollfds(struct pollfd* activeConnectionPollfds[], int newfd, int* fd_count, int* fd_size);

// Remove an index from the set
void delete_from_activeConnectionPollfds(struct pollfd activeConnectionPollfds[], int i, int* fd_count);

int define_clients_sockets_and_poll(std::vector<int>& clientSockets, WSAPOLLFD fds[FD_SETSIZE]);
int check_about_new_client_connection(int& ListenSocket, WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets);
void accept_message(std::vector<int>& clientSockets, int& i, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
void checking_incoming_data_for_each_client(WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
void cleanup(int ListenSocket);

void send_message_to_all(int& sockfd, char* message);
int send_message_to_drone(int clientSocket);