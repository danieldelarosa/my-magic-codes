/*
 * template
 *
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panLoader  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panLoader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panLoader; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 12/28/2011
 */
 
#include "regtable.h"
#include "swap.h"
#include "swcommand.h"

#define RFCHANNEL  0  //canal de comunicacion
#define SYNCWORD1  0xB5 // palabra de seguridad uno
#define SYNCWORD0 0x47  //palabra de seguridad dos
#define SOURCE_ADDR 5   //Direccion del dispositivo


byte value[]={'A','B'};

SWCOMMAND com = SWCOMMAND(0x04,0,0x04,11,value,sizeof(value));
/**
 * setup
 *
 * Arduino setup function
 */
void setup()
{
  
  //panstamp.radio.setChannel(RFCHANNEL);  // asignamos el canal de transmision
  //panstamp.radio.setSyncWord(SYNCWORD1,SYNCWORD0); // configuramos las palabras clave
  //panstamp.radio.setDevAddress(SOURCE_ADDR); // le damos una direccion local al dispositivo
  //panstamp.radio.setCCregs();  // registramos las credenciales
  //panstamp.setHighTxPower(); // aumentamos la potencia de transmision
  // Init SWAP stack
  
  //swap.nvolatToFactoryDefaults();
  swap.init();
  
  // Transmit product code
  swap.getRegister(REGI_PRODUCTCODE)->getData();
  
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW); 
  
 
}

/**
 * loop
 *
 * Arduino main loop
 */
void loop()
{
  digitalWrite(LED,HIGH);
  //swap.getRegister(REGI_DATATEST)->getData();
  com.send();
  delay(500);
  digitalWrite(LED,LOW);
  panstamp.sleepSec(5);
   
}

