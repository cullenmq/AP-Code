// 
// 
// 

#include "controller.h"
#include "RF24.h"
static byte addresses[][8] = { "1Node","2Node" };
Controller::Controller(RF24* radio, uint8_t channel, bool isController)
:Controller(radio, channel, RF24_PA_MIN, isController) {}
Controller::Controller(RF24* radio, uint8_t channel,rf24_pa_dbm_e PALevel, bool isController) 
:mWireless(radio),mChannel(channel),mPALevel(PALevel),mWriting(isController){}

void Controller::init() {
	mWireless->begin();
	mWireless->setPALevel(RF24_PA_HIGH);
	mWireless->setPayloadSize(sizeof(rx_values_t));
	mWireless->setChannel(mChannel);
	// RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
	mWireless->setDataRate(RF24_2MBPS);
	// RF24_CRC_8 or RF24_CRC_16 CRC
	//cyclic redundancy check (CRC) detects accidental changes to raw data
	mWireless->setCRCLength(RF24_CRC_16);
	mWireless->setRetries(2, 6);
	if (mWriting) {
		mWireless->openWritingPipe(addresses[1]);
		mWireless->openReadingPipe(1, addresses[0]);
	}
	else {
		mWireless->openWritingPipe(addresses[0]);
		mWireless->openReadingPipe(1, addresses[1]);
	}
	mWireless->startListening();
}

uint8_t Controller::receive(rx_values_t* rxValues) {
	int i = 0;
	while (mWireless->available()) {
		mWireless->read(rxValues, sizeof(rx_values_t));
			i++;
			if (rxValues->safety)
				isRunning = false;
			else
				isRunning=true;
	}
	return i;
}
uint8_t Controller::send(rx_values_t* rxValues) {
	mWireless->stopListening();
	bool isSent=mWireless->write(rxValues, sizeof(*rxValues));
	mWireless->startListening();
	return isSent;
}