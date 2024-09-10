#include "../Socket_communication/DroneCommunicationManager.h"

int ma_in() {
	DroneCommunicationManager droneCommunicationManager;
	droneCommunicationManager.infinite_checking_for_incoming_messages();
	droneCommunicationManager.cleanup(droneCommunicationManager.getSockfd());
	return 0;
}