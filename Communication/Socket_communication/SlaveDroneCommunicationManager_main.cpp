#include "SlaveDroneCommunicationManager.h"
using namespace std;
int slaveDroneCommunicationManager_main() {
	SlaveDroneCommunicationManager slaveDroneCommunicationManager;
	int sockfd = slaveDroneCommunicationManager.establishing_a_communication_infrastructure();
	if (sockfd == 1) return 1;
	slaveDroneCommunicationManager.send_message_to_server(sockfd);
	slaveDroneCommunicationManager.infinite_checking_for_incoming_messages(sockfd);
	slaveDroneCommunicationManager.cleanup(sockfd);
}
