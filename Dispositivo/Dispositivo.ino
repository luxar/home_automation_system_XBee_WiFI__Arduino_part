/**
* Copyright (c) 2009 Andrew Rapp. All rights reserved.
*
* This file is part of XBee-Arduino.
*
* XBee-Arduino is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* XBee-Arduino is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <XBee.h>

/*
This example is for Series 2 XBee
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x00000000, 0x00000000);

ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int statusLed = 10;
int errorLed = 12;
int dataLed = 11;
int led1 = 9;
boolean led1b= false;
boolean led2b= false;
int led2 = 8;
uint8_t who = 'Q';
uint8_t toread = 'R';
uint8_t towrite = 'W';
void flashLed(int pin, int times, int wait) {

	for (int i = 0; i < times; i++) {
		digitalWrite(pin, HIGH);
		delay(wait);
		digitalWrite(pin, LOW);

		if (i + 1 < times) {
			delay(wait);
		}
	}
}

void setup() {
	pinMode(statusLed, OUTPUT);
	pinMode(errorLed, OUTPUT);
	pinMode(dataLed,  OUTPUT);
	pinMode(led1,  OUTPUT);
	pinMode(led2,  OUTPUT);
	// start serial
	Serial.begin(9600);
	xbee.begin(Serial);

	flashLed(statusLed, 3, 50);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

	xbee.readPacket();

	if (xbee.getResponse().isAvailable()) {
		// got something

		if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
			// got a zb rx packet

			// now fill our zb rx class
			xbee.getResponse().getZBRxResponse(rx);

			if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
				// the sender got an ACK
				flashLed(statusLed, 10, 10);
			} else {
				// we got it (obviously) but sender didn't get an ACK
				flashLed(statusLed, 3, 50);
			}
			uint8_t comando = rx.getData(0);
			if (comando==who){
				uint8_t msg[]="AA0001";
				ZBTxRequest zbTx = ZBTxRequest(addr64, msg, sizeof(msg));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
			}else if(comando==toread){
				uint8_t msg[]="lectura de  datos";
				ZBTxRequest zbTx = ZBTxRequest(addr64, msg, sizeof(msg));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);

			}else if(comando==towrite){

				if(rx.getData(1)==0x01){
					if(rx.getData(2)==0x00){
						digitalWrite(led1, LOW);
						led1b= false;
					}else{
						digitalWrite(led1, HIGH);
						led1b= true;

					}

				}else if(rx.getData(1)==0x02){
					if(rx.getData(2)==0x00){
						digitalWrite(led2, LOW);
						led2b= false;
					}else{
						digitalWrite(led2, HIGH);
						led2b= true;
					}
					}else{
						flashLed(errorLed, 2, 50);
                                        }



						uint8_t msg[]="escritura de datos";
					ZBTxRequest zbTx = ZBTxRequest(addr64, msg, sizeof(msg));
					xbee.send(zbTx);
					flashLed(dataLed, 3, 50);
			}else{
				flashLed(errorLed, 3, 50);
			}



				// set dataLed PWM to value of the first byte in the data
				//        analogWrite(dataLed, rx.getData(0));
				//        
				//        
				//        char from[]="hola mundo mundial";
				//        
				//        uint8_t to[sizeof(from)];
				//        memcpy(to, from, sizeof(from));
				//        char from[100];
				//        char * pfrom[100] = &from;
				//       uint8_t to;
				//        to = *pfrom;
				//          uint8_t mensaje[100];
				//         for (int i = 0; i < rx.getDataLength(); i++) {
				//         mensaje[i]=rx.getData()[i];
				//        }





			} else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
				xbee.getResponse().getModemStatusResponse(msr);
				// the local XBee sends this response on certain events, like association/dissociation

				if (msr.getStatus() == ASSOCIATED) {
					// yay this is great.  flash led
					flashLed(statusLed, 10, 10);
				} else if (msr.getStatus() == DISASSOCIATED) {
					// this is awful.. flash led to show our discontent
					flashLed(errorLed, 5, 50);
				} else {
					// another status
					flashLed(statusLed, 5, 10);
				}
			} else {
				// not something we were expecting
				// flashLed(led1, 1, 25);    
			}
		} else if (xbee.getResponse().isError()) {
			//nss.print("Error reading packet.  Error code: ");  
			//nss.println(xbee.getResponse().getErrorCode());
		}
	}
