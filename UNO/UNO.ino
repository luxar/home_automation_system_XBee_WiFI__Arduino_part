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
int led1 = 2;
int inPinD = 8;
int inPinC = A0;
int led2 = 3;
int rele1 = 7;
int rele2 = 6;
int rele3= 5;
int rele4 = 4;
uint8_t who = 'Q';
uint8_t toread = 'R';
uint8_t towrite = 'W';
boolean led1b;
uint8_t led2H;
uint8_t led2L;
boolean vrele1;
boolean vrele2;
boolean vrele3;
boolean vrele4;


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
        pinMode(rele1,  OUTPUT);
        pinMode(rele2,  OUTPUT);
        pinMode(rele3,  OUTPUT);
        pinMode(rele4,  OUTPUT);
        pinMode(inPinD,  INPUT);
        pinMode(inPinC,  INPUT);
	// start serial
	Serial.begin(9600);
	xbee.begin(Serial);
        led1b = false;
        led2L=0;
        led2H=0;
        flashLed(errorLed, 1, 50);
flashLed(dataLed, 3, 50);
	flashLed(statusLed, 3, 50);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

	xbee.readPacket();

	if (xbee.getResponse().isAvailable()) {
		// got something
                
		if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
			// got a zb rx packet
                        flashLed(statusLed, 1, 50);
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
                        // se lee el primer byte que informa que tipo de peticion es
                        
			if (comando==who){
                                //entra una peticion who, arduino debe dar su numero de serie
				uint8_t msg[]={'Q',0,0,0,2};
				ZBTxRequest zbTx = ZBTxRequest(addr64, msg, sizeof(msg));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
			}else if(comando==toread){
                                //peticion de lectura de uno de los puertos
                                
                                
                                
                                if(rx.getData(1)==0x05){                               
                                  //peticion de estado del led 1
                                 uint8_t mensaje[4];    
                                mensaje[0]=toread;
                                mensaje[1]=0x05;
                                 mensaje[2]=0x00;
                                 if(led1b==true){
                                   mensaje[3]=0xFF;
                                   }else{
                                     mensaje[3]=0x00;
                                     }
                                 ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
				}else if(rx.getData(1)==0x01){
 
 
                                  //peticion de estado del rele 1
                                 uint8_t mensaje[4];    
                                mensaje[0]=toread;
                                mensaje[1]=0x01;
                                 mensaje[2]=0x00;
                                 if(vrele1==true){
                                   mensaje[3]=0xFF;
                                   }else{
                                     mensaje[3]=0x00;
                                     }
                                 ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
                                }else if(rx.getData(1)==0x02){
 
 
                                  //peticion de estado del rele 2
                                 uint8_t mensaje[4];    
                                mensaje[0]=toread;
                                mensaje[1]=0x02;
                                 mensaje[2]=0x00;
                                 if(vrele2==true){
                                   mensaje[3]=0xFF;
                                   }else{
                                     mensaje[3]=0x00;
                                     }
                                 ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
                                }else if(rx.getData(1)==0x03){
 
 
                                  //peticion de estado del rele 3
                                 uint8_t mensaje[4];    
                                mensaje[0]=toread;
                                mensaje[1]=0x03;
                                 mensaje[2]=0x00;
                                 if(vrele3==true){
                                   mensaje[3]=0xFF;
                                   }else{
                                     mensaje[3]=0x00;
                                     }
                                 ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
                                }else if(rx.getData(1)==0x04){
 
 
                                  //peticion de estado del rele 4
                                 uint8_t mensaje[4];    
                                mensaje[0]=toread;
                                mensaje[1]=0x04;
                                 mensaje[2]=0x00;
                                 if(vrele4==true){
                                   mensaje[3]=0xFF;
                                   }else{
                                     mensaje[3]=0x00;
                                     }
                                 ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
                                }else if(rx.getData(1)==0x06){
                                  //leer pin digital
			        uint8_t mensaje[4];    
                                mensaje[0]=toread;
                                mensaje[1]=0x06;
                                mensaje[2]=led2H;
                                mensaje[3]=led2L;
                                ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
                                flashLed(dataLed, 3, 50);
                                }else if(rx.getData(1)==0x07){	
                                  // int pin7 = analogRead(inPinD);
                                   
                                   uint8_t mensaje[4];    
                                    mensaje[0]=toread;
                                    mensaje[1]=0x07;
                                    if (digitalRead(inPinD) == HIGH){
                                    mensaje[2]=0x00; 
                                        mensaje[3]=0xFF; 
                                  }else{
                                  mensaje[2]=0x00; 
                                        mensaje[3]=0x00;
                                  }
                                   
                                ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
                                  flashLed(dataLed, 3, 50);
                                  		
				}else if(rx.getData(1)==0x08){
                                //leer pin analog	
                                   int pin15 = analogRead(inPinC);
                                   
                                   uint8_t mensaje[4];    
                                    mensaje[0]=toread;
                                    mensaje[1]=0x08;
                                    mensaje[2]=pin15 >> 8 & 0xff;;
                                   mensaje[3]=pin15 & 0xff;
                                ZBTxRequest zbTx = ZBTxRequest(addr64, mensaje, sizeof(mensaje));
				xbee.send(zbTx);
                                  flashLed(dataLed, 3, 50);
                                  		
				}else{
					flashLed(errorLed, 2, 50);
				}
				uint8_t msg[]="lectura de  datos";
                                
				

			}else if(comando==towrite){
                                 //peticion de escritura
				if(rx.getData(1)==0x05){
                                        //escritura led 1
					if(rx.getData(3)==0x00){
						digitalWrite(led1, LOW);
						led1b= false;
					}else{
						digitalWrite(led1, HIGH);
						led1b= true;

					}

				}else if(rx.getData(1)==0x06){
					//escritura analogica led 2
					analogWrite(led2,rx.getData(3));
                                        uint8_t valorRecibido[2];
                                        led2L=rx.getData(3);
                                        
                                        led2H=rx.getData(2);
                                        int valorIntermedio;
                                         
				}else if(rx.getData(1)==0x01){
					//escritura rele 1
					if(rx.getData(3)==0x00){
						digitalWrite(rele1, LOW);
						vrele1= false;
					}else{
						digitalWrite(rele1, HIGH);
						vrele1= true;

					}
                                         
				}else if(rx.getData(1)==0x02){
					//escritura rele2
					if(rx.getData(3)==0x00){
						digitalWrite(rele2, LOW);
						vrele2= false;
					}else{
						digitalWrite(rele2, HIGH);
						vrele2= true;

					}
                                         
				}else if(rx.getData(1)==0x03){
					//escritura rele3
					if(rx.getData(3)==0x00){
						digitalWrite(rele3, LOW);
						vrele3= false;
					}else{
						digitalWrite(rele3, HIGH);
						vrele3= true;

					}
                                         
				}else if(rx.getData(1)==0x04){
					//escritura rele 4
					if(rx.getData(3)==0x00){
						digitalWrite(rele4, LOW);
						vrele4= false;
					}else{
						digitalWrite(rele4, HIGH);
						vrele4= true;

					}
                                         
				}else{
					flashLed(errorLed, 2, 50);
				}

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
