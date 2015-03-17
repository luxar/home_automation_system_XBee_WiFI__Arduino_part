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

int statusLed = 48;
int errorLed = 38;
int dataLed = 40;
int inPinC = A0;
int rele1 = 7;
uint8_t who = 'Q';
uint8_t toread = 'R';
uint8_t towrite = 'W';
boolean vrele1;
boolean forzar;
uint8_t myArray [100];
int dirOut=0;
int dirIn=0;
int dirR=0;
int valinPinC=0;
int valObjPinC=0;
int isteresis=4;
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
        pinMode(rele1,  OUTPUT);
        pinMode(inPinC,  INPUT);
	// start serial
	Serial.begin(9600);
	xbee.begin(Serial);
        flashLed(errorLed, 1, 50);
        flashLed(dataLed, 3, 50);
	flashLed(statusLed, 3, 50);

        forzar = false;
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
                         //if (myArray != 0) {
                              //    delete [] myArray;
                             //   }
                               // myArray = new uint8_t [(rx.getDataLength()-1)*3+1];
			if (comando==who){
                                //entra una peticion who, arduino debe dar su numero de serie
				uint8_t msg[]={'Q',0,0,0,4};
				ZBTxRequest zbTx = ZBTxRequest(addr64, msg, sizeof(msg));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);
			}else if(comando==toread){
                                //peticion de lectura de uno de los puertos
                                dirIn=1;
                                myArray[0]=toread;
                                for(dirOut=1;dirOut<((rx.getDataLength()-1)*3+1);dirOut++){
                                  if(rx.getData(dirIn)==0x01){
                                 //peticion de estado forzado encendido
                                myArray[dirOut]=0x01;
                                 myArray[dirOut+1]=0x00;
                                 if(forzar==true){
                                   myArray[dirOut+2]=0xFF;
                                   }else{
                                     myArray[dirOut+2]=0x00;
                                     }
                                 
                                }else if(rx.getData(dirIn)==0x02){
                                //Peticion de temperatura objetivo	
                                    myArray[dirOut]=0x02;
                                    myArray[dirOut+1]=valObjPinC >> 8 & 0xff;;
                                    myArray[dirOut+2]=valObjPinC & 0xff;
                               
                                  		
				}else if(rx.getData(dirIn)==0x03){
                                //leer pin analog	
                                    
                                   
                                   
                                    myArray[dirOut]=0x03;
                                     myArray[dirOut+1]=valinPinC >> 8 & 0xff;;
                                  myArray[dirOut+2]=valinPinC & 0xff;
                               
                                  		
				}else if(rx.getData(dirIn)==0x04){
                                 //peticion de estado del rele 1
                                myArray[dirOut]=0x04;
                                 myArray[dirOut+1]=0x00;
                                 if(vrele1==true){
                                   myArray[dirOut+2]=0xFF;
                                   }else{
                                     myArray[dirOut+2]=0x00;
                                     }
                                }else{
					flashLed(errorLed, 2, 50);
				}
                                 dirOut++;
                                dirOut++;
                                 dirIn++;
                                
                                }//fin blucle
                              
				ZBTxRequest zbTx = ZBTxRequest(addr64, myArray, ((rx.getDataLength()-1)*3+1));
				xbee.send(zbTx);
				flashLed(dataLed, 3, 50);;
                              
				
                                
				

			}else if(comando==towrite){
                                 //peticion de escritura
                                 
                                 for(int dirR=1;dirR<rx.getDataLength();dirR++){
                                   if(rx.getData(dirR)==0x01){
                                        //escritura led 1
					if(rx.getData(dirR+2)==0x00){
						
						forzar= false;
					}else{
						
						forzar= true;

					}

				}else if(rx.getData(dirR)==0x02){
					//escritura del valor obd
					
                                   uint8_t  d1=rx.getData(dirR+2);
                                        
                                    uint8_t  d2=rx.getData(dirR+1);
                                         valObjPinC = ((int)d2 << 8) | d1;
                                   }else{
					flashLed(errorLed, 2, 50);
				}
                                dirR++;
                                dirR++;

                                   }//final bucle
				

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
valinPinC = analogRead(inPinC);
if(forzar== true)
{
  digitalWrite(rele1,HIGH);
  vrele1=true;
} else if( valinPinC>isteresis+valObjPinC){
  digitalWrite(rele1,LOW);
  vrele1=false;
  
  }else if(valinPinC<isteresis-valObjPinC){
    digitalWrite(rele1,HIGH);
  vrele1=true;
    }





}
