//#include "../Communication/doctest.h"
//#include "DroneCommunicationManager.h"
//#include <cstdlib>
//
//int sockfd, new_fd;
//struct addrinfo hints, * servinfo;
//int ListenSocket = -1;
//char recvbuf[DEFAULT_BUFLEN];
//int recvbuflen = DEFAULT_BUFLEN;
//DroneCommunicationManager droneCommunicationManager;
//TEST_CASE("initialize_winsock") {
//	int x = droneCommunicationManager.initialize_winsock();
//	printf("initialize_winsock=%d\n", x);
//	CHECK(x == 0);
//}
//
//TEST_CASE("get_server_info") {
//	int x = droneCommunicationManager.setupAddressInfo(&servinfo);
//	printf("get_server_info=%d\n", x);
//	CHECK(x == 0);
//}
//
//TEST_CASE("bind_to_first_available_socket") {
//	sockfd = droneCommunicationManager.bind_to_first_available_socket(servinfo);
//	printf("bind_to_first_available_socket=%d\n", sockfd);
//	CHECK(sockfd >1);
//}
//
//TEST_CASE("start_listening") {
//	int x = droneCommunicationManager.start_listening();
//	printf("start_listening=%d\n", x);
//	CHECK(x == 0);
//}
//
//TEST_CASE("the central while") {
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//	droneCommunicationManager.getFds()[0].fd = droneCommunicationManager.getSockfd();
//	droneCommunicationManager.getFds()[0].events = POLLRDNORM;
//	while (true)
//	{
//		cout << "<<the central while>>" << endl;
//		int result = droneCommunicationManager.define_clients_sockets_and_poll();
//		if (result == 1) break;
//		result = droneCommunicationManager.check_about_new_client_connection();
//		if (result == 1) continue;
//		result = droneCommunicationManager.checking_incoming_data_for_each_client(recvbuf, recvbuflen);
//	}
//	droneCommunicationManager.infinite_checking_for_incoming_messages();
//}
//
//TEST_CASE("clean up") {
//	droneCommunicationManager.cleanup(sockfd);
//	printf("close_all\n");
//}
