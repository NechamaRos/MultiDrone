#pragma once
#include <winsock2.h>
#include <vector>
#define DEFAULT_BUFLEN 512

#pragma region regular connection
int establishing_a_communication_infrastructure();
int initialize_winsock();
int setupAddressInfo(const char* hostname, struct addrinfo* hints, struct addrinfo** servinfo);
int bind_to_first_available_socket(int& sockfd, struct addrinfo* servinfo);
int start_listening(int& sockfd);
#pragma endregion
// Add a new file descriptor to the set
void add_to_activeConnectionPollfds(struct pollfd* activeConnectionPollfds[], int newfd, int* fd_count, int* fd_size);

// Remove an index from the set
void delete_from_activeConnectionPollfds(struct pollfd activeConnectionPollfds[], int i, int* fd_count);

int define_clients_sockets_and_poll(std::vector<int>& clientSockets, WSAPOLLFD fds[FD_SETSIZE]);
int check_about_new_client_connection(int& ListenSocket, WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets);
int accept_message(std::vector<int>& clientSockets, int& i, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
int checking_incoming_data_for_each_client(WSAPOLLFD fds[FD_SETSIZE], std::vector<int>& clientSockets, char recvbuf[DEFAULT_BUFLEN], int& recvbuflen);
void infinite_checking_for_incoming_messages(int sockfd);
void cleanup(int ListenSocket);

void send_message_to_all(const int& sockfd, const char* message);
int send_message_to_drone(int clientSocket, string chunk);
int send_mataData_to_drone(int clientSocket, const Meta_Data& metaData);